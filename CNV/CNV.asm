importlib kernel32,\
	GetCommandLineA,\
	GetProcessHeap,\
	HeapAlloc,\
	HeapReAlloc,\
	HeapFree,\
	SetLastError,\
	CreateFileA,\
	WriteFile,\
	CloseHandle,\
	SetConsoleCP,\
	SetConsoleOutputCP

importlib user32,\
	GetDC,\
	ReleaseDC

importlib gdi32,\
	GetDIBits

importlib msvcrt,\
	setlocale

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
	CNV._heap dptr ?
end if

if used CNV.argc
	CNV.argc dd ?
end if

if used CNV.argv
	CNV.argv dptr ?
end if

macro CNV._initHeap{
	if used CNV._heap
		$call [GetProcessHeap]()
		mov [CNV._heap], pax
	end if
	if used CNV.argc | used CNV.argv
		$call CNV|parseCMD(CNV.argv)
		mov [CNV.argc], eax
	end if
}

TLS_AddMacro CNV._initHeap

macro CNV.alloc size, flags=HEAP_ZERO_MEMORY{
	$call [HeapAlloc]([CNV._heap], flags, size)
}

macro CNV.realloc lpMem, size, flags=HEAP_ZERO_MEMORY{
	$call [HeapReAlloc]([CNV._heap], flags, lpMem, size)
}

macro CNV.free lpMem, flags=0{
	$call [HeapFree]([CNV._heap], flags, lpMem)
}

macro CNV.BMPFromFile path, x=0, y=0{
	$call [LoadImageA](NULL, path, IMAGE_BITMAP, x, y, LR_LOADFROMFILE)
}

FILL_NO_SAVE	= 0
FILL_SAVE_SI 	= 1
FILL_SAVE_DI 	= 2

; macro CNV.fill dest*, src*, size*, flags=FILL_SAVE_DI or FILL_SAVE_SI{
; 	local repeats, rem, _src, _dest, matched
; 	if size eqtype 0 & size relativeto 0
; 		repeats = (size) / 32
; 		rem = (size) mod 32
; 	else 
; 		repeats = 2
; 	end if
; 	if repeats > 1
; 		if defined current@frame
; 			if current@frame < pointer.size * 2
; 				current@frame = pointer.size * 2
; 			end if
; 			if flags and FILL_SAVE_SI
; 				mov [psp], psi
; 			end if
; 			if flags and FILL_SAVE_DI
; 				mov [psp + pointer.size], pdi
; 			end if
; 		else
; 			if flags and FILL_SAVE_SI
; 				push psi
; 			end if
; 			if flags and FILL_SAVE_DI
; 				push pdi
; 			end if
; 		end if
; 		fillParam pcx, size
; 		fillParam pdi, dest
; 		fillParam psi, src
; 		rep movsb
; 		if defined current@frame
; 			if flags and FILL_SAVE_SI
; 				mov psi, [psp]
; 			end if
; 			if flags and FILL_SAVE_DI
; 				mov pdi, [psp + pointer.size]
; 			end if
; 		else
; 			if flags and FILL_SAVE_DI
; 				pop pdi
; 			end if
; 			if flags and FILL_SAVE_SI
; 				pop psi
; 			end if
; 		end if
; 	else 
; 		inlineObj _dest, dest, pcx
; 		inlineObj _src, src, pdx
; 		local _repeats, _rem, _src_idx, _dest_idx
; 		_src_idx = 0
; 		_src equ _src + _src_idx
; 		_repeats = repeats
; 		_rem = rem
; 		if repeats = 1
; 			if (_src) relativeto 0 & ((_src) mod 32 = 0)
; 				vmovdqa ymm0, yword[_src]
; 			else 
; 				vmovdqu ymm0, yword[_src]
; 			end if
; 			_src_idx = _src_idx + 32
; 		end if
; 		repeats = rem / 16
; 		rem = rem mod 16
; 		if repeats = 1
; 			if (_src) relativeto 0 & ((_src) mod 16 = 0)
; 				vmovdqa xmm1, xword[_src]
; 			else 
; 				vmovdqu xmm1, xword[_src]
; 			end if
; 			_src_idx = _src_idx + 16
; 		end if
; 		repeats = rem / 8
; 		rem = rem mod 8
; 		if repeats = 1
; 			movq xmm3, qword[_src]
; 			_src_idx = _src_idx + 8
; 		end if
; 		repeats = rem / 4
; 		rem = rem mod 4
; 		if repeats = 1
; 			mov eax, dword[_src]
; 			_src_idx = _src_idx + 4
; 		end if
; 		repeats = rem / 2
; 		rem = rem mod 2
; 		if repeats = 1
; 			movzx ecx, word[_src]
; 			_src_idx = _src_idx + 2
; 		end if
; 		if rem = 1
; 			movzx edx, byte[_src]
; 		end if 
; 		repeats = _repeats
; 		rem = _rem
; 		_dest_idx = 0
; 		_dest equ _dest + _dest_idx
; 		if repeats = 1
; 			if (_dest) relativeto 0 & ((_dest) mod 32 = 0)
; 				vmovdqa yword[_dest], ymm0
; 			else 
; 				vmovdqu yword[_dest], ymm0
; 			end if
; 			_dest_idx = _dest_idx + 32
; 		end if
; 		repeats = rem / 16
; 		rem = rem mod 16
; 		if repeats = 1
; 			if (_dest) relativeto 0 & ((_dest) mod 16 = 0)
; 				vmovdqa xword[_dest], xmm1
; 			else 
; 				vmovdqu xword[_dest], xmm1
; 			end if
; 			_dest_idx = _dest_idx+16
; 		end if
; 		repeats = rem / 8
; 		rem = rem mod 8
; 		if repeats = 1
; 			movq qword[_dest], xmm3
; 			_dest_idx = _dest_idx+8
; 		end if
; 		repeats = rem / 4
; 		rem = rem mod 4
; 		if repeats = 1
; 			mov dword[_dest], eax
; 			_dest_idx = _dest_idx+4
; 		end if
; 		repeats = rem / 2
; 		rem = rem mod 2
; 		if repeats = 1
; 			mov word[_dest], cx
; 			_dest_idx = _dest_idx+2
; 		end if
; 		if rem = 1
; 			mov byte[_dest], dl
; 		end if 
; 	end if
; }


