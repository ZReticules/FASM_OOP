__std_call equ std

__obj_call equ c

__struc_call equ c

_oop_default fix c

macro static_esp_prologue procname,flag,parmbytes,localbytes,reglist { 
	local counter,loc,frame,current 
	counter = 0 
	irps reg, reglist \{ 
		push reg 
		counter = counter+1 
	\} 
	loc = (localbytes+3) and (not 3) 
	if frame & (counter+loc shr 3+1) and 1 
		loc = loc + 4 
	end if 
	framebytes@proc equ frame+loc 
	if framebytes@proc 
		sub esp,framebytes@proc 
	end if 
	localbase@proc equ esp + frame 
	regsbase@proc equ esp + frame+loc 
	parmbase@proc equ esp + frame+loc+counter*4+4 
	current = 0 
	current@frame equ current 
	size@frame equ frame 
} 

macro static_esp_epilogue procname,flag,parmbytes,localbytes,reglist { 
	if framebytes@proc 
		add esp,framebytes@proc 
	end if 
	irps reg, reglist \{ 
		\reverse 
		pop reg 
	\} 
   if flag and 10000b
		retn
   else
    	retn parmbytes
   end if
} 

macro static_esp_close procname,flag,parmbytes,localbytes,reglist { 
	size@frame = current@frame 
	restore size@frame,current@frame 
}

macro proc_noprologue{
	prologue@proc equ static_esp_prologue
	epilogue@proc equ static_esp_epilogue
	close@proc equ static_esp_close
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
				if param relativeto eax & param - eax relativeto 0 & param - eax = 0
					if ax_destroyed & ~(ax_restored = 1)
						mov eax, [psp - 4]
						ax_restored = 1
					end if
					mov dword[va@list + counter], eax
					break
				end if
				irp reg, ecx, edx, ebx, esp, ebp, esi, edi \{

				if param relativeto reg & param - reg relativeto 0 & param - reg = 0
					mov dword[va@list + counter], reg
					break
				end if

				\}
			end if
			if param eqtype eax
				if param eq eax & ax_destroyed & ~(ax_restored = 1)
					mov eax, [psp - 4]
					ax_restored = 1
				end if
				mov dword[va@list + counter], param
				break
			end if
			reg_in_addr check_reg, ax, param
			if check_reg & ax_destroyed & ~(ax_restored = 1)
				mov eax, [psp - 4]
				ax_restored = 0
			else if ax_need_save & ~ax_destroyed
				mov [psp - 4], eax
			end if
			ax_destroyed = 1
			lea eax,[param]
			mov dword[va@list + counter], eax
		end repeat
	else if type@param eq sep_qword
		match _high:_low, param\{
			if (_high eq eax | _low eq eax) & ax_destroyed & ~(ax_restored = 1)
				mov eax, [psp - 4]
				ax_restored = 1
			end if
			mov dword[va@list + counter], _low
			mov dword[va@list + counter + 4], _high
		\}
	else if param eqtype [0] | param eqtype byte [0]
		if size@param = 8
			movq xmm0, qword param
			movq qword[va@list + counter], xmm0
		else 
			match any1[addr], any1 param\{
				reg_in_addr check_reg, ax, addr
				if check_reg & ax_destroyed & ~(ax_restored = 1)
					mov eax, [psp - 4]
					ax_restored = 0
				else if ax_need_save & ~ax_destroyed
					mov [psp - 4], eax
				end if
				ax_destroyed = 1
			\}
			if size@param = 4
				mov eax,param
				mov dword[va@list + counter],eax
			else if size@param <> 0
				if ~(param eq al|param eq ax)
					movzx eax,param
				end if
				mov dword[va@list + counter], eax
			end if
		end if
	else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
		if size@param = 4
			movd dword[va@list + counter],param
		else if size@param = 8
			movq dword[va@list + counter],param
		end if
	else
		if size@param = 4
			if param eq eax & ax_destroyed & ~(ax_restored = 1)
				mov eax, [psp - 4]
				ax_restored = 1
			end if
			mov dword[va@list + counter],param
		else if size@param = 8
			if param eqtype 0
				mov dword[va@list + counter + 4], (param) shr 32
				mov dword[va@list + counter], (param) and 0FFFFFFFFh
			else
				virtual at $
					origin = $
					dq param
					load opcode qword from origin
				end virtual
				mov dword[va@list + counter + 4], (opcode) shr 32
				mov dword[va@list + counter], (opcode) and 0FFFFFFFFh
			end if
		else
			if (param eq ax | param eq al | param eq ah) & ax_destroyed & ~(ax_restored = 1)
				mov eax, [psp - 4]
				ax_restored = 1
			end if
			if size@param = 2
				mov word[va@list + counter], param
			else
				mov byte[va@list + counter], param
			end if
		end if
	end if

}

