__std_call equ std

__obj_call equ c

__struc_call equ c

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
		reverse
		counter = counter + 1
		define type@param
		define definition@param arg
		param@parser type@param, definition@param
		match param,definition@param\\{ 
			local opcode,origin
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
			else if param in <eax, ebx, ecx, edx, ebp, esp, edi, esi>
				size@param = 4
			else if param in <ax, bx, cx, dx, bp, sp, di, si>
				size@param = 2
			else if param in <al, ah, bl, bh, cl, ch, dl, dh, bp, sp, di, si>
				size@param = 1
			end if

			if defined current@frame
				if type@param eq addr
					if (param eqtype 0 & param relativeto 0) | param eq eax
						mov dword[esp+(..argscount-counter)*4], param
					else
						lea eax,[param]
						mov [esp+(..argscount-counter)*4], eax
					end if
				else if type@param eq sep_qword
					match _high:_low, param\\\{
						mov dword[esp+(..argscount-counter)*4], _low
						mov dword[esp+(..argscount-(counter-1))*4], _high
					\\\}
				else if type@param eq sep_qword
					match high:low, param\\\{
						mov dword[esp+(..argscount-counter)*4], low
						mov dword[esp+(..argscount-(counter-1))*4], high
					\\\}
				else if param eqtype [0] | param eqtype byte [0]
					if size@param = 8
						movq xmm0, qword param
						movq [esp+(..argscount-counter)*4], xmm0
					else if size@param = 4
						mov eax,param
						mov [esp+(..argscount-counter)*4],eax
					else if size@param <> 0
						if ~(param eq al|param eq ax)
							movzx eax,param
						end if
						mov [esp+(..argscount-counter)*4], eax
					end if
				else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
					if size@param = 4
						movd [esp+(..argscount-counter)*4],param
					else if size@param = 8
						movq [esp+(..argscount-counter)*4],param
					end if
				else
					if size@param = 4
						mov dword[esp+(..argscount-counter)*4],param
					else if size@param = 8
						if param >= 0
							mov dword[esp+(..argscount-(counter-1))*4], (param) shr 32
							mov dword[esp+(..argscount-counter)*4], (param) and 0FFFFFFFFh
						else 
							mov dword[esp+(..argscount-(counter-1))*4], 0FFFFFFFFh
							mov dword[esp+(..argscount-counter)*4], param
						end if
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