FILL_FORCEALIGN_SRC		= 4
FILL_FORCEALIGN_DST		= 8
FILL_FORCEALIGN_BOTH	= FILL_FORCEALIGN_SRC or FILL_FORCEALIGN_DST

.proc cdecl CNV.__fill(.dst, .src, .size) uses pdi psi 
	@larg pdi, @arg1, psi, @arg2, pcx, @arg3
	rep movsb
	ret
.endp

macro CNV.fill dest*, src*, size*, flags=0{
	local repeats, rem, _src, _dest, matched, ..src, ..dest, _src_base, ..test_base
	if size eqtype 0 & size relativeto 0
		repeats = (size) / 16
		rem = (size) mod 16
	else
		repeats = 1000
	end if
	if repeats <= 128
		local _repeats, _rem, _src_idx, _dest_idx, src_aligned, dest_aligned
		src_aligned 	= 0
		dest_aligned 	= 0
		inlineObj _src_base, src, pdx
		inlineObj _dest_base, dest, pcx
		_src_idx = 0
		virtual at _src_base
			..test_base = $
		end virtual
		if ~(..test_base relativeto 0 | ..test_base relativeto psp | ..test_base relativeto pbp)
			@loadGPR pdx, src
			virtual at pdx
				..src rptr 1
			end virtual
		else
			virtual at _src_base
				..src rptr 1
			end virtual
			if ((..dest) relativeto 0 & ((..dest) mod 16 = 0)) | flags and FILL_FORCEALIGN_SRC = FILL_FORCEALIGN_SRC
				src_aligned = 1
			end if
		end if
		virtual at _dest_base
			..test_base = $
		end virtual
		if ~(..test_base relativeto 0 | ..test_base relativeto psp | ..test_base relativeto pbp)
			@loadGPR pcx, dest
			virtual at pcx
				..dest rptr 1
			end virtual
		else
			virtual at _dest_base
				..dest rptr 1
			end virtual
			if ((..dest) relativeto 0 & ((..dest) mod 16 = 0)) | flags and FILL_FORCEALIGN_DST = FILL_FORCEALIGN_DST
				dest_aligned = 1
			end if
		end if
		_src equ (..src + _src_idx)
		_dest_idx = 0
		_dest equ (..dest + _dest_idx)
		local ..loop
		if repeats / 6
			if repeats / 6 > 1
				mov eax, (repeats / 6) * 96
				..loop:
					rept 6 cntr:0\{
						if src_aligned
							movaps xmm\#cntr, xword[_src + (pax - 96) + cntr * 16]
						else 
							movups xmm\#cntr, xword[_src + (pax - 96) + cntr * 16]
						end if
						; _src_idx = _src_idx + 16
					\}
					rept 6 cntr:0\{
						if dest_aligned
							movaps xword[_dest + (pax - 96) + cntr * 16], xmm\#cntr
						else 
							movups xword[_dest + (pax - 96) + cntr * 16], xmm\#cntr
						end if
						; _dest_idx = _dest_idx + 16
					\}
				sub eax, 96
				jnz ..loop
			else
				rept 6 cntr:0\{
					if src_aligned
						movaps xmm\#cntr, xword[_src + cntr * 16]
					else 
						movups xmm\#cntr, xword[_src + cntr * 16]
					end if
					; _src_idx = _src_idx + 16
				\}
				rept 6 cntr:0\{
					if dest_aligned
						movaps xword[_dest + cntr * 16], xmm\#cntr
					else 
						movups xword[_dest + cntr * 16], xmm\#cntr
					end if
					; _dest_idx = _dest_idx + 16
				\}
			end if
			_dest_idx = _dest_idx + (repeats / 6) * 96
			_src_idx = _src_idx + (repeats / 6) * 96
			display (repeats / 6)
		end if
		; end repeat
		repeats = repeats mod 6
		if repeats
			repeat 1
				rept 6 cntr:0\{
					if cntr = repeats
						break
					end if
					if dest_aligned
						movaps xmm\#cntr, xword[_src]
					else 
						movups xmm\#cntr, xword[_src]
					end if
					_src_idx = _src_idx + 16
				\}
			end repeat
			repeat 1
				rept 6 cntr:0\{
					if cntr = repeats
						break
					end if
					if dest_aligned
						movaps xword[_dest], xmm\#cntr
					else 
						movups xword[_dest], xmm\#cntr
					end if
					_dest_idx = _dest_idx + 16
				\}
			end repeat
		end if
		_rem = rem
		repeats = rem / 8
		rem = rem mod 8
		if repeats = 1
			movq xmm0, qword[_src]
			_src_idx = _src_idx + 8
			movq qword[_dest], xmm0
			_dest_idx = _dest_idx + 8
		end if
		repeats = rem / 4
		rem = rem mod 4
		if repeats = 1
			mov eax, dword[_src]
			_src_idx = _src_idx + 4
			mov dword[_dest], eax
			_dest_idx = _dest_idx + 4
		end if
		repeats = rem / 2
		rem = rem mod 2
		if repeats = 1
			movzx eax, word[_src]
			_src_idx = _src_idx + 2
			mov word[_dest], ax
			_dest_idx = _dest_idx + 2
		end if
		if rem = 1
			movzx eax, byte[_src]
			mov byte[_dest], al
		end if
	else
		$call c CNV.__fill(dest, src, size)
	end if 
}

