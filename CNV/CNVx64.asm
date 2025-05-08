; proc_noprologue

; proc CNV.parseCMD uses rbx, argvLp:qword
; 	; local lpargmem:QWORD
; 	mov [argvLp], rcx
; 	@call [GetCommandLineA]()
; 	mov rdx, rax
; 	xor rcx, rcx
; 	mov ebx, 1
; 	.mainloop:
; 		movzx r8d, byte[rdx+rcx]
; 		cmp r8d, '"'
; 		jne .noquotes
; 			.findquote:
; 				inc rcx
; 				movzx r8d, byte[rdx+rcx]
; 				cmp r8d, 0
; 					je .NoSpace
; 			cmp r8d, '"'
; 			jne .findquote
; 		.noquotes:
; 		cmp r8d, " "
; 		jne .NoSpace
; 			cmp byte[rdx+rcx-1], 0
; 			je .SpaceSeries				;серия пробелов
; 				push rdx
; 				inc ebx
; 			.SpaceSeries:
; 			lea rdx, [rdx+rcx+1]
; 			mov byte [rdx-1], 0
; 			mov rcx, -1
; 		.NoSpace:
; 		inc rcx
; 	cmp r8d, 0
; 	jne .mainloop
; 	push rdx
; 	@call CNV::alloc(addr rbx*8)
; 	mov [argvLp], rax
; 	mov rcx, rbx
; 	@@:
; 		pop qword[rax+(rcx-1)*8]
; 	dec rcx
; 	jnz @b
; 	mov rdx, [argvLp]
; 	mov rcx, rdx
; 	mov [rcx], rdx
; 	mov rax, rbx
; 	ret
; endp

struct BITMAPINFO
	bmiHeader BITMAPINFOHEADER
	bmiColors dq 1 dup(?)
ends

proc CNV.BMPToFile, hBmp, lpFname, bitCount
	mov [hBmp], rcx
	mov [lpFname], rdx
	mov [bitCount], r8

	local <bmInfo:BITMAPINFO <sizeof.BITMAPINFOHEADER, 0, 0, 0, 0, 0>>
	local tmpDC:QWORD, lpBmBits:QWORD
	@call [GetDC](NULL)
	mov [tmpDC], rax
	; mov [bmInfo.bmiHeader.biBitCount], 24
	@call [GetDIBits]([tmpDC], [hBmp], 0, 0, NULL, addr bmInfo, DIB_RGB_COLORS)
	test eax, eax
	jnz @f
		@call [ReleaseDC]([tmpDC])
		mov eax, 0
		ret
	@@:
	@call CNV::alloc([bmInfo.bmiHeader.biSizeImage])
	mov [lpBmBits], rax
	mov [bmInfo.bmiHeader.biCompression], BI_RGB 
	mov rax, [bitCount]
	mov [bmInfo.bmiHeader.biBitCount], ax
	@call [GetDIBits]([tmpDC], [hBmp], 0, [bmInfo.bmiHeader.biHeight], [lpBmBits], addr bmInfo, DIB_RGB_COLORS)
	@call [ReleaseDC](NULL, [tmpDC])

	local <bmfHeader:BITMAPFILEHEADER "BM", ?, 0, 0, sizeof.BITMAPFILEHEADER+sizeof.BITMAPINFO>
	mov eax, [bmInfo.bmiHeader.biSizeImage]
	add eax, [bmfHeader.bfOffBits]
	mov [bmfHeader.bfSize], eax

	local fHandle:QWORD
	mov [bmInfo.bmiHeader.biCompression], BI_RGB 
	mov rax, [bitCount]
	mov [bmInfo.bmiHeader.biBitCount], ax
	@call [CreateFileA]([lpFname], GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)
	mov [fHandle], rax
	@call [WriteFile]([fHandle], addr bmfHeader, sizeof.BITMAPFILEHEADER, NULL, NULL)
	@call [WriteFile]([fHandle], addr bmInfo, sizeof.BITMAPINFO, NULL, NULL)
	@call [WriteFile]([fHandle], [lpBmBits], [bmInfo.bmiHeader.biSizeImage], NULL, NULL)
	@call [CloseHandle]([fHandle])
	@jret CNV::free([lpBmBits])
endp

proc CNV.ui64div c, result, dividend, divisor
	virtObj .result:arg Divq at rcx
	mov [.result.reminder], rdx
	mov [.result.result], r8
	xor rax, rax
	xchg rax, rdx
	div r8
	mov [.result.reminder], rdx
	mov [.result.result], rax
	ret
endp

proc i32ToStr, lpStr, num, radix
	mov rax, rdx
	cdq
	shl rdx, 32
	add rdx, rax
	jmp intToStr
endp

if used CNV.ui64ToStr 
	CNV.ui64ToStr = CNV.uintToStr
end if

if used CNV.i64ToStr
	CNV.i64ToStr = CNV.intToStr
end if

if used CNV.ui64sqrt
	CNV.ui64sqrt = CNV.ui32sqrt
end if

; proc_resprologue
