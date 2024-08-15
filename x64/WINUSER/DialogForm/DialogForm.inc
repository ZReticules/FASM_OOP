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
	hWnd 					dq ?
	idCount 				dw ?
	bgColorBrush 			dq 0
	hDialogTemplate 		dq ?
	lpDialogFunc 			dq ?
	hWndParent 				dq 0
	WM_CLOSE 				event DIALOGFORM_WM_CLOSE
	WM_CTLCOLOREDIT  		event DIALOGFORM_WM_CTLCOLOR
	WM_CTLCOLORDLG  		event DIALOGFORM_WM_CTLCOLOR
	WM_CTLCOLORLISTBOX  	event DIALOGFORM_WM_CTLCOLOR
	WM_CTLCOLORSCROLLBAR  	event DIALOGFORM_WM_CTLCOLOR
	WM_CTLCOLORSTATIC  		event DIALOGFORM_WM_CTLCOLOR
	start 					dm this
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
ends

macro DIALOGFORM.getTextLen this{
	local _this
	inlineObj _this, this, rcx
	@call [GetWindowTextLengthA]([_this+CONTROL.hWnd])
}

macro DIALOGFORM.getText this, lpString, nMaxCount{
	local _this
	inlineObj _this, this, rcx
	@call [GetWindowTextA]([_this+CONTROL.hWnd], lpString, nMaxCount)
}

macro DIALOGFORM.setText this, lpString{
	local _this
	inlineObj _this, this, rcx
	@call [SetWindowTextA]([_this+CONTROL.hWnd], lpString)
}

macro DIALOGFORM.setIcon this, hIcon{
	local _this
	inlineObj _this, this, rcx
	@call [SendMessageA]([_this+CONTROL.hWnd], WM_SETICON, ICON_BIG, hIcon)
}

macro DIALOGFORM.close this{
	local _this
	inlineObj _this, this, rcx
	@call [SendMessageA]([_this+CONTROL.hWnd], WM_CLOSE, 0, 0)
}

struct params
	hWnd 	dq ?
	msg 	dq ?
	wparam 	dq ?
	lparam 	dq ?
ends

macro form formInfo&{
	local matched
	match formType _initvals, formInfo\{
		define matched
		formInfo NONE, formType\#.cdit, NONE, NONE, NONE, formType\#_DlgTemplate, formType\#_DialogProc, _initvals
	\}
	match =matched formType, matched formInfo\{
		formInfo NONE, formType\#.cdit, NONE, formType\#_DlgTemplate, formType\#_DialogProc
	\}
}

struc form formInfo&{
	make@form ., formInfo
}
macro make@form name, formInfo&{
	local matched
	match formType _initvals, formInfo\{
		define matched
		name formInfo NONE, formType\#.cdit, NONE, NONE, NONE, formType\#_DlgTemplate, formType\#_DialogProc, _initvals
	\}
	match =matched formType, matched formInfo\{
		name formInfo NONE, formType\#.cdit, NONE, formType\#_DlgTemplate, formType\#_DialogProc
	\}
}

include "CONTROL.inc"
include "DialogForm.asm"

; macro ShblDialog formType, _x = 0, _y = 0, _cx = 100, _cy = 50, _Text = "DialogForm", _style=WS_VISIBLE+WS_CAPTION+WS_SYSMENU+WS_MINIMIZEBOX+WS_MAXIMIZEBOX+DS_CENTER, _styleEx = NULL{
macro ShblDialog formType, _Text = "DialogForm", _style=WS_VISIBLE or WS_CAPTION or WS_SYSMENU or WS_MINIMIZEBOX or WS_MAXIMIZEBOX or DS_CENTER, _styleEx = NULL{
	; match formName formType, formInfo\{
		local controlCounter
		controlCounter = 0
		align 4
		formType\#_DlgTemplate DLGTEMPLATE \
			_style,\
			_styleEx,\
			formType\#.cdit,\
			formType\#._x,\
			formType\#._y,\
			formType\#._cx,\
			formType\#._cy
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
		irpv cntrl, formType#@ControlStack\{
			controlCounter = controlCounter + 1
			formType#.\#cntrl\#._id = controlCounter
			formType#.\#cntrl\#_DLGITEMTEMPLATE DLGITEMTEMPLATE \
				formType#.\#cntrl\#._style,\
				formType#.\#cntrl\#._styleEx,\
				formType#.\#cntrl\#._x,\
				formType#.\#cntrl\#._y,\
				formType#.\#cntrl\#._cx,\
				formType#.\#cntrl\#._cy,\
				formType#.\#cntrl\#._id
			match TName, cntrl\#.Type\\{
				du TName\\#.CType
				dw 00h
				du formType#.\#cntrl\#._Text
				dw 00h
			\\}
			db 4-($ mod 4) dup(?) 	; выравнивание
		\}
		formType#.cdit = controlCounter

		proc_noprologue

		proc formType#_DialogProc uses rbx, hwnddlg, msg, wparam, lparam
			virtObj .this:arg formType at rbx
			virtObj .cxThis:arg formType at rcx
			mov [hwnddlg], rcx
			mov [msg], rdx
			mov [wparam], r8
			mov [lparam], r9
			xor eax, eax
			cmp rdx, WM_INITDIALOG
			jne .nextEvent
				mov rbx, [lparam]
				mov [.this.hWnd], rcx
				@call [SetWindowLongPtrA]([hwnddlg], GWL_USERDATA, [lparam])
		irpv cntrl, formType#@ControlStack\{
				mov [.this.\#cntrl\#.ID], formType#.\#cntrl\#._id
				@call [GetDlgItem]([.this.hWnd], word [.this.\#cntrl\#.ID])
				mov [.this.\#cntrl\#.hWnd], rax
				@call [SetWindowLongPtrA]([.this.\#cntrl\#.hWnd], GWL_USERDATA, addr .this.\#cntrl)
		\}
			.nextEvent:
			@call [GetWindowLongPtrA]([hwnddlg], GWL_USERDATA)
			mov rbx, rax
			xor eax, eax
			mov rdx, [msg]
			mov r8, [wparam]
		irpv evnt, formType#@EventStack\{
			cmp rdx, evnt
			jne .nextEvent\#evnt
				@call [.cxThis.\#evnt](addr .this, addr hwnddlg)
				jmp .return
			.nextEvent\#evnt:
		\}
			cmp rdx, WM_COMMAND
			jne .NextEvent
		irpv cntrl, formType#@ControlStack\{
			
			match TName, cntrl\#.Type\\{
				\\local events_count, events_counter
				events_counter = 0

				if events_count > 1

				cmp r8w, formType#.\#cntrl\#._id
				jne .nextCntrl\#cntrl

				end if

				irpv evnt, TName\\#@EventStack\\\{
					cmp r8, evnt shl 16 + formType#.\#cntrl\#._id
					jne .nextEvnt\#cntrl\\\#evnt
						@call [.cxThis.\#cntrl\#.\\\#evnt](addr .this, addr hwnddlg, addr .this.\#cntrl)
						jmp .return
					.nextEvnt\#cntrl\\\#evnt:
					events_counter = events_counter + 1
				\\\}
				events_count = events_counter

				if events_count > 1

				.nextCntrl\#cntrl:
				
				end if
			\\}
		\}
			.NextEvent:
			xor eax, eax
			.return:ret
		endp
		proc_resprologue
		; formInfo NONE, formType\#.cdit, NONE, NONE, NONE, formType\#_DlgTemplate, formType\#_DialogProc, _initvals
	; \}
}
