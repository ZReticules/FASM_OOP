rept 1{
	local origin, opcode
	macro sizeof@addr result, mem\{
		virtual
			use64
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
			use32
		end virtual
	\}
}

rept 1{
	local _mp1, _mp2
	local _stack_counter
	local arg_type, arg_def, cntr, arg_cnt
	local _argN
	local checkAx

	macro cdecl@va_load args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, atype, adef\{
		; display adef
		match _mp1 _mp2, atype\\{
			match _mp3, cdecl@load_\\#_mp1\\#_\\#_mp2\\\{
				_mp3 args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef
			\\\}
		\\}
	\}
	macro cdecl@load_va_list args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, [adef]\{
		\common
		\local _stack_space 
		stack_counter = stack_counter + 4
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
			cdecl@va_load va@list, _stack_space, _stack_counter, ax_state, ax_final_flags, ax_place, _argN, arg_type, arg_def
		\\}
		\common
		_stack_space = _stack_counter
		if _stack_space > va_list@counter
			va_list@counter = _stack_space
		end if
		ax_state = ax_state or @$AX_DESTROYED
		lea eax, [va@list]
		if ~defined current@frame & args_start eq esp
			push eax
		else
			mov dword[args_start + stack_space - stack_counter], eax
		end if
	\}
	macro cdecl@load_sep_qword args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 8
		match _mp1:_mp2, adef\\{
			if _mp2 eq eax 
				ax_state = ax_state or @$AX_CAN_BE_DEST
				if ax_state and @$AX_DESTROYED
					ax_state = (ax_state or @$AX_NEED_SAVE) xor @$AX_DESTROYED
					mov eax, dword[ax_place]
				end if
			end if
			if ~defined current@frame & args_start eq esp
				push _mp2
			else
				mov dword[args_start + stack_space - stack_counter], _mp2
			end if
			if _mp1 eq eax 
				ax_state = ax_state or @$AX_CAN_BE_DEST
				if ax_state and @$AX_DESTROYED
					ax_state = (ax_state or @$AX_NEED_SAVE) xor @$AX_DESTROYED
					mov eax, dword[ax_place]
				end if
			end if
			if ~defined current@frame & args_start eq esp
				push _mp1
			else
				mov dword[args_start + stack_space - stack_counter + 4], _mp1
			end if
		\\}
	\}
	macro cdecl@load_addr args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		if adef relativeto 0
			if ~defined current@frame & args_start eq esp
				push adef
			else
				mov dword[args_start + stack_space - stack_counter], adef
			end if
		else if adef relativeto eax
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = (ax_state or @$AX_NEED_SAVE) xor @$AX_DESTROYED
				mov eax, dword[ax_place]
			end if
			if ~defined current@frame & args_start eq esp
				push eax
				if adef - eax <> 0
					add dword[esp], adef - eax
				end if
			else
				mov dword[args_start + stack_space - stack_counter], eax
				if adef - eax <> 0
					add dword[args_start + stack_space - stack_counter], adef - eax
				end if
			end if
		irp reg, ecx, edx, ebx, esi, edi, esp, ebp\\{
			else if adef relativeto reg
				if ~defined current@frame & args_start eq esp
					push reg
					if adef - reg <> 0
						add dword[esp], adef - reg
					end if
				else
					mov dword[args_start + stack_space - stack_counter], reg
					if adef - reg <> 0
						add dword[args_start + stack_space - stack_counter], adef - reg
					end if
				end if
		\\}
		else
			has@reg_in_addr checkAx, [adef], ax
			if checkAx
				ax_state = ax_state or @$AX_CAN_BE_DEST
				if ax_state and @$AX_DESTROYED
					ax_state = ax_state or @$AX_NEED_SAVE
					mov eax, dword[ax_place]
				end if
			end if
			ax_state = ax_state or @$AX_DESTROYED
			lea eax, [adef]
			if ~defined current@frame & args_start eq esp
				push eax
			else
				mov dword[args_start + stack_space - stack_counter], eax
			end if
		end if
	\}
	macro cdecl@load_qword_addr args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 8
		if adef relativeto 0
			if ~defined current@frame & args_start eq esp
				push 0
				push adef
			else
				mov dword[args_start + stack_space - stack_counter], adef
				mov dword[args_start + stack_space - stack_counter + 4], 0
			end if
		else if adef relativeto eax
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = (ax_state or @$AX_NEED_SAVE) xor @$AX_DESTROYED
				mov eax, dword[ax_place]
			end if
			if ~defined current@frame & args_start eq esp
				push 0
				push eax
				if adef - eax <> 0
					add dword[esp], adef - eax
				end if
			else
				mov dword[args_start + stack_space - stack_counter], eax
				if adef - eax <> 0
					add dword[args_start + stack_space - stack_counter], adef - eax
				end if
				mov dword[args_start + stack_space - stack_counter + 4], 0
			end if
		irp reg, ecx, edx, ebx, esi, edi, esp, ebp\\{
			else if adef relativeto reg
				if ~defined current@frame & args_start eq esp
					push 0
					push reg
					if adef - reg <> 0
						add dword[esp], adef - reg
					end if
				else
					mov dword[args_start + stack_space - stack_counter], reg
					if adef - reg <> 0
						add dword[args_start + stack_space - stack_counter], adef - reg
					end if
					mov dword[args_start + stack_space - stack_counter + 4], 0
				end if
		\\}
		else
			has@reg_in_addr checkAx, [adef], ax
			if checkAx
				ax_state = ax_state or @$AX_CAN_BE_DEST
				if ax_state and @$AX_DESTROYED
					ax_state = ax_state or @$AX_NEED_SAVE
					mov eax, dword[ax_place]
				end if
			end if
			ax_state = ax_state or @$AX_DESTROYED
			lea eax, [adef]
			if ~defined current@frame & args_start eq esp
				push 0
				push eax
			else
				mov dword[args_start + stack_space - stack_counter], eax
				mov dword[args_start + stack_space - stack_counter + 4], 0
			end if
		end if
	\}
	macro cdecl@load_dword_addr args&\{cdecl@load_addr args\}
	local litval
	macro cdecl@load_lit args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		litval = dword adef
		stack_counter = stack_counter + 4
		if ~defined current@frame & args_start eq esp
			push dword litval
		else
			mov dword[args_start + stack_space - stack_counter], litval
		end if
	\}
	macro cdecl@load_qword_lit args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		litval = qword adef
		stack_counter = stack_counter + 8
		if ~defined current@frame & args_start eq esp
			push dword litval shr 32
			push dword litval and 0xFFFFFFFF
		else
			mov dword[args_start + stack_space - stack_counter], litval and 0xFFFFFFFF
			mov dword[args_start + stack_space - stack_counter + 4], litval shr 32
		end if
	\}
	macro cdecl@load_dword_lit args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		litval = dword adef
		if ~defined current@frame & args_start eq esp
			push dword litval
		else
			mov dword[args_start + stack_space - stack_counter], litval
		end if
	\}
	macro cdecl@load_word_lit args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		litval = word adef
		stack_counter = stack_counter + 4
		if ~defined current@frame & args_start eq esp
			push dword litval
		else
			mov dword[args_start + stack_space - stack_counter], litval
		end if
	\}
	macro cdecl@load_byte_lit args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		litval = byte adef
		stack_counter = stack_counter + 4
		if ~defined current@frame & args_start eq esp
			push dword litval
		else
			mov dword[args_start + stack_space - stack_counter], litval
		end if
	\}
	macro cdecl@load_double_lit args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		litval = qword adef
		stack_counter = stack_counter + 8
		if ~defined current@frame & args_start eq esp
			push dword litval shr 32
			push dword litval and 0xFFFFFFFF
		else
			mov dword[args_start + stack_space - stack_counter], litval and 0xFFFFFFFF
			mov dword[args_start + stack_space - stack_counter + 4], litval shr 32
		end if
	\}
	macro cdecl@load_float_lit args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		litval = dword adef
		stack_counter = stack_counter + 4
		if ~defined current@frame & args_start eq esp
			push dword litval
		else
			mov dword[args_start + stack_space - stack_counter], litval
		end if
	\}
	macro cdecl@load_real_lit args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		litval = word adef
		stack_counter = stack_counter + 4
		if ~defined current@frame & args_start eq esp
			push dword litval
		else
			mov dword[args_start + stack_space - stack_counter], litval
		end if
	\}
	macro cdecl@load_qword_mem args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 8
		has@reg_in_addr checkAx, adef, ax
		if checkAx
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = (ax_state or @$AX_NEED_SAVE) xor @$AX_DESTROYED
				mov eax, dword[ax_place]
			end if
		end if
		if ~defined current@frame & args_start eq esp
			match [_mp1], adef\\{
				push dword[_mp1 + 4]
				push dword[_mp1]
			\\}
		else
			movq xmm5, qword adef
			movq qword[args_start + stack_space - stack_counter], xmm5
		end if
	\}
	macro cdecl@load_dword_mem args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		has@reg_in_addr checkAx, adef, ax
		if checkAx
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = (ax_state or @$AX_NEED_SAVE) xor @$AX_DESTROYED
				mov eax, dword[ax_place]
			end if
		end if
		if ~defined current@frame & args_start eq esp
			push dword adef
		else
			if ax_final_flags and @$AX_CAN_BE_DEST
				movd xmm5, dword adef
				movd dword[args_start + stack_space - stack_counter], xmm5
			else
				mov eax, dword adef
				mov dword[args_start + stack_space - stack_counter], eax
			end if
		end if
	\}
	macro cdecl@load_word_mem args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		has@reg_in_addr checkAx, adef, ax
		if checkAx
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = ax_state or @$AX_NEED_SAVE
				mov eax, dword[ax_place]
			end if
		end if
		if ~defined current@frame & args_start eq esp
			push word 0
			push word adef
		else
			ax_state = ax_state or @$AX_DESTROYED
			movzx eax, word adef
			mov dword[args_start + stack_space - stack_counter], eax
		end if
	\}
	macro cdecl@load_byte_mem args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		has@reg_in_addr checkAx, adef, ax
		if checkAx
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = ax_state or @$AX_NEED_SAVE
				mov eax, dword[ax_place]
			end if
		end if
		ax_state = ax_state or @$AX_DESTROYED
		movzx eax, byte adef
		if ~defined current@frame & args_start eq esp
			push eax
		else
			mov [args_start + stack_space - stack_counter], eax
		end if
	\}
	macro cdecl@load_double_mem args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 8
		has@reg_in_addr checkAx, adef, ax
		if checkAx
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = (ax_state or @$AX_NEED_SAVE) xor @$AX_DESTROYED
				mov eax, dword[ax_place]
			end if
		end if
		if ~defined current@frame & args_start eq esp
			match [_mp1], adef\\{
				push dword[_mp1 + 4]
				push dword[_mp1]
			\\}
		else
			movq xmm5, qword adef
			movq qword[args_start + stack_space - stack_counter], xmm5
		end if
	\}
	macro cdecl@load_float_mem args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		has@reg_in_addr checkAx, adef, ax
		if checkAx
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = (ax_state or @$AX_NEED_SAVE) xor @$AX_DESTROYED
				mov eax, dword[ax_place]
			end if
		end if
		if ~defined current@frame & args_start eq esp
			push dword adef
		else
			if ax_final_flags and @$AX_CAN_BE_DEST
				movd xmm5, dword adef
				movd dword[args_start + stack_space - stack_counter], xmm5
			else
				mov eax, dword adef
				mov dword[args_start + stack_space - stack_counter], eax
				ax_state = ax_state or @$AX_DESTROYED
			end if
		end if
	\}
	macro cdecl@load_real_mem args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		has@reg_in_addr checkAx, adef, ax
		if checkAx
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = ax_state or @$AX_NEED_SAVE
				mov eax, dword[ax_place]
			end if
		end if
		if ~defined current@frame & args_start eq esp
			push word 0
			push word adef
		else
			ax_state = ax_state or @$AX_DESTROYED
			movzx eax, word adef
			mov dword[args_start + stack_space - stack_counter], eax
		end if
	\}
	local origin, opcode, mem_size
	macro cdecl@load_unk_mem args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		sizeof@addr mem_size, adef
		if mem_size = 8
			cdecl@load_qword_mem args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef
		else if mem_size = 4
			cdecl@load_dword_mem args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef
		else if mem_size = 2
			cdecl@load_word_mem args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef
		else
			cdecl@load_byte_mem args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef
		end if
	\}
	macro cdecl@load_qword_SSE args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 8
		if ~defined current@frame & args_start eq esp
			sub esp, 8
			movq qword[esp], adef
		else
			movq qword[args_start + stack_space - stack_counter], adef
		end if
	\}
	macro cdecl@load_dword_SSE args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		if ~defined current@frame & args_start eq esp
			sub esp, 8
			movd dword[esp], adef
		else
			movd dword[args_start + stack_space - stack_counter], adef
		end if
	\}
	macro cdecl@load_double_SSE args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 8
		if ~defined current@frame & args_start eq esp
			sub esp, 8
			movq qword[esp], adef
		else
			movq qword[args_start + stack_space - stack_counter], adef
		end if
	\}
	macro cdecl@load_float_SSE args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		if ~defined current@frame & args_start eq esp
			sub esp, 8
			movd dword[esp], adef
		else
			movd dword[args_start + stack_space - stack_counter], adef
		end if
	\}
	macro cdecl@load_dword_GPR args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		match =eax, adef\\{
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = (ax_state or @$AX_NEED_SAVE) xor @$AX_DESTROYED
				mov eax, dword[ax_place]
			end if
		\\}
		if ~defined current@frame & args_start eq esp
			push adef
		else
			mov dword[args_start + stack_space - stack_counter], adef
		end if
	\}
	macro cdecl@load_word_GPR args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		match =ax, adef\\{
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = (ax_state or @$AX_NEED_SAVE) xor @$AX_DESTROYED
				mov eax, dword[ax_place]
			end if
		\\}
		if ~defined current@frame & args_start eq esp
			push word 0
			push adef
		else
			mov word[args_start + stack_space - stack_counter], adef
		end if
	\}
	macro cdecl@load_byte_GPR args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		match any1 =adef any2, [:] al ah [:]\\{
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = (ax_state or @$AX_NEED_SAVE) xor @$AX_DESTROYED
				mov eax, dword[ax_place]
			end if
		\\}
		if ~defined current@frame & args_start eq esp
			sub esp, 4
			mov byte[esp], adef
		else
			mov byte[args_start + stack_space - stack_counter], adef
		end if
	\}
	macro cdecl@load_float_GPR args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		match =eax, adef\\{
			ax_state = ax_state or @$AX_CAN_BE_DEST
			if ax_state and @$AX_DESTROYED
				ax_state = (ax_state or @$AX_NEED_SAVE) xor @$AX_DESTROYED
				mov eax, dword[ax_place]
			end if
		\\}
		if ~defined current@frame & args_start eq esp
			push adef
		else
			mov dword[args_start + stack_space - stack_counter], adef
		end if
	\}
	macro cdecl@load_double_FPU args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 8
		match =st0, adef\\{rept 0\\{\\}rept 1\\{
			fxch adef
		\\}
		if ~defined current@frame & args_start eq esp
			sub esp, 8
			fstp qword[esp]
		else
			fstp qword[args_start + stack_space - stack_counter]
		end if
	\}
	macro cdecl@load_float_FPU args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		match =st0, adef\\{rept 0\\{\\}rept 1\\{
			fxch adef
		\\}
		if ~defined current@frame & args_start eq esp
			sub esp, 4
			fstp dword[esp]
		else
			fstp dword[args_start + stack_space - stack_counter]
		end if
	\}
	macro cdecl@load_qword_FPU args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 8
		match =st0, adef\\{rept 0\\{\\}rept 1\\{
			fxch adef
		\\}
		if ~defined current@frame & args_start eq esp
			sub esp, 8
			fstp qword[esp]
		else
			fstp qword[args_start + stack_space - stack_counter]
		end if
	\}
	macro cdecl@load_dword_FPU args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, adef\{
		stack_counter = stack_counter + 4
		match =st0, adef\\{rept 0\\{\\}rept 1\\{
			fxch adef
		\\}
		if ~defined current@frame & args_start eq esp
			sub esp, 4
			fstp dword[esp]
		else
			fstp dword[args_start + stack_space - stack_counter]
		end if
	\}
	macro cdecl@load args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, atype, adef\{
		match _mp1 _mp2, atype\\{
			match _mp3 _mp4, cdecl@load_\\#_mp1\\#_\\#_mp2 adef\\\{
				_mp3 args_start, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, argn, _mp4
			\\\}
		\\}
	\}
}

