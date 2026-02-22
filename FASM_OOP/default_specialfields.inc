macro MStruct@GenMethods name{						;макрос для генерации методов
	name#.Table:
	match =1, name#.@abstract\{
		virtual at name#.Table
	\}
	irpv MethName, name#@MStack\{									;перебираем стек методов
		match =static, MethName\#.Type#name\\{					;если метод статический - устаналвиаем числовую переменную
			if used name#.Table.\\#MethName								;проверяем, что метод хоть раз был вызван
				name#.Table.\\#MethName = MethName\\#.Value#name			;в значение адреса обработчика
			end if
		\\}
		match =virtual, MethName\#.Type#name\\{
			if used name#.Table
				name#.Table.\\#MethName dptr MethName\\#.Value#name	;если метод не статический, значит виртуальный
				; match any, name#.Table.\\#MethName\\\{
				; 	display \\\`any, 0Ah
				; \\\}
			end if
			restore name#@Virtual								;заодно здесь мы проверяем, нужно ли создавать для обьектов
			name#@Virtual equ 1							;поле с указателем на таблицу виртуальных методов
		\\}
	\}
	match =1, name#.@abstract\{
		end virtual
	\}
}

macro use@table vtable{
	local table_used
	table_used = vtable
}

macro specialfields_define_default name{										;создаем макросы, которые позволят нам определять и
															;переопределять методы
	name#.used = 1
	struc dm val\{
		\local matched
		define __value val
		name#@MStack equ . 								;загружаем на вершину константы-стека новый 
		match _val/conv, __value\\{
			.Conv#name equ conv
			define __value _val
		rept 0\\{ \\} match conv, __obj_call\\{
			.Conv#name equ conv
		\\}
		match _val:type, __value\\{							;для виртуальных методов(и возможно дальнейшего расширения)
			.Value#name equ _val							;сразу закрепляем за ним значение(обработчик)
			.Type#name equ type							;и тип - пока что только виртуальные и инлайн методы
			define matched
			; \\local _size
			; _size = $
			; %display_num _size, 10
			; display 0ah
		\\}
		match =matched|_val, matched|__value\\{							;если тип не указан явно
			.Value#name equ _val							;значит статический
			.Type#name equ static
		\\}
		match =this, .Value#name\\{						;если значение обработчика - this, то это значит
			.Value#name equ name#.\\#.				;что надо подставить имя метода в формате StructName.MethodName
		\\}												;это работает, так как процедура это метка и доступна везде
	\}

	struc override NewVal\{									;переопределеие метода
		irpv MethName, name#@MStack\\{				;перебираем стековую переменную по значению с самого первого
														;подробнее irpv описан в документации
			match =MethName, .\\\{						;находим метод, имя которого совпадает с переопределяемым
				\\\local _newVal
				_newVal equ NewVal
				match _val/conv, _newVal\\\\{
					.Conv#name equ conv
					define _newVal _val
				rept 0\\\\{ \\\\} match conv, __obj_call\\\\{
					.Conv#name equ conv
				\\\\}
				match _val:type, _newVal\\\\{
					_newVal equ _val
					.Type#name equ type
					match =virtual, type\\\\\{
						display "can't change method's type to virtual"
						err
					\\\\\}
					match =virtual, .Type#name\\\\\{
						display "can't change method's type from virtual"
						err
					\\\\\}
				\\\\}
				match =this, _newVal\\\\{				;меняем ему значение. В случае, если это this
					_newVal equ name#.\\#.	;выполняем описанную ранее операцию
				\\\\}
				.Value#name equ _newVal
			\\\}										;в будущем планирую сделать переопределение типа метода
		\\}
	\}

	macro const args&\{
		match atrname atrtype atrval, args\\{
			name#@ConStack equ atrname
			name#.\\#atrname\\#@Type equ atrtype
			name#.\\#atrname\\#@Value equ atrval
			match ==, atrtype\\\{
				if used name#.\\#atrname
					name#.\\#atrname atrtype atrval
				end if
			rept 0\\\{\\\}rept 1\\\{
				name#.\\#atrname atrtype atrval
			\\\}
		\\}
	\}
}

macro secialfields_inherit_default parent, name{
	irpv ParentMethod, parent#@MStack\{				;загружаем потомку методы родителя
		ParentMethod dm ParentMethod\#.Value#parent:ParentMethod\#.Type#parent/ParentMethod\#.Conv#parent
	\}
	irpv ParentConst, parent#@ConStack\{
		const ParentConst parent#.\#ParentConst\#@Type parent#.\#ParentConst\#@Value
	\}
	local parent_used
	parent_used = parent#.used
}

macro specialfields_ends_default name{
	restruc dm  								;уаляем определения макросов для методов
	restruc override								;переопределения методов
	restruc const  									;и создания констант
	name#@Virtual equ 0						;по умолчанию считаем, что таблица виртуальных методов не нужна
	MStruct@GenMethods name					;при генерации определим так ли это
	; restore oldstruct@action		
	;здесь происходит дополнение стандартных
	;создаваемых struct макросов
	; match =1, name#@Virtual\{
	; 	sizeof.#name = sizeof.#name + pointer.size
	; 	; vtable equ dd name\#.Table				;если нужно создать указатель на таблице виртуальных методов, добавляем константу
	; \}
	
	struc name args&\{
		\common
		.Table equ name\#\\.Table				;прикрепляем таблицу статически
		; .Lp equ .								;каждому обьекту создаем точку начала
		; match =1, name#@Virtual \\{					;проверяем, нужно ли создать VTable для константы
		; 	restore .Lp
		; 	.Lp equ .VTable
		; \\}
        match , @struct \\{ 
            define field@struct .,name,<args> 
        \\}
        match no, @struct \\{
			match =name#.__interface, name#.__interface \\\{					;проверяем, нужно ли создать VTable
				. name args
			rept 0\\\{\\\}rept 1\\\{
				match any, args\\\\{
					. name name#.Table, args 
				rept 0\\\\{\\\\}rept 1\\\\{
					. name name#.Table
				\\\\}
			\\\}
        \\}
		.Type equ name
		.TypeName equ `name
		.size = sizeof.#name
		\local struct_used
		struct_used = name\#\\.used
	\}
	macro name args&\{						;если имени у обьекта нет, то стандартный обработчик
		\common
        match , @struct \\{ 
        	\\local anonymous
            define field@struct anonymous,name,<args> 
        \\}
        match no, @struct \\{ 
			match =name#.__interface, name#.__interface \\\{					;проверяем, нужно ли создать VTable
				name args
			rept 0\\\{\\\} rept 1\\\{
				match any, args\\\\{
					name name#.Table, args 
				rept 0\\\\{\\\\} rept 1\\\\{
					name name#.Table
				\\\\}
			\\\}
        \\}
		\local struct_used
		struct_used = name\#\\.used
	\}
	if defined name#.VTable
		P_#name.VTable = name#.VTable
	end if
	struc P_#name args&\{
        
        match |pname, @struct|P_#name \\{ 
            define field@struct .,pname,<args> 
        rept 0\\{\\}rept 1\\{
			match , args\\\{
				. dptr ?
			rept 0\\\{\\\} rept 1 \\\{
				. dptr args
			\\\}
			.Table equ name\#\\.Table
			.Type equ P_#name
			.BaseType equ name
			match any, P_#\name\\\{
				.TypeName equ \\\`any
			\\\}
			.size = pointer.size
    	\\}
	\}
	sizeof.P_#name = pointer.size
	macro P_#name args&\{
		match , args\\{
			dptr ?
		rept 0\\{\\} rept 1 \\{
			dptr args
		\\}
	\}
}

add_specialfields default
