@module String

	include "String\fstr.inc"
	include "String\fstr.fasm"
	include "ParsedDouble.inc"

	struct String
	    len     dd ?
	    union
	        __lpChars   dptr ?
	        __chars     db pointer.size dup(?)
	    ends
	    __size  dd ?					;Big Endian

	    print       dm this:inline		;()
	    getLpChars  dm this:inline		;()
	    flush       dm this:inline		;()
	    input       dm this:inline		;()
	    realloc 	dm this:inline		;(chars_count, minify=0)

	    make       		dm this			;(chars_count)
	    unmake        	dm this			;()
	    reallocParam	dm this			;(chars_count, minify:BOOLEAN)
	    fromChars   	dm this			;(lpChars, count) or (lpCstr, -1)
	    addChars    	dm this
	    cmpChars		dm this
	    fromString  	dm this			;(lpString)
	    addString   	dm this
	    cmpString 		dm this
	    fromChar    	dm this			;(byte, count)
	    addChar     	dm this
	    fromInt     	dm this			;(num, radix, isSigned)
	    addInt      	dm this
	    slice       	dm this			;(indexFrom,indexTo) 
	    sprintf     	dm this			;(fstr, ...)
	    streamRead		dm this			;(fp, delimiter, delimiter_callback, eof_callback, lParam)
	    read 			dm this			;(fstream)
	    parseInt		dm this			;(radix)

	    match =x64, __architecture\{
	    	fromInt64 	dm String.fromInt
	    	addInt64 	dm String.addInt
    	\}
	    match =x86, __architecture\{
	    	fromInt64 	dm this
	    	addInt64 	dm this
    	\}
	ends

	struct Wstring
	    len     dd ?
	    union
	        __lpChars   dptr ?
	        __chars     dw (pointer.size / 2) dup(?)
	    ends
	    __size  dd ?					;Big Endian
    ends

	include "String\String.fasm"

@endm
