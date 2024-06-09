struct DLGTEMPLATE
	style dd ?
	dwExtendedStyle dd ?
	cdit dw ?
	x dw ?
	y dw ?
	cx dw ?
	cy dw ?
ends

struct DIALOGFORM
	hWnd 					dq ?
	idCount 				dw ?
	bgColorBrush 			dq 0
	bkColor 				dd 0x0
	txColor					dd 0xFFFFFF
	hDialogTemplate 		dq ?
	lpDialogFunc 			dq ?
	hWndParent 				dq ?
	WM_CLOSE 				event DIALOGFORM_WM_CLOSE
	; WM_CTLCOLORBTN  		event DIALOGFORM_WM_CTLCOLOR
	WM_CTLCOLOREDIT  		event DIALOGFORM_WM_CTLCOLOR
	WM_CTLCOLORDLG  		event DIALOGFORM_WM_CTLCOLOR
	WM_CTLCOLORLISTBOX  	event DIALOGFORM_WM_CTLCOLOR
	WM_CTLCOLORSCROLLBAR  	event DIALOGFORM_WM_CTLCOLOR
	WM_CTLCOLORSTATIC  		event DIALOGFORM_WM_CTLCOLOR
	start 					dm this
	setText 				dm this
	getTextLen 				dm this
	getText 				dm this
	close 					dm this
	setBgColor				dm this
	setCaptionColor			dm this
	setTextColor			dm this
	setBorderColor			dm this
	setCornerType			dm this
	setTheme				dm this
	dispatchMessages		dm this
	startNM					dm this
ends

struct params
	hWnd 	dq ?
	msg 	dq ?
	wparam 	dq ?
	lparam 	dq ?
ends

macro form formInfo&{
	local matched
	match formName formType _initvals, formInfo\{
		define matched
		formInfo NONE, formType\#.cdit, NONE, NONE, NONE, formType\#_DlgTemplate, formType\#_DialogProc, _initvals
	\}
	match =matched formName formType, matched formInfo\{
		formInfo NONE, formType\#.cdit, NONE, NONE, NONE, formType\#_DlgTemplate, formType\#_DialogProc
	\}
}

include "CONTROL.inc"
include "DialogForm.asm"
