@module DIALOGFORM

	include "Winuser.inc"

	struct@aligner equ c_aligner

	struct DLG
		setPtr 		dm this:inline	; (handle, value) -> void
		getPtr 		dm this:inline	; (handle) -> ptr
		removePtr 	dm this:inline	; (handle) -> void
		db ?
	ends

	include "WINUSER\DIALOGFORM\DLG.asm"
	
	include "WINUSER\DialogForm\dialogform_specialfields.inc"

	match =x64, __architecture\{
		importlib user32, \
			SetWindowPtrA="SetWindowLongPtrA",\
			GetWindowPtrA="GetWindowLongPtrA"

		; SetWindowPtrA fix SetWindowLongPtrA
		; GetWindowPtrA fix GetWindowLongPtrA
		base_layout_cx equ 1
		base_layout_cy equ 12
	\}

	match =x86, __architecture\{
		importlib user32, \
			SetWindowPtrA="SetWindowLongA",\
			GetWindowPtrA="GetWindowLongA"

		; SetWindowPtrA fix SetWindowLongA
		; GetWindowPtrA fix GetWindowLongA
		base_layout_cx equ 1
		base_layout_cy equ 12
	\}

	include "WINUSER\layouts.inc"

	importlib uxtheme,\
	    OpenThemeData

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

	struct DLG_SERVICE_DATA
		baseRect			RECT ?, ?, ?, ?
		lpEvents 			dptr ?
		lpClOffsets			dptr ?
		lpHooks				dptr ?
		; lpClEvents 			dptr ?
		; lpClNotifications 	dptr ?
		mapDialog 			MapDialog
		defaultSize			SIZE
	ends

	include "WINUSER\WINAPI_WINDOW.inc"

	struct DIALOGFORM WINAPI_WINDOW
		const _x 				= 0
		const _y				= 0
		const _cx 				= 100
		const _cy 				= 50
		const _pointsize 		= 10
		const _weight   		= FW_DONTCARE
		const _italic  			= 0
		const _charset			= DEFAULT_CHARSET
		const _font 			equ "Segoe UI Variable"

		idCount 				dd ?
		sData 					DLG_SERVICE_DATA
		; bgColorBrush 			dptr ?
		hDialogTemplate 		dptr ?
		; lpDialogFunc 			dptr ?
		; hTheme 					dptr ?
		__close 				dptr ?

		; setBgColor				dm this
		; unsetBgColor			dm this
		setCaptionColor			dm this
		setTextColor			dm this
		setBorderColor			dm this
		setCornerType			dm this
		dispatchMessages		dm this
		startNM					dm this
		scaleChilds				dm this
		__scaleChildRects		dm this
		
		start 					dm this:inline
		close 					dm this:inline
		setIcon 				dm this:inline

		; getClientSize			dm this:inline
		; getWindowSize 			dm this:inline
	ends

	struct Params
		hWnd 	dptr ?
		uMsg 	dptr ?
		wParam 	dptr ?
		lParam 	dptr ?
	ends

	macro dForm formInfo&\{
		local ..anon
		make@form ..anon, formInfo
	\}

	struc dForm formInfo&\{
		make@form ., formInfo
	\}
	macro make@form name, formInfo&\{
		match formType _initvals, formInfo\\{
			name formInfo NONE, \
				formType\\#.cdit, \
				<\
					<formType\\#._x, formType\\#._y, formType\\#._cx, formType\\#._cy>, \
					formType\\#.events, \
					formType\\#.controls_offsets,\
					formType\\#.hooks\
				>, \
				formType\\#_DlgTemplate, \
				NONE, _initvals
		rept 0\\{\\}match formType, formInfo\\{
			name formInfo NONE, \
				formType\\#.cdit, \
				<\
					<formType\\#._x, formType\\#._y, formType\\#._cx, formType\\#._cy>, \
					formType\\#.events, \
					formType\\#.controls_offsets,\
					formType\\#.hooks\
				>, \
				formType\\#_DlgTemplate, \
				NONE
		\\}
	\}

	EScaleMode enum \
		simple,\
		saveStart,\
		saveEnd,\
		saveCenter,\
		saveSizeLeft,\	; _c[x/y]_sticker only
		saveSizeRight,\	; _[x/y]_sticker only
		centerScale,\	; _c[x/y]_sticker only
					\	; only if _[x/y]_sticker = 0
					\	; align control at center in autoscale
		ignore=255

	include "WINUSER\DIALOGFORM\CONTROL.inc"
	include "WINUSER\DIALOGFORM\DialogForm.asm"
	include "WINUSER\DIALOGFORM\DefaultEvents.asm"
	include "WINUSER\DIALOGFORM\MsgLoopProc.fasm"

	struct DLGTEMPLATE
		style dd ?
		dwExtendedStyle dd ?
		cdit dw ?
		x dw ?
		y dw ?
		cx dw ?
		cy dw ?
	ends

	struct DLGITEMTEMPLATE
		style dd ?
		dwExtendedStyle dd ?
		x dw ?
		y dw ?
		cx dw ?
		cy dw ?
		id dw ?
	ends

	macro ShblDialog_Mem formType, _Text = "DialogForm", _style=WS_VISIBLE or WS_CAPTION or WS_SYSMENU or WS_MINIMIZEBOX or WS_MAXIMIZEBOX or DS_CENTER, _styleEx = NULL\{
		if formType\#.hooks.counter > 0
			DLG.useMainHooks
		end if
		local const_mem
		macro const_mem\\{
			\\local controlCounter
			controlCounter = 0
			DLGTEMPLATE \
				_style,\
				_styleEx,\
				formType\#.cdit,\
				formType\#._x,\
				formType\#._y,\
				formType\#._cx,\
				formType\#._cy
			dw 00h ; menu
			dw 00h ; header
			cwstr _Text
			if (_style and DS_SETFONT) = DS_SETFONT
				dw formType\#._pointsize
				; dw formType\#._weight
				; db formType\#._italic
				; db formType\#._charset
				du formType\#._font
				dw 0
			end if
			align 4
			irpv cntrl, formType\#@ControlStack\\\{
				controlCounter = controlCounter + 1
				formType\#.\\\#cntrl\\\#._id = controlCounter
				formType\#.\\\#cntrl\\\#_DLGITEMTEMPLATE DLGITEMTEMPLATE \
					formType\#.\\\#cntrl\\\#._style,\
					formType\#.\\\#cntrl\\\#._styleEx,\
					formType\#.\\\#cntrl\\\#._x,\
					formType\#.\\\#cntrl\\\#._y,\
					formType\#.\\\#cntrl\\\#._cx,\
					formType\#.\\\#cntrl\\\#._cy,\
					formType\#.\\\#cntrl\\\#._id
				match TName, cntrl\\\#.Type\\\\{
					if TName\\\\#.CType eq "Button"
						dw 0xFFFF, 0x0080
					else if TName\\\\#.CType eq "Edit"
						dw 0xFFFF, 0x0081
					else if TName\\\\#.CType eq "Static"
						dw 0xFFFF, 0x0082
					else if TName\\\\#.CType eq "ListBox"
						dw 0xFFFF, 0x0083
					else if TName\\\\#.CType eq "ScrollBar"
						dw 0xFFFF, 0x0084
					else if TName\\\\#.CType eq "ComboBox"
						dw 0xFFFF, 0x0085
					else
						du TName\\\\#.CType
						dw 00h
					end if
					cwstr formType\#.\\\#cntrl\\\#._Text
				\\\\}
				db (4-($ mod 4)) dup(0) 	; выравнивание
			\\\}
			formType\#.cdit = controlCounter
			if controlCounter > 0
				DLG.useControls
			end if
		\\}
		@const_align equ 4
		formType\#_DlgTemplate @const const_mem
		restore @const_align
		purge const_mem 
	\}

	struct DLGTEMPLATEEX
		dlgVer		dw ?
		signature	dw ?
		helpID		dd ?
		exStyle		dd ?
		style		dd ?
		cDlgItems	dw ?
		x			dw ?
		y			dw ?
		cx			dw ?
		cy			dw ?
	 ends

	struct DLGITEMTEMPLATEEX
		helpID	dd ?
		exStyle	dd ?
		style	dd ?
		x		dw ?
		y		dw ?
		cx		dw ?
		cy		dw ?
		id		dd ?
	ends

	macro ShblDialog_Mem_Ex formType, _Text = "DialogForm", _style=WS_VISIBLE or WS_CAPTION or WS_SYSMENU or WS_MINIMIZEBOX or WS_MAXIMIZEBOX or DS_CENTER, _styleEx = NULL\{
		if formType\#.hooks.counter > 0
			DLG.useMainHooks
		end if
		local const_mem
		macro const_mem\\{
			\\local controlCounter
			controlCounter = 0
			DLGTEMPLATEEX 1, 0xFFFF, 0,\
				_styleEx,\
				_style,\
				formType\#.cdit,\
				formType\#._x,\
				formType\#._y,\
				formType\#._cx,\
				formType\#._cy
			dw 00h ; menu
			dw 00h ; header
			cwstr _Text
			if (_style and DS_SETFONT) = DS_SETFONT
				dw formType\#._pointsize
				dw formType\#._weight
				db formType\#._italic
				db formType\#._charset
				du formType\#._font
				dw 0
			end if
			align 4
			irpv cntrl, formType\#@ControlStack\\\{
				controlCounter = controlCounter + 1
				formType\#.\\\#cntrl\\\#._id = controlCounter
				formType\#.\\\#cntrl\\\#_DLGITEMTEMPLATE DLGITEMTEMPLATEEX 0, \
					formType\#.\\\#cntrl\\\#._styleEx,\
					formType\#.\\\#cntrl\\\#._style,\
					formType\#.\\\#cntrl\\\#._x,\
					formType\#.\\\#cntrl\\\#._y,\
					formType\#.\\\#cntrl\\\#._cx,\
					formType\#.\\\#cntrl\\\#._cy,\
					formType\#.\\\#cntrl\\\#._id
				match TName, cntrl\\\#.Type\\\\{
					if TName\\\\#.CType eq "Button"
						dw 0xFFFF, 0x0080
					else if TName\\\\#.CType eq "Edit"
						dw 0xFFFF, 0x0081
					else if TName\\\\#.CType eq "Static"
						dw 0xFFFF, 0x0082
					else if TName\\\\#.CType eq "ScrollBar"
						dw 0xFFFF, 0x0083
					else if TName\\\\#.CType eq "ComboBox"
						dw 0xFFFF, 0x0085
					else
						du TName\\\\#.CType
						dw 00h
					end if
					cwstr formType\#.\\\#cntrl\\\#._Text
				\\\\}
				dw 0 ;extraCount
				align 4
			\\\}
			formType\#.cdit = controlCounter
			if controlCounter > 0
				DLG.useControls
			end if
		\\}
		@const_align equ 4
		formType\#_DlgTemplate @const const_mem
		restore @const_align
		purge const_mem 
	\}
	restore struct@aligner
@endm
