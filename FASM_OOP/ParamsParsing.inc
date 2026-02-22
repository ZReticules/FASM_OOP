macro @fillGPR reg*, src*{
	local matched, definition, type
	define type
	define definition src
	param@parser type, definition
	match =addr, type\{
		if ~any eq reg
			lea reg, [definition]
		end if
		define matched
	\}
	match =sep_qword|high:low, type|definition\{
		mov dword[psp - 8], low
		mov dword[psp - 4], high
		mov reg, qword[psp - 8]
		define matched
	\}
	match any[addr], _ definition\{
		mov reg, definition
		define matched
	\}
	match =matched, matched\{
		if definition in <XMM_REGS>
			movptr reg, definition
		else if ~src eq reg
			if definition eqtype 0 & definition relativeto 0
				mov reg, definition
			else
				lea reg, [definition]
			end if
		end if
	\}
}

macro va_list@handler ax_destroyed, ax_restored, ax_need_save, [arg]{
	common
	local opcode, origin, counter
	counter = 0
	forward
	define type@param
	define definition@param arg
	param@parser type@param, definition@param
	match param, definition@param\{
		; parameter size calculation

		size@param = pointer.size
		if type@param eq double | type@param eq qword | type@param eq sep_qword
			size@param = 8
		else if param eqtype 0 | param eqtype 0f | type@param eq addr | type@param eq float
			size@param = pointer.size
		else if param eqtype byte 0 | param eqtype byte 0f | param eqtype byte [0]
			match prefix value,definition@param\\{ 
				if prefix eq qword
					size@param = 8
				else if prefix eq dword
					size@param = 4
				else if prefix eq word
					size@param = 2
				else if prefix eq byte
					size@param = 1
				end if 
			\\}
		else if ~ param in <XMM_REGS>
			virtual
				match =x86, __architecture\\{
					use64
				\\}
				origin = $	
				if type@param eq qword
					inc qword param
				else
					inc param
				end if
				load opcode byte from origin
				if opcode = 67h | opcode = 41h
					load opcode byte from origin+1
				end if
				if opcode and 0F8h = 48h
					size@param = 8
				else if opcode = 66h
					size@param = 2
				else if opcode = 0FFh
					size@param = 4
				else
					size@param = 1
				end if
				match =x86, __architecture\\{
					use32
				\\}
			end virtual
		end if

		; load parameter to memory
		va_list@load counter, ax_destroyed, ax_restored, ax_need_save, size@param, type@param, param
		
		if size@param > pointer.size
			counter = counter + size@param
		else
			counter = counter + pointer.size
		end if
	\}
	common
	if counter > va_list@counter
		va_list@counter = counter
	end if
}


