rept 1{
	local _mp1, _mp2

	macro bp@prologue procname, parambytes, localbytes, reglist\{ 
		\local loc,fill,counter
		loc = (localbytes + 15) and (not 15)
		parmbase@proc equ rbp + 16
		localbase@proc equ rbp - loc
		push rbp
		mov rbp,rsp
		if loc+fill
			sub rsp,loc+fill
		end if
		counter = 0
		irps reg, reglist \\{ 
			push reg
			counter = counter + 1 
		\\}
		fill = 8 * (counter and 1) 
	\}
	macro bp@epilogue procname, parambytes, localbytes, reglist\{ 
		irps reg, reglist \\{ 
			\\reverse 
			pop reg 
		\\}
	   	leave
		match _mp1, proc@name\\{
			match _mp2, _mp1\\#.conv\\\{
				_mp2\\\#@retn parambytes
			\\\}
		\\} 
	\}
	macro bp@close args&\{
		restore parmbase@proc, localbase@proc
	\}
	local paramshift
	macro bp@catch varname, procorigin, parambytes, localbytes\{
		paramshift = parambytes
		if paramshift > 0 & paramshift < 32
			paramshift = 32
		else if paramshift & 15
			paramshift = paramshift - paramshift and 15 + 16
		end if
		match _mp1, proc@name\\{
			virtual at procorigin\#varname + paramshift + stackshift@proc + procorigin\#.regs_count * 8 + 8 * (((_mp1\\#.regs_count and 1) + 1) and 1) + 8
				varname:
			end virtual
		\\}
	\}

	macro sp@prologue procname, parambytes, localbytes, reglist\{ 
		\local counter,loc,frame,current 
		counter = 0 
		irps reg, reglist \\{
			push reg 
			counter = counter + 1 
		\\}
		loc = (localbytes + 7) and (not 7) 
		if frame & (counter + loc shr 3 + 1) and 1 
			loc = loc + 8 
		end if 
		framebytes@proc equ frame+loc 
		if framebytes@proc 
			sub rsp,framebytes@proc 
		end if 
		localbase@proc equ rsp + frame 
		regsbase@proc equ rsp + frame + loc 
		parmbase@proc equ rsp + frame + loc + counter * 8 + 8 
		current = 0 
		current@frame equ current 
		size@frame equ frame 
	\}
	macro sp@epilogue procname, parambytes, localbytes, reglist\{ 
		if framebytes@proc 
			add rsp,framebytes@proc 
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
		restore size@frame, current@frame, parm@baseproc, regsbase@proc, localbase@proc, framebytes@proc
	\}
	macro sp@catch varname, procorigin, parambytes, localbytes\{
		match _mp1, proc@name\\{
			virtual at procorigin\#varname + framebytes@proc + 8 + 8 * _mp1\\#.regs_count
				varname:
			end virtual
		\\}
	\}
}

