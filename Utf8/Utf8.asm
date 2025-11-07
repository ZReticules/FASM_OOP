proc_noprologue

proc Utf8.toSysString c, lpChars, len
	@sarg @arg1, @arg2
	local lpBuf:POINTER, bufSize:DWORD, result:POINTER

	@call [MultiByteToWideChar](65001, 0, @arg1, @arg2, NULL, 0)
	mov [bufSize], eax
	inc eax
	shl pax, 1
	@call [lpBuf] = CNV::alloc(pax)
	@call [MultiByteToWideChar](65001, 0, [lpChars], [len], [lpBuf], [bufSize])
	mov eax, [bufSize]
	mov pdx, [lpBuf]
	mov word[pdx + pax * 2], 0
	@call [result] = [SysAllocString]([lpBuf])
	@call CNV::free([lpBuf])
	mov pax, [result]
	ret
endp

proc Utf8.fromWCharsz c, pWcharsz
	@sarg @arg1
	local lpBuf:POINTER, bufSize:DWORD
	@call [WideCharToMultiByte](65001, 0, [pWcharsz], -1, NULL, 0, NULL, NULL)
	mov [bufSize], eax
	@call CNV::alloc(pax)
	mov [lpBuf], pax
	@call [WideCharToMultiByte](65001, 0, [pWcharsz], -1, pax, [bufSize], NULL, NULL)
	mov pax, [lpBuf]
	ret
endp

proc Utf8.iterate c uses pbx psi pdi pbp, lpStr, len, func, lParam
	@larg psi, @arg1, pdx, @arg2
	@sarg @arg3, @arg4

	local ctx:Utf8.IterContext, lpSmb:POINTER

	lea pbx, [psi + pdx]
	@block
		xor eax, eax
		xor ebp, ebp
		mov edx, 0xFF
		mov [lpSmb], psi
		; int3
		mov [ctx.smbSize], 1
		lodsb
		switch eax
			case_default
				; @call c [putws](L "Некорректный кодпоинт")
				cmp psi, pbx
					jne @sb
			case 0
				inc [ctx.smbSize]
				@call c [func]([lpSmb], 0, [lParam], addr ctx)
				jmp .return
			case +0xF0 ... +0xF7
				inc [ctx.smbSize]
				and eax, 0x07
				shl eax, 18
				or ebp, eax
				mov edx, 0x3F
				lodsb
			case +0xE0 ... +0xEF
				inc [ctx.smbSize]
				mov ecx, 0x0F
				cmp edx, 0xFF
					cmove edx, ecx
				and eax, edx
				shl eax, 12
				or ebp, eax
				mov edx, 0x3F
				lodsb
			case +0xC0 ... +0xDF
				inc [ctx.smbSize]
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
		mov [ctx.skipBytes], 0
		@call c [func]([lpSmb], ebp, [lParam], addr ctx)
		cmp [ctx.skipBytes], 0
		@block < je @fb >
			mov psi, [lpSmb]
			add psi, [ctx.skipBytes]
		@endb
		test eax, eax
			jz .return
	cmp psi, pbx
	@endb < jne @sb >
	.return: ret 
endp

macro asciiToHex bReg{
	local ..lab1, ..lab2
	switch bReg
		case ae 'a'
			sub bReg, 'a' - 'A'
		case ae 'A'
			sub bReg, 'A' - 0xA - '0'
		case_default
			sub bReg, '0'
	end_switch
}

