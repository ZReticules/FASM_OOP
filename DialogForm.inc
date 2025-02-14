match =x64, __architecture{
	SetWindowPtrA fix SetWindowLongPtrA
	GetWindowPtrA fix GetWindowLongPtrA
	base_layout_cx equ 1
	base_layout_cy equ 12
}

match =x86, __architecture{
	SetWindowPtrA fix SetWindowLongA
	GetWindowPtrA fix GetWindowLongA
	base_layout_cx equ 1
	base_layout_cy equ 12
}

include "WINUSER\layouts.inc"

importlib user32,\
	SetWindowPtrA,\
	GetWindowPtrA,\
	GetClientRect,\
	EnumChildWindows,\
	MoveWindow,\
	MapWindowPoints,\
	InvalidateRect,\
	MapDialogRect

importlib uxtheme,\
    OpenThemeData

struct DLGTEMPLATE
	style dd ?
	dwExtendedStyle dd ?
	cdit dw ?
	x dw ?
	y dw ?
	cx dw ?
	cy dw ?
ends

define FW_DONTCARE		0
define FW_THIN			100
define FW_EXTRALIGHT	200
define FW_ULTRALIGHT	200
define FW_LIGHT			300
define FW_NORMAL		400
define FW_REGULAR		400
define FW_MEDIUM		500
define FW_SEMIBOLD		600
define FW_DEMIBOLD		600
define FW_BOLD			700
define FW_EXTRABOLD		800
define FW_ULTRABOLD		800
define FW_HEAVY			900
define FW_BLACK			900

define ANSI_CHARSET 		0
define BALTIC_CHARSET 		186
define CHINESEBIG5_CHARSET 	136
define DEFAULT_CHARSET 		1
define EASTEUROPE_CHARSET 	238
define GB2312_CHARSET 		134
define GREEK_CHARSET 		161
define HANGUL_CHARSET 		129
define MAC_CHARSET 			77
define OEM_CHARSET 			255
define RUSSIAN_CHARSET 		204
define SHIFTJIS_CHARSET 	128
define SYMBOL_CHARSET 		2
define TURKISH_CHARSET 		162
define VIETNAMESE_CHARSET 	163

struct DIALOGFORM
	const _x 				= 0
	const _y				= 0
	const _cx 				= 100
	const _cy 				= 50
	const _pointsize 		= 0
	; const _weight   		= FW_DONTCARE
	; const _italic  			= 0
	; const _charset			= DEFAULT_CHARSET
	const _font  			equ "MS Sans Serif"

	hWnd 					dptr ?
	idCount 				dw ?
	baseRect				RECT ?, ?, ?, ?
	bgColorBrush 			dptr 0
	hDialogTemplate 		dptr ?
	lpDialogFunc 			dptr ?
	hTheme 					dptr ?

	__close 				dptr ?

	start 					dm this:inline
	; ilStart 				dm this:inline
	setText 				dm this:inline
	getTextLen 				dm this:inline
	getText 				dm this:inline
	close 					dm this:inline
	setBgColor				dm this
	setCaptionColor			dm this
	setTextColor			dm this
	setBorderColor			dm this
	setCornerType			dm this
	dispatchMessages		dm this
	startNM					dm this
	setIcon 				dm this:inline
	setEnabled				dm this:inline
	setVisible				dm this:inline
	setFocus				dm this:inline
	invalidate 				dm this:inline
	; getClientSize			dm this:inline
	; getWindowSize 			dm this:inline
ends

struct params
	hWnd 	dptr ?
	uMsg 	dptr ?
	wParam 	dptr ?
	lParam 	dptr ?
ends