rept 1{
	local _counter, current@arg
	local _mp1, _mp2, _mp3, _mp4
	local _result
	local _any1, _any2
	local reg_qword_, reg_double_
	define reg_qword_#1 rcx
	define reg_qword_#2 rdx
	define reg_qword_#3 r8
	define reg_qword_#4 r9
	define reg_double_#1 xmm0
	define reg_double_#2 xmm1
	define reg_double_#3 xmm2
	define reg_double_#4 xmm3
	macro defargs@.proc@fastcall [arg] \{ 
		\common
		if ~ arg eq
			_counter equ 0
		   	\forward
				_counter equ _counter + 1
				match _mp1:_mp2, arg \\{
					current@arg equ _mp1
					match _mp3 =_mp2 _mp4, _any1 BYTE WORD DWORD PWORD QWORD TBYTE DQWORD QQWORD XWORD YWORD ZWORD _any2\\\{
				   		label _mp1 _mp2
				   		rb sizeof.\\#_mp2 + (8 - sizeof.\\#_mp2 mod 8) mod 8
					rept 0\\\{\\\} rept 1 \\\{ 
				   		_mp1 _mp2
				  	\\\} 
				rept 0\\{\\} rept 1 \\{ 
					current@arg equ arg
					arg rq 1
				\\}
				rept 1 _mp1:_counter\\{
					@arg\\#_mp1\\#mem equ [current@arg]
					@farg\\#_mp1\\#mem equ [current@arg]
					%preproc_cmp _result, _mp1, 4
					match =GT, _result\\\{
						@arg\#_mp1 equ [current@arg]
						@farg\#_mp1 equ [current@arg]
					rept 0\\\{\\\}rept 1\\\{
						@arg\#_mp1 equ reg_qword_\\#_mp1
						@farg\#_mp1 equ reg_double_\\#_mp1
					\\\}
				\\}
		   	\common
		end if 
	\}
	macro defargs@.proc@stdcall args&\{
		defargs@.proc@fastcall args
	\}
	macro defargs@.proc@c args&\{
		defargs@.proc@fastcall args
	\}
	macro defargs@.proc@cdecl args&\{
		defargs@.proc@fastcall args
	\}
}

rept 1{
	local _mp1, _mp2, _mp3, _mp4
	local _any1, _any2

	macro @set_fast_qword_GPR src\{
		match =rax, src\\{rept 0\\{\\}rept 1\\{
			mov rax, src
		\\}
	\}
	macro @set_fast_dword_GPR src\{
		match =eax, src\\{rept 0\\{\\}rept 1\\{
			mov eax, src
		\\}
	\}
	macro @set_fast_word_GPR src\{
		match =ax, src\\{rept 0\\{\\}rept 1\\{
			movzx eax, src
		\\}
	\}
	macro @set_fast_byte_GPR src\{
		match =al, src\\{rept 0\\{\\}rept 1\\{
			movzx eax, src
		\\}
	\}
	macro @set_fast_double_GPR src\{
		movd xmm0, src
	\}
	macro @set_fast_float_GPR src\{
		movd xmm0, src
	\}
	macro @set_fast_double_SSE src\{
		match =xmm0, src\\{rept 0\\{\\}rept 1\\{
			movq xmm0, src
		\\}
	\}
	macro @set_fast_float_SSE src\{
		match =xmm0, src\\{rept 0\\{\\}rept 1\\{
			movd xmm0, src
		\\}
	\}
	macro @set_fast_qword_SSE src\{
		movq rax, src
	\}
	macro @set_fast_dword_SSE src\{
		movd eax, src
	\}
	local floatbuf
	macro @set_fast_double_FPU src\{
		procbuf_alloc floatbuf, 8
		fstp qword[floatbuf]
		movq src, qword[floatbuf]
		procbuf_free floatbuf
	\}
	macro @set_fast_float_FPU src\{
		procbuf_alloc floatbuf, 4
		fstp dword[floatbuf]
		movd src, dword[floatbuf]
		procbuf_free floatbuf
	\}
	macro @set_fast_sep_qword src\{
		match _mp1:_mp2, src\\{
			mov eax, _mp2
			if ~_mp1 eq edx
				mov edx, _mp1
			end if
			shl rdx, 32
			or rax, rdx
		\\}
	\}
	macro @set_fast_qword_addr src\{
		if ~src relativeto rax
			lea rax, [src]
		else if src - rax <> 0
			add rax, src - rax
		end if
	\}
	macro @set_fast_dword_addr src\{
		if ~src relativeto eax
			lea eax, [src]
		else if src - eax <> 0
			add eax, src - eax
		end if
	\}
	local litval
	macro @set_fast_qword_lit src\{
		litval = qword src
		mov rax, litval
	\}
	macro @set_fast_dword_lit src\{
		litval = dword src
		mov eax, litval
	\}
	macro @set_fast_word_lit src\{
		litval = word src
		mov eax, litval
	\}
	macro @set_fast_byte_lit src\{
		litval = byte src
		mov eax, litval
	\}
	macro @set_fast_double_lit src\{
		litval = qword src
		mov rax, litval
		movq xmm0, rax
	\}
	macro @set_fast_float_lit src\{
		litval = dword src
		mov eax, litval
		movd xmm0, eax
	\}
	macro @set_fast_real_lit src\{
		litval = word src
		mov eax, litval
		movd xmm0, eax
	\}
	macro @set_fast_qword_mem src\{
		mov rax, src
	\}
	macro @set_fast_dword_mem src\{
		mov eax, src
	\}
	macro @set_fast_word_mem src\{
		movzx eax, word src
	\}
	macro @set_fast_byte_mem src\{
		movzx eax, byte src
	\}
	macro @set_fast_double_mem src\{
		movq xmm0, qword src
	\}
	macro @set_fast_float_mem src\{
		movd xmm0, dword src
	\}
	macro @set_fast_real_mem src\{
		movzx eax, word src
		movd xmm0, eax
	\}
	local mem_size
	macro @set_fast_unk_mem src\{
		sizeof@addr mem_size, src
		if mem_size = 8
			@get_fast_qword_mem src
		else if mem_size = 4
			@get_fast_dword_mem src
		else if mem_size = 2
			@get_fast_word_mem src
		else
			@get_fast_byte_mem src
		end if
	\}
	local src_type, src_def
	macro @set_result@fastcall src_type, src_def\{
		match _mp1 _mp2, src_type\\{
			match _mp3 _mp4, @set_fast_\\#_mp1\\#_\\#_mp2 src_def\\\{
				_mp3 _mp4
			\\\}
		\\}
	\}
	macro @set_result@stdcall args&\{@set_result@fastcall args\}
	macro @set_result@c args&\{@set_result@fastcall args\}
	macro @set_result@cdecl args&\{@set_result@fastcall args\}
	local _src_type, _src_def
	macro @set_result_fastcall src\{
		_parse@arg _src_type, _src_def, src
		@set_result@fastcall _src_type, _src_def
	\}
	macro @set_result_stdcall src&\{@set_result_fastcall src\}
	macro @set_result_c src&\{@set_result_fastcall src\}
	macro @set_result_cdecl src&\{@set_result_fastcall src\}
	macro @save_result@fastcall savebuf, type\{
		procbuf_alloc savebuf, 8
		match _mp1 _mp2, type\\{
			match _mp3 =_mp1 _mp4, _any1 real float double _any2\\\{
				movq qword[savebuf], xmm0
			rept 0\\\{\\\}rept 1\\\{
				mov qword[savebuf], rax
			\\\}
		\\}
	\}
	macro @save_result@stdcall args&\{@save_result@fastcall args\}
	macro @save_result@c args&\{@save_result@fastcall args\}
	macro @save_result@cdecl args&\{@save_result@fastcall args\}
	macro @load_result@fastcall savebuf, type\{
		match _mp1 _mp2, type\\{
			match _mp3 =_mp1 _mp4, _any1 real float double _any2\\\{
				movq xmm0, qword[savebuf]
			rept 0\\\{\\\}rept 1\\\{
				mov rax, qword[savebuf]
			\\\}
		\\}
		procbuf_free savebuf
	\}
	macro @load_result@stdcall args&\{@load_result@fastcall args\}
	macro @load_result@c args&\{@load_result@fastcall args\}
	macro @load_result@cdecl args&\{@load_result@fastcall args\}
}