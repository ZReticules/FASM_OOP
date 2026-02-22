; include 'macro\struct.inc'
; include 'macro\proc64.inc'
; dm fix method
;обращаю внимание на вложенность. Чем больше уровень вложенности макроконструкций, тем больше нужно косых черт
;это определяет, на каком уровне вложенности обработается конкретное действие, выражение или переменная

; @__specialfields equ
include "ParamsParsing.inc"

macro add_specialfields [names]{
	forward
	@__specialfields equ names
}

include "default_specialfields.inc"

macro allow_specialfields{
	match name, struct@lastname\{
		irpv _name, @__specialfields\\{
			match def@macros, specialfields_define_\\\#_name\\\{
				def@macros name
			\\\}
		\\}
	\}
}

struc def@label type{
	label . type 
}

macro def@label arg{

}

macro def@sublabel field, type, def{
	field type def
}

macro struct NameParent{
	struc label type\{
		local any
		match any, field@struct\\{
			restore field@struct
			struct
				def@sublabel any
				field@struct equ ., def@label, type
			ends
		\\}
	\}
	struct NameParent
	match name parent, NameParent\{
		define matched@params
		struct@lastname equ name							;в этой переменной между struct и ends сохраняется 
															;имя последней использованной структуры
		name\#@SParent equ parent							;к имени так же привязывается родитель
		name\#@New equ TRUE
		allow_specialfields
		match =TRUE, parent\#@New\\{
			irpv _name, @__specialfields\\\{
				match inherit@macros, secialfields_inherit_\\\#_name\\\\{
					inherit@macros parent, name
				\\\\}
			\\\}
		\\}
	rept 0\{\}rept 1\{
		NameParent\#@New equ TRUE
		struct@lastname equ NameParent
		allow_specialfields
	\}
}

macro make@atr name, atrname, atrtype, atrval{
	name#.#atrname atrtype atrval
}

macro ends@handler {
	match name, struct@lastname\{

		irpv _name, @__specialfields\\{
			reverse
			match ends@macros, specialfields_ends_\\#_name\\\{
				ends@macros name
			\\\}
		\\}
		restore struct@lastname
	\}
}

; define ends@handler ends@default

