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
	proc_noprologue

	CNV._heap dq ?

	proc CNV._heapinit
		@call [GetProcessHeap]()
		mov [CNV._heap], rax
		ret
	endp

	TLS_AddCallback CNV._heapinit

	proc_resprologue
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
	if flags and FILL_SAVE_RSI
		if defined current@frame
			if current@frame<8
				current@frame = 8
			end if
			mov [rsp], rdi
		else
			push rdi
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
	if flags and FILL_SAVE_RSI
		if defined current@frame
			mov rdi, [rsp]
		else
			pop rdi
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
		if flags and STRMOV_SAVE_RSI
			push rsi
		end if
		if flags and STRMOV_SAVE_RDI
			push rdi
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
		if flags and STRMOV_SAVE_RDI
			pop rdi
		end if
		if flags and STRMOV_SAVE_RSI
			pop rsi
		end if
	end if
}

include "CNV.asm"