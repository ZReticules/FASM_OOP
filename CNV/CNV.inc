@module CNV

	struct Divq
		reminder 	dq ?
		result 		dq ?
	ends

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
		udivq		dm this
		ui32sqrt	dm this
		ui64sqrt 	dm this:inline
		db ?
	ends

	include "CNV.asm"

	struct Random
		__seed 	dd ?

		new 	dm this:inline
		next 	dm this:inline
	ends

	struct Random64
		__seed 	dq ?

		new 	dm this:inline
		next 	dm this:inline
	ends

	include "Random.asm"

@endm