STRLEN_NO_SAVE 		= 0
STRLEN_SAVE_DI 		= 1
STRLEN_REZULT_CX	= 2

macro CNV.strlen src*, flags = STRLEN_SAVE_DI{
	if flags and STRLEN_SAVE_DI
		if defined current@frame
			if current@frame < pointer.size * 2
				current@frame = pointer.size * 2
			end if
			mov [psp - pointer.size], pdi
		else
			push pdi pdi
		end if
	end if
	fillParam pdi, src
	xor eax, eax
	mov pcx, -1
	repnz scasb
	if flags and STRLEN_REZULT_CX
		lea pcx, [pcx+2]
		neg pcx
	else
		lea pax, [pcx+2]
		neg pax
	end if
	if flags and STRLEN_SAVE_DI
		if defined current@frame
			mov pdi, [psp - pointer.size]
		else
			pop pdi pdi
		end if
	end if
}

macro CNV.wstrlen src*, flags = STRLEN_SAVE_DI{
	if flags and STRLEN_SAVE_DI
		if defined current@frame
			if current@frame < pointer.size * 2
				current@frame = pointer.size * 2
			end if
			mov [psp - pointer.size], pdi
		else
			push pdi pdi
		end if
	end if
	fillParam pdi, src
	xor eax, eax
	mov pcx, -1
	repnz scasw
	if flags and STRLEN_REZULT_CX
		lea pcx, [pcx+2]
		neg pcx
	else
		lea pax, [pcx+2]
		neg pax
	end if
	if flags and STRLEN_SAVE_DI
		if defined current@frame
			mov pdi, [psp - pointer.size]
		else
			pop pdi pdi
		end if
	end if
}

