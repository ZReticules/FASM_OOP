; include 'macro\struct.inc'
; include 'macro\proc64.inc'
dm fix method
;обращаю внимание на вложенность. Чем больше уровень вложенности макроконструкций, тем больше нужно косых черт
;это определяет, на каком уровне вложенности обработается конкретное действие, выражение или переменная
macro allow_methods{										;создаем макросы, которые позволят нам определять и
															;переопределять методы
	struc method val\{
		\local matched
		match name, struct@lastname\\{							;в этой константе хранится имя последней обьявленной структуры
			name\\#@MStack equ . 								;загружаем на вершину константы-стека новый метод
			match _val:type, val\\\{							;для виртуальных методов(и возможно дальнейшего расширения)
				.Value\\#name equ _val							;сразу закрепляем за ним значение(обработчик)
				.Type\\#name equ type							;и тип - пока что только виртуальные
				define matched
			\\\}
			match =matched, matched\\\{							;если тип не указан явно
				.Value\\#name equ val							;значит статический
				.Type\\#name equ static
			\\\}
			match =this, .Value\\#name\\\{						;если значение обработчика - this, то это значит
				.Value\\#name equ name\\\#\\\.\\\#.				;что надо подставить имя метода в формате StructName.MethodName
			\\\}												;это работает, так как процедура это метка и доступна везде
		\\}	
	\}

	struc override NewVal\{									;переопределеие метода
		match name, struct@lastname\\{
			irpv MethName, name\\#@MStack\\\{				;перебираем стековую переменную по значению с самого первого
				\\\local matched								;подробнее irpv описан в документации
				match =MethName, .\\\\{						;находим метод, имя которого совпадает с переопределяемым
					\\\\local _newVal
					_newVal equ NewVal
					match _val:type, _newVal\\\\\{
						_newVal equ _val
						.Type\\#name equ type
						match =virtual, type\\\\\\{
							display "can't change method's type to virtual"
							err
						\\\\\\}
						match =virtual, .Type\\#name\\\\\\{
							display "can't change method's type from virtual"
							err
						\\\\\\}
					\\\\\}
					match =this, _newVal\\\\\{				;меняем ему значение. В случае, если это this
						_newVal equ name\\#\\.\\#.	;выполняем описанную ранее операцию
					\\\\\}
					.Value\\#name equ _newVal
				\\\\}										;в будущем планирую сделать переопределение типа метода
			\\\}
		\\}
	\}

	macro const args\{
		match atrname atrtype atrval name, args struct@lastname\\{
			name\\#@ConStack equ atrname
			name\\#.\\#atrname\\#@Type equ atrtype
			name\\#.\\#atrname\\#@Value equ atrval
			name\\#.\\#atrname atrtype atrval
		\\}
	\}

	struc event procaddr\{
		match name, struct@lastname\\{	
			name\\#@EventStack equ .
			. dd procaddr
		\\}	
	\} 

}

macro make@Const arg1, arg2, arg3{
	arg1 arg2 arg3
}

struc def@label type{
	label . type 
}

macro def@label arg{

}

lb fix label

macro def@sublabel field, type, def{
	field type def
}

