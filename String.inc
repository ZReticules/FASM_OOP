@module String

	include "cstdio.inc"
	include "String\fstr.inc"
	include "String\fstr.fasm"
	include "ParsedDouble.inc"

	; struct IString @interface
	; 	const @abstract equ 1

	; 	getBytes	dm ?:virtual
	; 	freeBytes	dm ?:virtual
	; 	getWords	dm ?:virtual
	; 	freeWords 	dm ?:virtual
		
	; 	getLen 		dm ?:virtual
	; ends

	struct String; IString
		; const @abstract equ 0

		const SHORT_STRING_LEN = 24
		const SHORT_STRING_LEN_BE = WString.SHORT_STRING_LEN * 01000000h

	    len     dd ?
	    union
	        __lpChars   dptr ?
	        __chars     db String.SHORT_STRING_LEN dup(?)
	    ends
	    __size  dd ?					;Big Endian

	    print       dm this:inline		;()
	    println     dm this:inline		;()
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
	    
	    match =x64, __architecture\{
	    	fromInt64 	dm String.fromInt
	    	addInt64 	dm String.addInt
    	\}
	    match =x86, __architecture\{
	    	fromInt64 	dm this
	    	addInt64 	dm this
    	\}
	    slice       	dm this			;(indexFrom,indexTo) 
	    vsprintf 		dm this			;(fstr, va_list)
	    sprintf     	dm this			;(fstr, ...)
	    streamRead		dm this			;(fp, delimiter, delimiter_callback, eof_callback, lParam)
	    								;cdecl delimiter_callback(lpStrilgLine, lParam)
	    read 			dm this			;(fstream)
	    parseInt		dm this			;(radix) -> i64

    	countChar  		dm this			;(wVal) -> i32
    	findChar 		dm this			;(index, wVal) -> i32
	ends

	include "String\String.fasm"

	struct WString; IString
		; const @abstract equ 0

		const SHORT_STRING_LEN = 12
		const SHORT_STRING_LEN_BE = WString.SHORT_STRING_LEN * 01000000h

	    len     dd ?
	    union
	        __lpWChars   dptr ?
	        __wchars     dw WString.SHORT_STRING_LEN dup(?)
	    ends
	    __size  dd ?					;Big Endian

	    print      	dm this:inline		;()
	    println    	dm this:inline		;()
	    getLpWChars dm this:inline		;()
	    flush      	dm this:inline		;()
	    input      	dm this:inline		;()
	    realloc 	dm this:inline		;(chars_count, minify=0)

	    make       		dm this			;(chars_count)
	    unmake        	dm this			;()

	    reallocParam	dm this			;(chars_count, minify:BOOLEAN)

	    fromWChars   	dm this			;(lpWChars, count) or (lpCwstr, -1)
	    addWChars    	dm this
	    cmpWChars		dm this

	    fromWString  	dm this			;(lpWString)
	    addWString   	dm this
	    cmpWString 		dm this
	    
	    fromWChar    	dm this			;(byte, count)
	    addWChar     	dm this
	    
	    fromInt     	dm this			;(num, radix, isSigned)
	    addInt      	dm this
	    match =x64, __architecture\{
	    	fromInt64 	dm WString.fromInt
	    	addInt64 	dm WString.addInt
    	\}
	    match =x86, __architecture\{
	    	fromInt64 	dm this
	    	addInt64 	dm this
    	\}
	    
	    slice       	dm this			;(indexFrom,indexTo) 
	    vsprintf 		dm this			;(fstr, va_list)
	    sprintf     	dm this			;(fstr, ...)
	    
	    streamRead		dm this			;(fp, delimiter, delimiter_callback, eof_callback, lParam)
	    								;cdecl delimiter_callback(lpStrilgLine, lParam)
	    read 			dm this			;(fstream)
	    parseInt		dm this			;(radix) -> i64

    	countWChar  	dm this			;(wVal) -> i32
    	findWChar 		dm this			;(index, wVal) -> i32

    	findWChars 		dm this			;(pWChars, len) -> i32
    	replaceWChars	dm this			;(index:DWORD, len:DWORD, new:P_WCHARS, len:DWORD)
    ends

    include "String\WString.fasm"

@endm