macro param@parser type@param, definition@param{
	match any1[any2], _ definition@param \{ rept 0\{ \} match any1:any2, definition@param\{
		define type@param sep_qword 
	\}
	match =double value,definition@param\{ 
		define definition@param value
		define type@param double 
	\}
	match =qword value,definition@param\{ 
		define definition@param value
		define type@param qword 
	\}
	match =float value,definition@param\{ 
		define definition@param value
		define type@param float 
	\}
	match =addr value,definition@param\{ 
		define type@param addr 
		define definition@param value
		match any1 ( any2, [:] value [:]\\{
			\\local ..label_addr
			virtual at value
				..label_addr:
			end virtual
			define definition@param ..label_addr
		rept 0\\{\\} rept 1\\{
			define definition@param value
		\\}
	\}
	match =data value,definition@param\{ 
		\local arr@name
		var@litStack equ arr@name:
		var@litStack equ value
		define definition@param arr@name
		define type@param addr 
	\}
	match =obj value,definition@param\{ 
		\local arr@name
		var@litStack equ arr@name value
		define definition@param arr@name
		define type@param addr 
	\}
	match =const value, definition@param\{
		\local ..arr@name
		..arr@name @const value
		define definition@param ..arr@name
	\}
	match =const_obj value, definition@param\{
		\local ..arr@name, ..arr@bufName
		..arr@bufName @const value
		virtual at ..arr@bufName
			..arr@name value
		end virtual
		define definition@param ..arr@name
	\}
	match =va_list list, definition@param\{
		define type@param va_list
		define definition@param list
	rept 0\{\}rept 1\{
		match start=,any, definition@param\\{ 
			\local arr@name
			match type firstarg, start\\\{
				arr@name @const definition@param
			rept 0\\\{\\\} rept 1\\\{
				arr@name @const db definition@param, 0
			\\\}
			define definition@param arr@name
		\\}
		match =L any, definition@param\\{ 
			\\local arr@name
			match \\`any,any\\\{
				arr@name @const cwstr any
				define definition@param arr@name
			\\\} 
		\\}
		match any, definition@param\\{ 
			match \\`any,any\\\{
				\\\local arr@name
				arr@name @const cstr definition@param
				define definition@param arr@name
			\\\} 
		\\}
	\}
}

macro inline@parse param@result, [args]{
	common
	local args@count
	args@count equ 0
	; __argscount__ equ
	param@result equ 
	forward
	local paramdef
	paramdef equ args
	paramdef#._Type equ
	match any1:any2, args\{
		match ptype, paramdef#._Type\\{
			define ptype sep_qword
		\\}
	rept 0\{\}rept 1\{
		param@parser paramdef#._Type, paramdef
	\}
	match any, param@result\{
		param@result equ param@result,
	\}
	match =? any, paramdef#._Val\{
		paramdef equ any
	\}
	param@result equ param@result <paramdef#._Type paramdef>
	args@count equ args@count+1
	common
	local _counter
	rept 1 _counter:args@count\{
		__argscount__ equ _counter
	\}\
}


macro define@call{
	macro @call args&\{
		\define@call
		\local call_conv, argums, dest, matched
		define argums args
		dest equ
		match dst == any, argums\\{
			; restore dest
			dest equ dst
			; restore argums
			define argums any
		\\}
		match conv, __std_call\\{
			call_conv equ conv
		\\}
		match obj->proc(_argums, argums\\{
			define matched
			restore call_conv
			\\local _obj, _obj_addr, any1, any2
			define _obj obj
			define _obj_addr addr _obj
			match any1 )->any2, (_argums\\\{
				@call obj->proc any1)
				@call obj->any2
				; restore _obj
				define _obj
				; restore argums
				define argums
			\\\}
			match any1 [any2], _obj\\\{
				define _obj any2
				define _obj_addr [any2]
				define call_conv any1
			\\\}
			match [any2], _obj\\\{
				define _obj any2
				define _obj_addr [any2]
			\\\}
			match _obj_|_obj_addr_|base_conv, _obj|_obj_addr|__obj_call \\\{
				\\\local __type
				match =_obj_\\\#.BaseType, _obj_\\\#.BaseType\\\\{
					__type equ _obj_\\\#.Type
				rept 0\\\\{\\\\}rept 1\\\\{
					__type equ _obj_\\\#.BaseType
				\\\\}
				match MethTable ObjType, _obj_\\\#.Table __type \\\\{
					match =call_conv|_call_conv, call_conv|proc\\#.Conv\\\\#ObjType \\\\\{
						call_conv equ _call_conv
					\\\\\}
					argums equ MethTable\\\\#.\\#proc, _obj_addr_
					match any), _argums \\\\\{
						argums equ argums, any
					\\\\\}
					; local time@argums
					match =virtual _call_conv _argums_, proc\\#.Type\\\\#ObjType call_conv argums\\\\\{
						match _argums_, argums\\\\\\{
							inline@parse argums, _argums_
						\\\\\\}
						macro call arg1\\\\\\{
							__virtobj@call _obj_addr, ObjType, proc, MethTable
							; match [any1], _obj_addr\\\\\\\{
							; 	mov pax, [_obj_]
							; 	mov pax, [pax + ObjType\\\\#.VTable]
							; rept 0\\\\\\\{\\\\\\\}rept 1\\\\\\\{
							; 	mov pax, [_obj + ObjType\\\\#.VTable]
							; \\\\\\\}
							; call [pax + MethTable\\\\#.\\\\\#proc-MethTable]
						\\\\\\}
						_call_conv\\\\\#call _argums_
						purge call
						define argums
					\\\\\}
					match =inline _MacName _proc=,_argums_, proc\\#.Type\\\\#ObjType proc\\#.Value\\\\#ObjType argums\\\\\{
						inline@parse argums, _argums_
						match __argums_, argums\\\\\\{
							_MacName __argums_
						\\\\\\}
						restore __argscount__
						define argums
					\\\\\}
				\\\\}
			\\\}
		\\}
		match _Struct::>proc(_argums, argums\\{
			define matched
			; restore call_conv
			\\local __Struct
			define __Struct _Struct
			match _call_conv _Struct_, _Struct\\\{
				define __Struct _Struct_
				call_conv equ _call_conv
			rept 0\\\{\\\} match _call_conv, proc\\#.Conv\\#_Struct \\\{
				call_conv equ _call_conv
			\\\}
			match _Struct_, __Struct\\\{
				use@table _Struct_\\\#.Table
				argums equ _Struct_\\\#.Table\\\#.\\#proc
				; match =virtual, proc\\#.Type\\\#_Struct_\\\\{
				; 	argums equ call_conv pointer[argums]
				; \\\\}
				match any), _argums \\\\{
					argums equ argums, any
				\\\\}
				; local time@argums
				match =virtual _call_conv _argums_, proc\\#.Type\\\#_Struct_ call_conv argums\\\\{
					\\\\local arg1, any, _obj_addr, __proc
					match __proc=, arg1=, any, _argums_\\\\\{
						_obj_addr equ arg1
					rept 0 \\\\\{\\\\\}match __proc=, arg1, _argums_\\\\\{
						_obj_addr equ arg1
						; display arg1
					\\\\\}
					match =addr arg1, _obj_addr\\\\\{
						_obj_addr equ arg1
					\\\\\}
					match _argums_, argums\\\\\{
						inline@parse argums, _argums_
					\\\\\}
					macro call arg1\\\\\{
						match any1], _obj_addr\\\\\\{
							mov pax, _obj_addr
							mov pax, [pax + _Struct_\\\#.VTable]
						rept 0\\\\\\{\\\\\\}rept 1\\\\\\{
							mov pax, [_obj_addr + _Struct_\\\#.VTable]
						\\\\\\}
						call [pax + _Struct_\\\#.Table\\\\#.\\\\\#proc-_Struct_\\\#.Table]
					\\\\\}
					_call_conv\\\\#call _argums_
					purge call
					define argums
				\\\\}
				match =inline _MacName _proc=,_argums_, proc\\#.Type\\\#_Struct_ proc\\#.Value\\\#_Struct_ argums\\\\{
					inline@parse argums, _argums_
					match __argums_, argums\\\\\{
						_MacName __argums_
					\\\\\}
					restore __argscount__
					define argums
				\\\\}
				match =inline _MacName _proc, proc\\#.Type\\\#_Struct_ proc\\#.Value\\\#_Struct_ argums\\\\{
					_MacName
					define argums
				\\\\}
			\\\}
		\\}
		match =matched [::] _Struct::proc(_argums, matched [::] argums\\{
			define matched
			; restore call_conv
			local __Struct
			define __Struct _Struct
			match _call_conv _Struct_, _Struct\\\{
				define __Struct _Struct_
				call_conv equ _call_conv
			rept 0\\\{\\\} match _call_conv, proc\\#.Conv\\#_Struct \\\{
				call_conv equ _call_conv
			\\\}
			match _Struct_, __Struct\\\{
				use@table _Struct_\\\#.Table
				argums equ _Struct_\\\#.Table\\\#.\\#proc
				match =virtual, proc\\#.Type\\\#_Struct_\\\\{
					argums equ call_conv pointer[argums]
				\\\\}
				match any), _argums \\\\{
					argums equ argums, any
				\\\\}
				; local time@argums
				match =inline _MacName _proc=,_argums_, proc\\#.Type\\\#_Struct_ proc\\#.Value\\\#_Struct_ argums\\\\{
					inline@parse argums, _argums_
					match __argums_, argums\\\\\{
						_MacName __argums_
					\\\\\}
					restore __argscount__
					define argums
				\\\\}
				match =inline _MacName _proc, proc\\#.Type\\\#_Struct_ proc\\#.Value\\\#_Struct_ argums\\\\{
					_MacName
					define argums
				\\\\}
			\\\}
		\\}
		match [:=matched:]_proc ( _argums , [:matched:]argums\\{
			define argums _proc
			match any), _argums\\\{
				argums equ argums, any
			\\\}
		\\}
		match _call_conv _argums, argums\\{
			call_conv equ _call_conv
			define argums _argums
			match =[, call_conv\\\{
				restore argums
				restore call_conv
			\\\}
			match =, any, _argums\\\{
				restore argums
				restore call_conv
			\\\}
		\\}
		match _argums_, argums\\{
			inline@parse argums, _argums_
		\\}
		match _call_conv _argums, call_conv argums\\{
			; display \\`_call_conv
			_call_conv\\#call _argums
			restore __argscount__
		\\}
		match any, dest\\{
			fill@dest_proc any
		\\}
	\}
}

; match , {
; 	local call_conv, argums, dest, matched
; 	local _obj, _obj_addr, any1, any2
; 	local __type, __Struct
; 	local arg1, any, _obj_addr, __proc
; 	macro define@call\{
; 		macro @call args&\\{
; 			\define@call
; 			; \local matched
; 			define matched not_matched
; 			match conv, __std_call\\\{
; 				call_conv equ conv
; 			\\\}
; 			define argums args
; 			dest equ
; 			match dst == any, argums\\\{
; 				; restore dest
; 				dest equ dst
; 				; restore argums
; 				define argums any
; 			\\\}
; 			match obj->proc(_argums, argums\\\{
; 				define matched
; 				restore call_conv
; 				define _obj obj
; 				define _obj_addr addr _obj
; 				match any1 )->any2, (_argums\\\\{
; 					@call obj->proc any1)
; 					@call obj->any2
; 					; restore _obj
; 					define _obj
; 					; restore argums
; 					define argums
; 				\\\\} 
; 				; match _call_conv _obj_, _obj\\\\{
; 				; 	define _obj _obj_
; 				; 	; call_conv equ _call_conv
; 				; \\\\}
; 				match any1 [any2], _obj\\\\{
; 					define _obj any2
; 					define _obj_addr [any2]
; 					define call_conv any1
; 				\\\\}
; 				match [any2], _obj\\\\{
; 					define _obj any2
; 					define _obj_addr [any2]
; 				\\\\}
; 				match _obj_|_obj_addr_|base_conv, _obj|_obj_addr|__obj_call \\\\{
; 					; display \\\`_obj_, 0Ah
; 					; match any, _obj_\\\#.BaseType\\\\\{
; 					; 	display \\\\`any, 0Ah
; 					; \\\\\}
; 					match =_obj_\\\\#.BaseType, _obj_\\\\#.BaseType\\\\\{
; 						__type equ _obj_\\\\#.Type
; 					rept 0\\\\\{\\\\\}rept 1\\\\\{
; 						__type equ _obj_\\\\#.BaseType
; 					\\\\\}
; 					match MethTable ObjType, _obj_\\\\#.Table __type \\\\\{
; 						match =call_conv|_call_conv, call_conv|proc\\\#.Conv\\\\\#ObjType \\\\\\{
; 							call_conv equ _call_conv
; 						\\\\\\}
; 						argums equ MethTable\\\\\#.\\\#proc, _obj_addr_
; 						match any), _argums \\\\\\{
; 							argums equ argums, any
; 						\\\\\\}
; 						; local time@argums
; 						; match type, proc\\\#.Type\\\\\#ObjType\\\\\\{
; 						; 	display \\\\\\`type, 0Ah
; 						; \\\\\\}
; 						match =virtual _call_conv _argums_, proc\\\#.Type\\\\\#ObjType call_conv argums\\\\\\{
; 							match _argums_, argums\\\\\\\{
; 								inline@parse argums, _argums_
; 							\\\\\\\}
; 							macro call arg1\\\\\\\{
; 								match [any1], _obj_addr\\\\\\\\{
; 									mov pax, [_obj_]
; 									mov pax, [pax + ObjType\\\\\#.VTable]
; 								rept 0\\\\\\\\{\\\\\\\\}rept 1\\\\\\\\{
; 									mov pax, [_obj + ObjType\\\\\#.VTable]
; 								\\\\\\\\}
; 								call [pax + MethTable\\\\\#.\\\\\\#proc-MethTable]
; 							\\\\\\\}
; 							_call_conv\\\\\\#call _argums_
; 							purge call
; 							define argums
; 						\\\\\\}
; 						match =inline _MacName _proc=,_argums_, proc\\\#.Type\\\\\#ObjType proc\\\#.Value\\\\\#ObjType argums\\\\\\{
; 							inline@parse argums, _argums_
; 							match __argums_, argums\\\\\\\{
; 								_MacName __argums_
; 							\\\\\\\}
; 							restore __argscount__
; 							define argums
; 						\\\\\\}
; 					\\\\\}
; 				\\\\}
; 			\\\}
; 			match _Struct::>proc(_argums, argums\\\{
; 				define matched
; 				; restore call_conv
; 				define __Struct _Struct
; 				match _call_conv _Struct_, _Struct\\\\{
; 					define __Struct _Struct_
; 					call_conv equ _call_conv
; 				rept 0\\\\{\\\\} match _call_conv, proc\\\#.Conv\\\#_Struct \\\\{
; 					call_conv equ _call_conv
; 				\\\\}
; 				match _Struct_, __Struct\\\\{
; 					argums equ _Struct_\\\\#.Table\\\\#.\\\#proc
; 					; match =virtual, proc\\#.Type\\\#_Struct_\\\\\{
; 					; 	argums equ call_conv pointer[argums]
; 					; \\\\\}
; 					match any), _argums \\\\\{
; 						argums equ argums, any
; 					\\\\\}
; 					; local time@argums
; 					match =virtual _call_conv _argums_, proc\\\#.Type\\\\#_Struct_ call_conv argums\\\\\{
; 						match __proc=, arg1=, any, _argums_\\\\\\{
; 							_obj_addr equ arg1
; 						rept 0 \\\\\\{\\\\\\}match __proc=, arg1, _argums_\\\\\\{
; 							_obj_addr equ arg1
; 							; display arg1
; 						\\\\\\}
; 						match =addr arg1, _obj_addr\\\\\\{
; 							_obj_addr equ arg1
; 						\\\\\\}
; 						match _argums_, argums\\\\\\{
; 							inline@parse argums, _argums_
; 						\\\\\\}
; 						macro call arg1\\\\\\{
; 							match any1], _obj_addr\\\\\\\{
; 								mov pax, _obj_addr
; 								mov pax, [pax + _Struct_\\\\#.VTable]
; 							rept 0\\\\\\\{\\\\\\\}rept 1\\\\\\\{
; 								mov pax, [_obj_addr + _Struct_\\\\#.VTable]
; 							\\\\\\\}
; 							call [pax + _Struct_\\\\#.Table\\\\\#.\\\\\\#proc-_Struct_\\\\#.Table]
; 						\\\\\\}
; 						_call_conv\\\\\#call _argums_
; 						purge call
; 						define argums
; 					\\\\\}
; 					match =inline _MacName _proc=,_argums_, proc\\#.Type\\\\#_Struct_ proc\\#.Value\\\#_Struct_ argums\\\\\{
; 						inline@parse argums, _argums_
; 						match __argums_, argums\\\\\\{
; 							_MacName __argums_
; 						\\\\\\}
; 						restore __argscount__
; 						define argums
; 					\\\\\}
; 					match =inline _MacName _proc, proc\\\#.Type\\\\#_Struct_ proc\\\#.Value\\\\#_Struct_ argums\\\\\{
; 						_MacName
; 						define argums
; 					\\\\\}
; 				\\\\}
; 			\\\}
; 			match =not_matched [::] _Struct::proc(_argums, matched [::] argums\\\{
; 				define matched
; 				; restore call_conv
; 				define __Struct _Struct
; 				match _call_conv _Struct_, _Struct\\\\{
; 					define __Struct _Struct_
; 					call_conv equ _call_conv
; 				rept 0\\\\{\\\\} match _call_conv, proc\\\#.Conv\\\#_Struct \\\\{
; 					call_conv equ _call_conv
; 				\\\\}
; 				match _Struct_, __Struct\\\\{
; 					argums equ _Struct_\\\\#.Table\\\\#.\\\#proc
; 					match =virtual, proc\\\#.Type\\\\#_Struct_\\\\\{
; 						argums equ call_conv pointer[argums]
; 					\\\\\}
; 					match any), _argums \\\\\{
; 						argums equ argums, any
; 					\\\\\}
; 					; local time@argums
; 					match =inline _MacName _proc=,_argums_, proc\\\#.Type\\\\#_Struct_ proc\\\#.Value\\\\#_Struct_ argums\\\\\{
; 						inline@parse argums, _argums_
; 						match __argums_, argums\\\\\\{
; 							_MacName __argums_
; 						\\\\\\}
; 						restore __argscount__
; 						define argums
; 					\\\\\}
; 					match =inline _MacName _proc, proc\\\#.Type\\\\#_Struct_ proc\\\#.Value\\\\#_Struct_ argums\\\\\{
; 						_MacName
; 						define argums
; 					\\\\\}
; 				\\\\}
; 			\\\}
; 			match [:=not_matched:]_proc ( _argums , [:matched:]argums\\\{
; 				define argums _proc
; 				match any), _argums\\\\{
; 					argums equ argums, any
; 				\\\\}
; 			\\\}
; 			match _call_conv _argums, argums\\\{
; 				call_conv equ _call_conv
; 				define argums _argums
; 				match =[, call_conv\\\\{
; 					restore argums
; 					restore call_conv
; 				\\\\}
; 				match =, any, _argums\\\\{
; 					restore argums
; 					restore call_conv
; 				\\\\}
; 			\\\}
; 			match _argums_, argums\\\{
; 				inline@parse argums, _argums_
; 			\\\}
; 			match _call_conv _argums, call_conv argums\\\{
; 				; display \\`_call_conv
; 				_call_conv\\\#call _argums
; 				restore __argscount__
; 			\\\}
; 			match any, dest\\\{
; 				fill@dest_proc any
; 			\\\}
; 		\\}
; 	\}
; }
define@call