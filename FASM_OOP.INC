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
			if used name\\#\\.Table.\\#.
				name\\#@MStack equ . 							;загружаем на вершину константы-стека новый метод
				match _val:type, val\\\{						;для виртуальных методов(и возможно дальнейшего расширения)
					.Value equ _val								;сразу закрепляем за ним значение(обработчик)
					.Type equ type								;и тип - пока что только виртуальные
					define matched
				\\\}
				match =matched, matched\\\{						;если тип не указан явно
					.Value equ val								;значит статический
					.Type equ static
				\\\}
				match =this, .Value\\\{							;если значение обработчика - this, то это значит
					.Value equ name\\\#\\\.\\\#.				;что надо подставить имя метода в формате StructName.MethodName
				\\\}											;это работает, так как процедура это метка и доступна везде
			end if
		\\}	
	\}

	struc override NewVal{									;переопределеие метода
		match name, struct@lastname\\{
			irpv MethName, name\\#@MStack\\\{				;перебираем стековую переменную по значению с самого первого
			\\\local matched								;подробнее irpv описан в документации
				if MethName eq .							;находим метод, имя которого совпадает с переопределяемым
					match =this, NewVal\\\\{				;меняем ему значение. В случае, если это this
						.Value equ name\\#\\.\\#.			;выполняем описанную ранее операцию
						define matched
					\\\\}
					match =matched, matched\\\\{
						.Value equ NewVal					;иначе просто устанавливаем новое значение
					\\\\}
				end if										;в будущем планирую сделать переопределение типа метода
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
			. dq procaddr
		\\}	
	\} 

}

macro make@Const arg1, arg2, arg3{
	arg1 arg2 arg3
}

macro struct NameParent{
	local matched@endt, matched@params						;тут происходит определение разных локалок для двух отдельных
															;матчевых if...else
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
				name\#@MStack equ ParentMethod
			\\\}
			irpv ParentConst, parent\#@ConStack\\\{
				; atrname\\#@Type equ atrtype
				; atrname\\#@Value equ atrval
				const ParentConst parent\#.\\\#ParentConst\\\#@Type parent\#.\\\#ParentConst\\\#@Value
			\\\}
			irpv ParentControl, parent\#@ConstrolStack\\\{
				name\#@ConstrolStack equ ParentControl
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
	\} 														;с таким же именем используется старое определение
															;это разработчиком компилятора сделано намеренно, дабы избежать рекурсии
	struct NameParent
}

macro MStruct@GenMethods name, StackName{					;макрос для генерации методов
	name#.Table:
	irpv MethName, StackName\{								;перебираем стек методов
	local matched
		match =static, MethName\#.Type\\{					;если метод статический - устаналвиаем числовую переменную
			if used .\\#MethName
				.\\#MethName = MethName\\#.Value				;в значение адреса обработчика
			end if
			define matched
		\\}
		match =matched, matched\\{
			name#.Table.\\#MethName dq MethName\\#.Value	;если метод не статический, значит виртуальный
			restore name#@Virtual							;заодно здесь мы проверяем, нужно ли создавать для обьектов
			name#@Virtual equ "TRUE"						;поле с указателем на таблицу виртуальных методов
		\\}
		restore matched
	\}
}

macro make@atr name, atrname, atrtype, atrval{
	name#.#atrname atrtype atrval
}