macro base@call clean_stack, action, proc, [arg]{
	common 
	local stackspace,..argscount,counter, args@count
	args@count equ 0
	stackspace = ..argscount*4
	counter = 0
	if stackspace & defined current@frame & current@frame < stackspace
		current@frame = stackspace
	end if
	match any, arg\{

		local ax_destroyed, ax_restored, ax_need_save, check_reg, opcode,origin
		ax_destroyed = 0
		ax_restored = 2

		reverse
		counter = counter + 1
		define type@param
		define definition@param arg
		param@parser type@param, definition@param
		match =va_list|_va_list|param, type@param|va@list|definition@param\\{
			va_list@handler ax_destroyed, ax_restored, ax_need_save, param
			type@param equ addr
			define definition@param _va_list
		\\}
		match param,definition@param\\{ 
			size@param = 4
			if param eq NONE
				size@param = 0
				if type@param eq double | type@param eq qword
					counter = counter + 1
				end if
			else if type@param eq double | type@param eq qword | type@param eq sep_qword
				size@param = 8
				counter = counter + 1
			else if param eqtype 0 | param eqtype 0f | type@param eq addr | type@param eq float
				size@param = 4
			else if param eqtype byte 0 | param eqtype byte 0f | param eqtype byte [0]
				match prefix value,definition@param\\\{ 
					if prefix eq qword
						size@param = 8
						counter = counter + 1
					else if prefix eq dword
						size@param = 4
					else if prefix eq word
						size@param = 2
					else if prefix eq byte
						size@param = 1
					end if 
				\\\}
			else if ~ param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
				virtual
					use64
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
						counter = counter + 1
					else if opcode = 66h
						size@param = 2
					else if opcode = 0FFh
						size@param = 4
					else
						size@param = 1
					end if
					use32
				end virtual
			end if
			; else if param in <eax, ebx, ecx, edx, ebp, esp, edi, esi>
			; 	size@param = 4
			; else if param in <ax, bx, cx, dx, bp, sp, di, si>
			; 	size@param = 2
			; else if param in <al, ah, bl, bh, cl, ch, dl, dh, bp, sp, di, si>
			; 	size@param = 1
			; end if


			if defined current@frame
				if type@param eq addr
					repeat 1
						if param eqtype 0
							if param relativeto 0
								mov dword[esp+(..argscount-counter)*4], param
								break
							end if
							if param relativeto eax & param - eax relativeto 0 & param - eax = 0
								if ax_destroyed & ~(ax_restored = 1)
									mov eax, [psp - 4]
									ax_restored = 1
								end if
								mov dword[esp+(..argscount-counter)*4], eax
								break
							end if
							irp reg, ecx, edx, ebx, esp, ebp, esi, edi \\\{

							if param relativeto reg & param - reg relativeto 0 & param - reg = 0
								mov dword[esp+(..argscount-counter)*4], reg
								break
							end if

							\\\}
						end if
						if param eqtype eax
							if param eq eax & ax_destroyed & ~(ax_restored = 1)
								mov eax, [psp - 4]
								ax_restored = 1
							end if
							mov dword[esp+(..argscount-counter)*4], param
							break
						end if
						reg_in_addr check_reg, ax, param
						if check_reg & ax_destroyed & ~(ax_restored = 1)
							mov eax, [psp - 4]
							ax_restored = 0
						else if ax_need_save & ~ax_destroyed
							mov [psp - 4], eax
						end if
						ax_destroyed = 1
						lea eax,[param]
						mov dword[esp+(..argscount-counter)*4], eax
					end repeat
				else if type@param eq sep_qword
					match _high:_low, param\\\{
						if (_high eq eax | _low eq eax) & ax_destroyed & ~(ax_restored = 1)
							mov eax, [esp - 4]
							ax_restored = 1
						end if
						mov dword[esp+(..argscount-counter)*4], _low
						mov dword[esp+(..argscount-(counter-1))*4], _high
					\\\}
				; else if type@param eq sep_qword
				; 	match high:low, param\\\{
				; 		mov dword[esp+(..argscount-counter)*4], low
				; 		mov dword[esp+(..argscount-(counter-1))*4], high
				; 	\\\}
				else if param eqtype [0] | param eqtype byte [0]
					if size@param = 8
						movq xmm0, qword param
						movq [esp+(..argscount-counter)*4], xmm0
					else 
						match any1[addr], any1 param\\\{
							reg_in_addr check_reg, ax, addr
							if check_reg & ax_destroyed & ~(ax_restored = 1)
								mov eax, [esp - 4]
								ax_restored = 0
							else if ax_need_save & ~ax_destroyed
								mov [esp - 4], eax
							end if
							ax_destroyed = 1
						\\\}
						if size@param = 4
							mov eax,param
							mov [esp+(..argscount-counter)*4],eax
						else if size@param <> 0
							if ~(param eq al|param eq ax)
								movzx eax,param
							end if
							mov [esp+(..argscount-counter)*4], eax
						end if
					end if
				else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
					if size@param = 4
						movd [esp+(..argscount-counter)*4],param
					else if size@param = 8
						movq [esp+(..argscount-counter)*4],param
					end if
				else
					if size@param = 4
						if param eq eax & ax_destroyed & ~(ax_restored = 1)
							mov eax, [esp - 4]
							ax_restored = 1
						end if
						mov dword[esp+(..argscount-counter)*4],param
					else if size@param = 8
						if param eqtype 0
							; if param >= 0
							mov dword[esp+(..argscount-(counter-1))*4], (param) shr 32
							mov dword[esp+(..argscount-counter)*4], (param) and 0FFFFFFFFh
							; else 
							; 	mov dword[esp+(..argscount-(counter-1))*4], (param) shr 32
							; 	mov dword[esp+(..argscount-counter)*4], (param) and 0FFFFFFFFh
							; end if
						else
							virtual at $
								origin = $
								dq param
								load opcode qword from origin
							end virtual
							mov dword[esp+(..argscount-(counter-1))*4], (opcode) shr 32
							mov dword[esp+(..argscount-counter)*4], (opcode) and 0FFFFFFFFh
						end if
					else
						if (param eq ax | param eq al | param eq ah) & ax_destroyed & ~(ax_restored = 1)
							mov eax, [esp - 4]
							ax_restored = 1
						end if
						mov [esp+(..argscount-counter)*4],param
					end if
				end if
			else
				if param eq NONE
					if type@param eq double | type@param eq qword
						lea esp, [esp - 8]
					else
						lea esp, [esp - 4]
					end if
				else if type@param eq addr
					if (param eqtype 0 & param relativeto 0) | param eq rax
						push param
					else
						lea eax,[param]
						push eax
					end if
				else if type@param eq sep_qword
					match _high:_low, param\\\{
						push _low
						push _high
					\\\}
				else if type@param eq sep_qword
					match high:low, param\\\{
						push high
						push low
					\\\}
				else if param eqtype [0] | param eqtype byte [0]
					local _addr
					match prefix [value],definition@param\\\{ 
						_addr equ value		
					rept 0\\\{\\\} match [value],definition@param\\\{
						_addr equ value	
					\\\}
					if size@param = 8
						push dword [_addr+4]
						push dword [_addr]
					else if size@param <> 0
						push dword [_addr]
					end if
				else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
					if size@param = 4
						lea esp, [esp-4]
						movd [esp], param
					else if size@param = 8
						lea esp, [esp - 8]
						movq [esp], param
					end if
				else
					if size@param = 4
						push param
					else if size@param = 8
						if param >= 0
							push param shr 32
							push param and 0FFFFFFFFh
						else 
							push 0FFFFFFFFh
							push param
						end if
					end if
				end if
			end if
		\\}
		args@count equ args@count+1
		common
		if ax_restored = 2
			ax_need_save = 0
		else
			ax_need_save = 1
		end if
	\}
	local _counter
	rept 1 _counter:args@count\{
		__argscount__ equ _counter
	\}
	..argscount = counter
	call proc
	if stackspace & clean_stack
		; display "lol", 10
		lea esp, [esp action stackspace]
	end if 
}

macro @jret argums&{
	local stub
	stub = 0
	current@frame equ stub
	macro retn _\{
		macro call argum\\{
			jmp \\argum
		\\}
		local _new_args
		match any1(any2, argums\\{
			_new_args equ any1(NONE
			match any3), any2\\\{
				_new_args equ _new_args, any3
			\\\} 
			_new_args equ _new_args)
		\\}
		match __new_args, _new_args\\{
			@call c __new_args
		\\}
		irpv __new_args, _new_args\\{
			restore _new_args
		\\}
		\purge call
	\}
	ret
	purge retn
	restore current@frame
}

macro stdcall args&{ 
	base@call defined current@frame, -, args
}

macro ccall args&{ 
	base@call ~defined current@frame, +, args
}