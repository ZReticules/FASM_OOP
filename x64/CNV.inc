@module CNV

	struct CNV
		; getLine 	dm this
		parseCMD 	dm this
		alloc  		dm this:inline
		realloc 	dm this:inline
		free  		dm this:inline
		fill  		dm this:inline
		strlen		dm this:inline
		strmov  	dm this:inline
		BMPFromFile	dm this:inline
		BMPToFile	dm this
		BMP2File	dm this:inline
		db ?
	ends

	include "CNV.asm"

@endm