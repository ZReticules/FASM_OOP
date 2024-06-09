importlib kernel32,\
			GetCommandLineA

importlib msvcrt,\
			malloc

proc ParseCommandLine uses rbx, argvLp:qword
local lpargmem:QWORD
	mov [argvLp], rcx
	@call [GetCommandLineA]()
	mov rdx, rax
	xor rcx, rcx
	mov ebx, 1
	.mainloop:
		movzx r8d, byte[rdx+rcx]
		cmp r8d, '"'
		jne .noquotes
			.findquote:
				inc rcx
				movzx r8d, byte[rdx+rcx]
				cmp r8d, 0
					je .NoSpace
			cmp r8d, '"'
			jne .findquote
		.noquotes:
		cmp r8d, " "
		jne .NoSpace
			cmp byte[rdx+rcx-1], 0
				je .SpaceSeries			;серия пробелов
			push rdx
			inc ebx
			.SpaceSeries:
			lea rdx, [rdx+rcx+1]
			mov byte [rdx-1], 0
			mov rcx, -1
		.NoSpace:
		inc rcx
	cmp r8d, 0
	jne .mainloop
	push rdx
	@call [malloc](addr rbx*8)
	mov [lpargmem], rax
	cmp rbx, 0
	je .noargs
		mov rcx, rbx
		@@:
			pop qword[rax+(rcx-1)*8]
		dec rcx
		jnz @b
	.noargs:
	mov rdx, [lpargmem]
	mov rcx, [argvLp]
	mov [rcx], rdx
	mov rax, rbx
	ret
endp