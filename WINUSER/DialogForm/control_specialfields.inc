macro specialfields_define_control name{

	macro @control _control*, [argums]\{
		\common
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
		match cname ctype, _control\\{
			define matched
			restore _cname
			restore _ctype
			_cname equ cname
			_ctype equ ctype
		\\}
		\forward
		local _thisarg
		define _thisarg argums
		match _name_ : _val, _thisarg\\{
			_name_\\#a equ _val
		\\}
		\common
		match _name_|_type, _cname|_ctype\\{
			initvals equ NONE, NONE, \
				<<name#.\\#_name_\\#._x, name#.\\#_name_\\#._y, name#.\\#_name_\\#._cx, name#.\\#_name_\\#._cy>,\
				_type\\#.events, _type\\#.notifications, _type\\#.hooks>
		\\} 
		match any, _initvals#a\\{
			initvals equ initvals, _initvals#a
		\\}
		match _cname_ _ctype_ _initvals_, _cname _ctype initvals\\{
			_cname_ _ctype_ _initvals_	
			name#@ControlStack equ _cname_
			name#.\\#_cname_\\#._x = _x#a
			name#.\\#_cname_\\#._y = _y#a
			name#.\\#_cname_\\#._cx = _cx#a
			name#.\\#_cname_\\#._cy = _cy#a
			name#.\\#_cname_\\#._rx = _x#a+_cx#a
			name#.\\#_cname_\\#._ry = _y#a+_cy#a
			name#.\\#_cname_\\#._style = _style#a
			name#.\\#_cname_\\#._styleEx = _style_ex#a
			name#.\\#_cname_\\#._Text equ _text#a
		\\}	
		\restore _x#a, _y#a, _cx#a, _cy#a, _style#a, _text#a, _style_ex#a
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