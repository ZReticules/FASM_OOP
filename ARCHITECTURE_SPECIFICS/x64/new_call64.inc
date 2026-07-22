rept 1{
	local origin, opcode
	macro sizeof@addr result, mem\{
		virtual
			origin = $
			inc mem
			load opcode byte from origin
			if opcode = 02Eh | opcode = 036h | opcode = 03Eh | opcode = 026h | opcode = 064h | opcode = 065h
				origin = origin + 1
				load opcode byte from origin
			end if
			if opcode = 67h | opcode = 41h
				origin = origin + 1
				load opcode byte from origin
			end if
			if opcode and 0F8h = 48h
				result = 8
			else if opcode = 66h
				result = 2
			else if opcode = 0FFh
				result = 4
			else
				result = 1
			end if
		end virtual
	\}
}

rept 1{
	local reg_qword_, reg_dword_, reg_double_
	define reg_qword_#1 rcx
	define reg_qword_#2 rdx
	define reg_qword_#3 r8
	define reg_qword_#4 r9
	define reg_dword_#1 ecx
	define reg_dword_#2 edx
	define reg_dword_#3 r8d
	define reg_dword_#4 r9d
	define reg_double_#1 xmm0
	define reg_double_#2 xmm1
	define reg_double_#3 xmm2
	define reg_double_#4 xmm3
	local cmpres
	local _mp1, _mp2
	local _stack_counter
	local arg_type, arg_def, cntr, arg_cnt
	local _argN

	macro fast@va_load args_start, stack_space, stack_counter, argn, atype, adef\{
		match _mp1 _mp2, atype\\{
			match _mp3, fast@load_\\#_mp1\\#_\\#_mp2\\\{
				_mp3 args_start, stack_space, stack_counter, argn, adef
			\\\}
		\\}
	\}
	macro fast@load_va_list args_start, stack_space, stack_counter, argn, [adef]\{
		\common
		\local _stack_space 
		stack_counter = stack_counter + 8
		_stack_counter = 0
		cntr equ 0
		arg_cnt equ 4
		\forward
		rept 1 _argN:arg_cnt + 1\\{
			; restore arg_cnt
			arg_cnt equ _argN
		\\}
		\reverse
		rept 1 _argN:cntr + 1\\{
			; restore cntr
			cntr equ _argN
		\\}
		rept 1 _argN:arg_cnt - cntr + 1\\{
			_parse@arg arg_type, arg_def, adef
			fast@va_load va@list, _stack_space, _stack_counter, _argN, arg_type, arg_def
		\\}
		\common
		_stack_space = _stack_counter
		if _stack_space > va_list@counter
			va_list@counter = _stack_space
		end if
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			lea r11, [va@list]
			mov [args_start + stack_space - stack_counter], r11
		rept 0\\{\\} rept 1\\{
			lea reg_qword_\#argn, [va@list]
		\\}
	\}
	macro fast@load_sep_qword args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT _mp1:_mp2, cmpres adef\\{
			mov dword[args_start + stack_space - stack_counter], _mp2
			mov dword[args_start + stack_space - stack_counter + 4], _mp1
		rept 0\\{\\}match _mp1:_mp2, adef\\{
			mov r11d, _mp1
			shl r11, 32
			mov reg_dword_\#argn, _mp2
			or reg_qword_\#argn, r11
		\\}
	\}
	macro fast@load_addr args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			if adef relativeto 0
				mov qword[args_start + stack_space - stack_counter], adef
			irp reg, rax, rcx, rdx, rbx, rsi, rdi, rsp, rbp, r8, r9, r10, r11, r12, r13, r14, r15\\\{
				else if adef relativeto reg & adef - reg = 0
					mov qword[args_start + stack_space - stack_counter], reg
			\\\}
			else
				lea r11, [adef]
				mov qword[args_start + stack_space - stack_counter], r11
			end if
		rept 0\\{\\}rept 1\\{
			if ~ (adef relativeto reg_qword_\#argn & adef - reg_qword_\#argn = 0)
				lea reg_qword_\#argn, [adef]
			end if
		\\}
	\}
	macro fast@load_qword_addr args&\{fast@load_addr args\}
	macro fast@load_dword_addr args&\{fast@load_addr args\}
	local litval
	macro fast@load_lit args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		litval = qword adef
		match =GT, cmpres\\{
			mov dword[args_start + stack_space - stack_counter], litval and 0xFFFFFFFF
			mov dword[args_start + stack_space - stack_counter + 4], litval shr 32
		rept 0\\{\\}rept 1\\{
			if litval = 0
				xor reg_dword_\#argn, reg_dword_\#argn
			else
				mov reg_qword_\#argn, litval
			end if
		\\}
	\}
	macro fast@load_qword_lit args&\{
		fast@load_lit args
	\}
	macro fast@load_dword_lit args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		litval = dword adef
		match =GT, cmpres\\{
			mov dword[args_start + stack_space - stack_counter], litval
		rept 0\\{\\}rept 1\\{
			if litval = 0
				xor reg_dword_\#argn, reg_dword_\#argn
			else
				mov reg_dword_\#argn, litval
			end if
		\\}
	\}
	macro fast@load_word_lit args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		litval = word adef
		match =GT, cmpres\\{
			mov dword[args_start + stack_space - stack_counter], litval
		rept 0\\{\\}rept 1\\{
			if litval = 0
				xor reg_dword_\#argn, reg_dword_\#argn
			else
				mov reg_dword_\#argn, litval
			end if
		\\}
	\}
	macro fast@load_byte_lit args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		litval = byte adef
		match =GT, cmpres\\{
			mov dword[args_start + stack_space - stack_counter], litval
		rept 0\\{\\}rept 1\\{
			if litval = 0
				xor reg_dword_\#argn, reg_dword_\#argn
			else
				mov reg_dword_\#argn, litval
			end if
		\\}
	\}
	macro fast@load_double_lit args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		litval = qword adef
		match =GT, cmpres\\{
			mov dword[args_start + stack_space - stack_counter], litval and 0xFFFFFFFF
			mov dword[args_start + stack_space - stack_counter + 4], litval shr 32
		rept 0\\{\\}rept 1\\{
			if litval = 0
				pxor reg_double_\#argn, reg_double_\#argn
			else if litval = 0xFFFFFFFFFFFFFFFF
				pcmpeqq reg_double_\#argn, reg_double_\#argn
			else
				mov r11, litval
				movq reg_double_\#argn, r11
			end if
		\\}
	\}
	macro fast@load_float_lit args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		litval = dword adef
		match =GT, cmpres\\{
			mov dword[args_start + stack_space - stack_counter], litval
		rept 0\\{\\}rept 1\\{
			if litval = 0
				pxor reg_double_\#argn, reg_double_\#argn
			else if litval = 0xFFFFFFFF
				pcmpeqq reg_double_\#argn, reg_double_\#argn
			else
				mov r11d, litval
				movd reg_double_\#argn, r11d
			end if
		\\}
	\}
	macro fast@load_real_lit args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		litval = word adef
		match =GT, cmpres\\{
			mov dword[args_start + stack_space - stack_counter], litval
		rept 0\\{\\}rept 1\\{
			if litval = 0
				pxor reg_double_\#argn, reg_double_\#argn
			else if litval = 0xFFFF
				pcmpeqq reg_double_\#argn, reg_double_\#argn
			else
				mov r11d, litval
				movd reg_double_\#argn, r11d
			end if
		\\}
	\}
	macro fast@load_qword_mem args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			mov r11, qword adef
			mov qword[args_start + stack_space - stack_counter], r11
		rept 0\\{\\}rept 1\\{
			mov reg_qword_\#argn, qword adef
		\\}
	\}
	macro fast@load_dword_mem args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			mov r11d, dword adef
			mov dword[args_start + stack_space - stack_counter], r11d
		rept 0\\{\\}rept 1\\{
			mov reg_dword_\#argn, dword adef
		\\}
	\}
	macro fast@load_word_mem args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			movzx r11d, word adef
			mov dword[args_start + stack_space - stack_counter], r11d
		rept 0\\{\\}rept 1\\{
			movzx reg_dword_\#argn, word adef
		\\}
	\}
	macro fast@load_byte_mem args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			movzx r11d, byte adef
			mov dword[args_start + stack_space - stack_counter], r11d
		rept 0\\{\\}rept 1\\{
			movzx reg_dword_\#argn, byte adef
		\\}
	\}
	macro fast@load_double_mem args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			mov r11, qword adef
			mov qword[args_start + stack_space - stack_counter], r11
		rept 0\\{\\}rept 1\\{
			movq reg_double_\#argn, qword adef
		\\}
	\}
	macro fast@load_float_mem args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			mov r11d, dword adef
			mov dword[args_start + stack_space - stack_counter], r11d
		rept 0\\{\\}rept 1\\{
			movd reg_double_\#argn, dword adef
		\\}
	\}
	macro fast@load_real_mem args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		movzx r11d, word adef
		match =GT, cmpres\\{
			mov dword[args_start + stack_space - stack_counter], r11d
		rept 0\\{\\}rept 1\\{
			movd reg_double_\#argn, r11d
		\\}
	\}
	local mem_size
	macro fast@load_unk_mem args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		sizeof@addr mem_size, adef
		match =GT, cmpres\\{
			if mem_size = 8
				mov r11, adef
			else if mem_size = 4
				mov r11d, adef
			else
				movzx r11d, adef
			end if
			mov qword[args_start + stack_space - stack_counter], r11
		rept 0\\{\\}rept 1\\{
			if mem_size = 8
				mov reg_qword_\#argn, adef
			else if mem_size = 4
				mov reg_dword_\#argn, adef
			else
				movzx reg_dword_\#argn, adef
			end if
		\\}
	\}
	macro fast@load_qword_SSE args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			movq qword[args_start + stack_space - stack_counter], adef
		rept 0\\{\\}rept 1\\{
			movq reg_qword_\#argn, adef
		\\}
	\}
	macro fast@load_dword_SSE args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			movd dword[args_start + stack_space - stack_counter], adef
		rept 0\\{\\}rept 1\\{
			movd reg_dword_\#argn, adef
		\\}
	\}
	macro fast@load_double_SSE args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			movq qword[args_start + stack_space - stack_counter], adef
		rept 0\\{\\}rept 1\\{
			if ~reg_double_\#argn eq adef
				movq reg_double_\#argn, adef
			end if
		\\}
	\}
	macro fast@load_float_SSE args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			movd dword[args_start + stack_space - stack_counter], adef
		rept 0\\{\\}rept 1\\{
			if ~reg_double_\#argn eq adef
				movq reg_double_\#argn, adef
			end if
		\\}
	\}
	macro fast@load_qword_GPR args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			mov qword[args_start + stack_space - stack_counter], adef
		rept 0\\{\\}rept 1\\{
			if ~reg_qword_\#argn eq adef
				mov reg_qword_\#argn, adef
			end if
		\\}
	\}
	macro fast@load_dword_GPR args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			mov dword[args_start + stack_space - stack_counter], adef
		rept 0\\{\\}rept 1\\{
			if ~reg_dword_\#argn eq adef
				mov reg_dword_\#argn, adef
			end if
		\\}
	\}
	macro fast@load_word_GPR args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			mov word[args_start + stack_space - stack_counter], adef
		rept 0\\{\\}rept 1\\{
			movzx reg_dword_\#argn, adef
		\\}
	\}
	macro fast@load_byte_GPR args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			mov byte[args_start + stack_space - stack_counter], adef
		rept 0\\{\\}rept 1\\{
			movzx reg_dword_\#argn, adef
		\\}
	\}
	macro fast@load_double_GPR args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			mov qword[args_start + stack_space - stack_counter], adef
		rept 0\\{\\}rept 1\\{
			movq reg_double_\#argn, adef
		\\}
	\}
	macro fast@load_float_GPR args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			mov dword[args_start + stack_space - stack_counter], adef
		rept 0\\{\\}rept 1\\{
			movd reg_double_\#argn, adef
		\\}
	\}
	macro fast@load_double_FPU args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		match =st0, adef\\{rept 0\\{\\}rept 1\\{
			fxch adef
		\\}
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			fstp qword[args_start + stack_space - stack_counter]
		rept 0\\{\\}rept 1\\{
			fstp qword[args_start + stack_space - stack_counter]
			movq reg_double_\#argn, qword[args_start + stack_space - stack_counter]
		\\}
	\}
	macro fast@load_float_FPU args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		match =st0, adef\\{rept 0\\{\\}rept 1\\{
			fxch adef
		\\}
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			fstp dword[args_start + stack_space - stack_counter]
		rept 0\\{\\}rept 1\\{
			fstp dword[args_start + stack_space - stack_counter]
			movd reg_double_\#argn, dword[args_start + stack_space - stack_counter]
		\\}
	\}
	macro fast@load_qword_FPU args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		match =st0, adef\\{rept 0\\{\\}rept 1\\{
			fxch adef
		\\}
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			fstp qword[args_start + stack_space - stack_counter]
		rept 0\\{\\}rept 1\\{
			fstp qword[args_start + stack_space - stack_counter]
			mov reg_qword_\#argn, qword[args_start + stack_space - stack_counter]
		\\}
	\}
	macro fast@load_dword_FPU args_start, stack_space, stack_counter, argn, adef\{
		stack_counter = stack_counter + 8
		match =st0, adef\\{rept 0\\{\\}rept 1\\{
			fxch adef
		\\}
		%preproc_cmp cmpres, argn, 4
		match =GT, cmpres\\{
			fstp dword[args_start + stack_space - stack_counter]
		rept 0\\{\\}rept 1\\{
			fstp dword[args_start + stack_space - stack_counter]
			mov reg_dword_\#argn, dword[args_start + stack_space - stack_counter]
		\\}
	\}
	macro fast@load args_start, stack_space, stack_counter, argn, atype, adef\{
		match _mp1 _mp2, atype\\{
			match _mp3 _mp4, fast@load_\\#_mp1\\#_\\#_mp2 adef\\\{
				_mp3 args_start, stack_space, stack_counter, argn, _mp4
			\\\}
		\\}
	\}
}

