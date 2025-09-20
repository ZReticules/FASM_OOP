proc_noprologue

proc IterateUtf8 c uses pbx psi pdi pbp, lpStr, len, func, lParam
	@larg psi, @arg1, pdx, @arg2
	@sarg @arg3, @arg4

	local skipBytes:POINTER, lpSmb:POINTER

	lea pbx, [psi + pdx]
	@block
		xor eax, eax
		xor ebp, ebp
		mov edx, 0xFF
		mov [lpSmb], psi
		lodsb
		switch eax
			case_default
				@call c [putws](L "Некорректный кодпоинт")
				cmp psi, pbx
					jne @sb
			case 0
				@call c [func](xmm0, 0, [lParam])
				jmp .return
			case +0xF0 ... +0xF7
				and eax, 0x07
				shl eax, 18
				or ebp, eax
				mov edx, 0x3F
				lodsb
			case +0xE0 ... +0xEF
				mov ecx, 0x0F
				cmp edx, 0xFF
					cmove edx, ecx
				and eax, edx
				shl eax, 12
				or ebp, eax
				mov edx, 0x3F
				lodsb
			case +0xC0 ... +0xDF
				mov ecx, 0x1F
				cmp edx, 0xFF
					cmove edx, ecx
				and eax, edx
				shl eax, 6
				or ebp, eax
				mov edx, 0x3F
				lodsb
			case +1 ... +0x7F
				and eax, edx
				or ebp, eax
		end_switch
		mov [skipBytes], 0
		@call c [func]([lpSmb], ebp, [lParam], addr skipBytes)
		cmp [skipBytes], 0
		@block < je @fb >
			mov psi, [lpSmb]
			add psi, [skipBytes]
		@endb
		test eax, eax
			jz .return
	cmp psi, pbx
	@endb < jne @sb >
	.return: ret 
endp

proc_resprologue