macro ends {
	local any
	match any, sub@struct\{
		ends
	rept 0\{\}rept 1\{
		restruc label
		ends
		ends@handler
	\}
}

struct @interface
	const __interface equ 1
	VTable dptr ?
ends

macro fillParam reg*, src*{
	local matched
	match =addr _src, src\{
		if ~any eq reg
			lea reg, [_src]
		end if
		define matched
	\}
	match any[addr], _ src\{
		mov reg, src
		define matched
	\}
	match =matched, matched\{
		if ~src eq reg
			if src eqtype 0 & src relativeto 0
				mov reg, src
			else
				lea reg, [src]
			end if
		end if
	\}
}

macro inlineObj name*, val*, reg{
	name equ val
	match [any], val\{
		match , reg\\{
			display "Object must be static or register-based!"
			err
		\\}
		mov reg, val
		name equ reg
	\}
	match =ptr any, val\{		
		match , reg\\{
			display "Object must be static or register-based!"
			err
		\\}
		mov reg, val
		name equ reg
	\}
	match _any[any], val\{		
		match , reg\\{
			display "Object must be static or register-based!"
			err
		\\}
		mov reg, val
		name equ reg
	\}
	match _any =ptr any, val\{
		match , reg\\{
			display "Object must be static or register-based!"
			err
		\\}
		mov reg, val
		name equ reg
	\}
	match =addr any, val\{
		name equ any
	\}
	match & any, val\{
		name equ any
	\}
}

macro virtObj args&{
	local register, typeof, nameof, args_statement, source
	source equ
	register equ pcx
	args_statement equ args
	match any =from srcval, args_statement\{
		source equ srcval
		; restore args_statement
		args_statement equ any
	\}
	match any =at reg, args_statement\{
		register equ reg
		; restore args_statement
		args_statement equ any
	\}
	match name:=arg type, args_statement\{
		nameof equ name
		typeof equ type
		; match =1|reg, type\#@Virtual|register\\{
		; 	restore register
		; 	register equ reg - pointer.size
		; \\}
		; restore args_statement
		args_statement equ
	\}
	match name type, args_statement\{
		nameof equ name
		typeof equ type
	\}
	match reg|name|type, register|nameof|typeof\{
		virtual at reg
			name type
		end virtual
		match =addr any, source\\{
			if ~reg eq any
				if any eqtype 0 & any relativeto 0
					mov reg, any
				else
					lea reg, [any]
				end if
			end if
		rept 0\\{\\} match any, source\\{
			if ~source eq reg
				mov reg, source
			end if
		\\}
	\}
}

; macro def_def@fields@virt{
; 	macro def@fields@virt sub_struct, base_addr, obj_name, struct_name, [name, type, def]\{
; 		\common
; 		match =1, sub_struct\\{ rept 0\\{\\} rept 1\\{
; 			; display \`obj_name, ' ', \`struct_name, 0Ah
; 			; match vars, all@vars \\\{ 
; 			; 	all@vars equ all@vars, 
; 			; \\\}
; 			all@vars equ obj_name, obj_name\#.Type, obj_name\#.size, obj_name\#.Table
; 			obj_name\#.Type equ sub_struct
; 			; define obj_name\#.TypeName  \`struct_name
; 			obj_name\#.size equ (sizeof.\#struct_name)
; 			obj_name\#.Table equ struct_name\#.Table
; 			obj_name equ base_addr
; 		\\}
; 		\forward
; 		match any, name\\{
; 		; match =substruct, struct_name\\\{
; 		; 	display name, type, def
; 		; \\\}
; 			; match fname, struct_name\#.\#name\\\{
; 			; 	display 09h, \\\`fname, " ", \`type, 0Ah
; 			; \\\}
; 			match =fields@\#type, fields@\#type\\\{
; 				; match vars, all@vars \\\\{ 
; 				; 	all@vars equ all@vars, 
; 				; \\\\}
; 				all@vars equ obj_name\#.\#name
; 				obj_name\#.\#name equ base_addr + struct_name\#.\#name
; 			rept 0\\\{\\\} match any, fields@\#type\\\{
; 				\def_def@fields@virt
; 				def@fields@virt type, base_addr, obj_name\#.\#name, struct_name\#.\#name, any
; 				; display any
; 			\\\}
; 		rept 0\\{\\} match =,any, def\\{
; 			\def_def@fields@virt
; 			; display \`type, 0Ah
; 			def@fields@virt 1, base_addr, obj_name, struct_name, any
; 		\\}
; 	\}
; }

; def_def@fields@virt

; macro @virtObj [argums]{
; 	common
; 	local register, typeof, nameof, argums_statement, source
; 	source equ
; 	register equ pcx
; 	define argums_statement argums
; 	match any =from srcval, argums_statement\{
; 		source equ srcval
; 		restore argums_statement
; 		define argums_statement any
; 	\}
; 	match any =at reg, argums_statement\{
; 		register equ reg
; 		restore argums_statement
; 		define argums_statement any
; 	\}
; 	match name:=arg type, argums_statement\{
; 		name\#.Lp equ register
; 		nameof equ name
; 		typeof equ type
; 		match =1|reg, type\#@Virtual|register\\{
; 			restore register
; 			register equ reg + pointer.size
; 		\\}
; 		restore argums_statement
; 		define argums_statement
; 	\}
; 	match name type, argums_statement\{
; 		name\#.Lp equ register
; 		nameof equ name
; 		typeof equ type
; 	\}
; 	match reg|name|type, register|nameof|typeof\{
; 		match any, fields@\#type\\{
; 			def@fields@virt type, reg, name, type, any
; 		\\}
; 		match =addr any, source\\{
; 			if ~reg eq any
; 				if any eqtype 0 & any relativeto 0
; 					mov reg, any
; 				else
; 					lea reg, [any]
; 				end if
; 			end if
; 		rept 0\\{\\} match any, source\\{
; 			if ~source eq reg
; 				mov reg, source
; 			end if
; 		\\}
; 	\}
; }

macro def@fields@virt name, def, [val]{
	; match any, name\{
	; 	display \`any, 0Ah
	; \}
	; match =all@vars, all@vars\{
	; 	match vars, all@vars \\{ 
	; 		all@vars equ all@vars, 
	; 	\\}
	; 	all@vars equ all@vars name
	; \}
	; local ..var,..tmp
	; display `..var, " ", `name, " ", `def, 0Ah
	; ..var def val
	; name equ ..var
	common
	; match vars, all@vars \{ 
	; 	_all@vars equ _all@vars, 
	; \}
	local all_wars
	match all_vars, all@vars\{
		all_vars equ name
	\} 
   	forward
	local ..var
	..var def val
   	common
	match first rest, ..var, \{ 
		name equ first 
	\} 
}

macro @virtObj [argums]{
	common
	local register, typeof, nameof, argums_statement, source
	source equ
	register equ pcx
	define argums_statement argums
	match any =from srcval, argums_statement\{
		source equ srcval
		; restore argums_statement
		define argums_statement any
	\}
	match any =at reg, argums_statement\{
		register equ reg
		; restore argums_statement
		define argums_statement any
	\}
	match name:=arg type, argums_statement\{
		nameof equ name
		typeof equ type
		; match =1|reg, type\#@Virtual|register\\{
		; 	restore register
		; 	register equ reg - pointer.size
		; \\}
		; restore argums_statement
		define argums_statement
	\}
	match name type, argums_statement\{
		; display \`name
		nameof equ name
		typeof equ type
	\}
	match reg|name|type, register|nameof|typeof\{
		; match vars, all@vars \\{ 
		; 	all@vars equ all@vars, 
		; \\}
		; all@vars equ all@vars name
		macro label def \\{ 
			match . type, def> \\\{ 
				def@fields@virt .,label,<type 
			\\\} 
		\\}
		irp def, db, du, dw, dp, dd, dt, dq \\{
			struc def val& \\\{ 
				def@fields@virt ., def, val 
			\\\}
		\\}
		irp def, rb, rw, rp, rd, rt, rq\\{
			struc def cnt& \\\{ 
				def@fields@virt ., def cnt, 
			\\\}
		\\}
		virtual at reg
			forward
			name type
			common
		end virtual
		purge label
		restruc db,du,dw,dp,dd,dt,dq
		restruc rb,rw,rp,rd,rt,rq
		match =addr any, source\\{
			if ~reg eq any
				if any eqtype 0 & any relativeto 0
					mov reg, any
				else
					lea reg, [any]
				end if
			end if
		rept 0\\{\\} match any, source\\{
			if ~source eq reg
				mov reg, source
			end if
		\\}
	\}
}

macro @init_liters{
	match =@init_liters, @init_liters\{
		if ~(var@litStack eq)
			irpv liter, var@litStack\\{
				liter
			\\}
		end if
		define @init_liters
	\}
}

total_postpone equ @init_liters

; macro initials at_base { 
; 	match =at base, at_base\{
; 		virtual at base
; 	  	macro label def \\{ 
; 	  		match . type,def> \\\{ 
; 	  			defdynamic@proc .,label,<type 
;   			\\\} 
; 		\\}
; 	  	struc db [val] \\{ 
; 	  		\common defdynamic@proc .,db,val 
;   		\\}
; 		struc du [val] \\{ 
; 			\common defdynamic@proc .,du,val 
; 		\\}
; 		struc dw [val] \\{ 
; 			\common defdynamic@proc .,dw,val 
; 		\\}
; 		struc dp [val] \\{ 
; 			\common defdynamic@proc .,dp,val 
; 		\\}
; 		struc dd [val] \\{ 
; 			\common defdynamic@proc .,dd,val 
; 		\\}
; 		struc dt [val] \\{ 
; 			\common defdynamic@proc .,dt,val 
; 		\\}
; 		struc dq [val] \\{ 
; 			\common defdynamic@proc .,dq,val 
; 		\\}
; 		struc rb cnt \\{ 
; 			defdynamic@proc .,rb cnt, 
; 		\\}
; 		struc rw cnt \\{ 
; 			defdynamic@proc .,rw cnt, 
; 		\\}
; 		struc rp cnt \\{ 
; 			defdynamic@proc .,rp cnt, 
; 		\\}
; 		struc rd cnt \\{ 
; 			defdynamic@proc .,rd cnt, 
; 		\\}
; 		struc rt cnt \\{ 
; 			defdynamic@proc .,rt cnt, 
; 		\\}
; 		struc rq cnt \\{ 
; 			defdynamic@proc .,rq cnt, 
; 		\\} 
; 	\}
; }
; 	macro endi { 
; 	purge label
; 	restruc db,du,dw,dp,dd,dt,dq
; 	restruc rb,rw,rp,rd,rt,rq
; 	end virtual 
; }

; macro defdynamic@proc name,def,[val] {
; 	common
;    	forward
; 	local ..var,..tmp
; 	..var def val
; 	match =?, val \{ 
; 		..tmp equ 
; 	\}
; 	match any =?, val \{ 
; 		..tmp equ 
; 	\}
; 	match any (=?), val \{ 
; 		..tmp equ 
; 	\}
; 	match =label, def \{ 
; 		..tmp equ 
; 	\}
; 	match tmp : value, ..tmp : val \{ 
; 		tmp: end virtual
; 		initdynamic@proc ..var,def value
; 		virtual at tmp
; 	\}
;    	common
; 	match first rest, ..var, \{ 
; 		name equ first 
; 	\} 
; }

; macro initdynamic@proc name,def { 
; 	virtual at name
; 		def
; 		size@initdynamic = $ - name
; 	end virtual
; 	position@initdynamic = 0
; 	while size@initdynamic > position@initdynamic
; 		virtual at name
; 			def
; 			if size@initdynamic - position@initdynamic < 2
; 				current@initdynamic = 1
; 				load byte@initlocal byte from name+position@initdynamic
; 			else if size@initdynamic - position@initdynamic < 4
; 				current@initdynamic = 2
; 				load word@initlocal word from name+position@initdynamic
; 			else if size@initdynamic - position@initdynamic < 8
; 				current@initdynamic = 4
; 				load dword@initlocal dword from name+position@initdynamic
; 			else
; 				load qword@initlocal qword from name+position@initdynamic
; 				if ( qword@initlocal > 0 & qword@initlocal < 80000000h ) | ( qword@initlocal < 0 & qword@initlocal >= -80000000h )
; 					current@initdynamic = 8
; 				else
; 					current@initdynamic = 4
; 					dword@initlocal = qword@initlocal and 0FFFFFFFFh
; 				end if
; 			end if
; 		end virtual
; 		if current@initdynamic = 1
; 			mov byte [name+position@initdynamic],byte@initlocal
; 		else if current@initdynamic = 2
; 			mov word [name+position@initdynamic],word@initlocal
; 		else if current@initdynamic = 4
; 			mov dword [name+position@initdynamic],dword@initlocal
; 		else
; 			mov qword [name+position@initdynamic],qword@initlocal
; 		end if
; 		position@initdynamic = position@initdynamic + current@initdynamic
; 	end while 
; }