rept 1{
	local _mp1, _mp2

	macro bp@prologue procname, parambytes, localbytes, reglist\{  
		\local loc
		loc = (localbytes + 3) and (not 3)
		parmbase@proc equ ebp + 8
		localbase@proc equ ebp - loc
		stackshift@proc equ loc
		if parambytes | localbytes
			push ebp
			mov ebp,esp
			if localbytes
				sub esp, stackshift@proc
			end if
		end if
		irps reg, reglist \\{ 
			push reg 
		\\} 
	\}
	macro bp@epilogue procname, parambytes, localbytes, reglist\{ 
		irps reg, reglist \\{ 
			\\reverse 
			pop reg 
		\\}
		if parambytes | localbytes
			leave
		end if
		match _mp1, proc@name\\{
			match _mp2, _mp1\\#.conv\\\{
				_mp2\\\#@retn parambytes
			\\\}
		\\} 
	\}
	macro bp@close args&\{
		restore parmbase@proc, localbase@proc, stackshift@proc
	\}
	macro bp@catch varname, procorigin, parambytes, localbytes\{
		if parambytes | localbytes
			; if procorigin\#varname - ebp > 0
			\local loc
			loc = procorigin\#.locals_size
			if loc and 0x3
				loc = (loc and 0xFFFFFFFFFFFFFFFC) + 0x4
			end if
			; %display_num parambytes + procorigin\#.locals_size + stackshift@proc + procorigin\#.regs_count * 4 + 8, 16
			virtual at procorigin\#varname + parambytes + procorigin\#.locals_size + stackshift@proc + procorigin\#.regs_count * 4 + 8
				varname:
			end virtual
		else
			virtual at procorigin\#varname; + procorigin\#.locals_size + procorigin\#.regs_count * 4; + 4
				varname:
			end virtual
		end if
	\}

	macro sp@prologue procname, parambytes, localbytes, reglist\{ 
		\local counter, loc, frame, current 
		counter = 0 
		irps reg, reglist \\{
			push reg 
			counter = counter+1 
		\\}
		loc = (localbytes + 3) and (not 3) 
		if frame & (counter + loc shr 3 + 1) and 1 
			loc = loc + 4 
		end if 
		framebytes@proc equ frame+loc 
		if framebytes@proc 
			sub esp, framebytes@proc 
		end if 
		localbase@proc equ esp + frame 
		regsbase@proc equ esp + frame + loc 
		parmbase@proc equ esp + frame + loc + counter * 4 + 4 
		current = 0 
		current@frame equ current 
		size@frame equ frame 
	\}
	macro sp@epilogue procname, parambytes, localbytes, reglist\{ 
		if framebytes@proc 
			add esp,framebytes@proc 
	   	end if 
	   	irps reg, reglist \\{ 
	   		\\reverse 
	   		pop reg 
		\\}
		match _mp1, proc@name\\{
			match _mp2, _mp1\\#.conv\\\{
				_mp2\\\#@retn parambytes
			\\\}
		\\}
	\}
	macro sp@close procname, parambytes, localbytes, reglist\{ 
		size@frame = current@frame 
		restore size@frame, current@frame, localbase@proc, regsbase@proc, parmbase@proc, framebytes@proc
	\}
	macro sp@catch varname, procorigin, parambytes, localbytes\{
		match _mp1, proc@name\\{
			virtual at procorigin\#varname + framebytes@proc + 4 + _mp1\\#.regs_count * 4
				varname:
			end virtual
		\\}
	\}
}

