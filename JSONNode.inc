@module JSONNode
	include "ParsedDouble.inc"
	include "SimpleDict.inc"
	include "IterateUtf8.inc"

	JSONTypes enum \
		number,\
		string,\
		bool,\
		array,\
		object,\
		null,\
		parse_string

	struct JSONNode_unaligned
		union
			struct
				subNodes		rptr 1
				selectedNode	rptr 1
			ends
			struct
				value rq 1
			ends
			struct
				lpStr 	rptr 1
				len 	rd 1
				isConst rb 1
			ends
		ends
		jType rb 1

		const ARRAY_DEFAULT_SIZE = 16
	ends

	struct JSONFlatArray
		capacity 	dd ?
		len 		dd ?
		data 		rb 0
	ends

	struct JSONNode JSONNode_unaligned
		rb (pointer.size - (sizeof.JSONNode_unaligned mod pointer.size)) mod pointer.size

		make 	dm this
		select 	dm this
		reset 	dm this
		append 	dm this
		forEach dm this
		getLen 	dm this

		unmake 	dm this
	ends

	; %display_num sizeof.JSONNode, 10


	JSONRetCodes enum \
		success,\
		unusedType,\
		unknownType,\
		invalidKey

	include "JSONNode\JSONNode.fasm"
@endm
