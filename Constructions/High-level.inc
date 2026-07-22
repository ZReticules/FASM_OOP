; module
	@endm fix }

	macro @module name{
		match =name#_included, name#_included{
			define name#_included
	}

; enum
	macro __set@enum parent, name, value{
		parent#.#name = value
	}

	struc enum [args]{
		common
		local counter
		counter = 0
		forward
		match argum==num, args\{
			counter = num
			__set@enum ., argum, counter
		rept 0\{\} rept 1\{
			__set@enum ., args, counter
		\}
		counter = counter + 1
	}

	macro enum [args]{
		common
		local counter
		counter = 0
		forward
		match argum==num, args\{
			counter = num
			argum = counter
		rept 0\{\} rept 1\{
			args = counter
		\}
		counter = counter + 1
	}


; switch
	macro switch variable=NULL{
		local _switch, ..end_case, ..start_case, ..case_code, ..switch_megalabel, len_data
		end_case equ ..end_case
		start_case equ ..case_code
		case@default equ ..end_case
		switch@lastcode equ ..case_code
		switch@last equ _switch
		switched@last equ variable
		switch_megalabel@last equ ..switch_megalabel
		switch@datalen equ len_data
		..case_code:
		rb len_data
		virtual at ..case_code
			..switch_megalabel::
		end virtual
	}

	macro case [values]{
		common
		local ..ins_label
		match variable, switched@last\{
			forward
			virtual switch_megalabel@last
				match first =... second, values\\{
					local ..next, _ins
					_ins equ jl

					local start_suff, _start
					define start_suff e
					_start equ first
					match =u any, _start\\\{
						restore _ins
						_ins equ jb
						; restore _start
						_start equ any
					\\\}
					match + any, _start\\\{
						define start_suff
						; restore _start
						_start equ any
					\\\}

					local end_suff, _end
					define end_suff
					_end equ second
					match + any, _end\\\{

						define end_suff e
						; restore _end
						_end equ any
					\\\}


					cmp variable, _start
					match __ins | suff, _ins | start_suff\\\{
						__ins\\\#suff ..next
					rept 0\\\{\\\}rept 1\\\{
						_ins ..next
					\\\}
						cmp variable, _end
							match __ins|suff, _ins|end_suff\\\{
								__ins\\\#suff ..ins_label
							rept 0\\\{\\\}rept 1\\\{
								_ins ..ins_label
							\\\}
					..next:
				rept 0\\{\\} rept 1\\{

					\\local matched, any, _value
					match any _value, values\\\{
						match =t, any\\\\{
							define matched
							test variable, _value
							jnz ..ins_label
						\\\\}
						match =tn, any\\\\{
							define matched
							test variable, _value
							jz ..ins_label
						\\\\}
						match =F, any\\\\{
							define matched
							irp flag, _value\\\\\{
								j\\\\\#flag ..ins_label
							\\\\\}
						\\\\}
						\\\local any1, any2, any3
						match any1 =any any2, any3 z nz e ne l le nle b be nbe g ge nge a ae nae any3\\\\{
							define matched
							cmp variable, _value
							j\\\#any ..ins_label
						\\\\}
					\\\}
					match =matched, matched\\\{
						cmp variable, values
						je ..ins_label
					\\\}
				\\}
			end virtual
			common
			..ins_label:
		\}
		; virtual switch_megalabel@last
	}

	; macro fcase [values]{
	; 	common
	; 	end virtual
	; 	local ..ins_label
	; 	match variable, switched@last\{
	; 		forward
	; 		match first&second, values\\{
	; 			local ..next, _ins
	; 			_ins equ jb

	; 			local start_suff, _start
	; 			start_suff equ e
	; 			_start equ first

	; 			match + any, _start\\\{
	; 				start_suff equ
	; 				restore _start
	; 				_start equ any
	; 			\\\}

	; 			local end_suff, _end
	; 			end_suff equ
	; 			_end equ second
	; 			match + any, _end\\\{
	; 				end_suff equ e
	; 				restore _end
	; 				_end equ any
	; 			\\\}

	; 			f
	; 			fucomip st0, _start
	; 			match __ins | suff, _ins | start_suff\\\{
	; 				__ins\\\#suff ..next
	; 			rept 0\\\{\\\}rept 1\\\{
	; 				_ins ..next
	; 			\\\}
	; 				cmp variable, _end
	; 					match __ins|suff, _ins|end_suff\\\{
	; 						__ins\\\#suff ..ins_label
	; 					rept 0\\\{\\\}rept 1\\\{
	; 						_ins ..ins_label
	; 					\\\}
	; 			..next:
	; 		rept 0\\{\\}rept 1\\{
	; 		cmp variable, values
	; 			je ..ins_label
	; 		\\}
	; 		common
	; 	\}
	; 	virtual switch_megalabel@last
	; 		..ins_label:
	; }

	macro case_default {
		restore case@default
		local ..ins_label
		case@default equ ..ins_label
		..ins_label:
	}

	macro end_switch {
		local len_data, byte_val
		virtual switch_megalabel@last
			if case@default <> switch@lastcode + switch@datalen
				; %display_num case@default, 10
				; display 0Ah
				; %display_num switch@lastcode, 10
				jmp case@default
			end if
		switch@datalen = $ - switch@lastcode
		end virtual
		repeat switch@datalen
			load byte_val byte from switch_megalabel@last:switch@lastcode + %-1
			store byte byte_val at switch@lastcode + %-1
		end repeat
		end_case:
		restore switch@lastcode, switch@last, switched@last, switch_megalabel@last, switch@datalen, case@default
		restore end_case, start_case
	}


; with
	; define debug@with
	macro with [args]{
		common
		local with_name, ..break_with
		define last@with with_name

		break_with equ ..break_with
		forward
		match name:Type any, args [:|:]\{
			with_name#dels equ name

			local matched
			match < _definits >(initvals [:|:], any\\{
				match =name, name\\\{ 
					locals
						name Type _definits
					endl
				\\\}
				$call name::make(initvals
				define matched
			\\}
			match =matched|< _definits > [:|:], matched|any\\{
				match =name, name\\\{ 
					locals
						name Type _definits
					endl
				\\\}
				define matched
			\\}
			match =matched|(initvals [:|:], matched|any\\{
				match =name, name\\\{ 
					locals
						name Type
					endl
				\\\}
				$call name::make(initvals
				define matched
			\\}
			match =matched|=name, matched|name\\{
				locals
					name Type
				endl
			\\}
		\}
		; common
		; macro ret operand\{
		; 	match any, operand\\{
		; 		ret operand
		; 	rept 0\\{\\} rept 1\\{
		; 		irpv all_with, last@with\\\{
		; 			\\\reverse
		; 			irpv del, all_with\\\#dels\\\\{
		; 				@call del->unmake()	
		; 			\\\\}
		; 		\\\}
		; 		ret
		; 	\\}
		; \}
	}

	macro end_with {
		break_with:
		restore break_with
		; purge ret
		match last_with, last@with\{
			irpv del, last_with\#dels\\{
				\\reverse
				$call del::unmake()	
			\\}
		\}
		restore last@with
	}

	; to unmake all layers of with
	macro exit_with {
		irpv all_with, last@with\{
			\reverse
			irpv del, all_with\#dels\\{
				$call del::unmake()	
			\\}
		\}
	}

	; to unmake corrent layer of with
	macro broke_with {
		match this_with, last@with\{
			irpv del, this_with\#dels\\{
				$call del::unmake()	
			\\}
		\}
	}

; typedef
	macro typedef name, origin{
		match =1, origin#@builtin\{
			struc name val=?\\{
			    match , @struct \\\{ 
			    	common
			    	field@struct equ struct@aligner, ., origin#@datadef, <val> 
				\\\}
			    match no, @struct \\\{
					label . origin
			        origin#@datadef val
			    \\\}
			\\}
			macro name val=?\\{
			    match , @struct \\\{ 
			    	common
	        		\local anonymous
			    	field@struct equ struct@aligner, anonymous, origin#@datadef, <val> 
				\\\}
			    match no, @struct \\\{
					label . origin
			        origin#@datadef val
			    \\\}
			\\}
			name#@typedef equ 1
			name#@defined equ 1
			as_#name equ origin
			match _mp1, origin#@datadef\\{
				sizeof.#name = sizeof.\\#_mp1
			\\}
		\}
		match =1|origin_t, origin#@typedef|as_#origin\{
			struc name val=?\\{
			    match , @struct \\\{ 
			    	common
			    	field@struct equ struct@aligner, ., origin_t\#@datadef, <val> 
				\\\}
			    match no, @struct \\\{
					label . origin_t
			        origin_t\#@datadef val
			    \\\}
			\\}
			macro name val=?\\{
			    match , @struct \\\{ 
			    	common
	        		\local anonymous
			    	field@struct equ struct@aligner, anonymous, origin_t\#@datadef, <val> 
				\\\}
			    match no, @struct \\\{
					label . origin_t
			        origin_t\#@datadef val
			    \\\}
			\\}
			name#@typedef equ 1
			name#@defined equ 1
			as_#name equ origin_t
			match _mp1, origin_t\#@datadef\\{
				sizeof.#name = sizeof.\\#_mp1
				alignof.#name = alignof.\\#_mp1
			\\}
		\}
		match =1, origin#@is_struct\{
			struct name origin
			ends
		\}
	}
	macro typedef name, origin{
		match =1, origin#@builtin\{
			\local align_val, alignof
			match _mp1|_mp2, origin#@datadef|struct@aligner\\{
				_mp2 align_val, alignof, type, 0, sizeof.\\#_mp1
				alignof.#name = alignof
				sizeof.#name = sizeof.\\#_mp1 + align_val
			\\}
			struc name val=?\\{
			    match , @struct \\\{ 
			    	common
			    	field@struct equ struct@aligner, ., origin#@datadef, <val> 
				\\\}
			    match no, @struct \\\{
					label . origin
			        origin#@datadef val
			        rb align_val
			    \\\}
			\\}
			macro name val=?\\{
			    match , @struct \\\{ 
			    	common
	        		\local anonymous
			    	field@struct equ struct@aligner, anonymous, origin#@datadef, <val> 
				\\\}
			    match no, @struct \\\{
					label . origin
			        origin#@datadef val
			        rb align_val
			    \\\}
			\\}
			name#@typedef equ 1
			name#@defined equ 1
			as_#name equ origin
			typedef P_#name, pointer
		\}
		match =1|origin_t, origin#@typedef|as_#origin\{
			\local align_val, alignof
			match _mp1|_mp2, origin_t\#@datadef|struct@aligner\\{
				_mp2 align_val, alignof, type, 0, sizeof.\\#_mp1
				alignof.#name = alignof
				sizeof.#name = sizeof.\\#_mp1 + align_val
			\\}
			struc name val=?\\{
			    match , @struct \\\{ 
			    	common
			    	field@struct equ struct@aligner, ., origin_t\#@datadef, <val> 
				\\\}
			    match no, @struct \\\{
					label . origin_t
			        origin_t\#@datadef val
			        rb align_val
			    \\\}
			\\}
			macro name val=?\\{
			    match , @struct \\\{ 
			    	common
	        		\local anonymous
			    	field@struct equ struct@aligner, anonymous, origin_t\#@datadef, <val> 
				\\\}
			    match no, @struct \\\{
					label . origin_t
			        origin_t\#@datadef val
			        rb align_val
			    \\\}
			\\}
			name#@typedef equ 1
			name#@defined equ 1
			as_#name equ origin_t
			typedef P_#name, pointer
		\}
		match =1, origin#@is_struct\{
			struct name origin
			ends
		\}
	}
	; macro typedef name, type{
	; 	virtual at 0
	; 		type ?
	; 		sizeof.#name = $
	; 	end virtual
	; 	struc name args&\{
	; 		match , args\\{
	; 			. type ?
	; 		rept 0\\{\\}rept 1\\{
	; 			. type args
	; 		\\}
	; 	\}
	; 	macro name args&\{
	; 		match , args\\{
	; 			type ?
	; 		rept 0\\{\\}rept 1\\{
	; 			type args
	; 		\\}
	; 	\}
	; }

; zero-initialize allocation, initializator in Low-level
	bss_count = 0

	macro @init_bss {
		match =@init_bss, @init_bss\{
			bss@count = bss_count
			align 64
			bss_start:
				rb bss@count
			define @init_bss
		\}
	}

	@bss_align equ 1
	macro @bss [args]{
		common
		virtual at bss_start + bss_count
			..start = $
			align @bss_align
			forward
			match name Type, args\{
				name = $
				Type
			\}
			common
			bss_count = bss_count + ($ - (bss_start + bss_count))
		end virtual
	}

	macro @bss_alloc [args]{
		common
		virtual at bss_start + bss_count
			align @bss_align
			forward
			args
			common
			bss_count = bss_count + ($ - (bss_start + bss_count))
		end virtual
	}

	macro no_inits{
		virtual at bss_start + bss_count
	}

	macro endni{
			bss_count = bss_count + ($ - (bss_start + bss_count))
		end virtual
	}

	macro no_init [var] { 
		common
		no_inits
		local done
		forward 
		done equ
		match varname[count]:vartype, var \{ 
			match =BYTE, vartype \\{ 
				varname rb count
				restore done 
			\\}
			match =WORD, vartype \\{ 
				varname rw count
				restore done 
			\\}
			match =DWORD, vartype \\{ 
				varname rd count
				restore done 
			\\}
			match =PWORD, vartype \\{ 
				varname rp count
				restore done 
			\\}
			match =QWORD, vartype \\{ 
				varname rq count
				restore done 
			\\}
			match =TBYTE, vartype \\{ 
				varname rt count
				restore done 
			\\}
			match =DQWORD, vartype \\{ 
				label varname dqword
				rq count*2
				restore done 
			\\}
			match =QQWORD, vartype \\{ 
				label varname qqword
				rq count*4
				restore done 
			\\}
			match =XWORD, vartype \\{ 
				label varname xword
				rq count*2
				restore done 
			\\}
			match =YWORD, vartype \\{ 
				label varname yword
				rq count*4
				restore done 
			\\}
		   	match , done \\{ 
		   		virtual
					varname vartype
				end virtual
				rb count * sizeof.\#vartype
				restore done 
			\\} 
		\}
		match :varname:vartype, done:var\{ 
		   	match =BYTE, vartype \\{ 
		   		varname db ?
				restore done 
			\\}
		   	match =WORD, vartype \\{ 
		   		varname dw ?
				restore done 
			\\}
		   	match =DWORD, vartype \\{ 
		   		varname dd ?
				restore done 
			\\}
		   	match =PWORD, vartype \\{ 
		   		varname dp ?
				restore done 
			\\}
		   	match =QWORD, vartype \\{ 
		   		varname dq ?
				restore done 
			\\}
		   	match =TBYTE, vartype \\{ 
		   		varname dt ?
				restore done 
			\\}
			match =DQWORD, vartype \\{ 
				label varname dqword
				dq ?,?
				restore done 
			\\}
			match =QQWORD, vartype \\{ 
				label varname qqword
				dq ?,?,?,?
				restore done 
			\\}
			match =XWORD, vartype \\{ 
				label varname xword
				dq ?,?
				restore done 
			\\}
			match =YWORD, vartype \\{ 
				label varname yword
				dq ?,?,?,?
				restore done 
			\\}
		   	match , done \\{ 
		   		varname vartype
				restore done 
			\\} 
		\}
		match ,done\{ 
			var
		   	restore done 
		\}
	   	common
		endni 
	}

	macro bss [args]{
		common
		match any, args\{
			no_init args
		rept 0\{\} rept 1\{
			no_inits
		\}
	}
	macro endb {
		endni
	}

; @const macro, initializator in Low-level
	virtual at 0
		_consts@addrspace::
	end virtual
	_consts.len = 0

	@const_align equ 1
	__i = 0
	struc @const argum&{
		local ..src_const, ..src_start, src_len, ..before_align, j
		virtual at 0
			..src_const::
			..src_start:
			argum
			src_len = $ - ..src_start
		end virtual
		.len = src_len
		if used .
			if src_len > _consts.len
				virtual _consts@addrspace
					..before_align:
					align @const_align
					_consts.len = _consts.len + ($ - ..before_align)
					. = _consts_base + $
					j = 0
					while j < src_len
						if src_len - j >= 8
							load src_byte qword from ..src_const:j
							dq src_byte
							j = j + 8
						else if src_len - j >= 4
							load src_byte dword from ..src_const:j
							dd src_byte
							j = j + 4
						else if src_len - j >= 2
							load src_byte word from ..src_const:j
							dw src_byte
							j = j + 2
						else
							load src_byte byte from ..src_const:j
							db src_byte
							j = j + 1
						end if
					end while
				end virtual
				_consts.len = _consts.len + src_len
			else if src_len = 0
				. = 0
			else
				local i, j, found_place, src_byte, const_byte
				i = 0
				found_place = 0
				while (i <= _consts.len - src_len) & ~found_place
					j = 0
					found_place = 1
					while j < src_len
						if src_len - j >= 8 & _consts.len - (i + j) >= 8
							load src_byte qword from ..src_const:j
							load const_byte qword from _consts@addrspace:i + j
							if src_byte <> const_byte
								found_place = 0
								break
							end if
							j = j + 8
						else if src_len - j >= 4 & _consts.len - (i + j) >= 4
							load src_byte dword from ..src_const:j
							load const_byte dword from _consts@addrspace:i + j
							if src_byte <> const_byte
								found_place = 0
								break
							end if
							j = j + 4
						else if src_len - j >= 2 & _consts.len - (i + j) >= 2
							load src_byte word from ..src_const:j
							load const_byte word from _consts@addrspace:i + j
							if src_byte <> const_byte
								found_place = 0
								break
							end if
							j = j + 2
						else
							load src_byte byte from ..src_const:j
							load const_byte byte from _consts@addrspace:i + j
							if src_byte <> const_byte
								found_place = 0
								break
							end if
							j = j + 1
						end if
					end while
					i = i + @const_align
				end while
				if found_place
					. = _consts_base + i - @const_align
				else
					virtual _consts@addrspace
					..before_align:
					align @const_align
					_consts.len = _consts.len + ($ - ..before_align)
						if ~. eq ColoredTask_config
							. = _consts_base + $
							__i = __i + 1
						else
							. = _consts_base + $
							__i = __i + 1
							__e = $
						end if
						j = 0
						while j < src_len
							if src_len - j >= 8 & src_len <= 8
								load src_byte qword from ..src_const:j
								dq src_byte
								j = j + 8
							else if src_len - j >= 4 & src_len <= 4
								load src_byte dword from ..src_const:j
								dd src_byte
								j = j + 4
							else if src_len - j >= 2 & src_len <= 2
								load src_byte word from ..src_const:j
								dw src_byte
								j = j + 2
							else
								load src_byte byte from ..src_const:j
								db src_byte
								j = j + 1
							end if
						end while
					end virtual
					_consts.len = _consts.len + src_len
				end if
			end if
		end if
	}

	macro @init_consts{
		match =@init_consts, @init_consts\{
			align 64
			consts@real_addrspace::
			_consts_base:
				rb consts@len
			define @init_consts
		\}
	}

	macro @postpone_consts{
		consts@len = _consts.len
		@init_consts
		local const_byte
		repeat _consts.len
			load const_byte byte from _consts@addrspace:% - 1
			store byte const_byte at consts@real_addrspace:_consts_base + % - 1
		end repeat
	}

; statics
	if defined statics@initaddr
		virtual at statics@initaddr
			statics@addrspace::
		end virtual
	end if

	macro statics{
		virtual statics@addrspace
	}

	macro endst{
		end virtual
	}

	macro @init_statics{
		match =@init_statics, @init_statics\{
			statics@real_addrspace::
			align 64
			statics@initaddr:
				rb statics@size	
			define @init_statics
		\}
	}

	macro postpone@statics{
		@init_statics
		if defined statics@initaddr
			virtual statics@addrspace
				statics@size = $ - statics@initaddr
			end virtual
			local _byte
			repeat statics@size
				load _byte byte from statics@addrspace: statics@initaddr + % - 1
				store byte _byte at statics@real_addrspace: statics@initaddr + % - 1
			end repeat
		end if
	}

	macro static [var] { 
		common
		statics
		local done
		forward 
		done equ
		match varname[count]:vartype, var \{ 
			match =BYTE, vartype \\{ 
				varname rb count
				restore done 
			\\}
			match =WORD, vartype \\{ 
				varname rw count
				restore done 
			\\}
			match =DWORD, vartype \\{ 
				varname rd count
				restore done 
			\\}
			match =PWORD, vartype \\{ 
				varname rp count
				restore done 
			\\}
			match =QWORD, vartype \\{ 
				varname rq count
				restore done 
			\\}
			match =TBYTE, vartype \\{ 
				varname rt count
				restore done 
			\\}
			match =DQWORD, vartype \\{ 
				label varname dqword
				rq count*2
				restore done 
			\\}
			match =QQWORD, vartype \\{ 
				label varname qqword
				rq count*4
				restore done 
			\\}
			match =XWORD, vartype \\{ 
				label varname xword
				rq count*2
				restore done 
			\\}
			match =YWORD, vartype \\{ 
				label varname yword
				rq count*4
				restore done 
			\\}
		   	match , done \\{ 
		   		virtual
					varname vartype
				end virtual
				rb count * sizeof.\#vartype
				restore done 
			\\} 
		\}
		match :varname:vartype, done:var\{ 
		   	match =BYTE, vartype \\{ 
		   		varname db ?
				restore done 
			\\}
		   	match =WORD, vartype \\{ 
		   		varname dw ?
				restore done 
			\\}
		   	match =DWORD, vartype \\{ 
		   		varname dd ?
				restore done 
			\\}
		   	match =PWORD, vartype \\{ 
		   		varname dp ?
				restore done 
			\\}
		   	match =QWORD, vartype \\{ 
		   		varname dq ?
				restore done 
			\\}
		   	match =TBYTE, vartype \\{ 
		   		varname dt ?
				restore done 
			\\}
			match =DQWORD, vartype \\{ 
				label varname dqword
				dq ?,?
				restore done 
			\\}
			match =QQWORD, vartype \\{ 
				label varname qqword
				dq ?,?,?,?
				restore done 
			\\}
			match =XWORD, vartype \\{ 
				label varname xword
				dq ?,?
				restore done 
			\\}
			match =YWORD, vartype \\{ 
				label varname yword
				dq ?,?,?,?
				restore done 
			\\}
		   	match , done \\{ 
		   		varname vartype
				restore done 
			\\} 
		\}
		match ,done\{ 
			var
		   	restore done 
		\}
	   	common
		endst 
	}

; @block
	macro @block [arg] {
		common
		local ..start_block_label, ..end_block_label, any
		@sb equ ..start_block_label
		@fb equ ..end_block_label
		forward
		arg
		common
		..start_block_label:

	}
	macro @endb [arg] {
		common
		local any
		@fb:
		forward
		arg
		common
		restore @sb, @fb
	}


; new macros for .proc
	rept 1{
		local @$define_foreach_by_pattern
		macro @$define_foreach_by_pattern\{
			\local _mp1, _mp2
			macro @$foreach_by_pattern action, pattern, datadef&\\{
				match _mp1 pattern _mp2, datadef\\\{
					action _mp1
					@$define_foreach_by_pattern
					@$foreach_by_pattern action, pattern, _mp2
				rept 0\\\{\\\} rept 1\\\{
					action datadef 
				\\\}
			\\}
		\}
		@$define_foreach_by_pattern
		local datadef, callargs, _type, conv, mtable, object
		local inline_result, _proc
		local _mp1, _mp2, _mp3, _mp4, _mp5, _mp6, _mp7, _mp8, _mp9, _mp10, _mp11
		local _any1
		macro define$call\{
			\local redefined, result
			macro $call args&\\{
				define result
				define datadef args
				define conv std
				define callargs
				match _mp1 == _mp2, datadef\\\{
					define result _mp1
					define datadef _mp2
				\\\}
				match _mp1::_mp2(_mp3, datadef\\\{
					match _mp4), _mp3\\\\{
						define callargs _mp4
					\\\\}
					match _mp4)::_mp5, _mp3\\\\{
						define callargs _mp4
					\\\\}
					match )::_mp4, _mp3\\\\{
						define callargs
					\\\\}
					define conv cdecl
					match _mp4[_mp5], _mp1\\\\{
						define conv _mp4
						_type equ _mp5\\\\#.BaseType
						; mtable equ _mp5\\\\#.Table
						define object [_mp5]
					rept 0 \\\\{\\\\}rept 1\\\\{
						match _mp4 _mp5, _mp1\\\\\{
							define conv _mp4
							_type equ _mp5\\\\\#.Type
							define object addr _mp5
						rept 0\\\\\{\\\\\} match _mp4, _mp1\\\#.Type\\\\\{
							_type equ _mp4
							define object addr _mp1
							match _mp5, _mp2\\\#.Conv\\\\\#_mp4 \\\\\\{
								define conv _mp5
							\\\\\\}
						\\\\\}
					\\\\}
					match [_mp4], _mp1\\\\{
						match _mp5, _mp4\\\\#.BaseType\\\\\{
							_type equ _mp5
							define object [_mp4]
							match _mp6, _mp2\\\#.Conv\\\\\#_mp5 \\\\\\{
								define conv _mp6
							\\\\\\}
						\\\\\}
					\\\\}
					match _mp4, _type\\\\{
						mtable equ _mp4\\\\#.Table
					\\\\}
					match , callargs\\\\{
						callargs equ object
					rept 0 \\\\{\\\\} rept 1\\\\{
						callargs equ object, callargs
					\\\\}
					match _mp4|_mp5|_mp6, _type|mtable|conv\\\\{
						match =static | _mp7 | _mp8, _mp2\\\#.Type\\\\#_mp4 | @\\\\#_mp6\\\\#call | callargs\\\\\{
							_mp7 _mp5\\\\#.\\\\#_mp2, _mp8
						\\\\\}
						match =virtual | _mp7 | _mp8, _mp2\\\#.Type\\\\#_mp4 | @\\\\#_mp6\\\\#call | callargs\\\\\{
							macro call arg1\\\\\\{
								__virtobj@call object, _mp4, _mp2, _mp5
							\\\\\\}
							_mp7 _mp5\\\\#.\\\\#_mp2, _mp8
							purge call
							define argums
						\\\\\}
						match =inline | _mp8 | _mp7, _mp2\\\#.Type\\\\#_mp4 | _mp2\\\#.Value\\\\#_mp4 | callargs\\\\\{
							define$call
							define redefined
							_inline@parse inline_result, _mp7
							match _mp9, inline_result\\\\\\{
								_mp8 _mp9
							\\\\\\}
							restore __argscount__
						\\\\\}
						match =_mp2\\\#.Type\\\\#_mp4, _mp2\\\#.Type\\\\#_mp4\\\\\{
							Unknown method \\\`_mp2 for structure \\\\`_mp4
						\\\\\}
					\\\\}
					match _mp4)::_mp5, _any1 _mp3\\\\{
						match =redefined, redefined\\\\\{
							define$call
						\\\\\}
						@$foreach_by_pattern $call _mp1::, ::, _mp5
					\\\\}
					define datadef
				\\\}
				match _mp1|_mp2(_mp3, datadef\\\{
					match _mp4), _mp3\\\\{
						define callargs _mp4
					\\\\}
					define conv cdecl
					match _mp4 _mp5, _mp1\\\\{
						define conv _mp4
						_type equ _mp5
						mtable equ _mp5\\\\#.Table
					rept 0\\\\{\\\\} match _mp4, _mp2\\\#.Conv\\\#_mp1 \\\\{
						_type equ _mp1
						mtable equ _mp1\\\#.Table
						define conv _mp4
					\\\\}
					match _mp4|_mp5|_mp6, _type|mtable|conv\\\\{
						match =static | _mp7 | _mp8, _mp2\\\#.Type\\\\#_mp4 | @\\\\#_mp6\\\\#call | callargs\\\\\{
							_mp7 _mp5\\\\#.\\\\#_mp2, _mp8
						\\\\\}
						match =static | _mp7 | , _mp2\\\#.Type\\\\#_mp4 | @\\\\#_mp6\\\\#call | callargs\\\\\{
							_mp7 _mp5\\\\#.\\\\#_mp2
						\\\\\}
						match =virtual | _mp7 | _mp8 | _mp9, _mp2\\\#.Type\\\\#_mp4 | @\\\\#_mp6\\\\#call | _mp2\\\#.Value\\\\#_mp4 | callargs\\\\\{
							use@table _mp4\\\\#.Table
							match , _mp9\\\\\\{
								_mp7 _mp8
							rept 0\\\\\\{\\\\\\}rept 1\\\\\\{
								_mp7 _mp8, _mp9
							\\\\\\}
						\\\\\}
						match =inline | _mp7 | _mp8, _mp2\\\#.Type\\\\#_mp4 | _mp2\\\#.Value\\\\#_mp4 | callargs\\\\\{
							define$call
							define redefined
							_inline@parse inline_result, _mp8
							match _mp9, inline_result\\\\\\{
								_mp7 _mp9
							\\\\\\}
							restore __argscount__
						\\\\\}
						match =inline | _mp7 | , _mp2\\\#.Type\\\\#_mp4 | _mp2\\\#.Value\\\\#_mp4 | callargs\\\\\{
							define$call
							define redefined
							define __argscount__ 0
							_mp7
							restore __argscount__
						\\\\\}
						match =_mp2\\\#.Type\\\\#_mp4, _mp2\\\#.Type\\\\#_mp4\\\\\{
							Unknown method \\\`_mp2 for structure \\\\`_mp4
						\\\\\}
					\\\\}
					define datadef
				\\\}
				match _mp1<_mp2>(_mp3, datadef\\\{
					match _mp4), _mp3\\\\{
						define callargs _mp4
					\\\\}
					define conv cdecl
					match _mp4 _mp5, _mp1\\\\{
						define conv _mp4
						_proc equ _mp5
					rept 0\\\\{\\\\} match _mp4, _mp1\\\#.Conv\\\#_mp2 \\\\{
						_proc equ _mp1
						define conv _mp4
					\\\\}
					mtable equ _mp2\\\#.Table
					match _mp4|_mp5|_mp6, _proc|mtable|conv\\\\{
						match =static | _mp7 | _mp8, _mp4\\\\#.Type\\\#_mp2 | @\\\\#_mp6\\\\#call | callargs\\\\\{
							match , _mp8\\\\\\{
								_mp7 _mp5\\\\#.\\\\#_mp1
							rept 0 \\\\\\{\\\\\\} rept 1\\\\\\{
								_mp7 _mp5\\\\#.\\\\#_mp1, _mp8
							\\\\\\}
						\\\\\}
						match =virtual | _mp7 | _mp8, _mp4\\\\#.Type\\\#_mp2 | @\\\\#_mp6\\\\#call | callargs\\\\\{
							match _mp9=,_mp10, _mp8\\\\\\{
								define object _mp9
							rept 0\\\\\\{\\\\\\}rept 1\\\\\\{
								define object _mp8
							\\\\\\}
							use@table _mp2\\\#.Table
							macro call arg1\\\\\\{
								__virtobj@call object, _mp2, _mp1, _mp5
							\\\\\\}
							_mp7 NULL, _mp8
							purge call
							define argums
						\\\\\}
						match =inline | _mp7 | _mp8, _mp4\\\\#.Type\\\#_mp2 | _mp4\\\\#.Value\\\#_mp2 | callargs\\\\\{
							define$call
							define redefined
							match , _mp8\\\\\\{
								define __argscount__ 0
								_mp7
								restore __argscount__
							rept 0 \\\\\\{\\\\\\} rept 1\\\\\\{
								_inline@parse inline_result, _mp8
								match _mp9, inline_result\\\\\\\{
									_mp7 _mp9
								\\\\\\\}
								restore __argscount__
							\\\\\\}
						\\\\\}
						match =_mp4\\\\#.Type\\\#_mp2, _mp4\\\\#.Type\\\#_mp2\\\\\{
							Unknown method \\\\`_mp4 for structure \\\`_mp2
						\\\\\}
					\\\\}
					define datadef
				\\\}
				match _mp1(_mp2, datadef\\\{
					match _mp3), _mp2\\\\{
						define callargs _mp3
					\\\\}
					_proc equ _mp1
					define datadef _mp1
					match _mp3[_mp4], datadef\\\\{
						define conv _mp3
						_proc equ [_mp4]
						define datadef
					\\\\}
					match _mp3 _mp4, conv\\\\{
						define conv _mp3
						_proc equ _mp4 _proc
					\\\\}
					match [_mp3], datadef\\\\{
						match =_mp3\\\\#.conv, _mp3\\\\#.conv\\\\\{rept 0\\\\\{\\\\\}match _mp4, _mp3\\\\#.conv\\\\\{
							define conv _mp4
						\\\\\}
						_proc equ [_mp3]
						define datadef
					\\\\}
					match _mp3 _mp4, datadef\\\\{
						define conv _mp3
						_proc equ _mp4
						define datadef
					\\\\}
					match , datadef\\\\{rept 0\\\\{\\\\}match _mp3, datadef\\\\{
						match =_mp3\\\\#.conv, _mp3\\\\#.conv\\\\\{rept 0\\\\\{\\\\\}match _mp4, _mp3\\\\#.conv\\\\\{
							define conv _mp4
						\\\\\}
						_proc equ _mp3
						define datadef
					\\\\}
					match _mp3, conv\\\\{
						match _mp4, callargs\\\\\{
							@\\\\#_mp3\\\\#call _proc, _mp4
						rept 0\\\\\{\\\\\}rept 1\\\\\{
							@\\\\#_mp3\\\\#call _proc
						\\\\\}
					\\\\}
					define datadef
				\\\}
				match _mp1|_mp2, result|conv\\\{
					@get_result_\\\#_mp2 _mp1
				\\\}
			\\}
		\}
		define$call
	}

	rept 1{
		local _mp1
		local _start
		macro $return val\{
			\local _size
			match _mp1, proc@name\\{
				\\local _buf, src_type, src_def
				match , val\\\{rept 0\\\{\\\}match _mp2, _mp1\\#.conv\\\{
					_parse@arg src_type, src_def, val
					@set_result@\\\#_mp2 src_type, src_def
					if _size <> 0
						@save_result@\\\#_mp2 _buf, src_type
					end if
					_start = $
				\\\}
				exit@with
				match =last@catch, last@catch\\\{rept 0\\\{\\\}irp _mp1, last@catch\\\{
					$call Exception|__delVEH()
				\\\}
				match , val\\\{rept 0\\\{\\\}match _mp2, _mp1\\#.conv\\\{
					_size = $ - _start
					if _size <> 0
						@load_result@\\\#_mp2 _buf, src_type
					end if
				\\\}
				ret
			\\}
		\}
	}

	rept 1{
		local _mp1, _mp2, _mp3, _mp4, _mp5
		local datadef
		local _any1
		macro .with [args]\{
			\common
			\local with_name, ..break_with
			define last@with with_name

			break_with equ ..break_with
			\forward
			match _mp1:_mp2 _mp3, args _any1\\{
				match =unmake.Value\\#_mp2, unmake.Value\\#_mp2\\\{rept 0\\\{\\\}rept 1\\\{
					with_name\#dels equ _mp1
				\\\}
				define datadef _mp3
				.locals
					_mp1 _mp2
				.endl
				match < _mp4 > _mp5, datadef\\\{
					$call CNV|fill(addr _mp1, <const _mp2 _mp4>, sizeof.\\#_mp2)
					define datadef _mp5
				\\\}
				match <> _mp4, datadef\\\{
					$call CNV|fill(addr _mp1, <const _mp2>, sizeof.\\#_mp2)
					define datadef _mp4
				\\\}
				match (_mp4 =_any1, datadef\\\{
					$call _mp1::make(_mp4
				\\\}
			\\}
		\}

		macro .end_with \{
			break_with:
			restore break_with
			; purge ret
			match _mp1, last@with\\{
				irpv _mp2, _mp1\\#dels\\\{
					\\\reverse
					$call _mp2::unmake()	
				\\\}
			\\}
			restore last@with
		\}

		; to unmake all layers of with
		macro exit@with \{
			irpv _mp1, last@with\\{
				\\reverse
				irpv _mp2, _mp1\\#dels\\\{
					$call _mp2::unmake()	
				\\\}
			\\}
		\}

		; to unmake current layer of with
		macro break@with \{
			match _mp1, last@with\\{
				irpv _mp2, _mp1\#dels\\\{
					$call _mp2::unmake()	
				\\\}
			\\}
		\}
	}