importlib kernel32,\
	GetCommandLineA,\
	GetProcessHeap,\
	HeapAlloc,\
	HeapReAlloc,\
	HeapFree,\
	SetLastError,\
	CreateFileA,\
	WriteFile,\
	CloseHandle

importlib user32,\
	GetDC,\
	ReleaseDC

importlib gdi32,\
	GetDIBits

define DIB_RGB_COLORS 0

; if used CNV._module
; 	proc_noprologue

; 	CNV._module dq ?

; 	proc CNV._moduleinit, handle, reason, reserved
; 		@call [GetModuleHandleA](0)
; 		mov [CNV._module], rax
; 		ret
; 	endp

; 	TLS_AddCallback CNV._moduleinit

; 	proc_resprologue
; end if

; a = used CNV._heap

if used CNV._heap
	CNV._heap dq ?
end if

macro CNV._initHeap{
	if used CNV._heap
		@call [GetProcessHeap]()
		mov [CNV._heap], rax
	end if
}

TLS_AddMacro CNV._initHeap

macro CNV.alloc size, flags=HEAP_ZERO_MEMORY{
	@call [HeapAlloc]([CNV._heap], flags, size)
}

macro CMV.realloc lpMem, size, flags=HEAP_ZERO_MEMORY{
	@call [HeapReAlloc]([CNV._heap], flags, lpMem, size)
}

macro CNV.free lpMem, flags=0{
	@call [HeapFree]([CNV._heap], flags, lpMem)
}

macro CNV.BMPFromFile path, x=0, y=0{
	@call [LoadImageA](NULL, path, IMAGE_BITMAP, x, y, LR_LOADFROMFILE)
}

FILL_NO_SAVE	= 0
FILL_SAVE_RSI 	= 1
FILL_SAVE_RDI 	= 2

macro CNV.fill dest*, src*, size*, flags=FILL_SAVE_RDI or FILL_SAVE_RSI{
	local repeats, rem, _src, _dest, matched
	if size eqtype 0 & size relativeto 0
		repeats = (size) / 32
		rem = (size) mod 32
	else 
		repeats = 2
	end if
	if repeats > 1
		if defined current@frame
			if current@frame<16
				current@frame = 16
			end if
			if flags and FILL_SAVE_RSI
				mov [rsp], rsi
			end if
			if flags and FILL_SAVE_RDI
				mov [rsp+8], rdi
			end if
		else
			if flags and FILL_SAVE_RSI
				push rsi
			end if
			if flags and FILL_SAVE_RDI
				push rdi
			end if
		end if
		fillParam rcx, size
		fillParam rdi, dest
		fillParam rsi, src
		rep movsb
		if defined current@frame
			if flags and FILL_SAVE_RSI
				mov rsi, [rsp]
			end if
			if flags and FILL_SAVE_RDI
				mov rdi, [rsp+8]
			end if
		else
			if flags and FILL_SAVE_RDI
				pop rdi
			end if
			if flags and FILL_SAVE_RSI
				pop rsi
			end if
		end if
	else 
		inlineObj _dest, dest, rcx
		inlineObj _src, src, rdx
		local _repeats, _rem, _src_, _dest_
		_src_ = 0
		_src equ _src + _src_
		_repeats = repeats
		_rem = rem
		if repeats = 1
			if (_src) relativeto 0 & ((_src) mod 32 = 0)
				vmovdqa ymm0, yword[_src]
			else 
				vmovdqu ymm0, yword[_src]
			end if
			_src_ = _src_ + 32
		end if
		repeats = rem / 16
		rem = rem mod 16
		if repeats = 1
			if (_src) relativeto 0 & ((_src) mod 16 = 0)
				vmovdqa xmm1, xword[_src]
			else 
				vmovdqu xmm1, xword[_src]
			end if
			_src_ = _src_ + 16
		end if
		repeats = rem / 8
		rem = rem mod 8
		if repeats = 1
			movq xmm3, qword[_src]
			_src_ = _src_ + 8
		end if
		repeats = rem / 4
		rem = rem mod 4
		if repeats = 1
			mov eax, dword[_src]
			_src_ = _src_ + 4
		end if
		repeats = rem / 2
		rem = rem mod 2
		if repeats = 1
			movzx ecx, word[_src]
			_src_ = _src_ + 2
		end if
		if rem = 1
			movzx edx, byte[_src]
		end if 
		repeats = _repeats
		rem = _rem
		_dest_ = 0
		_dest equ _dest + _dest_
		if repeats = 1
			if (_dest) relativeto 0 & ((_dest) mod 32 = 0)
				vmovdqa yword[_dest], ymm0
			else 
				vmovdqu yword[_dest], ymm0
			end if
			_dest_ = _dest_+32
		end if
		repeats = rem / 16
		rem = rem mod 16
		if repeats = 1
			if (_dest) relativeto 0 & ((_dest) mod 16 = 0)
				vmovdqa xword[_dest], xmm1
			else 
				vmovdqu xword[_dest], xmm1
			end if
			_dest_ = _dest_+16
		end if
		repeats = rem / 8
		rem = rem mod 8
		if repeats = 1
			movq qword[_dest], xmm3
			_dest_ = _dest_+8
		end if
		repeats = rem / 4
		rem = rem mod 4
		if repeats = 1
			mov dword[_dest], eax
			_dest_ = _dest_+4
		end if
		repeats = rem / 2
		rem = rem mod 2
		if repeats = 1
			mov word[_dest], cx
			_dest_ = _dest_+2
		end if
		if rem = 1
			mov byte[_dest], dl
		end if 
	end if
}