proc Utf8.unEsc c, dest, src, srcLen
	@sarg @arg1
	local ctx[2]:POINTER
	@larg pax, @arg1, pcx, @arg2, pdx, @arg3
	mov [ctx], pax
	xor pax, pax
	mov [ctx + pointer.size], pax
	@call Utf8::iterate(pcx, pdx, .utf8_callback, addr ctx)
	mov pax, [ctx + pointer.size]
	ret

	proc .utf8_callback c uses pbx, lpSmb, codepoint:DWORD, lpCtx, lpUCtx
		; int3
		virtObj .uctx Utf8.IterContext at pbx from @arg4
		switch @arg2
			case '\'
				@larg pcx, @arg1, pdx, @arg3
				movzx eax, byte[pcx + 1]
				and eax, 0xFF
				
				mov [.uctx.skipBytes], 2
				inc pointer[pdx + pointer.size]
				
				switch eax
					case 'a'
						inc pointer[pdx]
						mov pdx, [pdx]
						mov byte[pdx - 1], 0x07
						jmp end_case
					case 'b'
						inc pointer[pdx]
						mov pdx, [pdx]
						mov byte[pdx - 1], 0x08
						jmp end_case
					case 't'
						inc pointer[pdx]
						mov pdx, [pdx]
						mov byte[pdx - 1], 0x09
						jmp end_case
					case 'n'
						inc pointer[pdx]
						mov pdx, [pdx]
						mov byte[pdx - 1], 0x0A
						jmp end_case
					case 'v'
						inc pointer[pdx]
						mov pdx, [pdx]
						mov byte[pdx - 1], 0x0B
						jmp end_case
					case 'f'
						inc pointer[pdx]
						mov pdx, [pdx]
						mov byte[pdx - 1], 0x0C
						jmp end_case
					case 'r'
						inc pointer[pdx]
						mov pdx, [pdx]
						mov byte[pdx - 1], 0xD
						jmp end_case
					case 'u'
						mov [.uctx.skipBytes], 6

						; movptr xmm0, pcx
						mov eax, dword[pcx + 2]
						
						; ascii nums to hex val

						; fourth symbol
						asciiToHex al
						; third symbol
						asciiToHex ah

						shl al, 4
						or al, ah

						rol eax, 16
						; second symbol
						asciiToHex al
						; first symbol
						asciiToHex ah

						shl al, 4
						or ah, al
						shr eax, 8

						and eax, 0xFFFF
						; int3

						switch eax
							case be 0x7F
								add pointer[pdx + pointer.size], 5 - 1
								
								inc pointer[pdx]
								mov pdx, [pdx]
								mov byte[pdx - 1], al
								jmp end_case
							case be 0x7FF
								add pointer[pdx + pointer.size], 5 - 2
								
								mov ecx, eax
								
								shr ecx, 6
								or ecx, 0xC0
								
								and eax, 0x3f
								or eax, 0x80
								
								shl eax, 8
								or eax, ecx

								add pointer[pdx], 2
								mov pdx, [pdx]
								mov word[pdx - 2], ax
								jmp end_case
							case + 0xD800 ... + 0xDBFF

								mov ecx, [pcx + 2 + 4 + 2]
								
								; fourth symbol
								asciiToHex cl
								; third symbol
								asciiToHex ch

								shl cl, 4
								or cl, ch

								rol ecx, 16
								; second symbol
								asciiToHex cl
								; first symbol
								asciiToHex ch

								shl cl, 4
								or ch, cl
								shr ecx, 8

								and ecx, 0xFFFF
								sub eax, 0xD800
								shl eax, 10
								sub ecx, 0xDC00

								lea eax, [eax + ecx + 0x10000]
								add pointer[pdx + pointer.size], 6
								mov [.uctx.skipBytes], 12
								jmp start_case
							case be 0xFFFF
								add pointer[pdx + pointer.size], 5 - 3
								

								movd xmm0, eax
								xor eax, eax

								movd ecx, xmm0
								shr ecx, 12
								or ecx, 0xE0
								or eax, ecx

								movd ecx, xmm0
								shr ecx, 6
								and ecx, 0x3F
								or ecx, 0x80
								shl ecx, 8
								or eax, ecx

								movd ecx, xmm0
								and ecx, 0x3F
								or ecx, 0x80
								shl ecx, 16
								or eax, ecx

								add pointer[pdx], 3
								mov pdx, [pdx]
								mov dword[pdx - 3], eax
								jmp end_case
							case_default
								add pointer[pdx + pointer.size], 5 - 4
								
								movd xmm0, eax
								xor eax, eax

								movd ecx, xmm0
								shr ecx, 18
								or ecx, 0xF0
								or eax, ecx

								movd ecx, xmm0
								shr ecx, 12
								and ecx, 0x3F
								or ecx, 0x80
								shl ecx, 8
								or eax, ecx

								movd ecx, xmm0
								shr ecx, 6
								and ecx, 0x3F
								or ecx, 0x80
								shl ecx, 16
								or eax, ecx

								movd ecx, xmm0
								and ecx, 0x3F
								or ecx, 0x80
								shl ecx, 24
								or eax, ecx
								; int3

								add pointer[pdx], 4
								mov pdx, [pdx]
								mov dword[pdx - 4], eax
								jmp end_case
						end_switch
						jmp end_case
					case_default
					; includes ", \, /
					; default just skip to next symbol
						mov [.uctx.skipBytes], 0
				end_switch
				jmp end_case
			case_default
				; int3
				@larg pcx, @arg1, pdx, @arg3
				mov eax, [.uctx.smbSize]
				add pointer[pdx], pax
				mov pdx, [pdx]
				dec pax
				@block jmp [.size_cases + pax * pointer.size]
					.size_cases dptr .one,\
						.two,\ 
						.three_four,\ 
						.three_four  
					.one:
						not pax
						movzx ecx, byte[pcx]
						mov byte[pdx + pax], cl
						jmp @fb
					.two:
						not pax
						movzx ecx, word[pcx]
						mov word[pdx + pax], cx
						jmp @fb
					.three_four:
						not pax
						mov ecx, dword[pcx]
						mov dword[pdx + pax], ecx
				@endb
		end_switch
		.return:
			mov eax, esp
			ret
	endp
endp

purge asciiToHex

proc_resprologue
