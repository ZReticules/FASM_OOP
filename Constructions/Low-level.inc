; address checker
	; eax, ecx, edx, ebx, esp, ebp, esi, edi
	reg_bits enum ax, \
		cx, \
		dx, \
		bx, \
		sp, \
		bp, \
		si, \
		di
	macro reg_in_addr result, reg_16, address{
		local matched
		match any1 =e#reg_16 any2, [:address:]\{
			result = 1
			define matched
		\}
		match any1 =r#reg_16 any2, [:address:]\{
			result = 1
			define matched
		\}
		match =matched, matched\{
			result = 0
			if address eqtype 0
				local ..opcode, mode_rm, sib, offset
				offset = 0
				virtual at $
					..opcode:
						inc dword[address]
					db 8 dup(0)
					mode_rm = 0
					while mode_rm <> 0xFF
						load mode_rm byte from ..opcode + offset
						offset = offset + 1
					end while
					load mode_rm byte from ..opcode + offset
					load sib byte from ..opcode + offset + 1
				end virtual
				if ~(mode_rm and 11000111b = 00000101b) & \
						((mode_rm and 11000000b <> 11000000b & mode_rm and 111b <> 100b & mode_rm and 111b = reg_bits.#reg_16) | \
							(mode_rm and 111b = 100b & (sib and 111b = reg_bits.#reg_16 | sib and 111000b = reg_bits.#reg_16 shl 3)))
					result = 1
				end if
			end if
		\}
	}

; local variables initialization control
	macro locals_no_init {
		macro initlocal@proc args&\{\}
	}

	macro locals_res_init {
		purge initlocal@proc
	}

; postpone controller(to make shure that all will initialize in reverse)
	
	; total_postpone equ
	postpone{
		irpv inits, total_postpone\{
			\reverse
			inits
		\}
	}

; high-level macro initializators

	total_postpone equ @init_bss

	total_postpone equ postpone@statics

	total_postpone equ @postpone_consts

; @loadGPR, @loadXMM
	rept 1{
		local membuf
		local src_type, src_def
		local @32bit, @64bit
		rax#@32bit	equ eax
		rbx#@32bit	equ ebx
		rcx#@32bit	equ ecx
		rdx#@32bit	equ edx
		rsi#@32bit	equ esi
		rdi#@32bit	equ edi
		rsp#@32bit	equ esp
		rbp#@32bit	equ ebp
		r8#@32bit	equ r8d
		r9#@32bit	equ r9d
		r10#@32bit	equ r10d
		r11#@32bit	equ r11d
		r12#@32bit	equ r12d
		r13#@32bit	equ r13d
		r14#@32bit	equ r14d
		r15#@32bit	equ r15d

		ax#@32bit	equ eax
		bx#@32bit	equ ebx
		cx#@32bit	equ ecx
		dx#@32bit	equ edx
		si#@32bit	equ esi
		di#@32bit	equ edi
		sp#@32bit	equ esp
		bp#@32bit	equ ebp
		r8w#@32bit	equ r8d
		r9w#@32bit	equ r9d
		r10w#@32bit	equ r10d
		r11w#@32bit	equ r11d
		r12w#@32bit	equ r12d
		r13w#@32bit	equ r13d
		r14w#@32bit	equ r14d
		r15w#@32bit	equ r15d
		macro @loadGPR_lit dest, size, val&\{
			if val = 0
				xor dest, dest
			else
				mov dest, val
			end if
		\}
		macro @loadGPR_SSE dest, size, val&\{
			movptr dest, val
		\}
		macro @loadGPR_FPU dest, size, val&\{
			procbuf_alloc membuf, pointer.size
			float equ dword
			double equ qword
			match =st0,val\\{rept 0\\{\\}rept 1\\{
				fxch val
			\\}
			fstp size[membuf]
			mov dest, [membuf]
			restore float, double
			procbuf_free membuf
		\}
		macro @loadGPR_addr dest, size, val&\{
			if val relativeto 0
				@loadGPR_lit dest, size, val
			else
				if ~(val relativeto dest & val - dest = 0)
					lea dest, [val]
				end if
			end if
		\}
		macro @loadGPR_GPR dest, size, val&\{
			match val, val\\{
				if sizeof.\#dest = sizeof.\\#val
					if ~dest eq val
						mov dest, val
					end if
				else if sizeof.\\#val < 4
					movzx dest, val
				else
					if ~dest\#@32bit eq val
						mov dest\#@32bit, val
					end if
				end if
			\\}
		\}
		match =x64, __architecture\{
			macro @loadGPR_qword dest, size, val&\\{
				match high:low, val\\\{
					if low eqtype 0 & high eqtype 0
						mov dest, low + (high shl 32)
					else
						mov r15d, high
						shl r15, 32
						mov dest\\#@32bit, low
						or dest, r15
					end if
				\\\}
			\\}
		\}
		match =x86, __architecture\{
			macro @loadGPR_qword dest, size, val&\\{
				match high:low, val\\\{
					mov dest, low
				\\\}
			\\}
		\}
		macro @loadGPR_mem dest, size, val&\{
			mov dest, pointer[val]
		\}
		macro @loadGPR dest, val\{
			_parse@arg src_type, src_def, val
			match size type, src_type\\{
				@loadGPR_\\#type dest, size, src_def
			\\}
		\}
	}

	rept 1{
		local membuf
		local src_type, src_def
		local @32bit
		ax#@32bit	equ eax
		bx#@32bit	equ ebx
		cx#@32bit	equ ecx
		dx#@32bit	equ edx
		si#@32bit	equ esi
		di#@32bit	equ edi
		sp#@32bit	equ esp
		bp#@32bit	equ ebp
		r8w#@32bit	equ r8d
		r9w#@32bit	equ r9d
		r10w#@32bit	equ r10d
		r11w#@32bit	equ r11d
		r12w#@32bit	equ r12d
		r13w#@32bit	equ r13d
		r14w#@32bit	equ r14d
		r15w#@32bit	equ r15d

		al#@32bit	equ eax
		bl#@32bit	equ ebx
		cl#@32bit	equ ecx
		dl#@32bit	equ edx
		sil#@32bit	equ esi
		dil#@32bit	equ edi
		spl#@32bit	equ esp
		bpl#@32bit	equ ebp
		r8b#@32bit	equ r8d
		r9b#@32bit	equ r9d
		r10b#@32bit	equ r10d
		r11b#@32bit	equ r11d
		r12b#@32bit	equ r12d
		r13b#@32bit	equ r13d
		r14b#@32bit	equ r14d
		r15b#@32bit	equ r15d

		ah#@32bit	equ eax
		bh#@32bit	equ ebx
		ch#@32bit	equ ecx
		dh#@32bit	equ edx

		local @preg
		ah#@preg	equ pax
		bh#@preg	equ pbx
		ch#@preg	equ pcx
		dh#@preg	equ pdx

		match =x64, __architecture\{
			macro @loadXMM_lit dest, size, val&\\{
				if val = 0
					pxor dest, dest
				else if val = -1
					pcmpeqq dest, dest
				else
					mov r11, val
					movq dest, r11
				end if
			\\}
			macro @loadXMM_addr dest, size, val&\\{
				if val relativeto 0
					@loadXMM_lit dest, size, val
				else
					lea r11, [val]
					movq dest, r11
				end if
			\\}
		\}
		match =x86, __architecture\{
			macro @loadXMM_lit dest, size, val&\\{
				if val = 0
					pxor dest, dest
				else if val = -1
					pcmpeqq dest, dest
				else
					\\local ..const_addr
					@const_align equ 8
					..const_addr @const dq val
					restore @const_align
					movq dest, qword[..const_addr]
				end if
			\\}
			macro @loadXMM_addr dest, size, val&\\{
				display sizeof.\\#size
				if val relativeto 0
					@loadXMM_lit dest, size, val
				else
					pxor dest, dest
					pinsrd dest, eax, 3
					lea eax, [val]
					pinsrd dest, eax, 0
					pextrd eax, dest, 3
				end if
			\\}
		\}
		macro @loadXMM_SSE dest, size, val&\{
			movq dest, val
		\}
		macro @loadXMM_FPU dest, size, val&\{
			procbuf_alloc membuf, pointer.size
			float equ dword
			double equ qword
			match =st0, val\\{rept 0\\{\\}rept 1\\{
				fxch val
			\\}
			fstp size[membuf]
			match =qword, size\\{
				movq dest, [membuf]
			rept 0\\{\\}rept 1\\{
				movd dest, size[membuf]
			\\}
			restore float, double
			procbuf_free membuf
		\}
		macro @loadXMM_GPR dest, size, val&\{
			match val, val\\{
				match =qword, size\\{
					movq dest, val
				\\}
				match =dword, size\\{
					movd dest, val
				\\}
				match =word, size\\{
					pxor dest, dest
					pinsrw dest, val\#@32bit, 0
				\\}
				match =byte, size\\{
					pxor dest, dest
					match any1 =val any2, [:] ah bh ch dh [:]\\\{
						ror val\#@preg, 8
						pinsrb dest, val\#@32bit, 0
						rol val\#@preg, 8
					rept 0\\\{\\\}rept 1\\\{
						pinsrb dest, val\#@32bit, 0
					\\\\}
				\\}
			\\}
		\}
		; for sep_qword
		macro @loadXMM_qword dest, size, val&\{
			match high:low, val\\{
				if low eqtype 0 & high eqtype 0
					@loadXMM_lit dest, qword, low + (high shl 32)
				else
					if low eqtype 0
						@loadXMM_lit dest, qword, val
					else
						movd dest, low
					end if
					if high eqtype 0
						@const_align equ 4
						\\local ..const_high
						..const_high @const dd high
						pinsrd dest, val, 1
						restore @const_align
					else
						pinsrd dest, val, 1
					end if
				end if
			\\}
		\}
		macro @loadXMM dest, val\{
			_parse@arg src_type, src_def, val
			match size type, src_type\\{
				@loadXMM_\\#type dest, size, src_def
			\\}
		\}
	}

