struc dxw [val] {
    match , @struct \{ 
    	common
    	field@struct equ struct@aligner, ., dxw, <val> 
	\}
    match no, @struct \{ 
		common
		\local cntr, val_def
		label . XWORD
        cntr = 1
        
        forward
        val_def equ val
        match _mp1 =dup _mp2, val_def\\{
        	cntr = _mp1
        	match (_mp3), _mp2\\\{
        		val_def equ _mp3
        	rept 0\\\{\\\} rept 1\\\{
        		val_def equ _mp2
    		\\\}
    	\\}
		match high:low, val_def\\{
			times cntr dq low, high
		rept 0\\{\\}rept 1\\{
			times cntr dq val_def, 0
		\\}
		common
    \}
}

macro dxw [val] {
    match , @struct \{ 
        \local anonymous
        define field@struct anonymous, dxw, <values> 
    \}
    match no, @struct \\\{
		common
		\local cntr, val_def
		label . XWORD
        cntr = 1
        
        forward
        val_def equ val
        match _mp1 =dup _mp2, val_def\\{
        	cntr = _mp1
        	match (_mp3), _mp2\\\{
        		val_def equ _mp3
        	rept 0\\\{\\\} rept 1\\\{
        		val_def equ _mp2
    		\\\}
    	\\}
		match high:low, val_def\\{
			times cntr dq low, high
		rept 0\\{\\}rept 1\\{
			match =?, val_def\\\{
				times cntr dq ?, ?
			rept 0\\\{\\\}rept 1\\\{
				times cntr dq val_def, 0
			\\\}
		\\}
    \\\} 
}

struc rxw cntr {
    match , @struct \{ 
    	common
        define field@struct ., dxw, cntr dup(?)
    \}
    match no, @struct \{ 
		label . XWORD
        rq 2 * cntr
    \}
}

macro rxw cntr {
    rq 2 * cntr
}

alignof.dxw = 16
sizeof.dxw = 16
alignof.rxw = 16
sizeof.rxw = 16

macro gen_sizeof [suff, size]{
	sizeof.#d#suff = size
	sizeof.#r#suff = size
	alignof.#d#suff = 1 shl (bsr size + (1 - (1 shr (size and (1 shl bsr size - 1)))))
	alignof.#r#suff = alignof.#d#suff
}

gen_sizeof b, 1, w, 2, u, 2, d, 4, p, 6, f, 6, q, 8, t, 10


macro c_aligner result, alignof, type, base, current{
	if ~definite alignof.#type
		alignof.#type = 1
	end if
	result = (alignof.#type - ((current - base) mod alignof.#type)) * (1 - (1 shr ((current - base) mod alignof.#type)))
	alignof = alignof.#type
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
		restore field@struct
		field@struct equ child,fields@\#parent 
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
		match name=,fields,fields@struct \\{ 
			restore @struct
			make@struct name, fields
			define fields@\\#name fields 
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
                define field@struct struct@aligner, ., name, <values> 
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
                define field@struct struct@aligner, anonymous, name, <values> 
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
    		match =struct _mp1 _mp2, var _any1\\\{
				\\\local aligner
				macro aligner any&\\\\{
					alignas_aligner real_align, any
				\\\\}
    			struct@aligner equ aligner
    			alignof.\\\#_mp1 = real_align
    			var
    			restore struct@aligner
			rept 0\\\{\\\}rept 1\\\{
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