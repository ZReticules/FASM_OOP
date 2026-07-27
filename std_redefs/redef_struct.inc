aligner@pack_align equ 0x7FFFFFFFFFFFFFFF
macro c_aligner result, alignof, type, base, current{
	; if ~definite alignof.#type
	; 	alignof.#type = 1
	; end if
	if alignof.#type <= aligner@pack_align
		result = (alignof.#type - ((current - base) mod alignof.#type)) * (1 - (1 shr ((current - base) mod alignof.#type)))
		alignof = alignof.#type
	else
		result = (aligner@pack_align - ((current - base) mod aligner@pack_align)) * (1 - (1 shr ((current - base) mod aligner@pack_align)))
		alignof = aligner@pack_align
	end if
}

macro alignas_aligner val, result, alignof, type, base, current{
	result = (val - ((current - base) mod val)) * (1 - (1 shr ((current - base) mod val)))
	alignof = val
}

macro default_aligner result, alignof, any&{
	result = 0
	alignof = 1
}

struct@aligner equ default_aligner

; Macroinstructions for defining data structures
macro struct name { 
	virtual at 0
	define @struct
	field@struct equ name
	struct@my_aligner equ struct@aligner
	match child parent, name \{ 
		child\#@defined equ 1
		child\#@parent equ parent
		child\#@is_struct equ 1
		restore field@struct
		field@struct equ child
		; field@struct equ child,fields@\#parent 
	rept 0\{\}rept 1\{
		name#@defined equ 1
		name#@parent equ
		name#@is_struct equ 1
	\}
	sub@struct equ
	irp suff, b, w, u, d, p, f, q, t\{
		struc d\#suff [val] \\{
			\\common
			field@struct equ struct@aligner, ., d\#suff, <val> 
		\\}
		struc r\#suff count \\{
			field@struct equ struct@aligner, ., d\#suff, count dup (?)
		\\}
		macro d\#suff [val] \\{
			\\common
			\\local anonymous
			field@struct equ struct@aligner, anonymous, d\#suff, <val> 
		\\}
		macro r\#suff count \\{
			\\local anonymous
			field@struct equ struct@aligner, anonymous, d\#suff, count dup (?)
		\\}
	\}
	macro union \{ 
		field@struct equ struct@aligner,,union,<
		sub@struct equ union 
	\}
	macro struct \{ 
		field@struct equ struct@aligner,,substruct,<
		sub@struct equ substruct 
	\} 
}