STRMOV_NO_SAVE 	= 0
STRMOV_SAVE_SI 	= 1
STRMOV_SAVE_DI 	= 2
STRMOV_RET_LEN 	= 4

macro CNV.strmov dest*, src*, flags=STRMOV_SAVE_SI or STRMOV_SAVE_DI{
	if defined current@frame
		if current@frame < pointer.size * 2
			current@frame = pointer.size * 2
		end if
		if flags and STRMOV_SAVE_SI
			mov [psp], psi
		end if
		if flags and STRMOV_SAVE_DI
			mov [psp + pointer.size], pdi
		end if
	else
		if (flags and STRMOV_SAVE_SI) and (flags and STRMOV_SAVE_DI)
			push psi pdi
		else if flags and STRMOV_SAVE_DI
			push pdi pdi
		else if flags and STRMOV_SAVE_SI
			push psi psi
		end if
	end if
	fillParam pdi, dest
	fillParam psi, src
	if flags and STRMOV_RET_LEN
		mov pax, -1
	end if
	local ..lab
	..lab:
		movsb
	if flags and STRMOV_RET_LEN
		inc pax
	end if
	cmp byte[psi-1], 0
	jne ..lab
	if defined current@frame
		if flags and STRMOV_SAVE_SI
			mov psi, [psp]
		end if
		if flags and STRMOV_SAVE_DI
			mov pdi, [psp + pointer.size]
		end if
	else
		if (flags and STRMOV_SAVE_SI) and (flags and STRMOV_SAVE_DI)
			pop pdi psi
		else if flags and STRMOV_SAVE_DI
			pop pdi pdi
		else if flags and STRMOV_SAVE_SI
			pop psi psi
		end if
	end if
}

macro CNV.wstrmov dest*, src*, flags=STRMOV_SAVE_SI or STRMOV_SAVE_DI{
	if defined current@frame
		if current@frame < pointer.size * 2
			current@frame = pointer.size * 2
		end if
		if flags and STRMOV_SAVE_SI
			mov [psp], psi
		end if
		if flags and STRMOV_SAVE_DI
			mov [psp + pointer.size], pdi
		end if
	else
		if (flags and STRMOV_SAVE_SI) and (flags and STRMOV_SAVE_DI)
			push psi pdi
		else if flags and STRMOV_SAVE_DI
			push pdi pdi
		else if flags and STRMOV_SAVE_SI
			push psi psi
		end if
	end if
	fillParam pdi, dest
	fillParam psi, src
	if flags and STRMOV_RET_LEN
		mov pax, -1
	end if
	local ..lab
	..lab:
		movsw
	if flags and STRMOV_RET_LEN
		inc pax
	end if
	cmp word[psi-2], 0
	jne ..lab
	if defined current@frame
		if flags and STRMOV_SAVE_SI
			mov psi, [psp]
		end if
		if flags and STRMOV_SAVE_DI
			mov pdi, [psp + pointer.size]
		end if
	else
		if (flags and STRMOV_SAVE_SI) and (flags and STRMOV_SAVE_DI)
			pop pdi psi
		else if flags and STRMOV_SAVE_DI
			pop pdi pdi
		else if flags and STRMOV_SAVE_SI
			pop psi psi
		end if
	end if
}

macro CNV.BMP2File hBmp, lpFname, bitCount=24{
	$call CNV|BMPToFile(hBmp, lpFname, bitCount)
}

MEMSET_NO_SAVE 		= 0
MEMSET_SAVE_RDI 	= 1

