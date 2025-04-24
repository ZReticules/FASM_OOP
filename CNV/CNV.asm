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
		@call [GetProcessHeap]()
		mov [CNV._heap], pax
	end if
	if used CNV.argc | used CNV.argv
		@call CNV::parseCMD(CNV.argv)
		mov [CNV.argc], eax
	end if
}

TLS_AddMacro CNV._initHeap

macro CNV.alloc size, flags=HEAP_ZERO_MEMORY{
	@call [HeapAlloc]([CNV._heap], flags, size)
}

macro CNV.realloc lpMem, size, flags=HEAP_ZERO_MEMORY{
	@call [HeapReAlloc]([CNV._heap], flags, lpMem, size)
}

macro CNV.free lpMem, flags=0{
	@call [HeapFree]([CNV._heap], flags, lpMem)
}

macro CNV.BMPFromFile path, x=0, y=0{
	@call [LoadImageA](NULL, path, IMAGE_BITMAP, x, y, LR_LOADFROMFILE)
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
; 		local _repeats, _rem, _src_, _dest_
; 		_src_ = 0
; 		_src equ _src + _src_
; 		_repeats = repeats
; 		_rem = rem
; 		if repeats = 1
; 			if (_src) relativeto 0 & ((_src) mod 32 = 0)
; 				vmovdqa ymm0, yword[_src]
; 			else 
; 				vmovdqu ymm0, yword[_src]
; 			end if
; 			_src_ = _src_ + 32
; 		end if
; 		repeats = rem / 16
; 		rem = rem mod 16
; 		if repeats = 1
; 			if (_src) relativeto 0 & ((_src) mod 16 = 0)
; 				vmovdqa xmm1, xword[_src]
; 			else 
; 				vmovdqu xmm1, xword[_src]
; 			end if
; 			_src_ = _src_ + 16
; 		end if
; 		repeats = rem / 8
; 		rem = rem mod 8
; 		if repeats = 1
; 			movq xmm3, qword[_src]
; 			_src_ = _src_ + 8
; 		end if
; 		repeats = rem / 4
; 		rem = rem mod 4
; 		if repeats = 1
; 			mov eax, dword[_src]
; 			_src_ = _src_ + 4
; 		end if
; 		repeats = rem / 2
; 		rem = rem mod 2
; 		if repeats = 1
; 			movzx ecx, word[_src]
; 			_src_ = _src_ + 2
; 		end if
; 		if rem = 1
; 			movzx edx, byte[_src]
; 		end if 
; 		repeats = _repeats
; 		rem = _rem
; 		_dest_ = 0
; 		_dest equ _dest + _dest_
; 		if repeats = 1
; 			if (_dest) relativeto 0 & ((_dest) mod 32 = 0)
; 				vmovdqa yword[_dest], ymm0
; 			else 
; 				vmovdqu yword[_dest], ymm0
; 			end if
; 			_dest_ = _dest_ + 32
; 		end if
; 		repeats = rem / 16
; 		rem = rem mod 16
; 		if repeats = 1
; 			if (_dest) relativeto 0 & ((_dest) mod 16 = 0)
; 				vmovdqa xword[_dest], xmm1
; 			else 
; 				vmovdqu xword[_dest], xmm1
; 			end if
; 			_dest_ = _dest_+16
; 		end if
; 		repeats = rem / 8
; 		rem = rem mod 8
; 		if repeats = 1
; 			movq qword[_dest], xmm3
; 			_dest_ = _dest_+8
; 		end if
; 		repeats = rem / 4
; 		rem = rem mod 4
; 		if repeats = 1
; 			mov dword[_dest], eax
; 			_dest_ = _dest_+4
; 		end if
; 		repeats = rem / 2
; 		rem = rem mod 2
; 		if repeats = 1
; 			mov word[_dest], cx
; 			_dest_ = _dest_+2
; 		end if
; 		if rem = 1
; 			mov byte[_dest], dl
; 		end if 
; 	end if
; }


