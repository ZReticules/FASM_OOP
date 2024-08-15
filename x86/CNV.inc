importlib kernel32,\
	GetProcessHeap,\
	HeapAlloc,\
	HeapReAlloc,\
	HeapFree

struct CNV
	; getLine 	dm this
	parseCMD 	dm this
	alloc  		dm this:inline
	realloc 	dm this:inline
	free  		dm this:inline
	fill  		dm this:inline
	strlen		dm this:inline
	strmov  	dm this:inline
	db ?
ends


if used CNV._heap

	CNV._heap dd ?

	proc CNV._heapinit
	_static_args .handle, .reason, .reserved
		@call [GetProcessHeap]()
		mov [CNV._heap], eax
		ret args_space
	endp

	TLS_AddCallback CNV._heapinit

end if

macro CNV.alloc size, flags=HEAP_ZERO_MEMORY{
	@call [HeapAlloc]([CNV._heap], flags, size)
}

macro CMV.realloc lpMem, size, flags=HEAP_ZERO_MEMORY{
	@call [HeapReAlloc]([CNV._heap], flags, lpMem, size)
}

macro CNV.free lpMem, flags=0{
	@call [HeapFree]([CNV._heap], flags, lpMem)
}

FILL_NO_SAVE	= 0
FILL_SAVE_ESI 	= 1
FILL_SAVE_EDI 	= 2
macro CNV.fill dest*, src*, size*, flags=FILL_SAVE_EDI or FILL_SAVE_ESI{
	local repeats, rem, _src, _dest, matched
	if size eqtype 0 & size relativeto 0
		repeats = (size) / 32
		rem = (size) mod 32
	else 
		repeats = 2
	end if
	if repeats > 1
		if flags and FILL_SAVE_ESI
			push esi
		end if
		if flags and FILL_SAVE_EDI
			push edi
		end if
		fillParam ecx, size
		fillParam edi, dest
		fillParam esi, src
		rep movsb
		if flags and FILL_SAVE_EDI
			pop edi
		end if
		if flags and FILL_SAVE_ESI
			pop esi
		end if
	else 
		inlineObj _dest, dest, ecx
		inlineObj _src, src, edx
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
STRLEN_SAVE_EDI 	= 1
STRLEN_REZULT_ECX 	= 2
macro CNV.strlen src*, flags = STRLEN_SAVE_EDI{
	if flags and FILL_SAVE_ESI
		push edi
	end if
	fillParam edi, src
	xor eax, eax
	mov ecx, -1
	repnz scasb
	if flags and STRLEN_REZULT_ECX
		lea ecx, [ecx+2]
		neg ecx
	else
		lea eax, [ecx+2]
		neg eax
	end if
	if flags and FILL_SAVE_ESI
		pop edi
	end if
}

STRMOV_NO_SAVE 	= 0
STRMOV_SAVE_ESI = 1
STRMOV_SAVE_EDI = 2
STRMOV_RET_LEN 	= 4
macro CNV.strmov dest*, src*, flags=STRMOV_SAVE_ESI or STRMOV_SAVE_EDI{
	if flags and STRMOV_SAVE_ESI
		push esi
	end if
	if flags and STRMOV_SAVE_EDI
		push edi
	end if
	fillParam edi, dest
	fillParam esi, src
	if flags and STRMOV_RET_LEN
		mov eax, -1
	end if
	local .lab
	.lab:
		movsb
	if flags and STRMOV_RET_LEN
		inc eax
	end if
	cmp byte[esi-1], 0
	jne .lab
	if flags and STRMOV_SAVE_EDI
		pop edi
	end if
	if flags and STRMOV_SAVE_ESI
		pop esi
	end if
}

include "CNV.asm"