macro form formInfo&{
	match formType _initvals, formInfo\{
		formInfo NONE, formType\#.cdit, <formType\#._x, formType\#._y, formType\#._cx, formType\#._cy>, NONE, formType\#_DlgTemplate, formType\#_DialogProc, NONE, _initvals
	rept 0\{\}match formType, formInfo\{
		formInfo NONE, formType\#.cdit, <formType\#._x, formType\#._y, formType\#._cx, formType\#._cy>, NONE, formType\#_DlgTemplate, formType\#_DialogProc, NONE
	\}
}

struc form formInfo&{
	make@form ., formInfo
}
macro make@form name, formInfo&{
	match formType _initvals, formInfo\{
		name formInfo NONE, formType\#.cdit, <formType\#._x, formType\#._y, formType\#._cx, formType\#._cy>, NONE, formType\#_DlgTemplate, formType\#_DialogProc, NONE, _initvals
	rept 0\{\}match formType, formInfo\{
		name formInfo NONE, formType\#.cdit, <formType\#._x, formType\#._y, formType\#._cx, formType\#._cy>, NONE, formType\#_DlgTemplate, formType\#_DialogProc, NONE
	\}
}

include "WINUSER\DIALOGFORM\DialogForm.asm"
include "WINUSER\DIALOGFORM\CONTROL.inc"
include "WINUSER\DIALOGFORM\DefaultEvents.asm"

struct DLGITEMTEMPLATE
	style dd ?
	dwExtendedStyle dd ?
	x dw ?
	y dw ?
	cx dw ?
	cy dw ?
	id dw ?
ends

macro ShblDialog_Mem formType, _Text = "DialogForm", _style=WS_VISIBLE or WS_CAPTION or WS_SYSMENU or WS_MINIMIZEBOX or WS_MAXIMIZEBOX or DS_CENTER, _styleEx = NULL{
	local const_mem
	macro const_mem\{
		\local controlCounter
		controlCounter = 0
		DLGTEMPLATE \
			_style,\
			_styleEx,\
			formType#.cdit,\
			formType#._x,\
			formType#._y,\
			formType#._cx,\
			formType#._cy
		dw 00h ; menu
		dw 00h ; header
		du _Text
		dw 0
		if (_style and DS_SETFONT) = DS_SETFONT
			dw formType#._pointsize
			; dw formType#._weight
			; db formType#._italic
			; db formType#._charset
			du formType#._font, 0
		end if
		align 4
		irpv cntrl, formType#@ControlStack\\{
			controlCounter = controlCounter + 1
			formType#.\\#cntrl\\#._id = controlCounter
			formType#.\\#cntrl\\#_DLGITEMTEMPLATE DLGITEMTEMPLATE \
				formType#.\\#cntrl\\#._style,\
				formType#.\\#cntrl\\#._styleEx,\
				formType#.\\#cntrl\\#._x,\
				formType#.\\#cntrl\\#._y,\
				formType#.\\#cntrl\\#._cx,\
				formType#.\\#cntrl\\#._cy,\
				formType#.\\#cntrl\\#._id
			match TName, cntrl\\#.Type\\\{
				du TName\\\#.CType
				dw 00h
				du formType#.\\#cntrl\\#._Text
				dw 00h
			\\\}
			db (4-($ mod 4)) dup(0) 	; выравнивание
		\\}
		formType#.cdit = controlCounter
	\}
	@const_align equ 4
	formType#_DlgTemplate @const const_mem
	restore @const_align
	purge const_mem 
}

macro ShblDialog_Mem_New formType, _Text = "DialogForm", _style=WS_VISIBLE or WS_CAPTION or WS_SYSMENU or WS_MINIMIZEBOX or WS_MAXIMIZEBOX or DS_CENTER, _styleEx = NULL{
	local const_mem
	macro const_mem\{
		\local controlCounter
		controlCounter = 0
		; align 4
		DLGTEMPLATE \
			_style,\
			_styleEx,\
			formType#.cdit,\
			formType#._x,\
			formType#._y,\
			formType#._cx,\
			formType#._cy
		dw 00h ; menu
		dw 00h ; header
		du _Text
		dw 0
		if (_style and DS_SETFONT) = DS_SETFONT
			dw formType#._pointsize
			; dw formType#._weight
			; db formType#._italic
			; db formType#._charset
			du formType#._font, 0
		end if
		db 4-($ mod 4) dup(0)
		irpv cntrl, formType#@ControlStack\\{
			controlCounter = controlCounter + 1
			formType#.\\#cntrl\\#._id = controlCounter
			formType#.\\#cntrl\\#_DLGITEMTEMPLATE DLGITEMTEMPLATE \
				formType#.\\#cntrl\\#._style,\
				formType#.\\#cntrl\\#._styleEx,\
				formType#.\\#cntrl\\#._x,\
				formType#.\\#cntrl\\#._y,\
				formType#.\\#cntrl\\#._cx,\
				formType#.\\#cntrl\\#._cy,\
				formType#.\\#cntrl\\#._id
			match TName, cntrl\\#.Type\\\{
				du TName\\\#.CType
				dw 00h
				du formType#.\\#cntrl\\#._Text
				dw 00h
			\\\}
			db 4-($ mod 4) dup(0) 	; выравнивание
		\\}
		formType#.cdit = controlCounter
	\}
	@const_align equ 4
	formType#_DlgTemplate @const const_mem
	restore @const_align
	purge const_mem 
}

