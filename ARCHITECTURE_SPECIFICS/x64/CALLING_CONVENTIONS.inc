__std_call equ @oop

__obj_call equ @oop

__struc_call equ @oop

_oop_default fix

macro static_rsp_close procname,flag,parmbytes,localbytes,reglist { 
	size@frame = current@frame 
	restore size@frame, current@frame, localbase@proc, regsbase@proc, parmbase@proc, framebytes@proc
}

macro proc_noprologue{
	prologue@proc equ static_rsp_prologue
	epilogue@proc equ static_rsp_epilogue
	close@proc equ static_rsp_close
}

macro proc_resprologue{
	restore prologue@proc 
	restore epilogue@proc 
	restore close@proc 
}


macro va_list@load counter, ax_destroyed, ax_restored, ax_need_save, size@param, type@param, param{
	if type@param eq addr
		repeat 1
			if param eqtype 0
				if param relativeto 0
					mov dword[va@list + counter], param
					break
				end if
				if param relativeto rax & param - rax relativeto 0 & param - rax = 0
					if ax_destroyed & ~(ax_restored = 1)
						mov rax, [psp - 8]
						ax_restored = 1
					end if
					mov qword[va@list + counter], eax
					break
				end if
				irp reg, rcx, rdx, rbx, rsp, rbp, rsi, rdi, r8, r9, r10, r11, r12, r13, r14, r15 \{

				if param relativeto reg & param - reg relativeto 0 & param - reg = 0
					mov qword[va@list + counter], reg
					break
				end if

				\}
			end if
			if param eqtype rax
				if param eq rax & ax_destroyed & ~(ax_restored = 1)
					mov rax, [rsp - 8]
					ax_restored = 1
				end if
				mov qword[va@list + counter], param
				break
			end if
			reg_in_addr check_reg, ax, param
			if check_reg & ax_destroyed & ~(ax_restored = 1)
				mov rax, [psp - 8]
				ax_restored = 0
			else if ax_need_save & ~ax_destroyed
				mov [psp - 8], rax
			end if
			ax_destroyed = 1
			lea rax, [param]
			mov qword[va@list + counter], rax
		end repeat
	else if type@param eq sep_qword
		match _high:_low, param\{
			if (_high eq eax | _low eq eax) & ax_destroyed & ~(ax_restored = 1)
				mov rax, [rsp - 8]
				ax_restored = 1
			end if
			mov dword[va@list + counter], _low
			mov dword[va@list + counter + 4], _high
		\}
	else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
		movq qword[va@list + counter], param
	else if param eqtype [0] | param eqtype byte [0]
		match any1[addr], any1 param\{
			reg_in_addr check_reg, ax, addr
			if check_reg & ax_destroyed & ~(ax_restored = 1)
				mov rax, [rsp - 8]
				ax_restored = 0
			else if ax_need_save & ~ax_destroyed
				mov [rsp - 8], rax
			end if
			ax_destroyed = 1
		\}
		if size@param = 8
			mov rax, qword param
		else if size@param = 4
			mov eax, param
		else if size@param = 2
			if param eqtype byte 0
				mov eax, param
			else if param eqtype byte 0f
				virtual at $
					origin = $
					match __pref val, param\{
						dw val
					\}
					load opcode word from origin
				end virtual
				mov eax, opcode
			else
				movzx eax, param
			end if
		else
			movzx eax, param
		end if
		mov qword[va@list + counter],rax
	else if size@param = 8
		virtual
			origin = $
			mov rax,param
			load opcode byte from origin+1
		end virtual
		if opcode = 0B8h
			if ax_need_save & ~ax_destroyed
				mov [rsp - 8], rax
			end if
			ax_destroyed = 1
			mov rax,param
			mov qword[va@list + counter],rax
		else
			if param eq rax & ax_destroyed & ~(ax_restored = 1)
				mov rax, [rsp - 8]
				ax_restored = 1
			end if
			mov qword[va@list + counter],param
		end if
	else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
		movq qword[va@list + counter], param
	else
		if (param eq rax | param eq rax) & ax_destroyed & ~(ax_restored = 1)
			mov rax, [rsp - 8]
			ax_restored = 1
		end if
		mov qword[va@list + counter], param
	end if
}

