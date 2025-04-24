; base parameters
; _align(base - start):
; 	start, end, center, evenly, evenly_edges, evenly_start_edge, evenly_end_edge
; _orientation(base - vertical):
; 	horizontal, vertical
; _cx/_cy(base - parent):
; 	parent, content, [n] %, [n]

; other parameters
; _position: absolute for absolute position
; _name - name of control/layout
; _x/y
; _style(only for controls)
; _text(only for controls)

last@layout equ 

macro $align_init_start layout_name, sufx{
	layout_name#._padding = 0
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y
	\}
}

macro $align_init_end layout_name, sufx{
	layout_name#._padding = 0
	local base_axis_len
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x
		base_axis_len equ layout_name#._cx
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y
		base_axis_len equ layout_name#._cy
	\}
	if layout_name#._elems_nosize_count = 0
		layout_name#._last_pos = layout_name#._last_pos + base_axis_len - layout_name#._elems_coord_count
	end if
}

macro $align_init_center layout_name, sufx{
	layout_name#._padding = 0
	local base_axis_len
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x
		base_axis_len equ layout_name#._cx
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y
		base_axis_len equ layout_name#._cy
	\}
	if layout_name#._elems_nosize_count = 0
		layout_name#._last_pos = layout_name#._last_pos + (base_axis_len - layout_name#._elems_coord_count) / 2
	end if
}

macro $align_init_evenly layout_name, sufx{
	layout_name#._padding = 0
	local base_axis_len
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x
		base_axis_len equ layout_name#._cx
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y
		base_axis_len equ layout_name#._cy
	\}
	if (layout_name#._elems_nosize_count = 0) & ((layout_name#._elems_count - 1) > 0)
		layout_name#._padding = (base_axis_len - layout_name#._elems_coord_count) / (layout_name#._elems_count - 1)
	end if
}

macro $align_init_evenly_edges layout_name, sufx{
	layout_name#._padding = 0
	local base_axis_len
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x
		base_axis_len equ layout_name#._cx
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y
		base_axis_len equ layout_name#._cy
	\}
	if (layout_name#._elems_nosize_count = 0)
		layout_name#._padding = (base_axis_len - layout_name#._elems_coord_count) / (layout_name#._elems_count + 1)
		layout_name#._last_pos = layout_name#._last_pos + layout_name#._padding
	end if
}

macro $align_init_evenly_start_edge layout_name, sufx{
	layout_name#._padding = 0
	local base_axis_len
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x
		base_axis_len equ layout_name#._cx
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y
		base_axis_len equ layout_name#._cy
	\}
	if (layout_name#._elems_nosize_count = 0) & (layout_name#._elems_count > 0)
		layout_name#._padding = (base_axis_len - layout_name#._elems_coord_count) / layout_name#._elems_count
	end if
}

macro $align_init_evenly_end_edge layout_name, sufx{
	layout_name#._padding = 0
	local base_axis_len
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x
		base_axis_len equ layout_name#._cx
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y
		base_axis_len equ layout_name#._cy
	\}
	if (layout_name#._elems_nosize_count = 0) & (layout_name#._elems_count > 0)
		layout_name#._padding = (base_axis_len - layout_name#._elems_coord_count) / layout_name#._elems_count
		layout_name#._last_pos = layout_name#._last_pos + layout_name#._padding
	end if
}