macro CNV.memset dst*, val*, countval*, sizeval = 1, flags = MEMSET_SAVE_RDI{
	if flags and MEMSET_SAVE_RDI
		if defined current@frame
			if current@frame < pointer.size * 2
				current@frame = pointer.size * 2
			end if
			mov [psp], pdi
		else
			push pdi pdi
		end if
	end if
	fillParam pdi, dst
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
			mov pdi, [psp]
		else
			pop pdi pdi
		end if
	end if
}

macro CNV.consoleToWin1251 {
	$call [SetConsoleCP](1251)
	$call [SetConsoleOutputCP](1251)
}
macro CNV.consoleToUtf8 {
	$call [SetConsoleCP](65001)
	$call [SetConsoleOutputCP](65001)
	$call c [setlocale](2, ".utf8")
}

macro CNV.consoleToUtf16 {
	$call [SetConsoleCP](65001)
	$call [SetConsoleOutputCP](65001)
	$call c [fileno]([stdout])
	$call c [setmode](pax, 0x00040000)
	$call c [fileno]([stdin])
	$call c [setmode](pax, 0x00040000)
	$call c [fileno]([stderr])
	$call c [setmode](pax, 0x00040000)
	$call c [setlocale](2, ".utf8")
}

.proc_frame_mode_static


struct BITMAPINFO
	bmiHeader BITMAPINFOHEADER
	bmiColors rptr 1
ends

.proc cdecl CNV.BMPToFile(.hBmp, .p_fname, .bitCount:DWORD)
	@sarg @arg1, @arg2, @arg3
	.local .bmInfo:BITMAPINFO
	$call CNV|fill(&.bmInfo, <const BITMAPINFO <sizeof.BITMAPINFOHEADER, 0, 0, 0, 0, 0>>, sizeof.BITMAPINFO)
	.local .tmpDC:DWORD, .p_bmBits:DWORD
	$call [GetDC](NULL)
	mov [.tmpDC], eax
	; mov [.bmInfo.bmiHeader.biBitCount], 24
	$call [GetDIBits]([.tmpDC], [.hBmp], 0, 0, NULL, addr .bmInfo, DIB_RGB_COLORS)
	test eax, eax
	jnz @f
		$call [ReleaseDC]([.tmpDC])
		mov eax, 0
		ret
	@@:
	$call CNV|alloc([.bmInfo.bmiHeader.biSizeImage])
	mov [.p_bmBits], eax
	mov [.bmInfo.bmiHeader.biCompression], BI_RGB 
	mov eax, [.bitCount]
	mov [.bmInfo.bmiHeader.biBitCount], ax
	$call [GetDIBits]([.tmpDC], [.hBmp], 0, [.bmInfo.bmiHeader.biHeight], [.p_bmBits], addr .bmInfo, DIB_RGB_COLORS)
	$call [ReleaseDC](NULL, [.tmpDC])

	.local .bmfHeader:BITMAPFILEHEADER
	$call CNV|fill(&.bmfHeader, <const BITMAPFILEHEADER "BM", ?, 0, 0, sizeof.BITMAPFILEHEADER + sizeof.BITMAPINFO>, sizeof.BITMAPFILEHEADER)
	mov eax, [.bmInfo.bmiHeader.biSizeImage]
	add eax, [.bmfHeader.bfOffBits]
	mov [.bmfHeader.bfSize], eax

	.local .fHandle:DWORD
	mov [.bmInfo.bmiHeader.biCompression], BI_RGB 
	mov eax, [.bitCount]
	mov [.bmInfo.bmiHeader.biBitCount], ax
	; int3
	; $call c [puts]([.p_fname])
	$call [CreateFileA]([.p_fname], GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)
	mov [.fHandle], eax
	$call [WriteFile]([.fHandle], addr .bmfHeader, sizeof.BITMAPFILEHEADER, NULL, NULL)
	$call [WriteFile]([.fHandle], addr .bmInfo, sizeof.BITMAPINFO, NULL, NULL)
	$call [WriteFile]([.fHandle], [.p_bmBits], [.bmInfo.bmiHeader.biSizeImage], NULL, NULL)
	$call [CloseHandle]([.fHandle])
	$call CNV|free([.p_bmBits])
	ret
.endp

@arch_include "CNV"

macro CNV.i64ToStr lpBuf, num, radix{
	$call CNV|i64ToStrVarchar(lpBuf, num, radix, 1)
}

macro CNV.i64ToWStr lpBuf, num, radix{
	$call CNV|i64ToStrVarchar(lpBuf, num, radix, 2)
}

