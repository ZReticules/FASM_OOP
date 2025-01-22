importlib user32,\
	HideCaret

macro CONTROL.hideCur this{
	local _this
	inlineObj _this, this, pcx
	@call [HideCaret]([_this+CONTROL.hWnd])
}

macro CONTROL.setTheme this, wstrLp{
	local _this
	inlineObj _this, this, pcx
	@call [SetWindowTheme]([_this+CONTROL.hWnd], wstrLp, 0)
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
	forward
	local _thisarg
	define _thisarg argums
	match _name_ =: _val, _thisarg\{
		_name_\#a equ _val
	\}
	common
	match _name_|_strname, _cname|struct@lastname\{
		initvals equ NONE, NONE, <_strname\#.\#_name_\#._x, _strname\#.\#_name_\#._y, _strname\#.\#_name_\#._cx, _strname\#.\#_name_\#._cy>
	\} 
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