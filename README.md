Полный набор всех создаваемых макросов и библиотек для FASM  
  
Включает в себя:  
1. FASM_OOP  
Реализует принципы ООП на макросах FASM. Реализованы следующие возможности(на данный момент):  
  1.1. Создание методов:  
       - Статических(функции, на уровне препроцессора привязанные к структуре);  
       - Виртуальных(хранятся в таблице виртуальных методов, создаваемой автоматически);  
       - Инлайн(макросы, на уровне препроцессора привязанные к структуре).    
       На данный момент допустимы следующие варианты синтаксиса обьявления методов:  
       <pre lang="fasm" title="fasm"><code>
       struct point2
            x 		dd ?
            y 		dd ?
            print 	dm point2_print:static	;статический метод print с процедурой-обработчиком point2_print
            print 	dm point2_print			;статический метод print с процедурой-обработчиком point2_print
            print   dm point2_print:virtual	;виртуальный метод print с процедурой-обработчиком point2_print
            print 	dm point2_print:inline	;инлайн метод print с макросом-обработчиком point2_print
            print 	dm this 				;статический метод print с процедурой-обработчиком point.print(спецификаторы типа метода так же доступны)
       ends
       </code></pre>  
       Обьявление методов структуры независимо от обьявления полей.  
       В более ранних версиях для обьявления методов использовалось ключевое словво method. Оно так же поддерживается.  
       **Важно! Если у структуры или ее предка имеется хоть 1 виртуальный метод, то для структуры создается таблица виртуальных методов, а каждый обьект будет так же содаржать в себе указатель на эту таблицу. Для совместимости с предками без виртуальных методов - по отрицательнму смещению.**  
  1.2. Наследование и полиморфизм.  
       Все методы предка передаются потомку при стандартном наследовании структур в FASM. Потомок может переопределить для себя поведение методов предка - для этого используется макрос override. Синтаксис следующий:

       ```fasm
       struct point3 point2
       	z dd ?
       	print override point3_print	;переопределяет унаследованный метод print для структуры point3. Тип метода остается прежним
       	print override this:inline	;перегрузка унаследованного метода print и смена типа метода.
       ends
       ```

       При перегрузке допустимо менять только типы static и inline. Не допускается смена типа виртуальных методов.  
  1.3. Вызов методов. Макрос @call  
       Есть два способа вызова методов: по имени структуры и по имени обьекта. Примеры:

       ```fasm
       myPoint point3 1, 2, 3
       
       proc main
       	@call myPoint->print()		;вызов метода напрямую через обьект
       	@call point3:print(myPoint)	;вызов метода через имя структуры
       	ret
       endp
       ```

       При использовании первого варианта адрес структуры myPoint неявно передается в качестве первого аргумента. Второй вариант подходит схорее в тех случаях, когда нужный нам адрес структуры находится, например, в памяти.  
       **Важно! Есть большая разница при вызое виртуальных методов. При вызове через второй вариант синтаксиса метод будет браться не из указателя на таблицу виртуальных методов обьекта, а напрямую по таблице той структуры, через имя которой вызывается метод. Таким образом для потомков, перегрузивших методы предка, можно вызывать оригинальные методы предка.**  
       На архитектуре х86 функции могут использовать различные соглашения о вызовах. Макрос @call на этой архитектуре учитывает эту особенность и позволяет указать соглашение, по которому должен вызываться метод(По умолчанию - stdcall). Пример:

       ```fasm
       @call c myPoint->print() ;вызвать метод print для обьекта myPoint по соглашению о вызовах ccall
       ```

       По умолчанию доступны соглашения о вызовах stdcall и ccall, однако в дальнейшем могут появиться и другие. Можно создавать и собственные соглашения о вызовах, для этого вы должны создать макрос-обработчик с параметрами аналогично стандартным макросам ccall и stdcall. Именование соответствующее, при вызове макрос @call склеит указанные до вызова самого метода символы с строкой call и попытается передать в макроос с получившимся именем частично отформатированные аргументы.  
       Так же макрос @call способен вызывать и обычные процедуры. Примеры:  

       ```fasm
       proc add, arg1, arg2
       	mov eax, [arg1]
       	add eax, [arg2]
       	ret
       endp
         
       proc main
       	@call add(5, 10)			;вызов статической процедуры
       	@call [printf]("%d", eax)	;вызов процедуры по адресу из памяти
       	ret
       endp
       ```

  1.4. Обработка аргументов макроса @call  
       Макрос @call поддерживает и расширяет возможности стандартных макросов в части использования сложных литералов. Перед отправкой в макросы вызова @call выполнет предварительную обработку аргументов на предмет их наличия. Все такие литералы более не создаются посреди кода с прыжком через него, а переносятся в конец посредством конструкции postpone. Они делятся на типы по указателю и по сохранности. По указателю литералы делятся на обьектные и data-указатели. По сохранности - на изменяемые и константные. По умолчанию литералы имеют тип константного data-указателя. (Тип обьектного указателя был введен специально для структур с таблицами виртуальных методов). В результате обработки в конечный обработчик/метод-макрос аргументы передаются в уже обработанном виде(addr lit_data), однако для инлайн методов перед параметром можно указать ?, чтобы он передался в макрос-обработчик в неизменном виде. Примеры с пояснениями:  
       1.4.1. Строковые литералы  

              ```fasm
              proc main
              	@call [printf]("%d", 10)
              	@call [printf]("%d", 15)
              	ret
              endp
              ```

              В данном примере константный строковый литерал "%d" является константным, создастся только 1 раз и оба вызова функции будут использовать один и тотже адрес. По умолчанию строка будет состоять из однобайтовых символов и в конец будет автоматически добавлен нуль-терминатор. Поиск соответствующих констант производится по совпадению текстовых значений. Аналогично это будет работать и для следующего примера:
              
              ```fasm
              proc main
              	@call [printf](<"%d", 0Ah>, 10)
              	@call [printf](<"%d", 0Ah>, 15)
              	ret
              endp
              ```

              Так же в случае если литерал является массивом байт, но явно тип мы не указываем, он так же считается строковым и дополняется нуль-терминатором.
              ***Важно! Так как автору не нравился порядок обработки аргументов на х64(по умолчанию fastcall заполняет аргументы слева направо) дефолтный обработчик аргументов oop_call отличается тем, что обрабатывет аргументы от последнего к первому. Таким образом указатель на обьект спокойно можно хранить в rcx и не опасаться, что при передаче параметров он будет затерт раньше времени***  
       1.4.2. Литералы с типом значения  
              Так же литерал может быть обьявлен по типу значения. В таком случае так же допускаетя неявное указание типа литерала, при условии, что в нем присутствует перечисление элементов. В случае, если нам не необходимо создавать перечисление, но нужен указатель на констаное значение, рекомендуется использовать ключевое слово const  
              
              ```fasm
              proc main
              	@call [wprintf](<du "%d", 0>, 10)		;допустимый синтаксис
              	@call [wprintf](<du "%d">, 15)			;недопустимый синтаксис
              	@call [wprintf](<const du "%d">, 15)	;допустимый синтаксис
              	ret
              endp
              ```

              В данном случае для двух валидных вариантов синтаксиса будут созданы отдельные обьекты в памяти, так как нет текстового соответствия между этими литералами.  
       1.4.3. Изменяемые литералы  
              Для изменяемого литерала вместо const используется ключевое слово data. Примеры:
              
              ```fasm
              proc main
              	@call [printf](<data db "%d", 0Ah, 0>, 10)
              	@call [printf](<data db "%d", 0Ah, 0>, 15)
              	ret
              endp
              ```

              При использовании данного типа литералов для каждого вызова в памяти будет создан отдельный обьект. Опускать тип обьекта и нуль-терминатор для строк в данном случае не допускается.  
       1.4.4. Обьектные литералы  
              Обьектные литералы были созданы для структур с виртуальными методами, но при передаче в методы рекомендуется использовать эти типы литералов вне зависимости от того, имеет структура виртуальные методы или нет. Для их передачи используется ключевое слово obj и const_obj для изменяемых и константных литералов соответственно.  
  1.5. Обработчики методов  
       Как уже было сказано ранее, обработчиком метода может стать любая процедура/макрос в зависимости от типа метода. Однако FASM_OOP предлагает средства, способные облегчить создание методов, такие как виртуальные обьекты и вспомогательные макросы для инлайн-методов.  
       1.5.1. Виртуальне обьекты  
              Для их создания используется макрос virtObj. Виртуальный обьект не инициализирует область памяти, в которой создается и предназначен в первую очередь для удобной работы с передаваемыми в методы адресами обьектов. Пример:
              
              ```fasm x64
              proc point2.print, this
              	virtObj .this:arg point2 at rcx								;после at следует адрес, указывать не обяательно, по умолчанию rcx(ecx на x86)
              	@call [printf](<"x: %d, y: %d", 0Ah>, [.this.x], [.this.y])
              	ret
              endp
              ```

              Параметр :arg является необязательным и используется для корректировки стартового адреса, если структура имеет виртуальные методы. Он предназначен для тех случаев, когда адрес после at является адресом самого обьекта. В случае же если мы имеем адрес начала данных обьекта или память под обьект была выделена динамически, данный ключ указывать не следует. Данный ключ рекомендуется использовать так же и для структур, не имеющих виртуальных методов - при создании виртуального обьекта корректировка не выполняется, если у данной структуры таких методов нет. Создаваемый таким образом обьект является глобальным, допускается использование локальных меток как в примере.  
       1.5.2. Инлайн методы: макросы для заполнения и адресной инициализации  
              В макросы-обработичики инлайн методов параметры передаются так, как если бы они передавались и в статические/виртуальные методы. Однако если в самом методе нам надо выполнить какие-то операции над обьектом, у нас могут возникнуть сложности с тем, чтобы получить значение поля обьекта или переместить параметр в регистр. Для решения этих проблем были созданы макросы fillParam и inlineObj. Пример использования inlineObj:
              
              ```fasm x64
              macro point2.print this{
              	local _this					;локальный идентификатор для адреса обьекта
              	inlineObj _this, this, rcx	;первый параметр - идентификатор для получившегося адреса, второй - источник значения, третий - регистр
              								;в который загрузится значение если параметр окажется памятью
              	@call [printf](<"x: %d, y: %d", 0Ah>, [_this+point2.x], [_this+point2.y])
              }
              ```

              Пример использования fillParam:
              
              ```fasm x86
              macro mem_mov dst, src, size{
              	fillParam ecx, size
              	fillParam esi, src
              	fillParam edi, dst
              	rep movsb
              }
              ```

  1.6. @jret (для x64)  
       Данный макрос используется для оптимизации выхода из процедур на х64. Он позволяет прыгнуть на процедуру(или метод) вместо ret с передаче параметров через регистры. Из этого следует ограничение - не более 4-х параметров и инлайн методы не должны совершать более одного вызова процедуры.  
  1.7. Константы структур  
       Макрос const может создавать константы этапа препроцессинга и константы этапа компиляции для структур. Пример:
       
       ```fasm
       struct MyStruct
       	dd ?
       	const a equ 5 	;создаст константное поле этапа препроцессинга
       	const b = 6 	;создаст константное поле этапа компиляции
       ends
       ```

       К создаваемым таким образом полям можно обращаться только через имя структуры(для вышеприведенного примера - MyStruct.a и MyStruct.b соответственно). Константные поля структур наследуются.  