macro CNV.ui64ToStr lpBuf, num, radix{
	$call CNV|ui64ToStrVarchar(lpBuf, num, radix, 1)
}

macro CNV.ui64ToWStr lpBuf, num, radix{
	$call CNV|ui64ToStrVarchar(lpBuf, num, radix, 2)
}

; next functions both returns count of chars 
.proc cdecl CNV.uintToStrVarchar(.p_str, .num, .radix, .charSize) uses pbx psi
	.locals
		.buf 	rw 128
	.endl
	cmp @arg2, 0
		je .zeroret
	@sarg @arg1, @arg4
	@larg pax, @arg3
	mov pbx, pax
	dec pax
	and pax, pbx
	@larg pax, @arg2
		jz .power_of_two
	xor ecx, ecx
	.loop1:
		xor edx, edx
		div pbx
		add edx, 30h
		cmp edx, 39h
		jle .decDigits
			add edx, 7
		.decDigits:
		mov [.buf + pcx], dx
		add pcx, [.charSize]
	test eax, eax
	jnz .loop1
	mov psi, pcx

	.migration_loop:
		mov pdx, [.p_str]
		mov ebx, dword[.charSize]
		.loop2:
			sub ecx, dword[.charSize]
			mov ax, [.buf + pcx]
			mov [pdx], ax
			lea pdx, [pdx + pbx]
		jnz .loop2
		mov eax, dword[.charSize]
		.loop3:
			dec eax
			mov byte[pdx + pax], 0
		jnz .loop3
		mov pax, psi
		bsr ecx, dword[.charSize]
		shr pax, cl
		ret

	.power_of_two:
		; @larg pdx, @arg1
		mov pdx, 0
		bsr pcx, pbx
		.loop4:
			xor ebx, ebx
			shrd pbx, pax, cl
			shr pax, cl
			rol pbx, cl
			add pbx, 30h
			cmp pbx, 39h
			jle .decDigits_binary
				add ebx, 7
			.decDigits_binary:
			mov [.buf + pdx], bx
			add pdx, [.charSize]
		test eax, eax
		jnz .loop4
		mov pcx, pdx
		mov psi, pdx
		jmp .migration_loop

	.zeroret:
		@larg pcx, @arg1
		mov dword[pcx], "0"
		mov pax, 1
		ret
.endp

.proc cdecl CNV.intToStrVarchar(.p_str, .num, .radix, .charSize)
	@sarg @arg2
	.local .sign:DWORD
	mov [.sign], 0
	cmp @arg2, 0
	jns .positive
		@larg pcx, @arg1, pax, @arg4
		mov [.sign], 1
		mov word[pcx], '-'
		neg @arg2
		add @arg1, pax
	.positive:
	$call c CNV.uintToStrVarchar(@arg1, @arg2, @arg3, @arg4)
	lea edx, [eax + 1]
	cmp [.sign], 0
		cmovne eax, edx
	ret
.endp

macro CNV.intToStr lpBuf, num, radix{
	$call CNV|intToStrVarchar(lpBuf, num, radix, 1)
}

macro CNV.intToWStr lpBuf, num, radix{
	$call CNV|intToStrVarchar(lpBuf, num, radix, 2)
}

macro CNV.uintToStr lpBuf, num, radix{
	$call CNV|uintToStrVarchar(lpBuf, num, radix, 1)
}

macro CNV.uintToWStr lpBuf, num, radix{
	$call CNV|uintToStrVarchar(lpBuf, num, radix, 2)
}