rept 1{
	local arg_type, arg_def, cntr, arg_cnt, stack_counter, stack_size
	local _argN
	macro @fastcall proc, [args]\{
		\common
		match , args\\{
			call proc
		rept 0\\{\\} rept 1\\{
			\\local stack_space
			stack_counter = 0
			cntr equ 0
			arg_cnt equ 0
			if stack_space
				stack_size = stack_space
				if stack_size < 32
					stack_size = 32
				else if stack_size and 15
					stack_size = stack_size + 16 - stack_size and 15
				end if
				if defined current@frame
					if current@frame < stack_size
						current@frame = stack_size
					end if
				else
					if stack_space
						lea rsp, [rsp - stack_size]
					end if
				end if
			end if
			\forward
			rept 1 _argN:arg_cnt + 1\\\{
				; restore arg_cnt
				arg_cnt equ _argN
			\\\}
			\reverse
			rept 1 _argN:cntr + 1\\\{
				; restore cntr
				cntr equ _argN
			\\\}
			rept 1 _argN:arg_cnt - cntr + 1\\\{
				_parse@arg arg_type, arg_def, <args>

				fast@load rsp, stack_space, stack_counter, _argN, arg_type, arg_def
			\\\}
			\common
			stack_space = stack_counter
			call proc
			if stack_space & ~defined current@frame
				lea rsp, [rsp + stack_size]
			end if 
		\\}
	\}
	macro @cdeclcall args&\{ @fastcall args \}
	macro @ccall args&\{ @fastcall args \}
	macro @stdcallcall args&\{ @fastcall args \}
	macro @stdcall args&\{ @fastcall args \}
	macro stdcall@retn parambytes\{
		retn
	\}
	macro cdecl@retn parambytes\{
		retn
	\}
	macro c@retn parambytes\{
		retn
	\}
	macro fastcall@retn parambytes\{
		retn
	\}
	macro stdcall@retn parambytes\{
		retn
	\}
}

rept 1{
	macro @get_fast_qword_GPR dest\{
		match =rax, dest\\{rept 0\\{\\}rept 1\\{
			mov dest, rax
		\\}
	\}
	macro @get_fast_dword_GPR dest\{
		match =eax, dest\\{rept 0\\{\\}rept 1\\{
			mov dest, eax
		\\}
	\}
	macro @get_fast_word_GPR dest\{
		match =ax, dest\\{rept 0\\{\\}rept 1\\{
			mov dest, ax
		\\}
	\}
	macro @get_fast_byte_GPR dest\{
		match =al, dest\\{rept 0\\{\\}rept 1\\{
			mov dest, al
		\\}
	\}
	macro @get_fast_double_GPR dest\{
		movq dest, xmm0
	\}
	macro @get_fast_float_GPR dest\{
		movd dest, xmm0
	\}
	macro @get_fast_double_SSE dest\{
		match =xmm0, dest\\{rept 0\\{\\}rept 1\\{
			movq dest, xmm0
		\\}
	\}
	macro @get_fast_float_SSE dest\{
		match =xmm0, dest\\{rept 0\\{\\}rept 1\\{
			movq dest, xmm0
		\\}
	\}
	macro @get_fast_qword_SSE dest\{
		movq dest, rax
	\}
	macro @get_fast_dword_SSE dest\{
		movd dest, eax
	\}
	macro @get_fast_double_FPU dest\{
		procbuf_alloc floatbuf, 8
		movq [floatbuf], xmm0
		fld qword[floatbuf]
		procbuf_free floatbuf
	\}
	macro @get_fast_float_FPU dest\{
		procbuf_alloc floatbuf, 4
		movd [floatbuf], xmm0
		fld dword[floatbuf]
		procbuf_free floatbuf
	\}
	macro @get_fast_sep_qword dest\{
		match _mp1:_mp2, dest\\{
			mov r11, rax
			if ~_mp2 eq eax
				mov _mp2, r11d
			end if
			shr r11, 32
			mov _mp1, r11d
		\\}
	\}
	macro @get_fast_qword_mem dest\{
		mov dest, rax
	\}
	macro @get_fast_dword_mem dest\{
		mov dest, eax
	\}
	macro @get_fast_word_mem dest\{
		mov dest, ax
	\}
	macro @get_fast_byte_mem dest\{
		mov dest, al
	\}
	macro @get_fast_double_mem dest\{
		movq dest, xmm0
	\}
	macro @get_fast_float_mem dest\{
		movd dest, xmm0
	\}
	macro @get_fast_float_mem dest\{
		movd eax, xmm0
		mov word dest, ax
	\}
	local mem_size
	macro @get_fast_unk_mem dest\{
		sizeof@addr mem_size, dest
		if mem_size = 8
			@get_fast_qword_mem dest
		else if mem_size = 4
			@get_fast_dword_mem dest
		else if mem_size = 2
			@get_fast_word_mem dest
		else
			@get_fast_byte_mem dest
		end if
	\}
	local dest_type, dest_def
	local _mp1, _mp2, _mp3, _mp4
	macro @get_result_fast dest\{
		_parse@arg dest_type, dest_def, dest
		match _mp1 _mp2, dest_type\\{
			match _mp3 _mp4, @get_fast_\\#_mp1\\#_\\#_mp2 dest_def\\\{
				_mp3 _mp4
			\\\}
		\\}
	\}
	macro @get_result_std dest&\{@get_result_fast dest\}
	macro @get_result_stdcall args&\{@get_result_fast args\}
	macro @get_result_c dest&\{@get_result_fast dest\}
	macro @get_result_cdecl dest&\{@get_result_fast dest\}
}