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
	const _x 				= 0
	const _y				= 0
	const _cx 				= 100
	const _cy 				= 50
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
	setText 				dm this
	getTextLen 				dm this
	getText 				dm this
	close 					dm this
	setBgColor				dm this
	setCaptionColor			dm this
	setTextColor			dm this
	setBorderColor			dm this
	setCornerType			dm this
	dispatchMessages		dm this
	startNM					dm this
	setIcon 				dm this
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
		formInfo NONE, formType\#.cdit, NONE, formType\#_DlgTemplate, formType\#_DialogProc
	\}
}

include "CONTROL.inc"
include "DialogForm.asm"