macro ShblDialog_Proc formType{

	proc_noprologue

	proc formType#_DialogProc uses pbx, hwndDlg, uMsg, wParam, lParam
		virtObj .this:arg formType at pbx
		@sarg @arg1, @arg2, @arg3, @arg4
		locals 
			si_save dptr ?
			di_save dptr ?
		endl
		xor eax, eax
		mov eax, dword[uMsg]
		; cmp @arg2, WM_MEASUREITEM
		; jne @f
		; int3
		; @@:
		cmp @arg2, WM_INITDIALOG
			je .initEvent
		@call [GetWindowPtrA]([hwndDlg], GWL_USERDATA)
		test pax, pax
			jz .return
		mov pbx, pax
		xor eax, eax
		mov edx, dword[uMsg]
		mov ecx, dword[wParam]
		irpv evnt, formType#@EventStack\{
			local matched
			match =.this.WM_INITDIALOG, .this.\#evnt\\{
				define matched
			\\}
			match =.this.WM_CLOSE, .this.\#evnt\\{
				define matched
			\\}
			match =matched, matched\\{
				cmp edx, evnt
					je .eventHandler_\#evnt
			\\}
		\}
		cmp edx, WM_CLOSE
			je .close_event
		cmp edx, WM_COMMAND
			je .commandEventsChecker
		cmp edx, WM_NOTIFY
			je .notificationsChecker
		xor eax, eax
		.return: ret

		.commandEventsChecker:
			cmp cx, [.this.idCount]
				ja .return
			mov [si_save], psi
			mov [di_save], pdi
			movzx edx, cx
			@call [GetDlgItem]([.this.hWnd], pdx)
			@call [GetWindowPtrA](pax, GWL_USERDATA)
			test eax, eax
				jz ._break
			virtObj .e_cntrl:arg CONTROL at psi from pax
			xor eax, eax
			movzx pdx, word[wParam]
			mov pdx, [.control_events_map + (pdx - 1) * pointer.size]
			cmp pdx, 0
				jz ._break
			movzx ecx, word[pdx]
				jecxz ._break
			add pdx, 2
			movzx eax, word[wParam + 2]
			.find_event:
				cmp ax, word[pdx + (pcx - 1) * 4]
				jne .not_my_event
					movzx pdx, word[pdx + (pcx - 1) * 4 + 2]
					mov pdi, [.e_cntrl + pdx]
					@call pdi(addr .this, addr hwndDlg, addr .e_cntrl)
					jmp ._break
				.not_my_event:
			loop .find_event
			._break:
			mov psi, [si_save]
			mov pdi, [di_save]
			jmp .return

			.control_events_map @const irpv cntrl, formType#@ControlStack\\{ match TName, cntrl\\#.Type\\\{ dptr TName\\\#.events \\\}\\}

		.notificationsChecker:
			mov [si_save], psi
			mov [di_save], pdi
			virtObj .nmhdr NMHDR at pdi from [lParam]
			mov eax, dword[.nmhdr.idFrom]
			cmp ax, [.this.idCount]
				ja ._break2
			@call [GetWindowPtrA]([.nmhdr.hwndFrom], GWL_USERDATA)
			test eax, eax
				jz ._break2
			virtObj .n_cntrl:arg CONTROL at psi from pax
			xor eax, eax
			mov edx, dword[.nmhdr.idFrom]
			mov pdx, [.control_notifications_map + (pdx - 1) * pointer.size]
			cmp pdx, 0
				jz ._break
			movzx ecx, word[pdx]
				jecxz ._break
			add pdx, 2
			mov eax, [.nmhdr.code]
			.find_notify:
				cmp ax, word[pdx + (pcx - 1) * 4]
				jne .not_my_notify
					; int3
					movzx pdx, word[pdx + (pcx - 1) * 4 + 2]
					mov pdi, [.e_cntrl + pdx]
					@call pdi(addr .this, [lParam], addr .e_cntrl)
					jmp ._break2
				.not_my_notify:
			loop .find_notify
			._break2:
			mov psi, [si_save]
			mov pdi, [di_save]
			jmp .return
			
			.control_notifications_map @const irpv cntrl, formType#@ControlStack\\{ match TName, cntrl\\#.Type\\\{ dptr TName\\\#.notifications \\\}\\}

			irpv cntrl, formType#@ControlStack\{
				match TName, cntrl\#.Type\\{
					match =TName\\#.events.def, TName\\#.events.def\\\{
						macro Make_\\#TName_events\\\\{
							TName\\#.events.counter = 0
							if ..start_events\\#TName <> ..end_events\\#TName
								dw TName\\#.events.count
							end if
							..start_events\\#TName:
							irpv evnt, TName\\#@EventStack\\\\\{
								dw evnt, TName\\#.\\\\\#evnt
								TName\\#.events.counter = TName\\#.events.counter + 1
							\\\\\}
							..end_events\\#TName:
							TName\\#.events.count = TName\\#.events.counter
						\\\\}
						TName\\#.events.def equ
						TName\\#.events @const Make_\\#TName_events
						purge Make_\\#TName_events
					\\\}
					match =TName\\#.notifications.def, TName\\#.notifications.def\\\{
						macro Make_\\#TName_notifications\\\\{
							TName\\#.notifications.counter = 0
							if ..start_notifications\\#TName <> ..end_notifications\\#TName
								dw TName\\#.notifications.count
							end if
							..start_notifications\\#TName:
							irpv ntfy, TName\\#@NotifyStack\\\\\{
								dw ntfy, TName\\#.\\\\\#ntfy
								TName\\#.notifications.counter = TName\\#.notifications.counter + 1
							\\\\\}
							..end_notifications\\#TName:
							TName\\#.notifications.count = TName\\#.notifications.counter
						\\\\}
						TName\\#.notifications.def equ
						TName\\#.notifications @const Make_\\#TName_notifications
						purge Make_\\#TName_notifications
					\\\}
				\\}
			\}

			.controlsOffsets @const irpv cntrl, formType#@ControlStack\{ dptr formType#.\\#cntrl \}

		.initEvent:
			@larg pcx, @arg1, \
				pbx, @arg4
			mov [.this.hWnd], pcx
			@call [SetWindowPtrA]([hwndDlg], GWL_USERDATA, [lParam])
		    @call [OpenThemeData]([.this.hWnd], L "OkButton;Button;Toolbar")
		    mov [.this.hTheme], pax
			if formType#.cdit > 0
				mov [si_save], psi
				mov [di_save], pdi
				mov pdi, formType#.cdit
				.init_control_loop:
					mov psi, [.controlsOffsets + (pdi - 1) * pointer.size]
					virtObj .cntrl:arg CONTROL at psi from addr psi + pbx
					mov [.cntrl.ID], di
					@call [GetDlgItem]([.this.hWnd], edi)
					mov [.cntrl.hWnd], pax
					@call [SetWindowPtrA]([.cntrl.hWnd], GWL_USERDATA, psi)
				dec pdi
				jnz .init_control_loop
				mov psi, [si_save]
				mov pdi, [di_save]
			end if
			if defined .this.WM_INITDIALOG
				@call [.this.WM_INITDIALOG](addr .this, addr hwndDlg)
			else
				xor eax, eax
			end if
			jmp .return
		
		.close_event:
			if defined .this.WM_CLOSE
				@call [.this.WM_CLOSE](addr .this, addr hwndDlg)
			else
				xor eax, eax
			end if
			@call [.this.__close](addr .this, addr hwndDlg, pax)
			jmp .return

		irpv evnt, formType#@EventStack\{
			local matched
			match =.this.WM_INITDIALOG, .this.\#evnt\\{
				define matched
			\\}
			match =.this.WM_CLOSE, .this.\#evnt\\{
				define matched
			\\}
			match =matched, matched\\{
				.eventHandler_\#evnt:
					@call [.this.\#evnt](addr .this, addr hwndDlg)
					jmp .return
			\\}
		\}

	endp
	
	proc_resprologue
}