macro @oopcall proc,[arg]{ 
	common 
	local stackspace,..argscount,counter, args@count
	args@count equ 0
	if ..argscount < 4
		stackspace = 4*8
	else if ..argscount and 1
		stackspace = (..argscount+1)*8
	else
		stackspace = ..argscount*8
	end if
	counter = 0
	if stackspace
		if defined current@frame
			if current@frame<stackspace
				current@frame = stackspace
			end if
		else
			if stackspace
				lea rsp, [rsp - stackspace]
			end if
		end if
	end if

	local ax_destroyed, ax_restored, ..ax_save, ax_need_save, check_reg
	ax_destroyed = 0
	ax_restored = 2

	reverse
	counter = counter + 1
	define type@param
	define definition@param arg
	param@parser type@param, definition@param
	match =va_list|_va_list|param, type@param|va@list|definition@param\{
		va_list@handler ax_destroyed, ax_restored, ax_need_save, param
		type@param equ addr
		define definition@param _va_list
	\}
	match param,definition@param\{ 
		local opcode,origin
		size@param = 0
		if type@param eq sep_qword
			size@param = 8
		else if type@param eq addr | type@param eq float
			size@param = 4
		else
			if param eqtype 0 | param eqtype 0f
				size@param = 8
			else if param eqtype byte 0 | param eqtype byte 0f
				match prefix value,definition@param\\{ 
					if prefix eq qword | prefix eq double
						size@param = 8
					else if prefix eq dword | prefix eq float
						size@param = 4
					else if prefix eq word
						size@param = 2
					else if prefix eq byte
						size@param = 1
					end if 
				\\}
			else if ~ param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				virtual
					; display type@param
					origin = $	
					if type@param eq qword
						inc qword param
					else
						inc param
					end if
					load opcode byte from origin
					if opcode = 67h | opcode = 41h
						load opcode byte from origin+1
					end if
					if opcode and 0F8h = 48h
						size@param = 8
					else if opcode = 66h
						size@param = 2
					else if opcode = 0FFh
						size@param = 4
					else
						size@param = 1
					end if
				end virtual
			end if
		end if

		if ..argscount-counter+1 = 1
			if type@param eq float | type@param eq double
				if ~ param eq xmm0
					if size@param = 4
						if param eqtype byte 0 | param eqtype byte 0f | param eqtype 0 | param eqtype 0f
							if ax_need_save & ~ax_destroyed
								mov [rsp - 8], rax
							end if
							ax_destroyed = 1
							mov eax, dword param
							movd xmm0,eax
						else
							movd xmm0, dword param
						end if
					else
						if param eqtype 0 | param eqtype 0f | param eqtype byte 0 | param eqtype byte 0f
							if ax_need_save & ~ax_destroyed
								mov [rsp - 8], rax
							end if
							ax_destroyed = 1
							mov rax, qword param
							movq xmm0,rax
						else
							if ~param in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15>
								movq xmm0, qword param
							else if ~param eq xmm0
								movsd xmm0, param
							end if
						end if
					end if
				end if
				if vararg@fastcall & ~ param eq rcx
					movq rcx,xmm0
				end if
			else if type@param eq addr
				if ~ param eq rcx
					reg_in_addr check_reg, ax, param
					if check_reg & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					lea rcx, [param]
				end if
			else if type@param eq sep_qword
				match _high:_low, param\\{
					mov dword[rsp + (..argscount - counter) * 8], _low
					mov dword[rsp + (..argscount-counter) * 8 + 4], _high
				\\}
				mov rcx, qword[rsp + (..argscount - counter) * 8]
			else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				movq rcx, param
			else if size@param = 8
				if ~ param eq rcx
					if param eq rax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					if param eqtype [0]
						mov rcx, qword param
					else
						mov rcx, param
					end if
				end if
			else if size@param = 4
				if ~ param eq ecx
					if param eq eax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					mov ecx,param
				end if
			else if size@param = 2
				if ~ param eq cx
					if param eq ax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					if param eqtype byte 0
						mov ecx, param
					else if param eqtype byte 0f
						virtual at $
							origin = $
							match __pref val, param\\{
								dw val
							\\}
							load opcode word from origin
						end virtual
						mov ecx, opcode
					else
						movzx ecx, param
					end if
				end if
			else if size@param = 1
				if ~ param eq cl
					if (param eq al | param eq ah) & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					movzx ecx,param
				end if
			end if
		else if ..argscount-counter+1 = 2
			if type@param eq float | type@param eq double
				if ~ param eq xmm1
					if size@param = 4
						if param eqtype byte 0 | param eqtype byte 0f | param eqtype 0 | param eqtype 0f
							if ax_need_save & ~ax_destroyed
								mov [rsp - 8], rax
							end if
							ax_destroyed = 1
							mov eax, dword param
							movd xmm1,eax
						else
							movd xmm1, dword param
						end if
					else
						if param eqtype 0 | param eqtype 0f | param eqtype byte 0 | param eqtype byte 0f
							if ax_need_save & ~ax_destroyed
								mov [rsp - 8], rax
							end if
							ax_destroyed = 1
							mov rax, qword param
							movq xmm1,rax
						else
							if ~param in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15>
								movq xmm1, qword param
							else if ~param eq xmm1
								movsd xmm1, param
							end if
						end if
					end if
				end if
				if vararg@fastcall & ~ param eq rdx
					movq rdx,xmm1
				end if
			else if type@param eq addr
				if ~ param eq rdx
					reg_in_addr check_reg, ax, param
					if check_reg & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					lea rdx, [param]
				end if
			else if type@param eq sep_qword
				match _high:_low, param\\{
					mov dword[rsp + (..argscount - counter) * 8], _low
					mov dword[rsp + (..argscount-counter) * 8 + 4], _high
				\\}
				mov rdx, qword[rsp + (..argscount - counter) * 8]
			else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				movq rdx, param
			else if size@param = 8
				if ~ param eq rdx
					if param eq rax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					if param eqtype [0]
						mov rdx, qword param
					else
						mov rdx, param
					end if
				end if
			else if size@param = 4
				if ~ param eq edx
					if param eq eax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					mov edx,param
				end if
			else if size@param = 2
				if ~ param eq dx
					if param eq ax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					if param eqtype byte 0
						mov edx, param
					else if param eqtype byte 0f
						virtual at $
							origin = $
							match __pref val, param\\{
								dw val
							\\}
							load opcode word from origin
						end virtual
						mov edx, opcode
					else
						movzx edx, param
					end if
				end if
			else if size@param = 1
				if ~ param eq dl
					if (param eq al | param eq ah) & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					movzx edx,param
				end if
			end if
		else if ..argscount-counter+1 = 3
			if type@param eq float | type@param eq double
				if ~ param eq xmm2
					if size@param = 4
						if param eqtype byte 0 | param eqtype byte 0f | param eqtype 0 | param eqtype 0f
							if ax_need_save & ~ax_destroyed
								mov [rsp - 8], rax
							end if
							ax_destroyed = 1
							mov eax, dword param
							movd xmm2,eax
						else
							movd xmm2, dword param
						end if
					else
						if param eqtype 0 | param eqtype 0f | param eqtype byte 0 | param eqtype byte 0f
							if ax_need_save & ~ax_destroyed
								mov [rsp - 8], rax
							end if
							ax_destroyed = 1
							mov rax, qword param
							movq xmm2,rax
						else
							if ~param in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15>
								movq xmm2, qword param
							else if ~param eq xmm2
								movsd xmm2, param
							end if
						end if
					end if
				end if
				if vararg@fastcall & ~ param eq r8
					movq r8,xmm2
				end if
			else if type@param eq addr
				if ~ param eq r8
					reg_in_addr check_reg, ax, param
					if check_reg & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					lea r8, [param]
				end if
			else if type@param eq sep_qword
				match _high:_low, param\\{
					mov dword[rsp + (..argscount - counter) * 8], _low
					mov dword[rsp + (..argscount-counter) * 8 + 4], _high
				\\}
				mov r8, qword[rsp + (..argscount - counter) * 8]
			else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				movq r8, param
			else if size@param = 8
				if ~ param eq r8
					if param eq rax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					if param eqtype [0]
						mov r8, qword param
					else
						mov r8, param
					end if
				end if
			else if size@param = 4
				if ~ param eq r8d
					if param eq eax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					mov r8d,param
				end if
			else if size@param = 2
				if ~ param eq r8w
					if param eq ax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					if param eqtype byte 0
						mov r8d, param
					else if param eqtype byte 0f
						virtual at $
							origin = $
							match __pref val, param\\{
								dw val
							\\}
							load opcode word from origin
						end virtual
						mov r8d, opcode
					else
						movzx r8d, param
					end if
				end if
			else if size@param = 1
				if ~ param eq r8b
					if (param eq al | param eq ah) & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					movzx r8d,param
				end if
			end if
		else if ..argscount-counter+1 = 4
			if type@param eq float  | type@param eq double
				if ~ param eq xmm3
					if size@param = 4
						if param eqtype byte 0 | param eqtype byte 0f | param eqtype 0 | param eqtype 0f
							if ax_need_save & ~ax_destroyed
								mov [rsp - 8], rax
							end if
							ax_destroyed = 1
							mov eax,param
							movd xmm3,eax
						else
							movd xmm3,param
						end if
					else
						if param eqtype 0 | param eqtype 0f | param eqtype byte 0 | param eqtype byte 0f
							if ax_need_save & ~ax_destroyed
								mov [rsp - 8], rax
							end if
							ax_destroyed = 1
							mov rax,param
							movq xmm3,rax
						else
							if ~param in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15>
								movq xmm3,param
							else if ~param eq xmm3
								movsd xmm3, param
							end if
						end if
					end if
				end if
				if vararg@fastcall & ~ param eq r9
					movq r9,xmm3
				end if
			else if type@param eq addr
				if ~ param eq r9
					reg_in_addr check_reg, ax, param
					if check_reg & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					lea r9, [param]
				end if
			else if type@param eq sep_qword
				match _high:_low, param\\{
					mov dword[rsp + (..argscount - counter) * 8], _low
					mov dword[rsp + (..argscount-counter) * 8 + 4], _high
				\\}
				mov r9, qword[rsp + (..argscount - counter) * 8]
			else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				movq r9, param
			else if size@param = 8
				if ~ param eq r9
					if param eq rax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					if param eqtype [0]
						mov r9, qword param
					else
						mov r9, param
					end if
				end if
			else if size@param = 4
				if ~ param eq r9d
					if param eq eax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					mov r9d,param
				end if
			else if size@param = 2
				if ~ param eq r9w
					if param eq ax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					if param eqtype byte 0
						mov r9d, param
					else if param eqtype byte 0f
						virtual at $
							origin = $
							match __pref val, param\\{
								dw val
							\\}
							load opcode word from origin
						end virtual
						mov r9d, opcode
					else
						movzx r9d, param
					end if
				end if
			else if size@param = 1
				if ~ param eq r9b
					if (param eq al | param eq ah) & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					movzx r9d, param
				end if
			end if
		else
			if type@param eq addr
				repeat 1
					if param eqtype 0
						if param relativeto 0
							mov dword[rsp + (..argscount - counter) * 8], param
							break
						end if
						if param relativeto rax & param - rax relativeto 0 & param - rax = 0
							if ax_destroyed & ~(ax_restored = 1)
								mov rax, [psp - 8]
								ax_restored = 1
							end if
							mov qword[rsp + (..argscount - counter) * 8], pax
							break
						end if
						irp reg, rcx, rdx, rbx, rsp, rbp, rsi, rdi, r8, r9, r10, r11, r12, r13, r14, r15 \\{

						if param relativeto reg & param - reg relativeto 0 & param - reg = 0
							mov qword[rsp + (..argscount - counter) * 8], reg
							break
						end if

						\\}
					end if
					if param eqtype rax
						if param eq rax & ax_destroyed & ~(ax_restored = 1)
							mov rax, [rsp - 8]
							ax_restored = 1
						end if
						mov qword[rsp + (..argscount - counter) * 8], param
						break
					end if
					reg_in_addr check_reg, ax, param
					if check_reg & ax_destroyed & ~(ax_restored = 1)
						mov rax, [psp - 8]
						ax_restored = 0
					else if ax_need_save & ~ax_destroyed
						mov [psp - 8], rax
					end if
					ax_destroyed = 1
					lea rax, [param]
					mov qword[rsp + (..argscount - counter) * 8], rax
				end repeat
			else if type@param eq sep_qword
				match _high:_low, param\\{
					if (_high eq eax | _low eq eax) & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					mov dword[rsp + (..argscount - counter) * 8], _low
					mov dword[rsp + (..argscount - counter) * 8 + 4], _high
				\\}
			else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				movq [rsp+(..argscount-counter)*8], param
			else if param eqtype [0] | param eqtype byte [0]
				match any1[addr], any1 param\\{
					reg_in_addr check_reg, ax, addr
					if check_reg & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 0
					else if ax_need_save & ~ax_destroyed
						mov [rsp - 8], rax
					end if
					ax_destroyed = 1
				\\}
				if size@param = 8
					mov rax, qword param
				else if size@param = 4
					mov eax, param
				else if size@param = 2
					if param eqtype byte 0
						mov eax, param
					else if param eqtype byte 0f
						virtual at $
							origin = $
							match __pref val, param\\{
								dw val
							\\}
							load opcode word from origin
						end virtual
						mov eax, opcode
					else
						movzx eax, param
					end if
				else
					movzx eax, param
				end if
				mov [rsp+(..argscount-counter)*8],rax
			else if size@param = 8
				virtual
					origin = $
					mov rax,param
					load opcode byte from origin+1
				end virtual
				if opcode = 0B8h
					if ax_need_save & ~ax_destroyed
						mov [rsp - 8], rax
					end if
					ax_destroyed = 1
					mov rax,param
					mov [rsp+(..argscount-counter)*8],rax
				else
					if param eq rax & ax_destroyed & ~(ax_restored = 1)
						mov rax, [rsp - 8]
						ax_restored = 1
					end if
					mov qword [rsp+(..argscount-counter)*8],param
				end if
			else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				movq [rsp+(..argscount-counter)*8],param
			else
				if (param eq rax | param eq rax) & ax_destroyed & ~(ax_restored = 1)
					mov rax, [rsp - 8]
					ax_restored = 1
				end if
				mov [rsp+(..argscount-counter)*8],param
			end if
		end if 
	\}
	args@count equ args@count+1
	common
	if ax_restored = 2
		ax_need_save = 0
	else
		ax_need_save = 1
	end if
	local _counter
	rept 1 _counter:args@count\{
		__argscount__ equ _counter
	\}
	..argscount = counter
	call proc
	if stackspace & ~defined current@frame
		lea rsp, [rsp + stackspace]
	end if 
}

