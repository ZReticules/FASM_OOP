@module ParsedDouble

	struct ParsedDouble
		intVal 	dq ?
		fracVal	dq ?
		order 	dd ?
		expVal 	dw ?
		sign 	dw ?

		parseDouble 	dm this
		parseDouble_2	dm this
		getDouble 		dm this
	ends

	local intLen, tenPow

	if used intLen
		intLen:
			xor ecx, ecx
			test eax, eax
			jz @f
				pxor xmm2, xmm2
				pcmpeqd xmm3, xmm3
				mov ecx, 10
				.loop_div:
					xor edx, edx
					div ecx
					psubd xmm2, xmm3
				test eax, eax
				jnz .loop_div
				movd ecx, xmm2
			@@: ret
	end if

	if used int64Len
		int64Len:
			mov ecx, edx
			or ecx, eax
			jz @f
				pxor xmm2, xmm2
				pcmpeqd xmm3, xmm3
				mov ecx, 10
				movd xmm0, eax
				movd xmm1, edx
				.loop_div:
					xor edx, edx
					movd eax, xmm1
					div ecx
					movd xmm1, eax
					movd eax, xmm0
					div ecx
					movd xmm0, eax
					psubd xmm2, xmm3
				test eax, eax
				jnz .loop_div
				movd ecx, xmm2
			@@: ret
	end if

	if used tenPow
		tenPow:
			fldl2t
			fmulp st1, st0
			fld st0
			fisttp qword[psp-8]
			fild qword[psp-8]
			fsub st1, st0
			fld1
			fscale
			fstp st1
			fxch
			f2xm1
			fld1
			faddp
			fmulp st1, st0
			ret
	end if

	macro scaleTen power\{
		\local ..scale_label
		if ~(power eq pdx | power eq edx)
			mov edx, power
		end if
		mov eax, 1
		..scale_label:
			lea pax, [pax * 5]
			add pax, pax
		dec edx
		jnz ..scale_label
	\}

	macro scaleTen64 power\{
		\local ..scale_label
		if ~(power eq pdx | power eq edx)
			mov edx, power
		end if
		mov eax, 1
		movd xmm0, eax
		mov eax, 3
		movd xmm1, eax
		..scale_label:
			vpsllq xmm1, xmm0, 3
			paddq xmm1, xmm0
			paddq xmm0, xmm1
		dec edx
		jnz ..scale_label
	\}

	proc_noprologue

	proc ParsedDouble.parseDouble c uses pbx pdi, this, num:QWORD, precision:DWORD, normalization:DWORD
		virtObj .this:arg ParsedDouble at pbx from @arg1
		@sarg @farg2, @arg3, @arg4
		; .max_default @const dq 1.0e5	; jae
		; .min_default @const dq 1.0e-5	; lbe
		locals
			intBuf 		dq ?
			floatBuf 	dt ?
		endl

		@call CNV::memset(addr .this, 0, sizeof.ParsedDouble, 1, MEMSET_NO_SAVE)
		; int3
		mov eax, dword[num + 4]
		mov edx, eax
		and eax, 7FF00000h
		cmp eax, 7FF00000h
			je .error_num
		fld [num]
		fldz
		fucomip st0, st1
			je .return
		jb .no_neg
			fchs
			mov [.this.sign], 1
		.no_neg:
		; @call CNV::ui32pow(10, @arg4)
		scaleTen64 [normalization]
		movq [intBuf], xmm0
		; fld qword[.max_default]
		fild [intBuf]
		fld st0
		fstp [floatBuf]
		fucomip st0, st1
			jb .higner
		; fld qword[.min_default]
		fld [floatBuf]
		fld1
		fdivrp st1, st0
		fucomip st0, st1
			ja .lower
		.ret_after_correct:
		fld st0
		fisttp [.this.intVal]
		fild [.this.intVal]
		fsubp st1, st0
		fstp [floatBuf]
		mov edx, dword[.this.intVal + 4]
		mov eax, dword[.this.intVal]
		call int64Len

		xor edx, edx
		xchg [precision], edx
		sub edx, ecx
		jle .no_fract
			mov [precision], edx

			
			scaleTen64 edx
			movq [intBuf], xmm0

			mov edx, [precision]
			fld [floatBuf]
			fild [intBuf]
			fmulp st1, st0
			fld st0
			fstp [floatBuf]
			fistp [.this.fracVal]
			mov edx, [precision]
			mov [precision], 0
			movq xmm0, [.this.fracVal]
			ptest xmm0, xmm0
			jz .no_fract
				mov [precision], edx
				mov eax, dword[.this.fracVal]
				mov edx, dword[.this.fracVal + 4]
				call .downZeros
				sub [precision], eax
				movq [.this.fracVal], xmm0
				mov eax, dword[.this.fracVal]
				mov edx, dword[.this.fracVal + 4]
				call int64Len

				sub [precision], ecx
				jns .no_incorrect_round
					match =x86, __architecture\{
						add dword[.this.intVal], 1
						adc dword[.this.intVal + 4], 0
					\}
					match =x64, __architecture\{
						inc [.this.intVal]
					\}
					mov eax, dword[.this.fracVal]
					mov edx, dword[.this.fracVal + 4]
					call .downZeros
					movq [.this.fracVal], xmm0
					mov eax, 1
					movd xmm1, eax
					pcmpeqq xmm0, xmm1
					ptest xmm0, xmm0
						jz .no_fract
					pxor xmm0, xmm0
					movq [.this.fracVal], xmm0
					mov [precision], 0
					jmp .no_fract
				.no_incorrect_round:
				je .no_zeros

					movq xmm0, [.this.intVal]
					ptest xmm0, xmm0
					jnz .no_prec_incorrect
						; if we have 0 in integer part, then we must recalculate ten shift
						; because there may be not enough valuable numbers
						; mov edi, [.this.fracVal]
						scaleTen64 [precision]
						movq [intBuf], xmm0
						fld [floatBuf]
						fild [intBuf]
						fmulp st1, st0
						fistp [intBuf]
						mov eax, dword[.this.fracVal]
						mov edx, dword[.this.fracVal + 4]
						call .downZeros
						movq xmm1, xmm0
						movq xmm0, [.this.fracVal]
						movq [.this.fracVal], xmm1
						
						mov eax, 1
						movd xmm1, eax
						pcmpeqq xmm0, xmm1
						ptest xmm0, xmm0
							jz .no_fract
						movq xmm0, [.this.fracVal]
						pcmpeqq xmm0, xmm1
						ptest xmm0, xmm0
							jnz .no_fract
						inc [precision]
						.round_no_change:
					.no_prec_incorrect:
				.no_zeros:
		.no_fract:
		mov eax, [precision] 
		mov [.this.order], eax
		.return: ret
		.lower:
		.higner:
			fld st0
			fldlg2
			fxch st1
			fyl2x
			; frndint
			; fld st0
			fisttp [.this.expVal]
			fild [.this.expVal]
			call tenPow
			fdivp st1, st0
			jmp .ret_after_correct

		.error_num:
			mov eax, -2
			mov ecx, -1
			test edx, 80000000h
				cmovnz eax, ecx
			lea ecx, [eax - 2]
			and edx, 0FFFFFh
			or edx, dword[num]
			test edx, edx
				cmovnz eax, ecx
			mov [.this.sign], ax
			ret

		; procedure to eliminate pointless zeros after fractional part
		.downZeros:
			mov dword[psp - 8], -1
			; movd xmm0, eax
			; movd xmm1, edx
			movd xmm2, eax
			movd xmm3, edx
			mov ecx, 10
			.down_zeros:
				movq xmm0, xmm2
				movq xmm1, xmm3
				inc dword[psp - 8]
				xor edx, edx
				movd eax, xmm3
				div ecx
				movd xmm3, eax
				movd eax, xmm2
				div ecx
				movd xmm2, eax
			test edx, edx
			jz .down_zeros
			pslldq xmm1, 4
			paddq xmm0, xmm1
			mov eax, dword[psp - 8]
			ret 0
	endp

	proc ParsedDouble.parseDouble_2 c uses pbx pdi, this, num:QWORD
		virtObj .this:arg ParsedDouble at pbx from @arg1
		@sarg @farg2, @arg3, @arg4
		.max_default @const dq 1.0e9	; jae
		.min_default @const dq 1.0e8	; lbe
		locals
			precision 	dd 9
			intBuf 		dd ?
			floatBuf 	dt ?
		endl

		@call CNV::memset(addr .this, 0, sizeof.ParsedDouble, MEMSET_NO_SAVE)
		mov eax, dword[num + 4]
		mov edx, eax
		and eax, 7FF00000h
		cmp eax, 7FF00000h
			je .error_num
		fld [num]
		fldz
		fucomip st0, st1
			je .return
		jb .no_neg
			fchs
			mov [.this.sign], 1
		.no_neg:
		fld qword[.max_default]
		fucomip st0, st1
			jbe .higner
		fld qword[.min_default]
		fucomip st0, st1
			ja .lower
		.ret_after_correct:
		fld st0
		fisttp [.this.intVal]
		fild [.this.intVal]
		fsubp st1, st0
		fstp [floatBuf]
		; mov eax, [.this.intVal]
		; call intLen

		; mov edx, [precision]
		fld [floatBuf]
		; mov [intBuf], 1000000000
		fld qword[.max_default]
		fmulp st1, st0
		fld st0
		fstp [floatBuf]
		fistp [.this.fracVal]
		mov edx, [precision]
		mov [precision], 0
		cmp [.this.fracVal], 0
		je .no_fract
			mov [precision], edx
			mov eax, [.this.fracVal]
			call .downZeros
			sub [precision], eax
			mov [.this.fracVal], edi
			mov eax, [.this.fracVal]
			call intLen

			sub [precision], ecx
			jns .no_incorrect_round
				inc [.this.intVal]
				mov eax, [.this.fracVal]
				call .downZeros
				mov [.this.fracVal], edi
				mov eax, [.this.fracVal]
				xor edx, edx
				cmp edi, 1
				cmove eax, edx
				mov [.this.fracVal], eax
				mov eax, [precision]
				cmove eax, edx
				mov [precision], eax
				jmp .no_fract
			.no_incorrect_round:
			je .no_zeros

				cmp [.this.intVal], 0
				jne .no_prec_incorrect
					; if we have 0 in integer part, then we must recalculate ten shift
					; because there may be not enough valuable numbers
					mov edi, [.this.fracVal]
					scaleTen [precision]
					mov [intBuf], eax
					fld [floatBuf]
					fild [intBuf]
					fmulp st1, st0
					fistp [.this.fracVal]
					mov eax, edi
					xchg eax, [.this.fracVal]
					call .downZeros
					xchg [.this.fracVal], edi
					cmp edi, 1
					jne .round_no_change
					cmp [.this.fracVal], 1
					je .round_no_change
						inc [precision]
					.round_no_change:
				.no_prec_incorrect:
			.no_zeros:
		.no_fract:
		mov eax, [precision] 
		mov [.this.order], eax
		.return: ret
		.lower:
		.higner:
			.nine @const dq 9.0
			fld st0
			fldlg2
			fxch st1
			fyl2x
			frndint
			fld qword[.nine]
			fsubp st1, st0
			fld st0
			fisttp [.this.expVal]
			call tenPow
			fdivp st1, st0
			jmp .ret_after_correct

		.error_num:
			mov eax, -2
			mov ecx, -1
			test edx, 80000000h
				cmovnz eax, ecx
			lea ecx, [eax - 2]
			and edx, 0FFFFFh
			or edx, dword[num]
			test edx, edx
				cmovnz eax, ecx
			mov [.this.sign], ax
			ret

		; procedure to eliminate pointless zeros after fractional part
		.downZeros:
			mov dword[psp - 8], -1
			mov edi, eax
			mov ecx, 10
			.down_zeros:
				mov edi, eax
				inc dword[psp - 8]
				xor edx, edx
				div ecx
			test edx, edx
			jz .down_zeros
			mov eax, dword[psp - 8]
			ret 0
	endp

	proc ParsedDouble.getDouble c uses pdi, this
		virtObj .this:arg ParsedDouble at pdi from @arg1
		locals 
			ftox dq ?
		endl
		; int3
		mov eax, [.this.fracVal]
		call intLen
		add ecx, [.this.order]
		; @call CNV::ui32pow(10, ecx)
		mov dword[ftox], ecx
		fild dword[ftox]
		call tenPow
		fstp [ftox]
		; mulsd xmm0, [ftox]
		movq xmm1, [ftox]
		cvtsi2sd xmm0, [.this.fracVal]
		divsd xmm0, xmm1
		cvtsi2sd xmm1, [.this.intVal]
		addsd xmm0, xmm1
		fild [.this.expVal]
		call tenPow
		fstp [ftox]
		mulsd xmm0, [ftox]
		ret
	endp

	; proc ParsedDouble.fromChars c uses pbx, this, lpCstr, len
	; 	virtObj .this ParsedDouble at pbx from @arg1
	; 	@sarg @arg2, @arg3
	; 	cmp @arg3, -1
	; 	jne .no_calc_len
	; 		@call CNV::strlen(@arg2)
	; 		mov [len], pax
	; 	.no_calc_len:
	; 	@call CNV::memset(addr .this, 0, sizeof.ParsedDouble)

	; 	ret
	; endp

	proc_resprologue

@endm
