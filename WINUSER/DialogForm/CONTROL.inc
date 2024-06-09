macro control _control*, initvals=NONE, _Text="Control", _x=0, _y=0, _cx=60, _cy=20, _style=WS_VISIBLE, _styleEx=NULL{
	local thislab, matched, _cname, _ctype
	match cname ctype, _control\{
		define matched
		cname ctype NONE, NONE, NONE, initvals
		_cname equ cname
		_ctype equ ctype
	\}
	match =matched, matched\{
		define matched
		thislab _control NONE, NONE, NONE, initvals
		_cname equ thislab
		_ctype equ _control
	\}
	match name _cname_ _ctype_, struct@lastname _cname _ctype\{	
		name\#@ControlStack equ _cname_
		name\#.\#_cname_\#._x = _x
		name\#.\#_cname_\#._y = _y
		name\#.\#_cname_\#._cx = _cx
		name\#.\#_cname_\#._cy = _cy
		name\#.\#_cname_\#._rx = _x+_cx
		name\#.\#_cname_\#._ry = _y+_cy
		name\#.\#_cname_\#._style = _style
		name\#.\#_cname_\#._styleEx = _styleEx
		name\#.\#_cname_\#._Text equ _Text
	\}	
}

struct DLGITEMTEMPLATE
	style dd ?
	dwExtendedStyle dd ?
	x dw ?
	y dw ?
	cx dw ?
	cy dw ?
	id dw ?
ends

struct CONTROL
	hWnd			dq ?
	ID				dw ?
	bgColorBrush 	dq 0
	bkColor 		dd 0xFFFFFF
	txColor			dd 0x000000
	getTextLen 		dm DIALOGFORM.getTextLen
	getText 		dm DIALOGFORM.getText
	setText 		dm DIALOGFORM.setText
	setBgColor		dm DIALOGFORM.setBgColor
	hideCur 		dm this
	setTheme 		dm this
ends

struct BUTTON CONTROL
	const CType equ "Button"
ends

struct STATIC CONTROL
	const CType equ "Static"
ends

struct LISTBOX CONTROL
	const CType equ "Listbox"
ends

struct TABCONTROL CONTROL
	const CType equ "SysTabControl32"
ends

include "CONTROL.asm"
include "EDIT.inc"
include "COMBOBOX.inc"