macro ends { 
	match , sub@struct \{ 
		restruc db,dw,du,dd,dp,dq,dt
		restruc rb,rw,rd,rp,rq,rt
		purge db,dw,du,dd,dp,dq,dt
		purge rb,rw,rd,rp,rq,rt
		purge union, struct
		irpv fields, field@struct \\{ 
			restore field@struct
			\\common 
			define fields@struct fields 
		\\}
		match name tail,fields@struct, \\{ 
			if $
				display 'Error: definition of ',\\`name,' contains illegal instructions.',0Dh,0Ah
				err
			end if 
		\\}
		match name=,aligner1=,fields, fields@struct \\{ 
			restore @struct
			match parent, name\\#@parent\\\{
				\\\local aligner
				macro aligner result, alignof, type, base, current\\\\{
					\\\\local res1, res2, aof1, aof2
					alignas_aligner alignof.\\\#parent, res1, aof1, type, base, current
					aligner1 res2, aof2, type, base, current
					if res1 > res2
						result = res1
						alignof = aof1
					else
						result = res2
						alignof = aof2
					end if
				\\\\}
				match parent_fields, fields@\\\#parent\\\\{
					make@struct name, parent_fields, aligner, fields
					define fields@\\#name parent_fields, aligner, fields
				\\\\}
			rept 0\\\{\\\}rept 1\\\{
				make@struct name, aligner1, fields
				define fields@\\#name aligner1, fields 
			\\\}
		rept 0\\{\\}match name, fields@struct\\{
			restore @struct
			match parent, name\\#@parent\\\{
				match parent_fields, fields@\\\#parent\\\\{
					make@struct name, parent_fields
					define fields@\\#name parent_fields
				\\\\}
			\\\}
		\\}
		end virtual 
	\}
   	match any, sub@struct \{ 
   		tmp@struct equ field@struct
		restore field@struct
		field@struct equ tmp@struct> 
	\}
	restore sub@struct 
}

; redefine substructs/subunions and usual fields
macro make@struct name, [aligner, field, type, def] { 
	common
	local fields_list
	fields_list equ name
	forward
	local sub
	match , field \{
		make@substruct aligner, <type>, name, sub def
		fields_list equ fields_list, aligner, ., sub, 
	\}
    match any, field \{ 
    	fields_list equ fields_list, aligner, .#field, type, <def> 
	\}
   	common
    match fields, fields_list \{ 
    	define@struct fields 
	\} 
}
; recursive redefinition for substructs/unions
macro enable@substruct { 
	macro make@substruct my_aligner, substruct, parent, name, [aligner, field, type, def] \{ 
		\common
		\local fields_list
		fields_list equ my_aligner, parent, name
       	\forward
		\local sub
		match , field \\{ 
			match any, type \\\{ 
				enable@substruct
		       	make@substruct aligner, <type>, parent, sub def
		       	purge make@substruct
		       	fields_list equ fields_list, aligner, ., sub, 
	       	\\\} 
       	\\}
		match any, field \\{ 
			fields_list equ fields_list, aligner, .\#field, type, <def> 
		\\}
       	\common
		match fields, fields_list \\{ 
			; define union or substruct
			define@\#substruct fields
		\\} 
	\} 
}

enable@substruct

macro define@struct name, [aligner, field, type, def]{ 
    common
    
    virtual
        db `name
        load initial@struct byte from 0
        if initial@struct = '.'
            display 'Error: name of structure should not begin with a dot.',0Dh,0Ah
            err
        end if
    end virtual

    local list
    local main_alignment, align_val, align_of
    local my_aligner
    main_alignment = 1
    list equ

    forward
	aligner align_val, align_of, type, 0, $
    if align_of > main_alignment
    	main_alignment = align_of
    end if
	rb align_val
    if ~ field eq .
    	name#field#@align_bytes = align_val
        name#field type def
        sizeof.#name#field = $ - name#field
    else
    	type#@align_bytes = align_val
        label name#.#type
        rb sizeof.#type
    end if
    local value
    match any, list \{ 
        list equ list, 
    \}
    list equ list <value>
    
    common
    match my_aligner, struct@my_aligner\{
		my_aligner align_val, alignof, name, 0, $
    \}
    if ~ definite alignof.#name
    	alignof.#name = main_alignment
    end if
    rb align_val
    sizeof.#name = $
    restruc name
    match values, list \{
        struc name value \\{ 
            \\local \\..base
            match , @struct \\\{ 
                field@struct equ struct@aligner, ., name, <values> 
            \\\}
            match no, @struct \\\{ 
                label \\..base
                
                forward
			    if field eq .
					rb type#@align_bytes
			    else
			    	rb name#field#@align_bytes
			    end if
                match , value \\\\{ 
                    field type def 
                \\\\}
                match any, value \\\\{
                	\\\\local ..start
                    field type value
    			    if ~ field eq .
						rb sizeof.#name#field - ($-field)
    			    end if 
                \\\\}
                
                common 
    			rb align_val
                label . at \\..base 
            \\\}
        \\}
        macro name value \\{
            match , @struct \\\{ 
                \\\local anonymous
                field@struct equ struct@aligner, anonymous, name, <values> 
            \\\}
            match no, @struct \\\{
                forward
			    if field eq .
					rb type#@align_bytes
			    else
			    	rb name#field#@align_bytes
			    end if
                match , value \\\\{ 
                    type def 
                \\\\}
                match any, value \\\\{ 
                    \\\\local ..field
                    ..field = $
                    type value
                    if ~ field eq .
                        rb sizeof.#name#field - ($-..field)
                    end if 
                \\\\}
                
                common 
    			rb align_val
            \\\} 
        \\} 
   \} 
}

macro define@union my_aligner, parent, name, [aligner, field, type, def] { 
	common
	local main_alignment, align_val, alignof
	main_alignment = 1
    virtual at parent#.#name
	   	
	   	forward
		my_aligner align_val, alignof, type, parent#.#name, $
	   	if alignof > main_alignment
	   		main_alignment = alignof
	   	end if
	    if ~ field eq .
	     	virtual at parent#.#name
	      		parent#field type def
	      		sizeof.#parent#field = $ - parent#field
	 		end virtual
			if sizeof.#parent#field > $ - parent#.#name
				rb sizeof.#parent#field - ($ - parent#.#name)
			end if
	    else
	     	virtual at parent#.#name
	      		label parent#.#type
	      		type def
	     	end virtual
			label name#.#type at parent#.#name
			if sizeof.#type > $ - parent#.#name
				rb sizeof.#type - ($ - parent#.#name)
			end if
	    end if
	   	
	   	common
	   	if ~definite alignof.#name
	   		alignof.#name = main_alignment
	   	end if
	    sizeof.#name = $ - parent#.#name
    end virtual
    struc name [value] \{ 
    	\common
    	label .\#name
    	last@union equ
   		
   		forward
	    match any, last@union \\{ 
	    	virtual at .\#name
	       		field type def
		    end virtual 
	    \\}
    	match , last@union \\{ 
    		match , value \\\{ 
    			field type def
    		\\\}
		   	match any, value \\\{
		   		field type value
	   		\\\}
   		\\}
    	last@union equ field
   		
   		common
   		rb sizeof.#name - ($ - .\#name)
	\}
    macro name [value] \{
    	\common 
    	\local ..anonymous
	  	..anonymous name value 
	\} 
}

macro define@substruct my_aligner, parent, name, [aligner, field, type, def] {
	common
	local align_val, alignof, main_alignment
	main_alignment = 1
	virtual at parent#.#name
		
		forward
		aligner align_val, alignof, type, parent#.#name, $
	   	if alignof > main_alignment
	   		main_alignment = alignof
	   	end if
	    rb align_val
		local value

		if ~ field eq .
			parent#field#@align_bytes = align_val
			parent#field type def
			sizeof.#parent#field = $ - parent#field
		else
			type#@align_bytes = align_val
			label parent#.#type
			rb sizeof.#type
		end if
		
		common
		if ~definite alignof.#name
	    	alignof.#name = main_alignment
	    end if
		my_aligner align_val, alignof, name, 0, $
	    rb align_val
		
		sizeof.#name = $ - parent#.#name
	end virtual
	struc name value \{
		\common
		label .\#name
		
		forward
		match , value \\{
			field type def
		\\}
		match any, value \\{
			field type value
			if ~ field eq .
				rb sizeof.#parent#field - ($-field)
			end if
		\\}
		
		common
	    rb align_val
	\}
	macro name value \{
		\local ..anonymous
		..anonymous name
	\}
}

macro alignas val&{
	match (align_val)var, val\{
		\local real_align
		if defined alignof.\#align_val
			real_align = alignof.\#align_val
		else
			real_align = align_val
		end if
    	match no, @struct \\{
    		; align
    		\\local matched
    		match =struct _mp1 _mp2, var _any1\\\{
				\\\local aligner
				macro aligner any&\\\\{
					alignas_aligner real_align, any
				\\\\}
    			struct@aligner equ aligner
    			alignof.\\\#_mp1 = real_align
    			var
    			restore struct@aligner
    			define matched
			\\\}
    		match =typedef _mp1=, _mp2, var _any1\\\{
				\\\local aligner
				macro aligner any&\\\\{
					alignas_aligner real_align, any
				\\\\}
    			struct@aligner equ aligner
    			alignof.\\\#_mp1 = real_align
    			var
    			restore struct@aligner
    			define matched
			\\\}
			match =matched, matched\\\{
				align real_align
			\\\} 
    	rept 0\\{\\}rept 1\\{
			\\local aligner
			macro aligner any&\\\{
				alignas_aligner real_align, any
			\\\}
    		struct@aligner equ aligner
    		var
    		restore struct@aligner
		\\}
	\}
}

macro .alignmode_cdecl{
	struct@aligner equ c_aligner
}

macro .alignmode_default{
	struct@aligner equ default_aligner
}

macro .alignmode_previous{
	restore struct@aligner
}

macro .alignmode_set_pack val{
	aligner@pack_align equ val
}

macro .alignmode_restore_pack{
	restore aligner@pack_align
}
