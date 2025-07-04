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
; 	@call CNV::alloc(addr ebx*4)
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
	local tmpDC:DWORD, lpBmBits:DWORD
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
	@call CNV::alloc([bmInfo.bmiHeader.biSizeImage])
	mov [lpBmBits], eax
	mov [bmInfo.bmiHeader.biCompression], BI_RGB 
	mov eax, [bitCount]
	mov [bmInfo.bmiHeader.biBitCount], ax
	@call [GetDIBits]([tmpDC], [hBmp], 0, [bmInfo.bmiHeader.biHeight], [lpBmBits], addr bmInfo, DIB_RGB_COLORS)
	@call [ReleaseDC](NULL, [tmpDC])

	local <bmfHeader:BITMAPFILEHEADER "BM", ?, 0, 0, sizeof.BITMAPFILEHEADER+sizeof.BITMAPINFO>
	mov eax, [bmInfo.bmiHeader.biSizeImage]
	add eax, [bmfHeader.bfOffBits]
	mov [bmfHeader.bfSize], eax

	local fHandle:DWORD
	mov [bmInfo.bmiHeader.biCompression], BI_RGB 
	mov eax, [bitCount]
	mov [bmInfo.bmiHeader.biBitCount], ax
	; int3
	; @call c [puts]([lpFname])
	@call [CreateFileA]([lpFname], GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)
	mov [fHandle], eax
	@call [WriteFile]([fHandle], addr bmfHeader, sizeof.BITMAPFILEHEADER, NULL, NULL)
	@call [WriteFile]([fHandle], addr bmInfo, sizeof.BITMAPINFO, NULL, NULL)
	@call [WriteFile]([fHandle], [lpBmBits], [bmInfo.bmiHeader.biSizeImage], NULL, NULL)
	@call [CloseHandle]([fHandle])
	@call CNV::free([lpBmBits])
	ret
endp

proc CNV.ui64div c uses pbx, result:POINTER, dividend:QWORD, divisor:QWORD
	virtObj .result:arg Divq at pbx from @arg1
	; int3
	@call CNV::fill(addr .result, addr dividend, 16)
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
	; movd xmm3, ecx
	; movq xmm2, [high_one]
	; psllq xmm2, xmm3

	sub ecx, edx
	movd xmm2, ecx
	psllq xmm0, xmm2
	movq xmm3, [.one]
	xor edx, edx
	inc ecx
	@@:
		shl edx, 1
		movq xmm2, xmm0
		pcmpgtq xmm2, xmm1
		ptest xmm2, xmm3
		jnz .no_subtract
			psubq xmm1, xmm0
			add edx, 1
		.no_subtract:
		psrlq xmm0, 1
		ptest xmm1, xmm1
			jz .zero_dividend 
	loop @b
	.zero_dividend:
	lea eax, [ecx-1]
	test ecx, ecx
	cmovnz ecx, eax
	shl edx, cl
	movq qword[.result.reminder], xmm1
	mov dword[.result.result], edx
	mov dword[.result.result + 4], 0
	; lea eax, [.result]
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
		; lea eax, [.result]
		ret

	.lazy_ret:
		movq [.result.reminder], xmm1
		pxor xmm0, xmm0
		movq [.result.result], xmm0
		; lea eax, [.result]
		ret

	; @const_align equ 16
	.one @const dq 1
	; restore @const_align
endp
	
proc CNV.ui64sqrt c, num:QWORD
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