rept 1{
	local arg_type, arg_def, cntr, arg_cnt, stack_counter, stack_size
	local _argN
	local ax_state, ax_place
	macro @cdeclcall proc, [args]\{
		\common
		match , args\\{
			call proc
		rept 0\\{\\} rept 1\\{
			\\local ax_final_flags, stack_space
			if ax_final_flags and @$AX_NEED_SAVE
				procbuf_alloc ax_place, 4
				mov dword[ax_place], eax
			end if
			stack_counter = 0
			ax_state = 0
			cntr equ 0
			arg_cnt equ 0
			if stack_space
				stack_size = stack_space
				if defined current@frame
					if current@frame < stack_size
						current@frame = stack_size
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
				cdecl@load esp, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, _argN, arg_type, arg_def
			\\\}
			\common
			if ax_final_flags and @$AX_NEED_SAVE
				procbuf_free ax_place
			end if
			stack_space = stack_counter
			call proc
			if stack_space & ~defined current@frame
				lea esp, [esp + stack_size]
			end if 
			ax_final_flags = ax_state and (@$AX_NEED_SAVE or @$AX_CAN_BE_DEST)
			; if ax_state and @$AX_NEED_SAVE
			; 	ax_final_flags = 1
			; else
			; 	ax_final_flags = 0
			; end if
		\\}
	\}
	macro @stdcall proc, [args]\{
		\common
		match , args\\{
			call proc
		rept 0\\{\\} rept 1\\{
			\\local ax_final_flags, stack_space
			if ax_final_flags and @$AX_NEED_SAVE
				procbuf_alloc ax_place, 4
				mov [ax_place], eax
			end if
			stack_counter = 0
			ax_state = 0
			cntr equ 0
			arg_cnt equ 0
			if stack_space
				stack_size = stack_space
				if defined current@frame
					if current@frame < stack_size
						current@frame = stack_size
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
				cdecl@load esp, stack_space, stack_counter, ax_state, ax_final_flags, ax_place, _argN, arg_type, arg_def
			\\\}
			\common
			if ax_final_flags and @$AX_NEED_SAVE
				procbuf_free ax_place
			end if
			stack_space = stack_counter
			call proc
			if stack_space & defined current@frame
				lea esp, [esp - stack_size]
			end if 
			ax_final_flags = ax_state and (@$AX_NEED_SAVE or @$AX_CAN_BE_DEST)
			; if ax_state and @$AX_NEED_SAVE
			; 	ax_final_flags = 1
			; else
			; 	ax_final_flags = 0
			; end if
		\\}
	\}
	macro @ccall args&\{ @cdeclcall args \}
	macro @stdcallcall args&\{ @stdcall args \}
	macro stdcall@retn parambytes\{
		retn parambytes
	\}
	macro cdecl@retn parambytes\{
		retn
	\}
	macro c@retn parambytes\{
		retn
	\}
}

