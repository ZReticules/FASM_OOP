@module Utf8

struct Utf8.IterContext
	skipBytes 	rptr 1
	smbSize 	rd 1
ends

struct Utf8
	iterate 		dm this ; (pChars, len, func, lParam)
							; func(pSmb, codepoint, lParam, P_Utf8.IterContext) -> continue:BOOL
	unEsc 			dm this ; (dest, src, srcLen) -> (srcLen - destLen)

	toSysString		dm this ; (lpChars, len)
	fromWCharsz		dm this ; (lpWCharsz)
	db ?
ends

include "Utf8\Utf8.asm"

@endm