proc CNV.ui64ToStr c uses pbx psi pdi pbp, lpStr, num:QWORD, radix
	locals
		buf 	db 70 dup ?
	endl
	cmp dword[num + 4], 0
		je .only_low
	mov ebx, [radix]
	; @sarg @arg1, @arg3
	; @larg pax, @arg3
	mov pcx, pbx
	dec pcx
	and pcx, pbx
		jz .power_of_two
	mov edi, dword[num + 4]
	mov ebp, dword[num]
	xor psi, psi
	.loop1:
		xor edx, edx
		mov eax, edi
		div ebx
		mov edi, eax
		mov eax, ebp
		div ebx
		mov ebp, eax

		add edx, 30h
		cmp edx, 39h
		jle .decDigits
			add edx, 7
		.decDigits:
		mov [buf + psi], dl
		inc psi
	test eax, eax
	jnz .loop1

	mov pcx, psi
	.migration_loop:
		mov pdx, [lpStr]
		.loop2:
			mov al, [buf + pcx - 1]
			mov [pdx], al
			inc pdx
		loop .loop2
		mov byte[pdx], 0
		mov pax, psi
		ret

	.power_of_two:
		; @larg pdx, @arg1
		mov eax, dword[num]
		mov edx, dword[num + 4]
		xor psi, psi
		bsr pcx, pbx
		.loop3:
			xor ebx, ebx
			shrd pbx, pax, cl
			shrd pax, pdx, cl
			shr pdx, cl
			rol pbx, cl
			add pbx, 30h
			cmp pbx, 39h
			jle .decDigits_binary
				add ebx, 7
			.decDigits_binary:
			mov [buf + psi], bl
			inc psi
		test eax, eax
		jnz .loop3
		mov pcx, psi
		; mov psi, pdx
		jmp .migration_loop

	.only_low:
		@call CNV::uintToStr([lpStr], dword[num], [radix])
		ret
endp

proc CNV.i64ToStr c, lpStr, num:QWORD, radix
	locals
		sign dd 0
	endl
	cmp dword[num+4], 0
	jns .positive
		mov [sign], 1
		mov ecx, [lpStr]
		mov byte[pcx], '-'
		neg dword[num]
		adc dword[num + 4], 0
		neg dword[num + 4]
		inc [lpStr]
	.positive:
	@call c CNV.ui64ToStr(@arg1, @arg2, @arg3)
	lea ecx, [eax + 1]
	cmp [sign], 0
		cmovne eax, ecx
	ret
endp

proc CNV.ui64mul c, a:QWORD, b:QWORD
	locals 
		buf rq 4
		dest dq ?, ?
	endl
	movq xmm1, [b]
	movq xmm0, [a]

	pshufd xmm0, xmm0, 01000100b
	vpmovzxdq ymm0, xmm0
	pshufd xmm1, xmm1, 01010000b
	vpmovzxdq ymm1, xmm1
	vpmuludq ymm0, ymm0, ymm1
	vmovups yword[buf], ymm0
	
	movq [dest], xmm0
	
	mov eax, dword[buf + 8]
	mov edx, dword[buf + 12]
	add dword[dest + 4], eax
	adc dword[dest + 8], edx
	adc dword[dest + 12], 0

	mov eax, dword[buf + 16]
	mov edx, dword[buf + 20]
	add dword[dest + 4], eax
	adc dword[dest + 8], edx
	adc dword[dest + 12], 0
	
	vextractf128 xmm0, ymm0, 1
	psrldq xmm0, 8
	pslldq xmm0, 8
	movups xmm1, xword[dest]
	paddq xmm0, xmm1
	movd eax, xmm0
	psrldq xmm0, 4
	movd edx, xmm0
	ret
endp

proc CNV.ui64pow c uses pbx, num:QWORD, exp:DWORD
	; @sarg @arg1, @arg2
	local result:dq 1, xmm0_save:QWORD

	mov ebx, [exp]
	movq xmm0, [num]
	.pow_loop:
		test ebx, 1
		jz .no_mul
			movq [xmm0_save], xmm0
			@call [result] = CNV::ui64mul([result], qword xmm0)
			movq xmm0, [xmm0_save]
		.no_mul:
		@call qword xmm0 = CNV::ui64mul(qword xmm0, qword xmm0)
		shr ebx, 1
	test ebx, ebx
	jnz .pow_loop
	mov eax, dword[result]
	mov edx, dword[result + 4]
	ret
endp

if used CMV.i32ToStr
	CMV.i32ToStr = CNV.intToStr
end if