rept 1{
	local _counter, current@arg
	local _mp1, _mp2, _mp3, _mp4
	local _any1, _any2
	macro defargs@.proc@stdcall [arg] \{ 
		\common
		if ~ arg eq
			_counter equ 0
		   	\forward
				_counter equ _counter + 1
				match _mp1:_mp2, arg \\{
					current@arg equ _mp1
					match _mp3 =_mp2 _mp4, _any1 BYTE WORD DWORD PWORD QWORD TBYTE DQWORD QQWORD XWORD YWORD ZWORD _any2\\\{
				   		label _mp1 _mp2
				   		rb sizeof.\\#_mp2 + (4 - sizeof.\\#_mp2 mod 4) mod 4
					rept 0\\\{\\\} rept 1 \\\{ 
				   		_mp1 _mp2
				  	\\\} 
				rept 0\\{\\} rept 1 \\{ 
					current@arg equ arg
					arg rd 1 
				\\}
				rept 1 _mp1:_counter\\{
					@arg\\#_mp1\\#mem equ [current@arg]
					@farg\\#_mp1\\#mem equ [current@arg]
					@arg\\#_mp1 equ [current@arg]
					@farg\\#_mp1 equ [current@arg]
				\\}
		   	\common
		end if 
	\}
	macro defargs@.proc@c args&\{
		defargs@.proc@stdcall args
	\}
	macro defargs@.proc@cdecl args&\{
		defargs@.proc@stdcall args
	\}
}

rept 1{
	local _mp1, _mp2
	macro @set_std_dword_GPR src\{
		match =eax, src\\{rept 0\\{\\}rept 1\\{
			mov eax, src
		\\}
	\}
	macro @set_std_word_GPR src\{
		match =ax, src\\{rept 0\\{\\}rept 1\\{
			movzx eax, src
		\\}
	\}
	macro @set_std_byte_GPR src\{
		match =al, src\\{rept 0\\{\\}rept 1\\{
			movzx eax, src
		\\}
	\}
	local floatbuf
	macro @set_std_float_GPR src\{
		procbuf_alloc floatbuf, 4
		mov dword[floatbuf], src
		fld dword[floatbuf]
		procbuf_free floatbuf
	\}
	macro @set_std_double_SSE src\{
		procbuf_alloc floatbuf, 8
		movq qword[floatbuf], src
		fld qword[floatbuf]
		procbuf_free floatbuf
	\}
	macro @set_std_float_SSE src\{
		procbuf_alloc floatbuf, 4
		movd dword[floatbuf], src
		fld dword[floatbuf]
		procbuf_free floatbuf
	\}
	macro @set_std_qword_SSE src\{
		; movd eax, src
		; match =xmm0, src\\{rept 0\\{\\}rept 1\\{
		; 	movq xmm0, src
		; \\}
		; psrlq xmm0, 32
		; movd edx, xmm0
		pextrd edx, src, 1
		movd eax, src
	\}
	macro @set_std_dword_SSE src\{
		movd eax, src
	\}
	macro @set_std_double_FPU src\{\}
	macro @set_std_float_FPU src\{\}
	macro @set_std_sep_qword src\{
		match _mp1:_mp2, src\\{
			if ~_mp2 eq eax
				mov eax, _mp2
			end if
			if ~_mp1 eq edx
				mov edx, _mp1
			end if
		\\}
	\}
	macro @set_std_qword_addr src\{
		if ~src relativeto eax
			lea eax, [src]
		else if src - eax <> 0
			add eax, src - eax
		end if
		xor edx, edx
	\}
	macro @set_std_dword_addr src\{
		if ~src relativeto eax
			lea eax, [src]
		else if src - eax <> 0
			add eax, src - eax
		end if
	\}
	local litval
	macro @set_std_qword_lit src\{
		litval = qword src
		if litval and 0xFFFFFFFF = 0
			xor eax, eax
		else
			mov eax, litval and 0xFFFFFFFF
		end if
		if litval shr 32 = 0
			xor edx, edx
		else
			mov edx, litval shr 32
		end if
	\}
	macro @set_std_dword_lit src\{
		litval = dword src
		if litval = 0
			xor eax, eax
		else
			mov eax, litval
		end if
	\}
	macro @set_std_word_lit src\{
		litval = word src
		if litval = 0
			xor eax, eax
		else
			mov eax, litval
		end if
	\}
	macro @set_std_byte_lit src\{
		litval = byte src
		if litval = 0
			xor eax, eax
		else
			mov eax, litval and 0xFFFFFFFF
		end if
	\}
	macro @set_std_double_lit src\{
		\local ..lits
		litval = qword src
		if litval = 0
			fldz
		else if litval = 1.0
			fld1
		else if litval = -1.0
			fld1
			fchs
		else
			..lits @const dq litval
			fld qword[..lits]
		end if
	\}
	local fcmplit1, fcmplitn1
	fcmplit1 = dword 1.0
	fcmplit1 = dword -1.0
	macro @set_std_float_lit src\{
		\local ..lits
		litval = dword src
		if litval = 0
			fldz
		else if litval = fcmplit1
			fld1
		else if litval = fcmplitn1
			fld1
			fchs
		else
			..lits @const dd litval
			fld dword[..lits]
		end if

	\}
	macro @set_std_real_lit src\{
		procbuf_alloc floatbuf, 4
		mov eax, litval
		movd xmm0, eax
		vcvtph2ps xmm0, xmm0
		movd dword[floatbuf], xmm0
		fld dword[floatbuf]
		procbuf_free floatbuf
	\}
	macro @set_std_qword_mem src\{
		match [_mp1], src\\{
			mov eax, dword[_mp1]
			mov edx, dword[_mp1 + 4]
		\\}
	\}
	macro @set_std_dword_mem src\{
		mov eax, dword src
	\}
	macro @set_std_word_mem src\{
		movzx eax, word src
	\}
	macro @set_std_byte_mem src\{
		movzx eax, byte src
	\}
	macro @set_std_double_mem src\{
		fld qword src
	\}
	macro @set_std_float_mem src\{
		fld dword src
	\}
	macro @set_std_real_mem src\{
		procbuf_alloc floatbuf, 4
		movzx eax, word src
		movd xmm0, eax
		vcvtph2ps xmm0, xmm0
		movd dword[floatbuf], xmm0
		fld dword[floatbuf]
		procbuf_free floatbuf
	\}
	local mem_size
	macro @set_std_unk_mem src\{
		sizeof@addr mem_size, src
		if mem_size = 8
			@set_std_qword_mem src
		else if mem_size = 4
			@set_std_dword_mem src
		else if mem_size = 2
			@set_std_word_mem src
		else
			@set_std_byte_mem src
		end if
	\}
	local src_type, src_def
	macro @set_result@stdcall src_type, src_def\{
		match _mp1 _mp2, src_type\\{
			match _mp3 _mp4, @set_std_\\#_mp1\\#_\\#_mp2 src_def\\\{
				_mp3 _mp4
			\\\}
		\\}
	\}
	macro @set_result@c src&\{@set_result@stdcall src\}
	macro @set_result@cdecl src&\{@set_result@stdcall src\}
	local _src_type, _src_def
	macro @set_result_stdcall src\{
		_parse@arg _src_type, _src_def, src
		@set_result@stdcall _src_type, _src_def
	\}
	macro @set_result_c src&\{@set_result_stdcall src\}
	macro @set_result_cdecl src&\{@set_result_stdcall src\}
	macro @save_result@stdcall savebuf, type\{
		procbuf_alloc savebuf, 16
		match _mp1 _mp2, type\\{
			match _mp3 =_mp1 _mp4, _any1 real float double _any2\\\{
				fstp tbyte[savebuf]
			rept 0\\\{\\\}rept 1\\\{
				mov dword[savebuf], eax
				mov dword[savebuf + 4], edx
			\\\}
		\\}
	\}
	macro @save_result@c args&\{@save_result@stdcall args\}
	macro @save_result@cdecl args&\{@save_result@stdcall args\}
	macro @load_result@stdcall savebuf, type\{
		match _mp1 _mp2, type\\{
			match _mp3 =_mp1 _mp4, _any1 real float double _any2\\\{
				fld tbyte[savebuf]
			rept 0\\\{\\\}rept 1\\\{
				mov eax, dword[savebuf]
				mov edx, dword[savebuf + 4]
			\\\}
		\\}
		procbuf_free savebuf
	\}
	macro @load_result@c args&\{@load_result@stdcall args\}
	macro @load_result@cdecl args&\{@load_result@stdcall args\}
}