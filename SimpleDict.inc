@module SimpleDict

	include "MurmurHash3.inc"
	
	struct SimpleDictKey
		key 	dptr ?
		len 	dd ?
		isConst dd ?
	ends

	struct SimpleDictItem
		union
			struct
				lpKey 	dptr ?
				keyLen 	dd ?
				isConst dd ?
			ends
			key SimpleDictKey
		ends
		hash 	dd ?
		value 	dptr ?
	ends

	struct SimpleDict
		__size 	dd ?
		len 	dd ?
		lpItems dptr ?			; SimpleDictItem[__size]

		make 		dm this		; (size)
		unmake 		dm this
		
								; key_len = -1 -> key is cstr, key_len = -2 -> key is wcstr
		set 		dm this		; (key, key_len, value, isConstKey) -> prevous_val
		get 		dm this		; (key, key_len, notFoundValue) -> value
		del			dm this		; (key, key_len) -> value
		forEach		dm this		; (func, lParam)
								; func(key: SimpleDictKey, value, lParam)
		clear 		dm this		; ()

		__findItem	dm this		; (key, key_len)

		hasItem		dm SimpleDict.__findItem
	ends

	include "SimpleDict\SimpleDict.fasm"
@endm