macro ends@handler{
	match name, struct@lastname\{
		local matched
		ends
		restruc method  								;уаляем определения макросов для методов
		restruc override
		restruc const
		match name, struct@lastname\\{
			name\\#@Virtual equ "FALSE"				;по умолчанию считаем, что таблица виртуальных методов не нужна
			MStruct@GenMethods name, name\\\#@MStack	;при генерации определим так ли это
		\\}
		; restore oldstruct@action		
		;здесь происходит переопределение стандартных
		;создаваемых struct макросов
		match ="TRUE", name\#@Virtual\\{					;если нужно создать указатель на таблице виртуальных методов
			define matched
			struc name [args]\\\{
				\\\common
				.VTable: 
				dq name\#.Table								;создаем на 8 байт раньше стандартного определения
				. name args
				; irpv CName, name\#@ConStack\\\\{
				; 	match CType CValue, CName\\\\#@Type CName\\\\#@Value\\\\\{
				; 		make@atr ., CName, CType, CValue
				; 	\\\\\} 
				; \\\\}
				.Table equ name\#.Table						;так же прикрепляем таблицу к обьекту статически
				.Type equ \name
				.TypeName equ \`name
			\\\}
			macro name [args]\\\{							;если имени у обьекта нет, то просто создаем таблицу со значением
				\\\common
				dq name\#.Table
				name args
			\\\}
		\\}
		match =matched, matched\\{							;если не требуется
			struc name [args]\\\{
				\\\common
				restore .Table
				.Table equ name\\#.Table					;то просто прикрепляем таблицу статически
				. name args
				; irpv CName, name\#@ConStack\\\\{
				; 	match CType CValue, CName\\\\#.Type\\\\\{
				; 		make@atr ., CName, CType, CValue	;создание констант. Намеренно завернуто в макрос
				; 	\\\\\} 
				; \\\\}
				.Type equ \name
				.TypeName equ \`name
			\\\}
			macro name [args]\\\{							;если имени у обьекта нет, то стандартный обработчик
				\\\common
				name args
			\\\}
		\\}
		restore struct@lastname
	\}
}

; define ends@handler ends@default

macro ends{
	ends@handler
}

macro @call ObjMethArgs&{
	common
	local matched, _ObjMethArgs, _ObjMeth 					;если будет локальная переменная, она может разматчиться
	define _ObjMethArgs ObjMethArgs
	match ObjMeth=(args=), _ObjMethArgs\{					;если в скобках есть аргументы
		define _ObjMeth ObjMeth  							;для локалок
		match [Obj->Meth], _ObjMeth\\{						;проверка на вызов виртуального метода обьекта
			define matched
			mov r10, [Obj\\#.VTable]						;вызов через r10
			match Table, Obj\\#.Table\\\{
					;тут мы к смещению обьекта прибавляем смещение таблицы
					;вместе с методом, а потом вычитаем смещение таблицы
					;таким образом, если методы располагаются на одинаковом смещении
					;и имеют одинаковые имена
					;то для структур-наследников может вызываться их собственный метод
					;вместо метода предка
					ccall qword[r10+Table\\\#.\\\#Meth-Table], addr Obj, args
			\\\}
		\\}
		match =matched Obj->Meth, matched _ObjMeth\\{		;для вызова статического метода обьекта
			define matched
			match Table, Obj\\#.Table\\\{
				ccall Table\\\#.\\\#Meth, addr Obj, args
			\\\}
		\\}
		match [Struct:Meth], _ObjMeth\\{						;для вызова виртуального метода по таблице структуры
															;с помощью этого можно передавать обьект в методы предков
			define matched
			ccall qword[Struct\\#.Table\\#.\\#Meth], args
		\\}
		match =matched Struct:Meth, matched _ObjMeth\\{		;для вызова статического метода по таблице
			define matched
			ccall Struct\\#.Table\\#.\\#Meth, args
		\\}
		match =matched, matched\\{							;иначе просто вызываем обычную процедуру
			ccall ObjMeth, args
		\\}
	\}
	match ObjMeth=(=), _ObjMethArgs\{						;аналогично для вызова без аргументов
		define _ObjMeth ObjMeth 							;для локалок
		match [Obj->Meth], _ObjMeth\\{
			define matched
			mov r10, [Obj\\#.VTable]
			match Table, Obj\\#.Table\\\{
				ccall qword[r10+Table\\\#.\\\#Meth-Table], addr Obj
			\\\}
		\\}
		match =matched Obj->Meth, matched _ObjMeth\\{
			define matched
			match Table, Obj\\#.Table\\\{
				ccall Table\\\#.\\\#Meth, addr Obj
			\\\}
		\\}
		match [Struct:Meth], ObjMeth\\{
			define matched
			ccall qword[Struct\\#.Table\\#.\\#Meth]
		\\}
		match =matched Struct:Meth, matched ObjMeth\\{
			define matched
			ccall Struct\\#.Table\\#.\\#Meth
		\\}
		match =matched, matched\\{
			ccall ObjMeth
		\\}
	\}
}

macro virtObj args&{
	local register, typeof, nameof, matched
	match name:=arg type =at reg, args\{
		match ="TRUE", type\#@Virtual\\{
			register equ reg-8
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
		match ="TRUE", type\#@Virtual\\{
			register equ rcx-8
			define matched
		\\}
		match =matched, matched\\{
			register equ rcx
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
		register equ rcx
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

macro union{
	union
	macro ends@handler\{
		ends
		purge ends@handler
		display "lol"
	\}
}