macro struct NameParent{
	local matched@endt, matched@params						;тут происходит определение разных локалок для двух отдельных
															;матчевых if...else 														;с таким же именем используется старое определение
															;это разработчиком компилятора сделано намеренно, дабы избежать рекурсии
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
	allow_methods
	match name parent, NameParent\{
		define matched@params
		; oldstruct@action equ name parent					;так как я активно использую старый struct, для него я сохраняю
		; 													;значения, которые ему передам для генерации основной структуры
		struct@lastname equ name							;в этой переменной между struct и ends сохраняется 
															;имя последней использованной структуры
		name\#@SParent equ parent							;к имени так же привязывается родитель
		name\#@New equ TRUE
		match =TRUE, parent\#@New\\{
			irpv ParentMethod, parent\#@MStack\\\{				;загружаем потомку методы родителя
				ParentMethod method ParentMethod\\\#.Value\\#parent:ParentMethod\\\#.Type\\#parent
				; display \\\`ParentMethod\\\#.Value\\#name
			\\\}
			irpv ParentConst, parent\#@ConStack\\\{
				; atrname\\#@Type equ atrtype
				; atrname\\#@Value equ atrval
				const ParentConst parent\#.\\\#ParentConst\\\#@Type parent\#.\\\#ParentConst\\\#@Value
			\\\}
			irpv ParentControl, parent\#@ConstrolStack\\\{
				name\#@ConstrolStack equ ParentControl
				name\#.\\\#ParentControl\\\#._x = parent\\#.\\\#ParentControl\\\#._x
				name\#.\\\#ParentControl\\\#._y = parent\\#.\\\#ParentControl\\\#._y
				name\#.\\\#ParentControl\\\#._cx = parent\\#.\\\#ParentControl\\\#._cx
				name\#.\\\#ParentControl\\\#._cy = parent\\#.\\\#ParentControl\\\#._cy
				name\#.\\\#ParentControl\\\#._rx = parent\\#.\\\#ParentControl\\\#._rx
				name\#.\\\#ParentControl\\\#._ry = parent\\#.\\\#ParentControl\\\#._ry
				name\#.\\\#ParentControl\\\#._style = parent\\#.\\\#ParentControl\\\#._style
				name\#.\\\#ParentControl\\\#._styleEx = parent\\#.\\\#ParentControl\\\#._styleEx
				name\#.\\\#ParentControl\\\#._Text equ parent\\#.\\\#ParentControl\\\#._Text
			\\\}
			irpv ParentEvent, parent\#@EventStack\\\{
				name\#@EventStack equ ParentEvent
			\\\}
		\\}
	\}
	match =matched@params, matched@params\{
		NameParent\#@New equ TRUE
		; oldstruct@action equ NameParent						;если родителя нет, параметры устанавливаются таким образом
		struct@lastname equ NameParent
	\}
}

macro MStruct@GenMethods name, StackName{						;макрос для генерации методов
	name#.Table:
	irpv MethName, StackName\{									;перебираем стек методов
	local matched
		match =static, MethName\#.Type#name\\{					;если метод статический - устаналвиаем числовую переменную
			if used .\\#MethName								;проверяем, что метод хоть раз был вызван
				.\\#MethName = MethName\\#.Value#name			;в значение адреса обработчика
			end if
			define matched
		\\}
		match =virtual, MethName\#.Type#name\\{
			name#.Table.\\#MethName dd MethName\\#.Value#name	;если метод не статический, значит виртуальный
			restore name#@Virtual								;заодно здесь мы проверяем, нужно ли создавать для обьектов
			name#@Virtual equ 1							;поле с указателем на таблицу виртуальных методов
		\\}
		restore matched
	\}
}

macro ends@handler{
	match name, struct@lastname\{
		restruc method  								;уаляем определения макросов для методов
		restruc override								;переопределения методов
		restruc const  									;и создания констант
		name\#@Virtual equ 0						;по умолчанию считаем, что таблица виртуальных методов не нужна
		MStruct@GenMethods name, name\\#@MStack		;при генерации определим так ли это
		; restore oldstruct@action		
		;здесь происходит переопределение стандартных
		;создаваемых struct макросов
		match =1, name\#@Virtual\\{
			sizeof.\#name = sizeof.\#name+4
			; vtable equ dd name\#.Table				;если нужно создать указатель на таблице виртуальных методов, добавляем константу
		\\}
		struc name args&\\{
			\\common
			.Table equ name\\#\\\.Table				;прикрепляем таблицу статически
			.Lp equ .								;каждому обьекту создаем точку начала
			match =1, name\#@Virtual \\\{					;проверяем, нужно ли создать VTable для константы
				restore .Lp
				.Lp equ .VTable
			\\\}
            match , @struct \\\{ 
                define field@struct .,name,<args> 
            \\\}
            match no, @struct \\\{ 
				match =1, name\#@Virtual \\\\{					;проверяем, нужно ли создать VTable
					.VTable dd name\#.Table
				\\\\}
				. name args
            \\\}
			.Type equ \name
			.TypeName equ \`name
			.size = sizeof.\#name
		\\}
		macro name args&\\{						;если имени у обьекта нет, то стандартный обработчик
			\\common
            match , @struct \\\{ 
            	\\\local anonymous
                define field@struct anonymous,name,<args> 
            \\\}
            match no, @struct \\\{ 
				match =1, name\#@Virtual\\\\{
					dd name\#.Table
				\\\\}
				name args
            \\\}
		\\}
		; \\}
		restore struct@lastname
	\}
}

; define ends@handler ends@default

macro ends{
	local matched, any
	match any, sub@struct\{
		define matched
		ends
	\}
	match =matched, matched\{
		restruc label
		ends
		ends@handler
	\}
}

macro param@parser type@param, definition@param{
	match =double value,definition@param\{ 
		define definition@param value
		define type@param double 
	\}
	match =addr value,definition@param\{ 
		define definition@param value
		define type@param addr 
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
		\local arr@name, const@name, arr@bufName, else@buf
		; define type@param addr
		const@litStack equ arr@bufName
		arr@bufName\#.enabled = 0
		arr@bufName\#.value equ value
		arr@bufName\#.name equ const@name
		define definition@param arr@name
		arr@name = 0
		else@buf equ
		irpv csl, const@litStack\\{
			else@buf if arr@name = 0 & csl\\#.len = arr@bufName\#.len & csl\\#.value eq value
				csl\\#.enabled = 1
				arr@name = csl
			else@buf equ else
		\\}
		end if
	\}
	match =const_obj value, definition@param\{
		\local arr@name, const@name, arr@bufName, else@buf
		; define type@param addr
		const@litStack equ arr@bufName
		arr@bufName\#.enabled = 0
		arr@bufName\#.value equ value
		arr@bufName\#.name equ const@name
		define definition@param arr@name
		arr@name = 0
		else@buf equ
		irpv csl, const@litStack\\{
			else@buf if arr@name = 0 & csl\\#.len = arr@bufName\#.len & csl\\#.value eq value
				csl\\#.enabled = 1
				arr@name = csl\\#.name
			else@buf equ else
		\\}
		end if
	\}
	match start=,any,definition@param\{ 
		\local arr@name, const@name, arr@bufName, matched1, else@buf
		; define type@param addr
		const@litStack equ arr@bufName
		arr@bufName\#.name equ const@name
		arr@bufName\#.enabled = 0
		match type firstarg, start\\{
			arr@bufName\#.value equ definition@param
			define matched1
		\\}
		match =matched1, matched1\\{
			arr@bufName\#.value equ db definition@param, 0
		\\}
		define definition@param arr@name
		arr@name = 0
		else@buf equ
		irpv csl, const@litStack\\{
			else@buf if arr@name = 0 & csl\\#.len = arr@bufName\#.len & csl\\#.value eq arr@bufName\#.value
				csl\\#.enabled = 1
				arr@name = csl
			else@buf equ else
		\\}
		end if
	\}
	match any,definition@param\{ 
		\local arr@name, const@name, arr@bufName, else@buf
		match \`any,any\\{ 
			define type@param addr
			const@litStack equ arr@bufName
			arr@bufName\#.enabled = 0
			arr@bufName\#.value equ db definition@param, 0
			arr@bufName\#.name equ const@name
			define definition@param arr@name
			arr@name = 0 
			else@buf equ
			irpv csl, const@litStack\\\{
				else@buf if arr@name = 0 & csl\\\#.len = arr@bufName\#.len & csl\\\#.value eq arr@bufName\#.value
					csl\\\#.enabled = 1
					arr@name = csl
				else@buf equ else
			\\\}
			end if
		\\} 
	\}
}

; @call_parser equ fastcall

; macro @oopcall proc,[arg]{ 
; 	common 
; 	local stackspace,argscount,counter
; 	if argscount < 4
; 		stackspace = 4*8
; 	else if argscount and 1
; 		stackspace = (argscount+1)*8
; 	else
; 		stackspace = argscount*8
; 	end if
; 	counter = 0
; 	if stackspace
; 		if defined current@frame
; 			if current@frame<stackspace
; 				current@frame = stackspace
; 			end if
; 		else
; 			if stackspace
; 				sub rsp,stackspace
; 			end if
; 		end if
; 	end if
; 	reverse
; 	counter = counter + 1
; 	define type@param
; 	define definition@param arg
; 	param@parser type@param, definition@param
; 	match param,definition@param\{ 
; 		local opcode,origin
; 		size@param = 0
; 		if param eqtype 0 | param eqtype 0f | type@param eq addr
; 			size@param = 8
; 		else if param eqtype byte 0 | param eqtype byte 0f
; 			match prefix value,definition@param\\{ 
; 				if prefix eq qword
; 					size@param = 8
; 				else if prefix eq dword
; 					size@param = 4
; 				else if prefix eq word
; 					size@param = 2
; 				else if prefix eq byte
; 					size@param = 1
; 				end if 
; 			\\}
; 		else if ~ param in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15>
; 			virtual
; 				origin = $
; 				inc param
; 				load opcode byte from origin
; 				if opcode = 67h | opcode = 41h
; 					load opcode byte from origin+1
; 				end if
; 				if opcode and 0F8h = 48h
; 					size@param = 8
; 				else if opcode = 66h
; 					size@param = 2
; 				else if opcode = 0FFh
; 					size@param = 4
; 				else
; 					size@param = 1
; 				end if
; 			end virtual
; 		end if
; 		if type@param eq float
; 			if ~ param eq xmm0
; 				if size@param = 4
; 					if param eqtype byte 0 | param eqtype byte 0f
; 						mov eax,param
; 						movd xmm0,eax
; 					else
; 						movd xmm0,param
; 					end if
; 				else
; 					if param eqtype 0 | param eqtype 0f | param eqtype byte 0 | param eqtype byte 0f
; 						mov rax,param
; 						movq xmm0,rax
; 					else
; 						movq xmm0,param
; 					end if
; 				end if
; 			end if
; 			if vararg@fastcall & ~ param eq rcx
; 				movq rcx,xmm0
; 			end if
; 		else if type@param eq addr
; 			if ~ param eq rcx
; 				lea rcx, [param]
; 			end if
; 		else if size@param = 8
; 			if ~ param eq rcx
; 				mov rcx,param
; 			end if
; 		else if size@param = 4
; 			if ~ param eq ecx
; 				mov ecx,param
; 			end if
; 		else if size@param = 2
; 			if ~ param eq cx
; 				movzx ecx,param
; 			end if
; 		else if size@param = 1
; 			if ~ param eq cl
; 				movzx ecx,param
; 			end if
; 		end if
; 	\}
; 	common
; 	argscount = counter
; 	call proc
; 	if stackspace & ~defined current@frame
; 		add rsp,stackspace
; 	end if 
; }

macro inline@parse param@result, [args]{
	common
	local args@count
	args@count equ 0
	__argscount__ equ
	param@result equ 
	forward
	local paramdef
	paramdef equ args
	paramdef#._Def equ args
	paramdef#._Type equ
	param@parser paramdef#._Type, paramdef
	match any, param@result\{
		param@result equ param@result,
	\}
	match =? any, paramdef#._Val\{
		paramdef equ any
	\}
	param@result equ param@result paramdef#._Type paramdef
	args@count equ args@count+1
	common
	local _counter
	rept args@count _counter\{
		args@count equ _counter
	\}
	__argscount__ equ args@count
}

macro fillParam reg*, src*{
	local matched
	match =addr _src, src\{
		if ~any eq reg
			lea reg, [_src]
		end if
		define matched
	\}
	match =matched, matched\{
		if ~src eq reg
			mov reg, src
		end if
	\}
}

macro inlineObj name*, val*, reg*{
	name equ val
	match [any], src\{
		if ~any eq reg
		mov reg, src
		name equ reg
	\}
	match =ptr any, src\{
		mov reg, src
		name equ reg
	\}
	match _any[any], src\{
		mov reg, src
		name equ reg
	\}
	match _any =ptr any, src\{
		mov reg, src
		name equ reg
	\}
	match =addr any, val\{
		name equ any
	\}
}

; @call_parser equ @oopcall

macro define@call{
	macro @call args&\{
		\define@call
		local call_conv, argums
		define argums args
		call_conv equ std
		match obj->proc(_argums, argums\\{
			local _obj
			define _obj obj
			match _call_conv _obj_, _obj\\\{
				define _obj _obj_
				call_conv equ _call_conv
			\\\}
			match _obj_, _obj\\\{
				match MethTable ObjType, _obj_\\\#.Table _obj_\\\#.Type\\\\{
					argums equ MethTable\\\\#.\\#proc, addr _obj_
					match any), _argums \\\\\{
						argums equ argums, any
					\\\\\}
					; local time@argums
					match =virtual _call_conv _argums_, proc\\#.Type\\\\#ObjType call_conv argums\\\\\{
						match _argums_, argums\\\\\\{
							inline@parse argums, _argums_
						\\\\\\}
						macro call arg1\\\\\\{
							mov eax, [_obj_\\\#.VTable]
							call [eax+MethTable\\\\#.\\\\\#proc-MethTable]
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
						define argums
					\\\\\}
				\\\\}
			\\\}
		\\}
		match _Struct:proc(_argums, argums\\{
			local __Struct
			define __Struct _Struct
			match _call_conv _Struct_, _Struct\\\{
				define __Struct _Struct_
				call_conv equ _call_conv
			\\\}
			match _Struct_, __Struct\\\{
				argums equ _Struct_\\\#.Table\\\#.\\#proc
				match =virtual, proc\\#.Type\\\#_Struct_\\\\{
					argums equ dword[argums]
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
					define argums
				\\\\}
				match =inline _MacName _proc, proc\\#.Type\\\#_Struct_ proc\\#.Value\\\#_Struct_ argums\\\\{
					_MacName
					define argums
				\\\\}
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
		match _proc ( _argums , argums\\{
			define argums _proc
			match any), _argums\\\{
				argums equ argums, any
			\\\}
		\\}
		match _argums_, argums\\{
			inline@parse argums, _argums_
		\\}
		match _call_conv _argums, call_conv argums\\{
			_call_conv\\#call _argums
		\\}
	\}
}

define@call

macro virtObj args&{
	local register, typeof, nameof, matched
	match name:=arg type =at reg, args\{
		match =1, type\#@Virtual\\{
			register equ reg-4
			define matched
		\\}
		match =matched, matched\\{
			register equ reg
			define matched
		\\}
		typeof equ type
		nameof equ name
	\}
	match =matched name:=arg type, matched args\{
		match =1, type\#@Virtual\\{
			register equ ecx-4
			define matched
		\\}
		match =matched, matched\\{
			register equ ecx
			define matched
		\\}
		typeof equ type
		nameof equ name
	\}
	match =matched name type at reg, matched args\{
		define matched
		register equ reg
		typeof equ type
		nameof equ name
	\}
	match =matched name type, matched args\{
		define matched
		register equ ecx
		typeof equ type
		nameof equ name
	\}
	match =matched, matched\{
		err "Wrong argument"
	\}
	match reg|name|type, register|nameof|typeof\{
		virtual at reg
			name type
		end virtual
	\}
}

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

postpone{
	if ~(var@litStack eq)
		irpv liter, var@litStack\{
			liter
		\}
	end if
	irpv const, const@litStack\{
		; if const\#.enabled eqtype 0 and const\#.enabled relativeto 0
		\local val@start, val@end
		virtual
			val@start:
			match any, const\#.value\\{
				any
			\\}
			val@end:
		end virtual
		const\#.len = val@end - val@start
		if defined const\#.enabled
			if const\#.enabled = 1
				const:
				match any, const\#.value\\{
					any
				\\}
			end if
		end if
	\}
}