STRLEN_NO_SAVE 		= 0
STRLEN_SAVE_RDI 	= 1
STRLEN_REZULT_RCX 	= 2

macro CNV.strlen src*, flags = STRLEN_SAVE_RDI{
	if flags and STRLEN_SAVE_RDI
		if defined current@frame
			if current@frame<16
				current@frame = 16
			end if
			mov [rsp], rdi
		else
			push rdi rdi
		end if
	end if
	fillParam rdi, src
	xor eax, eax
	mov rcx, -1
	repnz scasb
	if flags and STRLEN_REZULT_RCX
		lea rcx, [rcx+2]
		neg rcx
	else
		lea rax, [rcx+2]
		neg rax
	end if
	if flags and STRLEN_SAVE_RDI
		if defined current@frame
			mov rdi, [rsp]
		else
			pop rdi rdi
		end if
	end if
}

STRMOV_NO_SAVE 	= 0
STRMOV_SAVE_RSI = 1
STRMOV_SAVE_RDI = 2
STRMOV_RET_LEN 	= 4

macro CNV.strmov dest*, src*, flags=STRMOV_SAVE_RSI or STRMOV_SAVE_RDI{
	if defined current@frame
		if current@frame<16
			current@frame = 16
		end if
		if flags and STRMOV_SAVE_RSI
			mov [rsp], rsi
		end if
		if flags and STRMOV_SAVE_RDI
			mov [rsp+8], rdi
		end if
	else
		if (flags and STRMOV_SAVE_RSI) and (flags and STRMOV_SAVE_RDI)
			push rsi rdi
		else if flags and STRMOV_SAVE_RDI
			push rdi rdi
		else if flags and STRMOV_SAVE_RSI
			push rsi rsi
		end if
	end if
	fillParam rdi, dest
	fillParam rsi, src
	if flags and STRMOV_RET_LEN
		mov rax, -1
	end if
	local .lab
	.lab:
		movsb
	if flags and STRMOV_RET_LEN
		inc rax
	end if
	cmp byte[rsi-1], 0
	jne .lab
	if defined current@frame
		if flags and STRMOV_SAVE_RSI
			mov rsi, [rsp]
		end if
		if flags and STRMOV_SAVE_RDI
			mov rdi, [rsp+8]
		end if
	else
		if (flags and STRMOV_SAVE_RSI) and (flags and STRMOV_SAVE_RDI)
			pop rdi rsi
		else if flags and STRMOV_SAVE_RDI
			pop rdi rdi
		else if flags and STRMOV_SAVE_RSI
			pop rsi rsi
		end if
	end if
}

