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
	macro switch variable{
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
					start_suff equ e
					_start equ first
					match =u any, _start\\\{
						restore _ins
						_ins equ jb
						restore _start
						_start equ any
					\\\}
					match + any, _start\\\{
						start_suff equ
						restore _start
						_start equ any
					\\\}

					local end_suff, _end
					end_suff equ
					_end equ second
					match + any, _end\\\{
						end_suff equ e
						restore _end
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
				rept 0\\{\\}rept 1\\{

					\\local matched
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
	macro with [args]{
		common
		local with_name, ..break_with
		define last@width with_name

		break_with equ ..break_with
		forward
		match name:Type any, args [:|:]\{
			width_name#dels equ name

			local matched
			match < _definits >(initvals [:|:], any\\{
				restore rest
				rest equ any1
				match =name, name\\\{ 
					locals
						name Type _definits
					endl
				\\\}
				@call name->make(initvals
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
				@call name->make(initvals
				define matched
			\\}
			match =matched|=name, matched|name\\{
				locals
					name Type
				endl
			\\}
		\}
		common
		macro ret operand\{
			match any, operand\\{
				ret operand
			rept 0\\{\\} rept 1\\{
				irpv all_with, last@with\\\{
					\\\reverse
					irpv del, all_with\\\#dels\\\\{
						@call del->unmake()	
					\\\\}
				\\\}
				ret
			\\}
		\}
	}

	macro end_with{
		break_with:
		restore break_with
		purge ret
		match last_with, last@width\{
			irpv del, last_with\#dels\\{
				@call del->unmake()	
			\\}
		\}
		restore last@width
	}

; typedef
	macro typedef name, type{
		struc name args&\{
			. type args
		\}
		macro name args&\{
			type args
		\}
	}

; zero-initialize allocation, initializator in Low-level
	bss_count = 0

	macro @init_bss{
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
		local ..src_const, ..src_start, src_len, ..before_align
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
		@init_consts
		local const_byte
		consts@len = _consts.len

		repeat _consts.len
			load const_byte byte from _consts@addrspace:% - 1
			store byte const_byte at consts@real_addrspace:_consts_base + % - 1
		end repeat
	}

; statics
	virtual at statics@initaddr
		statics@addrspace::
	end virtual

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
		virtual statics@addrspace
			statics@size = $ - statics@initaddr
		end virtual
		local _byte
		repeat statics@size
			load _byte byte from statics@addrspace: statics@initaddr + % - 1
			store byte _byte at statics@real_addrspace: statics@initaddr + % - 1
		end repeat
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