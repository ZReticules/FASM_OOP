@module CNV

	struct Random
		__seed dq ?

		new 	dm this:inline
		next 	dm this:inline
	ends

	include "Random.asm"

	struct CNV
		; getLine 	dm this
		parseCMD 	dm this
		alloc  		dm this:inline
		realloc 	dm this:inline
		free  		dm this:inline
		fill  		dm this:inline
		strlen		dm this:inline
		strmov  	dm this:inline
		memset 		dm this:inline
		BMPFromFile	dm this:inline
		BMPToFile	dm this
		BMP2File	dm this:inline
		intToStr	dm this
		uintToStr	dm this
		db ?
	ends

	include "CNV.asm"

@endm