macro $orientation_horizontal name, alignment, sufx_component{
	match _alignment|_layout_name, alignment|last@layout\{
		name#._x = _layout_name\#._last_pos
		match =parent, _cx#sufx_component\\{
			restore _cx#sufx_component
			_layout_name\#._elems_nosize_counter = _layout_name\#._elems_nosize_counter + 1
			local _size
			if _layout_name\#._elems_nosize_counter <= ((_layout_name\#._cx - _layout_name\#._elems_coord_count) mod _layout_name\#._elems_nosize_count)
				_size = (_layout_name\#._cx - _layout_name\#._elems_coord_count) / _layout_name\#._elems_nosize_count + 1
			else
				_size = (_layout_name\#._cx - _layout_name\#._elems_coord_count) / _layout_name\#._elems_nosize_count
			end if
			_cx#sufx_component equ _size
		rept 0\\{\\}rept 1\\{
			_layout_name\#._elems_coord_counter = _layout_name\#._elems_coord_counter + name#._cx
		\\}
		name#._y = _layout_name\#._y
		match =parent, _cy#sufx_component\\{
			restore _cy#sufx_component
			_cy#sufx_component equ _layout_name\#._cy
		\\}
		_layout_name\#._last_pos = _layout_name\#._last_pos + name#._cx + _layout_name\#._padding
		if name#._cy > _layout_name\#._elems_coord_maxer
			_layout_name\#._elems_coord_maxer = name#._cy
		end if
	\}
}

macro $orientation_vertical name, alignment, sufx_component{
	match _alignment|_layout_name, alignment|last@layout\{
		name#._y = _layout_name\#._last_pos
		match =parent, _cy#sufx_component\\{
			restore _cy#sufx_component
			_layout_name\#._elems_nosize_counter = _layout_name\#._elems_nosize_counter + 1
			local _size
			if _layout_name\#._elems_nosize_counter <= ((_layout_name\#._cy - _layout_name\#._elems_coord_count) mod _layout_name\#._elems_nosize_count)
				_size = (_layout_name\#._cy - _layout_name\#._elems_coord_count) / _layout_name\#._elems_nosize_count + 1
			else
				_size = (_layout_name\#._cy - _layout_name\#._elems_coord_count) / _layout_name\#._elems_nosize_count
			end if
			_cy#sufx_component equ _size
		rept 0\\{\\}rept 1\\{
			_layout_name\#._elems_coord_counter = _layout_name\#._elems_coord_counter + name#._cy
		\\}
		name#._x = _layout_name\#._x
		match =parent, _cx#sufx_component\\{
			restore _cx#sufx_component
			_cx#sufx_component equ _layout_name\#._cx
		\\}
		_layout_name\#._last_pos = _layout_name\#._last_pos + name#._cy + _layout_name\#._padding
		if name#._cx > _layout_name\#._elems_coord_maxer
			_layout_name\#._elems_coord_maxer = name#._cx
		end if
	\}
}

macro $calculate_position layout, sufx_parent, name, sufx_component{
	match _orientation|_layout_name, _orientation#sufx_parent|last@layout \{
		_layout_name\#._elems_counter = _layout_name\#._elems_counter + 1
		match macro_orientation, $orientation_\#_orientation \\{
			match _precent =%, _cy#sufx_component\\\{
				restore _cy#sufx_component
				_cy#sufx_component equ _layout_name\#._cy * (_precent) / 100
			\\\}
			match _precent =%, _cx#sufx_component\\\{
				restore _cx#sufx_component
				_cx#sufx_component equ _layout_name\#._cx * (_precent) / 100
			\\\}
			macro_orientation name, _align#sufx_parent, sufx_component
		\\}
	\}
}

