; base parameters
; _position(base - relative): 
;	absolute - for absolute position
; 	relative - for standard relative position
; 	background - if you need to set your control on layout backround without effect on inner elements order(controls only)
; _align(base - start):
; 	start, end, center, evenly, evenly_edges, evenly_start_edge, evenly_end_edge
; _orientation(base - vertical):
; 	horizontal, vertical
; _x/_y(absolute only, base - 0):
;	[n]
; _cx/_cy(base - parent):
; 	parent, [n] %, [n], content(layouts only)
; _padding, _padding_left, _padding_top, _padding_right, _padding_bottom(base - 0):
; 	[n], [n] %

; other parameters
; _name
; _style(controls only)
; _style_ex(controls only)
; _text(controls only)

last@layout equ 
last@sufx equ

macro $align_init_start layout_name, sufx{
	layout_name#._padding = _space_between#sufx
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x + (_padding#sufx) + (_padding_left#sufx)
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y + (_padding#sufx) + (_padding_top#sufx)
	\}
}

macro $align_init_end layout_name, sufx{
	layout_name#._padding = _space_between#sufx
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x + (_padding#sufx) + (_padding_left#sufx)
		if layout_name#._elems_nosize_count = 0
			layout_name#._last_pos = layout_name#._last_pos + layout_name#._cx - layout_name#._elems_coord_count - (_padding#sufx) - (_padding_right#sufx)
		end if
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y + (_padding#sufx) + (_padding_top#sufx)
		if layout_name#._elems_nosize_count = 0
			layout_name#._last_pos = layout_name#._last_pos + layout_name#._cy - layout_name#._elems_coord_count - (_padding#sufx) - (_padding_bottom#sufx)
		end if
	\}
}

macro $align_init_center layout_name, sufx{
	layout_name#._padding = _space_between#sufx
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x + (_padding#sufx) + (_padding_left#sufx)
		if layout_name#._elems_nosize_count = 0
			layout_name#._last_pos = layout_name#._last_pos + (layout_name#._cx - layout_name#._elems_coord_count + (_space_between\#sufx) - (_padding_right#sufx) - (_padding_left#sufx)) / 2 - (_padding#sufx)
		end if
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y + (_padding#sufx) + (_padding_top#sufx)
		if layout_name#._elems_nosize_count = 0
			layout_name#._last_pos = layout_name#._last_pos + (layout_name#._cy - layout_name#._elems_coord_count + (_space_between\#sufx) - (_padding_bottom#sufx) - (_padding_top#sufx)) / 2 - (_padding#sufx)
		end if
	\}
}

macro $align_init_evenly layout_name, sufx{
	layout_name#._padding = _space_between#sufx
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x + (_padding#sufx) + (_padding_left#sufx)
		if (layout_name#._elems_nosize_count = 0) & ((layout_name#._elems_count - 1) > 0)
			layout_name#._padding = (layout_name#._cx - layout_name#._elems_coord_count + (_space_between\#sufx) * layout_name#._elems_count - (_padding_right#sufx) - (_padding_left#sufx) - (_padding#sufx) * 2) / (layout_name#._elems_count - 1)
			if layout_name#._padding < _space_between#sufx
				layout_name#._padding = _space_between#sufx
			end if
		end if
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y + (_padding#sufx) + (_padding_top#sufx)
		if (layout_name#._elems_nosize_count = 0) & ((layout_name#._elems_count - 1) > 0)
			layout_name#._padding = (layout_name#._cy - layout_name#._elems_coord_count + (_space_between\#sufx) * layout_name#._elems_count - (_padding_bottom#sufx) - (_padding_top#sufx) - (_padding#sufx) * 2) / (layout_name#._elems_count - 1)
			if layout_name#._padding < _space_between#sufx
				layout_name#._padding = _space_between#sufx
			end if
		end if
	\}
}

macro $align_init_evenly_edges layout_name, sufx{
	layout_name#._padding = _space_between#sufx
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x + (_padding#sufx) + (_padding_left#sufx)
		if (layout_name#._elems_nosize_count = 0)
			layout_name#._padding = (layout_name#._cx - layout_name#._elems_coord_count + (_space_between\#sufx) * layout_name#._elems_count - (_padding_right#sufx) - (_padding_left#sufx) - (_padding#sufx) * 2) / (layout_name#._elems_count + 1)
			if layout_name#._padding < _space_between#sufx
				layout_name#._padding = _space_between#sufx
			end if
		end if
		_padding_left#sufx equ (_padding_left#sufx) + layout_name#._padding
		_padding_right#sufx equ (_padding_right#sufx) + layout_name#._padding
		layout_name#._last_pos = layout_name#._last_pos + layout_name#._padding
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y + (_padding#sufx) + (_padding_top#sufx)
		if (layout_name#._elems_nosize_count = 0)
			layout_name#._padding = (layout_name#._cy - layout_name#._elems_coord_count + (_space_between\#sufx) * layout_name#._elems_count - (_padding_bottom#sufx) - (_padding_top#sufx) - (_padding#sufx) * 2) / (layout_name#._elems_count + 1)
			if layout_name#._padding < _space_between#sufx
				layout_name#._padding = _space_between#sufx
			end if
		end if
		_padding_top#sufx equ (_padding_top#sufx) + layout_name#._padding
		_padding_bottom#sufx equ (_padding_bottom#sufx) + layout_name#._padding
		layout_name#._last_pos = layout_name#._last_pos + layout_name#._padding
	\}
}

macro $align_init_evenly_start_edge layout_name, sufx{
	layout_name#._padding = _space_between#sufx
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x + (_padding#sufx) + (_padding_left#sufx)
		if (layout_name#._elems_nosize_count = 0) & (layout_name#._elems_count > 0)
			layout_name#._padding = (layout_name#._cx - layout_name#._elems_coord_count + (_space_between\#sufx) * layout_name#._elems_count - (_padding_right#sufx) - (_padding_left#sufx) - (_padding#sufx) * 2) / layout_name#._elems_count
		end if
		_padding_right#sufx equ (_padding_right#sufx) + layout_name#._padding
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y + (_padding#sufx) + (_padding_top#sufx)
		if (layout_name#._elems_nosize_count = 0) & (layout_name#._elems_count > 0)
			layout_name#._padding = (layout_name#._cy - layout_name#._elems_coord_count + (_space_between\#sufx) * layout_name#._elems_count - (_padding_bottom#sufx) - (_padding_top#sufx) - (_padding#sufx) * 2) / layout_name#._elems_count
		end if
		_padding_bottom#sufx equ (_padding_bottom#sufx) + layout_name#._padding
	\}
}

macro $align_init_evenly_end_edge layout_name, sufx{
	layout_name#._padding = _space_between#sufx
	match =horizontal, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._x + (_padding#sufx) + (_padding_left#sufx)
		if (layout_name#._elems_nosize_count = 0) & (layout_name#._elems_count > 0)
			layout_name#._padding = (layout_name#._cx - layout_name#._elems_coord_count + (_space_between\#sufx) * layout_name#._elems_count - (_padding_right#sufx) - (_padding_left#sufx) - (_padding#sufx) * 2) / layout_name#._elems_count
		end if
		_padding_left#sufx equ (_padding_left#sufx) + layout_name#._padding
		layout_name#._last_pos = layout_name#._last_pos + layout_name#._padding
	\}
	match =vertical, _orientation#sufx\{
		layout_name#._last_pos = layout_name#._y + (_padding#sufx) + (_padding_top#sufx)
		if (layout_name#._elems_nosize_count = 0) & (layout_name#._elems_count > 0)
			layout_name#._padding = (layout_name#._cy - layout_name#._elems_coord_count + (_space_between\#sufx) * layout_name#._elems_count - (_padding_bottom#sufx) - (_padding_top#sufx) - (_padding#sufx) * 2) / layout_name#._elems_count
		end if
		_padding_top#sufx equ (_padding_top#sufx) + layout_name#._padding
		layout_name#._last_pos = layout_name#._last_pos + layout_name#._padding
	\}
}

macro $orientation_horizontal name, alignment, sufx_component{
	match _alignment|_layout_name|sufx, alignment|last@layout|last@sufx\{
		name#._x = _layout_name\#._last_pos
		match =parent, _cx#sufx_component\\{
			restore _cx#sufx_component
			_layout_name\#._elems_nosize_counter = _layout_name\#._elems_nosize_counter + 1
			local _size, _saved_size
			_saved_size = _layout_name\#._cx - _layout_name\#._elems_coord_count - (_padding\#sufx) * 2 - (_padding_left\#sufx) - (_padding_right\#sufx) - (_layout_name\#._elems_nosize_count - 1) * _layout_name\#._padding
			if _layout_name\#._elems_nosize_counter <= (_saved_size mod _layout_name\#._elems_nosize_count)
				_size = _saved_size / _layout_name\#._elems_nosize_count + 1
			else
				_size = _saved_size / _layout_name\#._elems_nosize_count
			end if
			_cx#sufx_component equ _size
		rept 0\\{\\} rept 1\\{
			_layout_name\#._elems_coord_counter = _layout_name\#._elems_coord_counter + name#._cx + (_space_between\#sufx)
		\\}
		name#._y = _layout_name\#._y + (_padding\#sufx) + (_padding_top\#sufx)
		match =parent, _cy#sufx_component\\{
			restore _cy#sufx_component
			_cy#sufx_component equ (_layout_name\#._cy - (_padding\#sufx) * 2 - (_padding_top\#sufx) - (_padding_bottom\#sufx))
		\\}
		_layout_name\#._last_pos = _layout_name\#._last_pos + name#._cx + _layout_name\#._padding
		if name#._cy > _layout_name\#._elems_coord_maxer
			_layout_name\#._elems_coord_maxer = name#._cy
		end if
	\}
}

			; _cx: (_cx#a) - (_padding#a) * 2 - (_padding_left#a) - (_padding_right#a),\
			; _cy: (_cy#a) - (_padding#a) * 2 - (_padding_top#a) - (_padding_bottom#a),\
macro $orientation_vertical name, alignment, sufx_component{
	match _alignment|_layout_name|sufx, alignment|last@layout|last@sufx\{
		name#._y = _layout_name\#._last_pos
		match =parent, _cy#sufx_component\\{
			restore _cy#sufx_component
			_layout_name\#._elems_nosize_counter = _layout_name\#._elems_nosize_counter + 1
			local _size, _saved_size
			_saved_size = _layout_name\#._cy - _layout_name\#._elems_coord_count - (_padding\#sufx) * 2 - (_padding_top\#sufx) - (_padding_bottom\#sufx) - (_layout_name\#._elems_nosize_count - 1) * _layout_name\#._padding
			if _layout_name\#._elems_nosize_counter <= (_saved_size mod _layout_name\#._elems_nosize_count)
				_size = _saved_size / _layout_name\#._elems_nosize_count + 1
			else
				_size = _saved_size / _layout_name\#._elems_nosize_count
			end if
			_cy#sufx_component equ _size
		rept 0\\{\\}rept 1\\{
			_layout_name\#._elems_coord_counter = _layout_name\#._elems_coord_counter + name#._cy + (_space_between\#sufx)
		\\}
		name#._x = _layout_name\#._x + (_padding\#sufx) + (_padding_left\#sufx)
		match =parent, _cx#sufx_component\\{
			restore _cx#sufx_component
			_cx#sufx_component equ (_layout_name\#._cx - (_padding\#sufx) * 2 - (_padding_left\#sufx) - (_padding_right\#sufx))
		\\}
		_layout_name\#._last_pos = _layout_name\#._last_pos + name#._cy + _layout_name\#._padding
		if name#._cx > _layout_name\#._elems_coord_maxer
			_layout_name\#._elems_coord_maxer = name#._cx
		end if
	\}
}

macro $calculate_position layout, sufx_parent, name, sufx_component{
	match _orientation|_layout_name|sufx, _orientation#sufx_parent|last@layout|last@sufx \{
		_layout_name\#._elems_counter = _layout_name\#._elems_counter + 1
		match macro_orientation, $orientation_\#_orientation \\{
			match _precent =%, _cy#sufx_component\\\{
				restore _cy#sufx_component
				_cy#sufx_component equ (_layout_name\#._cy  - (_padding\#sufx) * 2 - (_padding_top\#sufx) - (_padding_bottom\#sufx)) * (_precent) / 100
			\\\}
			match _precent =%, _cx#sufx_component\\\{
				restore _cx#sufx_component
				_cx#sufx_component equ (_layout_name\#._cx  - (_padding\#sufx) * 2 - (_padding_left\#sufx) - (_padding_right\#sufx)) * (_precent) / 100
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
	_position#a			equ relative
	_padding#a 			equ 0
	_padding_left#a		equ 0
	_padding_right#a	equ 0
	_padding_top#a 		equ 0
	_padding_bottom#a 	equ 0
	_flags#a 			equ 0

	match any, last@sufx\{
		_x_sticker#a  		equ _x_sticker\#any
		_y_sticker#a  		equ _y_sticker\#any
		_cx_sticker#a  		equ _cx_sticker\#any
		_cy_sticker#a  		equ _cy_sticker\#any
	rept 0\{\} rept 1\{
		_x_sticker#a  		equ 0
		_y_sticker#a  		equ 0
		_cx_sticker#a  		equ 0
		_cy_sticker#a  		equ 0
	\}

	forward
	; local _thisarg
	; define _thisarg argums
	match _param : _val, argums\{
		_param\#a equ _val
	\}
	common
	match =relative|sufx_parent, _position#a|last@sufx\{
		\local name
		$calculate_position last@layout, sufx_parent, name, a
		name\#._cx = _cx#a
		name\#._cy = _cy#a
		_x#a equ name\#._x
		_y#a equ name\#._y
	\}
	match =background|layout_name, _position#a|last@layout\{
		_x#a equ (layout_name\#._x)
		_y#a equ (layout_name\#._y)
		_cx#a equ (layout_name\#._cx)
		_cy#a equ (layout_name\#._cy)
	\}

	local _val
	_val equ 0
	match val =%, _padding#a\{
		_padding#a equ 0
		_val equ val
	\}
	match val =%, _padding_left#a\{
		_padding_left#a equ ((_cx#a) * ((val) + (_val)) / 100)
	rept 0\{\} rept 1\{
		_padding_left#a equ (_padding_left#a + ((_cx#a) * (_val) / 100))
	\}
	match val =%, _padding_right#a\{
		_padding_right#a equ ((_cx#a) * ((val) + (_val)) / 100)
	rept 0\{\} rept 1\{
		_padding_right#a equ (_padding_right#a + ((_cx#a) * (_val) / 100))
	\}
	match val =%, _padding_top#a\{
		_padding_top#a equ ((_cy#a) * ((val) + (_val)) / 100)
	rept 0\{\} rept 1\{
		_padding_top#a equ (_padding_top#a + ((_cy#a) * (_val) / 100))
	\}
	match val =%, _padding_bottom#a\{
		_padding_bottom#a equ ((_cy#a) * ((val) + (_val)) / 100)
	rept 0\{\} rept 1\{
		_padding_bottom#a equ (_padding_bottom#a + ((_cy#a) * (_val) / 100))
	\}
	match cname|ctext, _name#a|_text#a\{
		control@create cname, _type,\
			(_x#a) + (_padding#a) + (_padding_left#a),\
			(_y#a) + (_padding#a) + (_padding_top#a),\
			(_cx#a) - (_padding#a) * 2 - (_padding_left#a) - (_padding_right#a),\
			(_cy#a) - (_padding#a) * 2 - (_padding_top#a) - (_padding_bottom#a),\
			_style#a,\
			_style_ex#a,\
			ctext,\
			<_x_sticker#a, _y_sticker#a, _cx_sticker#a, _cy_sticker#a>,\
			_flags#a,\
			_initvals#a
	\}
}

macro $layout [args] {
	common
	local sufx, name
	_name#sufx 				equ name
	_x#sufx					equ 0
	_y#sufx					equ 0
	; % в процентах от родителя, иначе в пунктах, не указывать для авто
	_cx#sufx 				equ parent
	_cy#sufx 				equ parent
	_orientation#sufx 		equ vertical
	_align#sufx				equ start
	_position#sufx			equ

	_padding#sufx 			equ 0
	_padding_left#sufx		equ 0
	_padding_right#sufx		equ 0
	_padding_top#sufx 		equ 0
	_padding_bottom#sufx	equ 0

	_space_between#sufx 	equ 0
	
	match any, last@sufx\{
		_x_sticker#sufx 		equ _x_sticker\#any
		_y_sticker#sufx 		equ _y_sticker\#any
		_cx_sticker#sufx 		equ _cx_sticker\#any
		_cy_sticker#sufx 		equ _cy_sticker\#any
	rept 0\{\} rept 1\{
		_x_sticker#sufx 		equ 0
		_y_sticker#sufx 		equ 0
		_cx_sticker#sufx 		equ 0
		_cy_sticker#sufx 		equ 0
	\}
	forward
	match name:val, args \{
		name\#sufx equ val
	\}
	common
	match _name_|struct_name, _name#sufx|struct@lastname \{
		_name#sufx equ struct_name\#.\#_name_
	\}
	match _name_, _name#sufx \{
		\local _val
		_val equ 0
		match val =%, _padding#sufx\\{
			_padding#sufx equ 0
			_val equ val
		\\}
		match val =%, _padding_left#sufx\\{
			_padding_left#sufx equ (_name_\#._cx * ((val) + (_val)) / 100)
		rept 0\\{\\} rept 1\\{
			_padding_left#sufx equ (_padding_left#sufx + (_name_\#._cx * (_val) / 100))
		\\}
		match val =%, _padding_right#sufx\\{
			_padding_right#sufx equ (_name_\#._cx * ((val) + (_val)) / 100)
		rept 0\\{\\} rept 1\\{
			_padding_right#sufx equ (_padding_right#sufx + (_name_\#._cx * (_val) / 100))
		\\}
		match val =%, _padding_top#sufx\\{
			_padding_top#sufx equ (_name_\#._cy * ((val) + (_val)) / 100)
		rept 0\\{\\} rept 1\\{
			_padding_top#sufx equ (_padding_top#sufx + (_name_\#._cy * (_val) / 100))
		\\}
		match val =%, _padding_bottom#sufx\\{
			_padding_bottom#sufx equ (_name_\#._cy * ((val) + (_val)) / 100)
		rept 0\\{\\} rept 1\\{
			_padding_bottom#sufx equ (_padding_bottom#sufx + (_name_\#._cy * (_val) / 100))
		\\}

		match val =%, _space_between#sufx\\{
			match =vertical, _orientation#sufx\\\{
		 		_space_between#sufx equ (_name_\#._cy * (val) / 100)
			\\\}
			match =horizontal, _orientation#sufx\\\{
		 		_space_between#sufx equ (_name_\#._cx * (val) / 100)
		 	\\\}
		\\}
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
		rept 0\\{\\} match name_parent|sufx_parent|_alignment, last@layout|last@sufx|_align#sufx \\{
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
			__cx = name\#._elems_coord_counter + (_padding\#sufx) * 2 + (_padding_left\#sufx) + (_padding_right\#sufx)
			__cy = name\#._elems_coord_max + (_padding\#sufx) * 2 + (_padding_top\#sufx) + (_padding_bottom\#sufx)
		\\}
		match =content, _cx\#sufx\\{
			_cx\#sufx equ __cx
		\\}
		name\#._cx = _cx\#sufx

		match =vertical, _orientation\#sufx \\{
			__cy = name\#._elems_coord_counter + (_padding\#sufx) * 2 + (_padding_top\#sufx) + (_padding_bottom\#sufx)
			__cx = name\#._elems_coord_max + (_padding\#sufx) * 2 + (_padding_left\#sufx) + (_padding_right\#sufx)
		\\}
		match =content, _cy\#sufx\\{
			_cy\#sufx equ __cy
		\\}
		name\#._cy = _cy\#sufx

		name\#._rx = name\#._x + name\#._cx
		name\#._ry = name\#._y + name\#._cy
	\}
	restore last@layout, last@sufx
}