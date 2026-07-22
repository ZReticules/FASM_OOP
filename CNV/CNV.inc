@module CNV

	struct Divq
		result 		dq ?
		reminder 	dq ?
	ends

	struct CNV
		; getLine 	dm this
		parseCMD 			dm this
		
		alloc  				dm this:inline	; (size, flags=HEAP_ZERO_MEMORY) -> lpMem
		realloc 			dm this:inline	; (lpMem, size, flags=HEAP_ZERO_MEMORY) -> lpMem
		
		free  				dm this:inline	; (size, flags=0)
		fill  				dm this:inline	; (lpDest, lpSrc, size, flags=0)
		strlen				dm this:inline	; (lpAsciiz, flags=STRLEN_SAVE_DI)
		wstrlen 			dm this:inline	; (lpWCharsz, flags=STRLEN_SAVE_DI)
		strmov  			dm this:inline	; (lpDest, lpSrcAsciiz, flags=STRMOV_SAVE_SI or STRMOV_SAVE_DI)
		wstrmov  			dm this:inline	; (lpDest, lpSrcWCharsz, flags=STRMOV_SAVE_SI or STRMOV_SAVE_DI)
		memset 				dm this:inline	; (lpDest, lpSrc, countval*, sizeval = 1, flags = MEMSET_SAVE_RDI) ; sizeval=1, 2, 4, 8
		
		BMPFromFile			dm this:inline	; (path, x=0, y=0) -> hBmp
		BMPToFile			dm this			; (hBmp, lpFname, bitCount)
		BMP2File			dm this:inline	; (hBmp, lpFname, bitCount=24)

		uintToStrVarchar	dm this			; (lpBuf, num, radix, charSize) -> len
		intToStrVarchar		dm this			; (lpBuf, num, radix, charSize) -> len
		intToStr			dm this:inline	; (lpBuf, num, radix) -> len
		uintToStr			dm this:inline	; (lpBuf, num, radix) -> len
		intToWStr			dm this:inline	; (lpBuf, num, radix) -> len
		uintToWStr			dm this:inline	; (lpBuf, num, radix) -> len
		
		match =x64, __architecture\{
			ui64div			dm this:inline	; (dest:Divq, dividened64, divisor64)
		\}
		match =x86, __architecture\{
			ui64div			dm this			; (dest:Divq, dividened64, divisor64)
		\}
		match =x64, __architecture\{
			ui64mul			dm this:inline	; (a64, b64)
		\}
		match =x86, __architecture\{
			ui64mul			dm this			; (a64, b64)
		\}
		ui32sqrt			dm this			; (num) -> num
		ui64sqrt 			dm this			; (num64) -> num64
		ui32pow				dm this			; (base, exp) -> num
		ui64pow				dm this			; (base64, exp) -> num
		
		ui64ToStrVarchar	dm this			; (lpStr, num64, radix, charSize) -> len
		i64ToStrVarchar		dm this			; (lpStr, num64, radix, charSize) -> len
		ui64ToStr			dm this:inline	; (lpStr, num64, radix) -> len
		i64ToStr			dm this:inline	; (lpStr, num64, radix) -> len
		ui64ToWStr			dm this:inline	; (lpStr, num64, radix) -> len
		i64ToWStr			dm this:inline	; (lpStr, num64, radix) -> len

		strVarcharToI64		dm this
		strVarcharToI32		dm this
		strToI64			dm this
		strToI32			dm this
		wstrToI64			dm this
		wstrToI32			dm this

		consoleToWin1251	dm this:inline
		consoleToUtf16		dm this:inline
		consoleToUtf8		dm this:inline
		db ?
	ends

	include "CNV.asm"

	struct Random
		__seed 	dd ?

		make 	dm this:inline
		next 	dm this:inline
		unmake 	dm this:inline
	ends

	struct Random64
		__seed 	dq ?

		make 	dm this:inline
		next 	dm this:inline
		unmake 	dm this:inline
	ends

	include "Random.asm"

@endm