rept 1{
	macro @get_std_dword_GPR dest\{
		match =eax, dest\\{rept 0\\{\\}rept 1\\{
			mov dest, eax
		\\}
	\}
	macro @get_std_word_GPR dest\{
		match =ax, dest\\{rept 0\\{\\}rept 1\\{
			mov dest, ax
		\\}
	\}
	macro @get_std_byte_GPR dest\{
		match =al, dest\\{rept 0\\{\\}rept 1\\{
			mov dest, al
		\\}
	\}
	local floatbuf
	macro @get_std_float_GPR dest\{
		procbuf_alloc floatbuf, 4
		fstp dword[floatbuf]
		mov dest, dword[floatbuf]
		procbuf_free floatbuf
	\}
	macro @get_std_double_SSE dest\{
		procbuf_alloc floatbuf, 8
		fstp qword[floatbuf]
		movq dest, qword[floatbuf]
		procbuf_free floatbuf
	\}
	macro @get_std_float_SSE dest\{
		procbuf_alloc floatbuf, 4
		fstp dword[floatbuf]
		movd dest, dword[floatbuf]
		procbuf_free floatbuf
	\}
	macro @get_std_qword_SSE dest\{
		movd dest, eax
		pinsrd dest, edx, 1
	\}
	macro @get_std_dword_SSE dest\{
		movd dest, eax
	\}
	macro @get_std_double_FPU dest\{\}
	macro @get_std_float_FPU dest\{\}
	macro @get_std_sep_qword dest\{
		match =eax:=edx, dest\\{
			xchg eax, edx
		rept 0\\{\\}match _mp1:_mp2, dest\\{
			if ~_mp2 eq edx
				if ~_mp2 eq eax
					mov _mp2, eax
				end if
				if ~_mp1 eq edx
					mov _mp1, edx
				end if
			else
				mov _mp1, edx
				mov _mp2, eax
			end if
		\\}
	\}
	local _mp1
	macro @get_std_qword_mem dest\{
		match [_mp1], dest\\{
			mov dword[_mp1], eax
			mov dword[_mp1 + 4], edx
		\\}
	\}
	macro @get_std_dword_mem dest\{
		mov dest, eax
	\}
	macro @get_std_word_mem dest\{
		mov dest, ax
	\}
	macro @get_std_byte_mem dest\{
		mov dest, al
	\}
	macro @get_std_double_mem dest\{
		fstp qword dest
	\}
	macro @get_std_float_mem dest\{
		fstp dword dest
	\}
	macro @get_std_real_mem dest\{
		procbuf_alloc floatbuf, 4
		fstp dword[floatbuf]
		movd xmm0, dword[floatbuf]
		vcvtps2ph xmm0, xmm0, 0
		movd eax, xmm0
		mov word dest, ax
		procbuf_free floatbuf
	\}
	local mem_size
	macro @get_std_unk_mem dest\{
		sizeof@addr mem_size, dest
		if mem_size = 8
			@get_std_qword_mem dest
		else if mem_size = 4
			@get_std_dword_mem dest
		else if mem_size = 2
			@get_std_word_mem dest
		else
			@get_std_byte_mem dest
		end if
	\}
	local dest_type, dest_def
	macro @get_result_std dest\{
		_parse@arg dest_type, dest_def, dest
		match _mp1 _mp2, dest_type\\{
			match _mp3 _mp4, @get_std_\\#_mp1\\#_\\#_mp2 dest_def\\\{
				_mp3 _mp4
			\\\}
		\\}
	\}
	macro @get_result_stdcall args&\{@get_result_std args\}
	macro @get_result_c dest&\{@get_result_std dest\}
	macro @get_result_cdecl dest&\{@get_result_std dest\}
}