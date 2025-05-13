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
		restore switch@lastcode, switch@last, switched@last, switch_megalabel@last, switch@datalen
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