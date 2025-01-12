; proc CNV.parseCMD c uses ebx, lpArgv:dword
; 	local lpArgMem:DWORD
; 	@call [GetCommandLineA]()
; 	mov edx, eax
; 	xor ecx, ecx
; 	mov ebx, 1
; 	.mainloop:
; 		movzx eax, byte[edx+ecx]
; 		cmp eax, '"'
; 		jne .noquotes
; 			.findquote:
; 				inc ecx
; 				movzx eax, byte[edx+ecx]
; 				cmp eax, 0
; 					je .NoSpace
; 			cmp eax, '"'
; 			jne .findquote
; 		.noquotes:
; 		cmp eax, " "
; 		jne .NoSpace
; 			cmp byte[edx+ecx-1], 0
; 			je .SpaceSeries			;серия пробелов
; 				push edx
; 				inc ebx
; 			.SpaceSeries:
; 			lea edx, [edx+ecx+1]
; 			mov byte [edx-1], 0
; 			mov ecx, -1
; 		.NoSpace:
; 		inc ecx
; 	cmp eax, 0
; 	jne .mainloop
; 	push edx
; 	@call CNV:alloc(addr ebx*4)
; 	mov [lpArgMem], eax
; 	mov ecx, ebx
; 	@@:
; 		pop dword[eax+(ecx-1)*4]
; 	dec ecx
; 	jnz @b
; 	mov edx, [lpArgMem]
; 	mov ecx, [lpArgv]
; 	mov [ecx], edx
; 	mov eax, ebx
; 	ret
; endp

struct BITMAPINFO
	bmiHeader BITMAPINFOHEADER
	bmiColors dd 1 dup(?)
ends

proc CNV.BMPToFile c, hBmp, lpFname, bitCount
	local <bmInfo:BITMAPINFO <sizeof.BITMAPINFOHEADER, 0, 0, 0, 0, 0>>
	local tmpDC:QWORD, lpBmBits:QWORD
	@call [GetDC](NULL)
	mov [tmpDC], eax
	; mov [bmInfo.bmiHeader.biBitCount], 24
	@call [GetDIBits]([tmpDC], [hBmp], 0, 0, NULL, addr bmInfo, DIB_RGB_COLORS)
	test eax, eax
	jnz @f
		@call [ReleaseDC]([tmpDC])
		mov eax, 0
		ret
	@@:
	@call CNV:alloc([bmInfo.bmiHeader.biSizeImage])
	mov [lpBmBits], eax
	mov [bmInfo.bmiHeader.biCompression], BI_RGB 
	mov eax, [bitCount]
	mov [bmInfo.bmiHeader.biBitCount], ax
	@call [GetDIBits]([tmpDC], [hBmp], 0, [bmInfo.bmiHeader.biHeight], [lpBmBits], addr bmInfo, DIB_RGB_COLORS)
	@call [ReleaseDC]([tmpDC])

	local <bmfHeader:BITMAPFILEHEADER "BM", ?, 0, 0, sizeof.BITMAPFILEHEADER+sizeof.BITMAPINFO>
	mov eax, [bmInfo.bmiHeader.biSizeImage]
	add eax, [bmfHeader.bfOffBits]
	mov [bmfHeader.bfSize], eax

	local fHandle:QWORD
	mov [bmInfo.bmiHeader.biCompression], BI_RGB 
	mov eax, [bitCount]
	mov [bmInfo.bmiHeader.biBitCount], ax
	@call [CreateFileA]([lpFname], GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)
	mov [fHandle], eax
	@call [WriteFile]([fHandle], addr bmfHeader, sizeof.BITMAPFILEHEADER, NULL, NULL)
	@call [WriteFile]([fHandle], addr bmInfo, sizeof.BITMAPINFO, NULL, NULL)
	@call [WriteFile]([fHandle], [lpBmBits], [bmInfo.bmiHeader.biSizeImage], NULL, NULL)
	@call [CloseHandle]([fHandle])
	@call CNV:free([lpBmBits])
	ret
endp

proc CNV.ui64div c, dividend:QWORD, divisor:QWORD
	virtual at dividend
		.result Divq
	end virtual
	bsr edx, dword[divisor + 4]
		jz .QD_div
	bsr ecx, dword[dividend + 4]
		jz .lazy_ret
	cmp edx, ecx
		ja .lazy_ret
	movq xmm0, [divisor]
	movq xmm1, [dividend]
	jnz .no_equal_powers
		pcmpgtq xmm0, xmm1
		movd eax, xmm0
		movq xmm0, [divisor]
		test eax, eax
			jnz .lazy_ret
	.no_equal_powers:
	sub ecx, edx
	movd xmm3, ecx
	psllq xmm0, xmm3
	movq xmm2, [high_one]
	psllq xmm2, xmm3
	xor edx, edx
	inc ecx
	@@:
		shl edx, 1
		ptest xmm1, xmm2
		jz .no_subtract
			psubq xmm1, xmm0
			add edx, 1
		.no_subtract:
		psrlq xmm0, 1 
		psrlq xmm2, 1
	loop @b
	movq qword[.result.reminder], xmm1
	mov dword[.result.result], edx
	mov dword[.result.result + 4], 0
	lea eax, [.result]
	ret

	.QD_div:
		mov ecx, dword[divisor]
		mov eax, dword[dividend + 4]
		xor edx, edx
		div ecx
		mov dword[.result.result + 4], eax
		mov eax, dword[dividend]
		div ecx
		mov dword[.result.result], eax
		mov dword[.result.reminder], edx
		mov dword[.result.reminder + 4], 0
		lea eax, [.result]
		ret

	.lazy_ret:
		mov dword[.result.result], 0
		mov dword[.result.result + 4], 0
		lea eax, [.result]
		ret

	high_one dq 0100000000h
endp
	
proc CNV.__ui64sqrt c, num:QWORD
	xor eax, eax
	bsr ecx, dword[num + 4]
	jnz .no_check_low
		@jret CNV.ui32sqrt()
	.no_check_low:
	add ecx, 32
	and ecx, 0FEh
	movq xmm4, [num]
	pxor xmm5, xmm5
	@@:
		shl eax, 1

		psllq xmm5, 2
		movq xmm1, xmm5	;xmm1 = (eax + 1) * (eax + 1)
		movd xmm2, eax
		paddq xmm1, xmm2
		lea edx, [eax+1]
		movd xmm2, edx
		paddq xmm1, xmm2

		movd xmm2, ecx
		movq xmm3, xmm4
		psrlq xmm3, xmm2
		vpcmpgtq xmm0, xmm1, xmm3
		psllq xmm0, 8
		ptest xmm0, xmm0
		jnz .overflow
			inc eax
			movq xmm5, xmm1
		.overflow:
	sub ecx, 2
	jns @b
	.return: ret
endp