macro $control _type*, [argums]{
	common
	local thislab, _ctype, any, a
	_initvals#a 		equ 
	_text#a 			equ ""
	_x#a 				equ
	_y#a 				equ
	_cx#a 				equ parent
	_cy#a 				equ parent
	_style#a 			equ WS_VISIBLE
	_style_ex#a 		equ NULL
	_name#a 			equ thislab
	_position#a			equ
	_padding#a 			equ 0
	_padding_left#a		equ 0
	_padding_right#a	equ 0
	_padding_top#a 		equ 0
	_padding_bottom#a 	equ 0

	forward
	local _thisarg
	define _thisarg argums
	match _name_ : _val, _thisarg\{
		restore _name_\#a
		_name_\#a equ _val
	\}
	common
	; match _name_|_strname, _name#a|struct@lastname\{
	; 	initvals equ NONE, NONE, <_strname\#.\#_name_\#._x, _strname\#.\#_name_\#._y, _strname\#.\#_name_\#._cx, _strname\#.\#_name_\#._cy>
	; \} 

	match |sufx_parent, _position#a|last@sufx\{
		\local name
		restore _x#a, _y#a
		$calculate_position last@layout, sufx_parent, name, a
		name\#._cx = _cx#a
		name\#._cy = _cy#a
		_x#a equ name\#._x
		_y#a equ name\#._y
	\}

	; match any, _initvals#a\{
	; 	initvals equ initvals, _initvals#a
	; \}
	match name|_initvals_, struct@lastname|initvals\{
		@control _name#a _type,\
			_x: (_x#a) + (_padding#a) + (_padding_left#a),\
			_y: (_y#a) + (_padding#a) + (_padding_top#a),\
			_cx: (_cx#a) - (_padding#a) * 2 - (_padding_left#a) - (_padding_right#a),\
			_cy: (_cy#a) - (_padding#a) * 2 - (_padding_top#a) - (_padding_bottom#a),\
			_style: _style#a,\
			_style_ex: _style_ex#a,\
			_text: _text#a,\
			_initvals: _initvals#a
		; _name_ _type _initvals_	
		; name\#@ControlStack equ _name_
		; name\#.\#_name_\#._x = _x#a
		; name\#.\#_name_\#._y = _y#a
		; name\#.\#_name_\#._cx = _cx#a
		; name\#.\#_name_\#._cy = _cy#a
		; name\#.\#_name_\#._rx = _x#a+_cx#a
		; name\#.\#_name_\#._ry = _y#a+_cy#a
		; name\#.\#_name_\#._style = _style#a
		; name\#.\#_name_\#._styleEx = _style_ex#a
		; name\#.\#_name_\#._Text equ _text#a
	\}	
	restore _x#a, _y#a, _cx#a, _cy#a, _style#a, _text#a, _style_ex#a
}

macro $layout [args] {
	common
	local sufx, name
	_name#sufx 			equ name
	_x#sufx				equ 0
	_y#sufx				equ 0
	; % в процентах от родителя, иначе в пунктах, не указывать для авто
	_cx#sufx 			equ parent
	_cy#sufx 			equ parent
	_orientation#sufx 	equ vertical
	_align#sufx			equ start
	_position#sufx		equ
	forward
	match name:val, args \{
		name\#sufx equ val
	\}
	common
	match _name_|struct_name, _name#sufx|struct@lastname \{
		_name#sufx equ struct_name\#.\#_name_
	\}
	match _name_, _name#sufx \{
		_name_\#._elems_coord_counter = 0
		_name_\#._elems_coord_maxer = 0
		_name_\#._elems_counter = 0
		_name_\#._elems_nosize_counter = 0
		match =absolute|_alignment, _position#sufx|_align#sufx \\{
			_name_\#._x = _x#sufx
			_name_\#._y = _y#sufx
			match macro_alignment, $align_init_\\#_alignment\\\{
				macro_alignment _name_, sufx
			\\\}
		rept 0\\{\\}match name_parent|sufx_parent|_alignment, last@layout|last@sufx|_align#sufx \\{
			$calculate_position last@layout, sufx_parent, _name_, sufx
			match macro_alignment, $align_init_\\#_alignment\\\{
				macro_alignment _name_, sufx
			\\\}
		\\}
		last@sufx equ sufx
		last@layout equ _name_
	\}
}

macro $endl {
	match sufx|name, last@sufx|last@layout\{
		name\#._elems_count = name\#._elems_counter
		name\#._elems_coord_count = name\#._elems_coord_counter
		name\#._elems_coord_max = name\#._elems_coord_maxer
		name\#._elems_nosize_count = name\#._elems_nosize_counter
		local __cx, __cy
		match =horizontal, _orientation\#sufx\\{
			__cx = name\#._elems_coord_counter
			__cy = name\#._elems_coord_max
		\\}
		match =vertical, _orientation\#sufx \\{
			__cy = name\#._elems_coord_counter
			__cx = name\#._elems_coord_max
		\\}
		match =content, _cx\#sufx\\{
			_cx\#sufx equ __cx
		\\}
		name\#._cx = _cx\#sufx
		match =content, _cy\#sufx\\{
			_cy\#sufx equ __cy
		\\}
		name\#._cy = _cy\#sufx
		name\#._rx = name\#._x + name\#._cx
		name\#._ry = name\#._y + name\#._cy
	\}
	restore last@layout, last@sufx
}