macro CNV.BMP2File hBmp, lpFname, bitCount=24{
	@call CNV.BMPToFile(hBmp, lpFname, bitCount)
}

MEMSET_NO_SAVE 		= 0
MEMSET_SAVE_RDI 	= 1

macro CNV.memset dst*, val*, countval*, sizeval = 1, flags = MEMSET_SAVE_RDI{
	if flags and MEMSET_SAVE_RDI
		if defined current@frame
			if current@frame<16
				current@frame = 16
			end if
			mov [rsp], rdi
		else
			push rdi rdi
		end if
	end if
	fillParam rdi, dst
	mov ecx, countval
	if sizeval = 1
		mov al, val
		rep stosb
	else if sizeval = 2
		mov ax, val
		rep stosw
	else if sizeval = 4
		mov eax, val
		rep stosd
	else if sizeval = 8
		mov rax, val
		rep stosq
	end if
	if flags and MEMSET_SAVE_RDI
		if defined current@frame
			mov rdi, [rsp]
		else
			pop rdi rdi
		end if
	end if
}

proc_noprologue

proc CNV.parseCMD uses rbx, argvLp:qword
	local lpargmem:QWORD
	mov [argvLp], rcx
	@call [GetCommandLineA]()
	mov rdx, rax
	xor rcx, rcx
	mov ebx, 1
	.mainloop:
		movzx r8d, byte[rdx+rcx]
		cmp r8d, '"'
		jne .noquotes
			.findquote:
				inc rcx
				movzx r8d, byte[rdx+rcx]
				cmp r8d, 0
					je .NoSpace
			cmp r8d, '"'
			jne .findquote
		.noquotes:
		cmp r8d, " "
		jne .NoSpace
			cmp byte[rdx+rcx-1], 0
			je .SpaceSeries				;серия пробелов
				push rdx
				inc ebx
			.SpaceSeries:
			lea rdx, [rdx+rcx+1]
			mov byte [rdx-1], 0
			mov rcx, -1
		.NoSpace:
		inc rcx
	cmp r8d, 0
	jne .mainloop
	push rdx
	@call CNV:alloc(addr rbx*8)
	mov [lpargmem], rax
	mov rcx, rbx
	@@:
		pop qword[rax+(rcx-1)*8]
	dec rcx
	jnz @b
	mov rdx, [lpargmem]
	mov rcx, [argvLp]
	mov [rcx], rdx
	mov rax, rbx
	ret
endp

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
	@call CNV:alloc([bmInfo.bmiHeader.biSizeImage])
	mov [lpBmBits], rax
	mov [bmInfo.bmiHeader.biCompression], BI_RGB 
	mov rax, [bitCount]
	mov [bmInfo.bmiHeader.biBitCount], ax
	@call [GetDIBits]([tmpDC], [hBmp], 0, [bmInfo.bmiHeader.biHeight], [lpBmBits], addr bmInfo, DIB_RGB_COLORS)
	@call [ReleaseDC]([tmpDC])

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
	@jret CNV:free([lpBmBits])
endp

proc CNV.intToStr, lpStr, num, radix
	test edx, edx
	jns .positive
		mov byte[rcx], '-'
		neg edx
		inc rcx
	.positive:
	@jret CNV:uintToStr(rcx, rdx, r8)
endp

proc CNV.uintToStr, lpStr, num, radix
	locals
		buf 	db 11 dup ?
	endl
	mov eax, edx
	xor r10, r10
	.loop1:
		xor edx, edx
		div r8d
		add edx, 30h
		cmp edx, 39h
		jle .decDigits
			add edx, 7
		.decDigits:
		mov [buf + r10], dl
		inc r10
	test eax, eax
	jnz .loop1
	xchg rcx, r10
	.loop2:
		mov al, [buf + rcx - 1]
		mov [r10], al
		inc r10
	loop .loop2
	mov byte[r10], 0
	ret
endp

proc_resprologue