; len CAN`T be -1
.proc cdecl CNV.strVarcharToI64(.p_str, .len:DWORD, .radix:DWORD, .charSize) uses pbx psi pdi pbp
    @sarg @arg1, @arg2, @arg3, @arg4
    @larg pax, @arg3

    .local .maxDigit:DWORD, .isNeg:DWORD
    lea edx, [eax + "0"]
    lea ecx, [eax + "A" - 10]
    cmp edx, "9" + 1
        cmova edx, ecx
    mov [.maxDigit], edx

    mov pbp, .mul
    mov edx, eax
    dec edx
    test eax, edx
    jnz .no_power_two
        mov pbp, .shift
        bsr eax, [.radix]
    .no_power_two:
    mov [.radix], eax

    mov eax, [.len]
    mov ecx, dword[.charSize]
    bsf ecx, ecx
    shl eax, cl
    mov pdi, [.p_str]
    add pdi, pax

    mov pbx, [.p_str]
    mov ecx, [.radix]
    mov edx, dword[.charSize]
    xor esi, esi
    cmp byte[pbx], "-"
        cmove esi, edx
    mov [.isNeg], esi
    add pbx, psi
    
    xor eax, eax
    xor edx, edx

    movzx edi, byte[pbx]
    switch edi
        case u "9" ... "A"
        case_default
        	jmp end_case
        case u +"a" ... +"f"
            and edi, 0DFh
            jmp start_case
        case u +"0" ... [.maxDigit]
            jmp pbp
            .after_digit:
            lea esi, [edi - "A" + 10]
            sub edi, "0"
            cmp edi, 9
                cmova edi, esi
            add eax, edi
            adc edx, 0
            add pbx, [.charSize]
            cmp pbx, pdi
            	je end_case
            movzx edi, byte[pbx]
            jmp start_case
    end_switch
    ; mov pcx, pax
    ; neg pcx
    cmp [.isNeg], 0
        je .return
    match =x64, __architecture{
        neg rax                    
    }
    match =x86, __architecture{
        movd xmm0, eax
        movd xmm1, edx
        psllq xmm1, 32
        paddq xmm0, xmm1
        pcmpeqq xmm1, xmm1      ; xmm1 = -1
        pxor xmm0, xmm1
        psubq xmm0, xmm1
        movd eax, xmm0
        psrlq xmm0, 32
        movd edx, xmm0
    }
    .return: ret

    .mul:
        xchg ecx, edx
        $call edx:eax = CNV|ui64mul(ecx:eax, 0:edx)
        mov ecx, dword[.radix]
        jmp .after_digit

    .shift:
        shld edx, eax, cl
        shl eax, cl
        jmp .after_digit
.endp

; len can be -1
.proc cdecl CNV.strToI64(.p_str, .len, .radix)
	@sarg @arg1, @arg2, @arg3
	cmp @arg2, -1
	jne @f
		$call [.len] = CNV|strlen(@arg1)
	@@:
	$call CNV|strVarcharToI64([.p_str], [.len], [.radix], 1)
	ret
.endp

; len can be -1
.proc cdecl CNV.wstrToI64(.p_str, .len, .radix)
	@sarg @arg1, @arg2, @arg3
	cmp @arg2, -1
	jne @f
		$call [.len] = CNV|wstrlen(@arg1)
	@@:
	$call CNV|strVarcharToI64([.p_str], [.len], [.radix], 2)
	ret
.endp

; len CAN`T be -1 
.proc cdecl CNV.strVarcharToI32(.p_str, .len:DWORD, .radix, .charSize) uses pbx psi pdi pbp
    @sarg @arg1, @arg2, @arg3, @arg4
    @larg pax, @arg3

    .local .maxDigit:DWORD, .isNeg:DWORD
    lea edx, [eax + "0"]
    lea ecx, [eax + "A" - 10]
    cmp edx, "9" + 1
        cmova edx, ecx
    mov [.maxDigit], edx

    mov pbp, .mul
    mov edx, eax
    dec edx
    test eax, edx
    jnz .no_power_two
        mov pbp, .shift
        bsr eax, [.radix]
    .no_power_two:
    mov [.radix], eax

    mov eax, [.len]
    mov ecx, dword[.charSize]
    bsf ecx, ecx
    shl eax, cl
    mov pdi, [.p_str]
    add pdi, pax

    mov pbx, [.p_str]
    mov ecx, [.radix]
    mov edx, dword[.charSize]
    xor esi, esi
    cmp byte[pbx], "-"
        cmove esi, edx
    mov [.isNeg], esi
    add pbx, psi
    
    xor eax, eax
    xor edx, edx

    movzx edi, byte[pbx]
    switch edi
        case u "9" ... "A"
        case_default
        	jmp end_case
        case u +"a" ... +"f"
            and edi, 0DFh
            jmp start_case
        case u +"0" ... [.maxDigit]
            jmp pbp
            .after_digit:
            lea esi, [edi - "A" + 10]
            sub edi, "0"
            cmp edi, 9
                cmova edi, esi
            add eax, edi
            add pbx, [.charSize]
            cmp pbx, pdi
            	je end_case
            movzx edi, byte[pbx]
            jmp start_case
    end_switch
    ; mov pcx, pax
    ; neg pcx
    cmp [.isNeg], 0
        je .return
    neg eax
    .return: ret

    .mul:
    	mul ecx
        jmp .after_digit

    .shift:
        shl eax, cl
        jmp .after_digit
