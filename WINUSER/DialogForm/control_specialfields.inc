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
	_x_sticker#a  	equ 0
	_y_sticker#a  	equ 0
	_cx_sticker#a  	equ 0
	_cy_sticker#a  	equ 0
	_flags#a 		equ 0

	_cname equ thislab
	_ctype equ _control
	match cname ctype, _control\{
		_cname equ cname
		_ctype equ ctype
	\}
	forward
	; local _thisarg
	; define _thisarg argums
	match _name_ : _val, argums\{
		_name_\#a equ _val
	\}
	common
	match _cname_ _ctype_, _cname _ctype\{
		control@create _cname_, _ctype_,\
			_x#a, _y#a, _cx#a, _cy#a,\ 
			_style#a, _style_ex#a, _text#a,\ 
			<_x_sticker#a, _y_sticker#a, _cx_sticker#a, _cy_sticker#a>,\
			_flags#a,\
			_initvals#a
	\}
}

control@scaler_id equ 0
control@childs_count equ 0
control@last_name equ

macro control@unzip_scale_flags name, cname, x, y, rx, ry{
	name#.#cname#._x_sticker 	= x
	name#.#cname#._y_sticker 	= y
	name#.#cname#._cx_sticker 	= rx
	name#.#cname#._cy_sticker 	= ry
}

macro specialfields_define_control name{
	macro control@create cname, ctype, _x, _y, _cx, _cy, _style, _style_ex, _text, scale_flags, flags, initvals&\{
		match any, initvals\\{
			cname ctype NONE, NONE, \
				<<name#.\#cname\#._x, name#.\#cname\#._y, name#.\#cname\#._rx, name#.\#cname\#._ry>,\
				ctype\#.events, ctype\#.notifications, ctype\#.hooks,\
				<scale_flags>, flags, <-1, -1, -1, -1>, control@scaler_id, control@childs_count>, any
		rept 0\\{\\} rept 1\\{
			cname ctype NONE, NONE, \
				<<name#.\#cname\#._x, name#.\#cname\#._y, name#.\#cname\#._rx, name#.\#cname\#._ry>,\
				ctype\#.events, ctype\#.notifications, ctype\#.hooks,\
				<scale_flags>, flags, <-1, -1, -1, -1>, control@scaler_id, control@childs_count>
		\\}
		name#@ControlStack equ cname
		name#.\#cname\#._x = _x
		name#.\#cname\#._y = _y
		name#.\#cname\#._cx = _cx
		name#.\#cname\#._cy = _cy
		name#.\#cname\#._rx = (_x) + (_cx)
		name#.\#cname\#._ry = (_y) + (_cy)
		name#.\#cname\#._style = _style
		name#.\#cname\#._styleEx = _style_ex

		control@unzip_scale_flags name, cname, scale_flags
		; match _x=,_y=,
			; _name_\#_x_sticker 	= _x_sticker#sufx
			; _name_\#_y_sticker 	= _y_sticker#sufx
			; _name_\#_cx_sticker = _cx_sticker#sufx
			; _name_\#_cy_sticker = _cy_sticker#sufx
		name#.\#cname\#._Text equ _text
		restore control@last_name
		control@last_name equ cname
	\}
}

macro secialfields_inherit_control parent, name{
	irpv ParentControl, parent#@ConstrolStack\{
		name#@ConstrolStack equ ParentControl
		name#.\#ParentControl\#._x = parent#.\#ParentControl\#._x
		name#.\#ParentControl\#._y = parent#.\#ParentControl\#._y
		name#.\#ParentControl\#._cx = parent#.\#ParentControl\#._cx
		name#.\#ParentControl\#._cy = parent#.\#ParentControl\#._cy
		name#.\#ParentControl\#._rx = parent#.\#ParentControl\#._rx
		name#.\#ParentControl\#._ry = parent#.\#ParentControl\#._ry
		name#.\#ParentControl\#._style = parent#.\#ParentControl\#._style
		name#.\#ParentControl\#._styleEx = parent#.\#ParentControl\#._styleEx
		name#.\#ParentControl\#._Text equ parent#.\#ParentControl\#._Text
	\}
}

macro specialfields_ends_control name{}

add_specialfields control