2. DIALOGFORM  
   Расширяет возможности FASM_OOP для создания диалоговых окон на основе шаблона в памяти(Windows). Пример программы с использованием и пояснениями:  
   
   ```fasm x64
   format pe64 GUI
   
   entry main
   
   section ".code" readable writeable executable
   
   include "TOOLS\x64\TOOLS.INC"
   include_once "encoding\Win1251.inc"
   include_once "TOOLS\x64\WINUSER\Winuser.inc"
   
   struct button BUTTON
   	BN_CLICKED event button_Clicked 					; создание события BN_CLICKED c обработчиком button_Clicked. Создаваемые таким образом события представляют собой калбеки, что позволяет перехватывать их в рантайме
   ends
   
   struct dForm1 DIALOGFORM
   	const _x 				= 0 						;положение окна по горизонтали
   	const _y 				= 0 						;положение окна по вертикали
   	const _cx 				= 500						;ширина окна
   	const _cy 				= 350						;высота окна
   	WM_INITDIALOG event form_Init						;событие инициализации окна
   	control static1 STATIC, NONE,\ 						;устаревший макрос создания элемента управления
   		"Нажми кнопку", (dForm1._cx-50)/2, 0, 50, 12
   	@control button1 button,\							;новый макрос создания элемента управления
   		_text: "Кнопка",\ 
   		_x: dForm1.static1._x,\							;обращение к значению координат элемента управления static1
   		_y: 12,\
   		_cx: 50,\
   		_cy: 12,\
   		_style: WS_VISIBLE or BS_DEFPUSHBUTTON
   ends
   
   proc button_Clicked, formLp, paramsLp, controlLp
   	virtObj .form:arg dForm1
   	@call WND:msgBox("Привет, мир!", "Приветствие", MB_OK, [.form.hWnd])
   	ret
   endp
   
   proc form_Init, formLp, paramsLp
   	virtObj .form:arg dForm1
   	@call WND:msgBox("Форма запущена!", "Приветствие", MB_OK, [.form.hWnd])
   	ret
   endp
   
   ShblDialog dForm1, "DialogForm"						;макрос для генерации шаблона формы и ее процедуры-обработчика
   
   myForm form dForm1									;макрос form для заполнения некоторых полей обьекта структуры нужными значениями
   
   proc main
   	@call myForm->startNM(NULL)     				;основной цикл с немодальным окном
   	.msgLoop:
   		@call myForm->dispatchMessages()
   	test eax, eax
   	jnz .msgLoop
   	ret
   endp
   ```

   Данный пример создает форму с одним лейблом и одной кнопкой. При запуске приложения выскакивает мессенджбокс об успешном создании формы, а при нажатии на кнопку - приветствие.  
   2.1. Макросы control и @control  
        Эти макросы служат для разметки формы и генерации процедуры-обработчика сообщений используются макросы control и более новая версия @control. Оба они имеют следующие параметры:
        ```
        _initvals - инициализирующие параметры, за исключением автоматически определяемых полей(по умолчанию NONE)
        _text - текст элемента(по умолчанию пустая строка)
        _x - положение элемента по оси x относительно формы(по умолчанию 0)
        _y - положение элемента по оси y относительно формы(по умолчанию 0)
        _cx - ширина элемента(по умолчанию 60)
        _cy - высота элемента(по умолчанию 20)
        _style - стили элемента управления(по умолчанию WS_VISIBLE)
        _style_ex -расширенные стили элемента управления(по умолчанию NULL)
        ```
        Как обращаться к этим параметрам из других элементов управления показано в примере. Создаваемые поля являются константами этапа компиляции. Так же для упрощения разметки автоматически генерируются поля `_rx` и `_ry` - координаты края элемента управления по соответствующим осям. Макрос control получает параметры напрямую в указанном по списку порядке. Макрос @control более удобен и читабелен, так как позволяет назначать значения поименно в любом порядке.  
   2.2. Макрос ShblDialog  
        Данный макрос создает шаблон диалогового окна. Он принимает три аргумента - текст формы(по умолчанию пустая строка), стили формы(по умолчанию WS_VISIBLE or WS_CAPTION or WS_SYSMENU or WS_MINIMIZEBOX or WS_MAXIMIZEBOX or DS_CENTER) и расширенные стили формы(по умолчанию NULL)