; macro ShblDialog_Proc_old formType{

; 	proc_noprologue

; 	proc formType#_DialogProc uses pbx, hwndDlg, uMsg, wParam, lParam
; 		virtObj .this:arg formType at pbx
; 		@sarg @arg1, @arg2, @arg3, @arg4
; 		xor eax, eax
; 		cmp @arg2, WM_INITDIALOG
; 			je .initEvent
; 		@call [GetWindowPtrA]([hwndDlg], GWL_USERDATA)
; 		mov pbx, pax
; 		xor eax, eax
; 		mov pdx, [uMsg]
; 		mov pcx, [wParam]
; 		irpv evnt, formType#@EventStack\{
; 			match =.this.WM_INITDIALOG, .this.\#evnt\\{
; 				rept 0\\{
; 			\\}
; 			rept 1\\{
; 				cmp edx, evnt
; 					je .eventHandler_\#evnt
; 			\\}
; 		\}
; 		cmp edx, WM_COMMAND
; 			je .commandEventsChecker
; 		xor eax, eax
; 		.return: ret

; 		.initEvent:
; 			@larg pcx, @arg1, \
; 				pbx, @arg4
; 			mov [.this.hWnd], pcx
; 			@call [SetWindowPtrA]([hwndDlg], GWL_USERDATA, [lParam])
; 			if formType#.cdit > 0
; 				locals 
; 					SavePsi	 	dptr ?
; 					SavePdi 	dptr ?
; 				endl
; 				mov [SavePsi], psi
; 				mov [SavePdi], pdi
; 				mov pdi, formType#.cdit
; 				.init_control_loop:
; 					mov psi, [.controlsOffsets + (pdi - 1) * pointer.size]
; 					virtObj .cntrl:arg CONTROL at psi from addr psi + pbx
; 					mov [.cntrl.ID], di
; 					@call [GetDlgItem]([.this.hWnd], edi)
; 					mov [.cntrl.hWnd], pax
; 					@call [SetWindowPtrA]([.cntrl.hWnd], GWL_USERDATA, psi)
; 					@call [GetWindowRect]([.cntrl.hWnd], addr .cntrl.baseRect)
; 					@call [MapWindowPoints](NULL, [.this.hWnd], addr .cntrl.baseRect.left, 2)
; 					mov eax, [.cntrl.baseRect.left]
; 					sub [.cntrl.baseRect.right], eax
; 					mov eax, [.cntrl.baseRect.top]
; 					sub [.cntrl.baseRect.bottom], eax
; 				dec pdi
; 				jnz .init_control_loop
; 				mov psi, [SavePsi]
; 				mov pdi, [SavePdi]
; 			end if
; 			@call CNV:fill(addr .this.baseRect, .DialogRect, sizeof.RECT)
; 			@call [MapDialogRect]([hwndDlg], addr .this.baseRect)
; 			if defined .this.WM_INITDIALOG
; 				@call [.this.WM_INITDIALOG](addr .this, addr hwndDlg)
; 			else
; 				xor eax, eax
; 			end if
; 			jmp .return

