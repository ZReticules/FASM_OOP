@module MurmurHash3

.proc_frame_mode_static

.proc cdecl MurmurHash3(.lpKey, .keyLen:DWORD, .seed) uses pbx pdi psi
	@larg pbx, @arg1, pcx, @arg2, pdi, @arg3

	mov esi, 4
	switch pcx
		case -1
			$call pcx = CNV|strlen(pbx)
			jmp end_case
		case -2
			$call pcx = CNV|wstrlen(pbx)
			shl pcx, 1
	end_switch
	mov [.keyLen], ecx

	cmp pcx, 4
	@block < jl @fb >
		mov eax, [pbx + psi - 4]
		
		imul eax, eax, 0xcc9e2d51
		rol eax, 15
		imul eax, eax, 0x1b873593
		xor edi, eax
		rol edi, 13
		lea edi, [edi * 5 + 0xe6546b64]
		
		add esi, 4
	cmp esi, ecx
	jl @sb
	@endb 
	
	xor eax, eax
	lea pbx, [pbx + psi -4]
	sub esi, ecx
	switch esi
		case 1
			movzx edx, byte[pbx + 2]
			shl edx, 16
			xor eax, edx
		case 2
			movzx edx, byte[pbx + 1]
			shl edx, 8
			xor eax, edx
		case 3
			movzx edx, byte[pbx]
			xor eax, edx

			imul eax, eax, 0xcc9e2d51
			rol eax, 15
			imul eax, eax, 0x1b873593
			xor edi, eax
	end_switch


	xor edi, ecx

	; start fmux
	mov eax, edi
	shr edi, 16
	xor eax, edi
	
	imul eax, eax, 0x85ebca6b

	mov edi, eax
	shr edi, 13
	xor eax, edi
	
	imul eax, eax, 0xc2b2ae35
	
	mov edi, eax
	shr edi, 16
	xor eax, edi
	; end fmux

	$return eax
.endp

.proc_frame_mode_previous

@endm
