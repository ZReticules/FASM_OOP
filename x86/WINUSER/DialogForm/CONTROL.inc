importlib user32,\
	HideCaret

macro control _control*, initvals=NONE, _Text="", _x=0, _y=0, _cx=60, _cy=20, _style=WS_VISIBLE, _styleEx=NULL{
	local thislab, _initvals, _cname, _ctype, any
	_cname equ thislab
	_ctype equ _control
	match cname ctype, _control\{
		define matched
		restore _cname
		restore _ctype
		_cname equ cname
		_ctype equ ctype
	\}
	_initvals equ NONE, NONE, NONE
	match any, initvals\{
		_initvals equ _initvals, initvals
	\}
	match name _cname_ _ctype_ _initvals_, struct@lastname _cname _ctype _initvals\{
		_cname_ _ctype_ _initvals_	
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

macro @control _control*, [argums]{
	common
	local thislab, _cname, _ctype, any, a
	_initvals#a 	equ 
	_text#a 		equ ""
	_x#a 			equ 0
	_y#a 			equ 0
	_cx#a 			equ 60
	_cy#a 			equ 20
	_style#a 		equ WS_VISIBLE
	_style_ex#a 	equ NULL
	_cname equ thislab
	_ctype equ _control
	match cname ctype, _control\{
		define matched
		restore _cname
		restore _ctype
		_cname equ cname
		_ctype equ ctype
	\}
	initvals equ NONE, NONE, NONE
	forward
	local _thisarg
	define _thisarg argums
	match _name_ =: _val, _thisarg\{
		_name_\#a equ _val
	\}
	common
	match any, _initvals#a\{
		initvals equ initvals, _initvals#a
	\}
	match name _cname_ _ctype_ _initvals_, struct@lastname _cname _ctype initvals\{
		_cname_ _ctype_ _initvals_	
		name\#@ControlStack equ _cname_
		name\#.\#_cname_\#._x = _x#a
		name\#.\#_cname_\#._y = _y#a
		name\#.\#_cname_\#._cx = _cx#a
		name\#.\#_cname_\#._cy = _cy#a
		name\#.\#_cname_\#._rx = _x#a+_cx#a
		name\#.\#_cname_\#._ry = _y#a+_cy#a
		name\#.\#_cname_\#._style = _style#a
		name\#.\#_cname_\#._styleEx = _style_ex#a
		name\#.\#_cname_\#._Text equ _text#a
	\}	
	restore _x#a, _y#a, _cx#a, _cy#a, _style#a, _text#a, _style_ex#a
}

struct DLGITEMTEMPLATE
	style 			dd ?
	dwExtendedStyle dd ?
	x 				dw ?
	y 				dw ?
	cx 				dw ?
	cy 				dw ?
	id 				dw ?
ends

struct CONTROL
	hWnd			dd ?
	ID				dw ?
	bgColorBrush 	dd 0
	getTextLen 		dm DIALOGFORM.getTextLen:inline
	getText 		dm DIALOGFORM.getText:inline
	setText 		dm DIALOGFORM.setText:inline
	setBgColor		dm DIALOGFORM.setBgColor
	hideCur 		dm this:inline
	setTheme 		dm this:inline
ends

struct _DLG_SPECIAL_CONTROL CONTROL
	const CType_ex	equ "STD"
	bkColor 		dd 0xFFFFFF
	txColor			dd 0x000000
ends

macro CONTROL.hideCur this{
	local _this
	inlineObj _this, this, ecx
	@call [HideCaret]([_this+CONTROL.hWnd])
}

macro CONTROL.setTheme this, wstrLp{
	local _this
	inlineObj _this, this, ecx
	@call [SetWindowTheme]([_this+CONTROL.hWnd], wstrLp, 0)
}

struct BUTTON CONTROL
	const CType equ "Button"
ends

struct STATIC _DLG_SPECIAL_CONTROL
	const CType_ex	equ "SPEC"
	const CType equ "Static"
ends

struct LISTBOX _DLG_SPECIAL_CONTROL
	const CType equ "Listbox"
ends

struct TABCONTROL CONTROL
	const CType equ "SysTabControl32"
ends

struct LISTVIEW CONTROL
	const CType equ "SysListView32"
ends

UDM_SETRANGE32 = 1135

struct UPDOWN CONTROL
	const CType equ "msctls_updown32"
ends

include "EDIT.inc"
include "COMBOBOX.inc"
include "TREEVIEW.inc"