; 			.DialogRect RECT 0, 0, formType#._cx, formType#._cy

; 			.controlsOffsets:
; 				irpv cntrl, formType#@ControlStack\{
; 					dptr formType#.\#cntrl
; 				\}

; 		irpv evnt, formType#@EventStack\{
; 			match =.this.WM_INITDIALOG, .this.\#evnt\\{
; 			rept 0\\{\\}rept 1\\{
; 				.eventHandler_\#evnt:
; 					@call [.this.\#evnt](addr .this, addr hwndDlg)
; 					jmp .return
; 			\\}
; 		\}

; 		.commandEventsChecker:
; 			irpv cntrl, formType#@ControlStack\{
; 				match TName, cntrl\#.Type\\{
; 					\\local events_count, events_counter
; 					events_counter = 0

; 					if events_count > 1
; 						cmp cx, formType#.\#cntrl\#._id
; 						jne .nextCntrl\#cntrl
; 					end if

; 					irpv evnt, TName\\#@EventStack\\\{
; 						cmp ecx, evnt shl 16 + formType#.\#cntrl\#._id
; 							je .controlEventHandler_\#cntrl\\\#evnt
; 						events_counter = events_counter + 1
; 					\\\}
; 					events_count = events_counter

; 					if events_count > 1
; 						.nextCntrl\#cntrl:
; 					end if
; 				\\}
; 			\}
; 			jmp .return

; 			irpv cntrl, formType#@ControlStack\{
; 				match TName, cntrl\#.Type\\{
; 					irpv evnt, TName\\#@EventStack\\\{
; 						.controlEventHandler_\#cntrl\\\#evnt:
; 						; int3
; 							@call [.this.\#cntrl\#.\\\#evnt](addr .this, addr hwndDlg, addr .this.\#cntrl)
; 							jmp .return
; 					\\\}
; 				\\}
; 			\}

; 	endp
	
; 	proc_resprologue
; }

; macro @S_Dialog FType, [args]{
; 	common
; 	local sufx
; 	_Text#sufx 		equ "DialogForm"
; 	_style#sufx 	equ WS_VISIBLE or WS_CAPTION or WS_SYSMENU or WS_MINIMIZEBOX or WS_MAXIMIZEBOX or DS_CENTER
; 	_styleEx#sufx 	equ NULL
; 	forward
; 	match name:val\{
; 		name\#sufx equ args
; 	\}
; 	ShblDialog _text#sufx, _style#sufx
; } 
