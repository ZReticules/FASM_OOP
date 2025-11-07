@module Utf8

struct Utf8
	iterate 		dm this
	unEsc 			dm this

	toSysString		dm this
	fromWCharsz		dm this
	db ?
ends

struct Utf8.IterContext
	skipBytes 	rptr 1
	smbSize 	rd 1
ends

include "Utf8\Utf8.asm"

@endm