macro @@oopcall args&{@oopcall args}

macro @jret argums&{
	local cntargs, cntcall, cntrcall, ..lastpoint, has_jret
	has_jret = 0
	cntargs = 0
	cntrcall = 0
	match any =(valargs=), argums\{
		local any
		irp any,valargs\\{
			cntargs = cntargs+1
		\\} 
	\}
	local stub, rep@byte, no_frame
	stub = 0
	current@frame equ stub
	macro call argum\{
		\local .._lastpoint
		cntrcall = cntrcall + 1
		if __argscount__ > 4
			display "Too many arguments for @jret, use @call"
			err
		end if
		if (.._lastpoint = ..lastpoint)
			has_jret = 1
			\local .startcode, ._start, ._end, rsize
			virtual at 0
			    .startcode::
			    ._start:
			   	ret
			   	._end:
			   	rsize = ._end-._start
			end virtual
			repeat ._end-._start-1
				load rep@byte byte from .startcode:%-1
				db rep@byte
			end repeat
			jmp argum
		else
			call argum
		end if
		.._lastpoint:
	\}
	@call argums
	purge call
	..lastpoint:
	cntcall = cntrcall
	if has_jret = 0
		ret
	end if
	restore current@frame
}

macro fill@dest_proc dest{
	local type, definition, size_param
	local origin, opcode
	define definition dest
	param@parser type, definition
	size_param = 4

	if type eq double | type eq qword | type eq sep_qword
		size_param = 8
	else if definition eqtype byte 0 | definition eqtype byte 0f | definition eqtype byte [0]
		match prefix value, definition\{ 
			if prefix eq qword
				size_param = 8
			else if prefix eq dword
				size_param = 4
			else if prefix eq word
				size_param = 2
			else if prefix eq byte
				size_param = 1
			end if 
		\}
	else if ~ definition in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
		virtual
			origin = $	
			if type eq qword
				inc qword definition
			else
				inc definition
			end if
			load opcode byte from origin
			if opcode = 67h | opcode = 41h
				load opcode byte from origin + 1
			end if
			if opcode and 0F8h = 48h
				size_param = 8
			else if opcode = 66h
				size_param = 2
			else if opcode = 0FFh
				size_param = 4
			else
				size_param = 1
			end if
		end virtual
	end if

	if type eq sep_qword
		match _high:_low, definition\{
			if ~_low eq eax
				mov _low, eax
			end if
			rol rax, 32
			if ~_high eq eax
				mov _high, eax
				rol rax, 32
			end if
		\}
	else if definition eqtype [0] | definition eqtype byte [0]
		if size_param = 8
			if type eq double
				fld qword definition
			else
				mov definition, rax
			end if
		else if size_param = 4
			if type eq float
				fld definition
			else
				mov definition, eax
			end if
		else if size_param = 2
			mov definition, ax
		else if size_param = 1
			mov definition, al
		end if
	else if definition in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
		if size_param = 4
			if type eq float
				if ~definition eq xmm0
					movd definition, xmm0
				end if
			else
				movd definition, eax
			end if
		else if size_param = 8
			if type eq double
				if ~definition eq xmm0
					movq definition, xmm0
				end if
			else
				movq definition, rax
			end if
		end if
	else
		mov definition, rax
	end if
}

macro __virtobj@call object, type, procName, methTable{
	mov r11, [rcx + type#.VTable]
	call [r11 + methTable#.#procName - methTable]
} 