macro CNV.fill dest*, src*, size*, any{
	local repeats, rem, _src, _dest, matched
	repeats = (size) / 32
	rem = (size) mod 32
	inlineObj _dest, dest, pax
	inlineObj _src, src, pdx
	local _repeats, _rem, _src_, _dest_
	_src_ = 0
	_src equ (_src + _src_)
	_dest_ = 0
	_dest equ (_dest + _dest_)
	repeat repeats
		if _src relativeto 0 & (_src mod 32 = 0)
			vmovdqa ymm0, yword[_src]
		else 
			vmovdqu ymm0, yword[_src]
		end if
		_src_ = _src_ + 32
		if (_dest) relativeto 0 & ((_dest) mod 32 = 0)
			vmovdqa yword[_dest], ymm0
		else 
			vmovdqu yword[_dest], ymm0
		end if
		_dest_ = _dest_ + 32
	end repeat
	_rem = rem
	repeats = rem / 16
	rem = rem mod 16
	if repeats = 1
		if (_src) relativeto 0 & ((_src) mod 16 = 0)
			vmovdqa xmm1, xword[_src]
		else 
			vmovdqu xmm1, xword[_src]
		end if
		_src_ = _src_ + 16
		if (_dest) relativeto 0 & ((_dest) mod 16 = 0)
			vmovdqa xword[_dest], xmm1
		else 
			vmovdqu xword[_dest], xmm1
		end if
		_dest_ = _dest_ + 16
	end if
	repeats = rem / 8
	rem = rem mod 8
	if repeats = 1
		movq xmm3, qword[_src]
		_src_ = _src_ + 8
		movq qword[_dest], xmm3
		_dest_ = _dest_ + 8
	end if
	repeats = rem / 4
	rem = rem mod 4
	if repeats = 1
		mov ecx, dword[_src]
		_src_ = _src_ + 4
		mov dword[_dest], ecx
		_dest_ = _dest_ + 4
	end if
	repeats = rem / 2
	rem = rem mod 2
	if repeats = 1
		movzx ecx, word[_src]
		_src_ = _src_ + 2
		mov word[_dest], cx
		_dest_ = _dest_ + 2
	end if
	if rem = 1
		movzx ecx, byte[_src]
		mov byte[_dest], cl
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
			mov [psp], pdi
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
			mov pdi, [psp]
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
	local .lab
	.lab:
		movsb
	if flags and STRMOV_RET_LEN
		inc pax
	end if
	cmp byte[psi-1], 0
	jne .lab
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
	@call CNV::BMPToFile(hBmp, lpFname, bitCount)
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

proc_noprologue

@arch_include "CNV"

; next functions both returns count of chars 
proc CNV.intToStr c, lpStr, num, radix
	@sarg @arg2
	locals
		sign dd 0
	endl
	cmp @arg2, 0
	jns .positive
		@larg pcx, @arg1
		mov [sign], 1
		mov byte[pcx], '-'
		neg @arg2
		inc @arg1
	.positive:
	@call c CNV.uintToStr(@arg1, @arg2, @arg3)
	lea edx, [eax + 1]
	cmp [sign], 0
		cmovne eax, edx
	ret
endp

proc CNV.uintToStr c uses pbx psi, lpStr, num, radix
	locals
		buf 	db 65 dup ?
	endl
	cmp @arg2, 0
		je .zeroret
	@sarg @arg1, @arg3
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
		mov [buf + pcx], dl
		inc ecx
	test eax, eax
	jnz .loop1
	mov psi, pcx
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
		mov pdx, 0
		bsr pcx, pbx
		.loop3:
			xor ebx, ebx
			shrd pbx, pax, cl
			shr pax, cl
			rol pbx, cl
			add pbx, 30h
			cmp pbx, 39h
			jle .decDigits_binary
				add ebx, 7
			.decDigits_binary:
			mov [buf + pdx], bl
			inc pdx
		test eax, eax
		jnz .loop3
		mov pcx, pdx
		mov psi, pdx
		jmp .migration_loop

	.zeroret:
		@larg pcx, @arg1
		mov word[pcx], "0"
		mov pax, 1
		ret
endp

proc CNV.ui32sqrt c uses pbx psi pdi, num:POINTER
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
endp

proc CNV.ui32pow c, num, exp:DWORD
	@sarg @arg1, @arg2
	mov ecx, [exp]
	mov pdx, [num]
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
endp

proc_resprologue

proc CNV.parseCMD c uses pbx, lpArgv:POINTER
	@sarg @arg1
	local lpArgMem:POINTER
	@call [GetCommandLineA]()
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
	@call CNV::alloc(addr pbx * pointer.size)
	mov [lpArgMem], pax
	mov ecx, ebx
	@@:
		pop pointer[pax + (pcx - 1) * pointer.size]
	loop @b
	mov pdx, [lpArgMem]
	mov pcx, [lpArgv]
	mov [pcx], pdx
	mov eax, ebx
	ret
endp
