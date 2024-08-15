importlib kernel32,\
			GetCommandLineA

proc CNV.parseCMD stdcall uses ebx, lpArgv:dword
	local lpArgMem:DWORD
	@call [GetCommandLineA]()
	mov edx, eax
	xor ecx, ecx
	mov ebx, 1
	.mainloop:
		movzx eax, byte[edx+ecx]
		cmp eax, '"'
		jne .noquotes
			.findquote:
				inc ecx
				movzx eax, byte[edx+ecx]
				cmp eax, 0
					je .NoSpace
			cmp eax, '"'
			jne .findquote
		.noquotes:
		cmp eax, " "
		jne .NoSpace
			cmp byte[edx+ecx-1], 0
			je .SpaceSeries			;серия пробелов
				push edx
				inc ebx
			.SpaceSeries:
			lea edx, [edx+ecx+1]
			mov byte [edx-1], 0
			mov ecx, -1
		.NoSpace:
		inc ecx
	cmp eax, 0
	jne .mainloop
	push edx
	@call CNV:alloc(addr ebx*4)
	mov [lpArgMem], eax
	mov ecx, ebx
	@@:
		pop dword[eax+(ecx-1)*4]
	dec ecx
	jnz @b
	mov edx, [lpArgMem]
	mov ecx, [lpArgv]
	mov [ecx], edx
	mov eax, ebx
	ret
endp