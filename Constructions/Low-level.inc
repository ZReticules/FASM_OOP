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
	macro locals_no_init{
		macro initlocal@proc args&\{\}
	}

	macro locals_res_init{
		purge initlocal@proc
	}

; postpone controller
	
	; total_postpone equ
	postpone{
		irpv inits, total_postpone\{
			\reverse
			inits
		\}
	}

; zero-initialize allocation
	bss_count = 0

	macro @init_bss{
		match =@init_bss, @init_bss\{
			align 64
			bss_start:
				rb bss@count
			define @init_bss
		\}
	}

	postpone{
		bss@count = bss_count 
		@init_bss
	}

	@bss_align equ 1
	macro @bss [args]{
		common
		local ..start, ..end
		virtual at bss_start + bss_count
			..start = $
			align @bss_align
			forward
			match name Type, args\{
				name = $
				Type
			\}
			common
			..end = $
		end virtual
		bss_count = bss_count + (..end-..start)
	}

	macro @bss_alloc [args]{
		common
		local ..start, ..end
		virtual at bss_start + bss_count
			..start:
			align @bss_align
			forward
			args
			common
			..end:
		end virtual
		bss_count = bss_count + (..end-..start)

	}

