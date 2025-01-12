__std_call equ @oop

__obj_call equ @oop

__struc_call equ @oop

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
	reverse
	counter = counter + 1
	define type@param
	define definition@param arg
	param@parser type@param, definition@param
	match param,definition@param\{ 
		local opcode,origin
		size@param = 0
		if param eq NONE
			size@param = 0
		else if type@param eq sep_qword
			size@param = 8
		else if param eqtype 0 | param eqtype 0f | type@param eq addr
			size@param = 8
		else if param eqtype byte 0 | param eqtype byte 0f
			match prefix value,definition@param\\{ 
				if prefix eq qword
					size@param = 8
				else if prefix eq dword
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
		if ..argscount-counter+1 = 1
			if type@param eq float | type@param eq double
				if ~ param eq xmm0
					if size@param = 4
						if param eqtype byte 0 | param eqtype byte 0f
							mov eax, dword param
							movd xmm0,eax
						else
							movd xmm0, dword param
						end if
					else
						if param eqtype 0 | param eqtype 0f | param eqtype byte 0 | param eqtype byte 0f
							mov rax, dword param
							movq xmm0,rax
						else
							movq xmm0, qword param
						end if
					end if
				end if
				if vararg@fastcall & ~ param eq rcx
					movq rcx,xmm0
				end if
			else if type@param eq addr
				if ~ param eq rcx
					lea rcx, [param]
				end if
			else if type@param eq sep_qword
				match _high:_low, param\\{
					mov dword[esp+(..argscount-counter)*4], _low
					mov dword[esp+(..argscount-(counter-1))*4], _high
				\\}
				mov rcx, qword[esp+(..argscount-counter)*4]
			else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				movq rcx, param
			else if size@param = 8
				if ~ param eq rcx
					mov rcx,param
				end if
			else if size@param = 4
				if ~ param eq ecx
					mov ecx,param
				end if
			else if size@param = 2
				if ~ param eq cx
					movzx ecx,param
				end if
			else if size@param = 1
				if ~ param eq cl
					movzx ecx,param
				end if
			end if
		else if ..argscount-counter+1 = 2
			if type@param eq float | type@param eq double
				if ~ param eq xmm1
					if size@param = 4
						if param eqtype byte 0 | param eqtype byte 0f
							mov eax, dword param
							movd xmm1,eax
						else
							movd xmm1, dword param
						end if
					else
						if param eqtype 0 | param eqtype 0f | param eqtype byte 0 | param eqtype byte 0f
							mov rax, dword param
							movq xmm1,rax
						else
							movq xmm1, qword param
						end if
					end if
				end if
				if vararg@fastcall & ~ param eq rdx
					movq rdx,xmm1
				end if
			else if type@param eq addr
				if ~ param eq rdx
					lea rdx, [param]
				end if
			else if type@param eq sep_qword
				match _high:_low, param\\{
					mov dword[esp+(..argscount-counter)*4], _low
					mov dword[esp+(..argscount-(counter-1))*4], _high
				\\}
				mov rdx, qword[esp+(..argscount-counter)*4]
			else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				movq rdx, param
			else if size@param = 8
				if ~ param eq rdx
					mov rdx,param
				end if
			else if size@param = 4
				if ~ param eq edx
					mov edx,param
				end if
			else if size@param = 2
				if ~ param eq dx
					movzx edx,param
				end if
			else if size@param = 1
				if ~ param eq dl
					movzx edx,param
				end if
			end if
		else if ..argscount-counter+1 = 3
			if type@param eq float | type@param eq double
				if ~ param eq xmm2
					if size@param = 4
						if param eqtype byte 0 | param eqtype byte 0f
							mov eax, dword param
							movd xmm2,eax
						else
							movd xmm2, dword param
						end if
					else
						if param eqtype 0 | param eqtype 0f | param eqtype byte 0 | param eqtype byte 0f
							mov rax, dword param
							movq xmm2,rax
						else
							movq xmm2, qword param
						end if
					end if
				end if
				if vararg@fastcall & ~ param eq r8
					movq r8,xmm2
				end if
			else if type@param eq addr
				if ~ param eq r8
					lea r8, [param]
				end if
			else if type@param eq sep_qword
				match _high:_low, param\\{
					mov dword[esp+(..argscount-counter)*4], _low
					mov dword[esp+(..argscount-(counter-1))*4], _high
				\\}
				mov r8, qword[esp+(..argscount-counter)*4]
			else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				movq r8, param
			else if size@param = 8
				if ~ param eq r8
					mov r8,param
				end if
			else if size@param = 4
				if ~ param eq r8d
					mov r8d,param
				end if
			else if size@param = 2
				if ~ param eq r8w
					movzx r8d,param
				end if
			else if size@param = 1
				if ~ param eq r8b
					movzx r8d,param
				end if
			end if
		else if ..argscount-counter+1 = 4
			if type@param eq float  | type@param eq double
				if ~ param eq xmm3
					if size@param = 4
						if param eqtype byte 0 | param eqtype byte 0f
							mov eax,param
							movd xmm3,eax
						else
							movd xmm3,param
						end if
					else
						if param eqtype 0 | param eqtype 0f | param eqtype byte 0 | param eqtype byte 0f
							mov rax,param
							movq xmm3,rax
						else
							movq xmm3,param
						end if
					end if
				end if
				if vararg@fastcall & ~ param eq r9
					movq r9,xmm3
				end if
			else if type@param eq addr
				if ~ param eq r9
					lea r9, [param]
				end if
			else if type@param eq sep_qword
				match _high:_low, param\\{
					mov dword[esp+(..argscount-counter)*4], _low
					mov dword[esp+(..argscount-(counter-1))*4], _high
				\\}
				mov r9, qword[esp+(..argscount-counter)*4]
			else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				movq r9, param
			else if size@param = 8
				if ~ param eq r9
					mov r9,param
				end if
			else if size@param = 4
				if ~ param eq r9d
					mov r9d,param
				end if
			else if size@param = 2
				if ~ param eq r9w
					movzx r9d,param
				end if
			else if size@param = 1
				if ~ param eq r9b
					movzx r9d,param
				end if
			end if
		else
			if type@param eq addr
				if (param eqtype 0 & param relativeto 0) | param eq rax
					mov qword[rsp+(..argscount-counter)*8], param
				else
					lea rax,[param]
					mov [rsp+(..argscount-counter)*8],rax
				end if
			else if type@param eq sep_qword
				match _high:_low, param\\{
					mov dword[esp+(..argscount-counter)*4], _low
					mov dword[esp+(..argscount-(counter-1))*4], _high
				\\}
			else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				movq [rsp+(..argscount-counter)*8], param
			else if param eqtype [0] | param eqtype byte [0]
				if size@param = 8
					mov rax, param
				else if size@param = 4
					mov eax, param
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
					mov rax,param
					mov [rsp+(..argscount-counter)*8],rax
				else
					mov qword [rsp+(..argscount-counter)*8],param
				end if
			else if param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
				movq [rsp+(..argscount-counter)*8],param
			else
				mov [rsp+(..argscount-counter)*8],param
			end if
		end if 
	\}
	args@count equ args@count+1
	common
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