.endp

.proc cdecl CNV.strToI32(.p_str, .len, .radix) uses pbx psi pdi
	@sarg @arg1, @arg2, @arg3
	cmp @arg2, -1
	jne @f
		$call [.len] = CNV|strlen(@arg1)
	@@:
	$call CNV|strVarcharToI32([.p_str], [.len], [.radix], 1)
	ret
.endp

.proc cdecl CNV.wstrToI32(.p_str, .len, .radix) uses pbx psi pdi
	@sarg @arg1, @arg2, @arg3
	cmp @arg2, -1
	jne @f
		$call [.len] = CNV|wstrlen(@arg1)
	@@:
	$call CNV|strVarcharToI32([.p_str], [.len], [.radix], 2)
	ret
.endp

.proc cdecl CNV.ui32sqrt(.num:POINTER) uses pbx psi pdi
	@larg pax, @arg1
	bsr pcx, pax
		jz .return

	and ecx, 0FEh
	xor edx, edx
	xchg pdx, pax
	xor ebx, ebx
	@@:
		shl eax, 1

		shl pbx, 2
		lea pdi, [pbx + pax * 2 + 1]
		
		mov psi, pdx
		shr psi, cl
		cmp pdi, psi
		lea esi, [eax + 1]
		cmovbe eax, esi
		cmovbe pbx, pdi
	sub ecx, 2
	jns @b
	.return: ret
.endp

.proc cdecl CNV.ui32pow(.num, .exp:DWORD)
	match =x64, __architecture{
		xchg rcx, rdx
	}
	match =x86, __architecture{
		mov ecx, [.exp]
		mov edx, [.num]
	}
	mov eax, 1
	.pow_loop:
		test ecx, 1
		jz .no_mul
			imul pax, pdx
		.no_mul:
		imul pdx, pdx
		shr ecx, 1
	test ecx, ecx
	jnz .pow_loop
	ret
.endp

.proc_frame_mode_previous
.proc_frame_mode_standard

.proc cdecl CNV.parseCMD(.p_argv:POINTER) uses pbx
	@sarg @arg1
	.local .p_argMem:POINTER
	$call [GetCommandLineA]()
	mov pdx, pax
	xor ecx, ecx
	mov ebx, 1
	.mainloop:
		movzx eax, byte[pdx + pcx]
		cmp eax, '"'
		jne .noquotes
			.findquote:
				inc ecx
				movzx eax, byte[pdx + pcx]
				cmp eax, 0
					je .NoSpace
			cmp eax, '"'
			jne .findquote
		.noquotes:
		cmp eax, " "
		jne .NoSpace
			cmp byte[pdx + pcx - 1], 0
			je .SpaceSeries			;серия пробелов
				; int3
				; @@: test eax, eax
				; jnz @b
				push pdx
				; movzx pdx, byte[pdx + pcx - 1]
				; mov pdx, [psp]
				inc ebx
			.SpaceSeries:
			lea pdx, [pdx + pcx + 1]
			mov byte[pdx - 1], 0
			mov ecx, -1
		.NoSpace:
		inc ecx
	cmp eax, 0
	jne .mainloop
	dec pbx
	cmp byte[pdx], 0
	je @f
		inc pbx
		push pdx
	@@:
	match =x64, __architecture{
		test pbx, 1
		jz @f
			sub rsp, 8
		@@:
	}
	$call CNV|alloc(addr pbx * pointer.size)
	match =x64, __architecture{
		test pbx, 1
		jz @f
			add rsp, 8
		@@:
	}
	mov [.p_argMem], pax
	mov ecx, ebx
	@@:
		pop pointer[pax + (pcx - 1) * pointer.size]
	loop @b
	mov pdx, [.p_argMem]
	mov pcx, [.p_argv]
	mov [pcx], pdx
	mov eax, ebx
	ret
.endp

.proc_frame_mode_previous
