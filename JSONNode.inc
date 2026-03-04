@module JSONNode
	include "ParsedDouble.inc"
	include "SimpleDict.inc"
	include "Utf8.inc"
	include "Except.inc"

	JSONTypes enum \
		NUMBER,\
		STRING,\
		BOOL,\
		ARRAY,\
		OBJECT,\
		NULL,\
		PARSE_STRING,\
		INTEGER

	struct JSONNode_unaligned
		union
			struct
				subNodes		P_SimpleDict
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
		get  	dm this
		getLen 	dm this

		unmake 	dm this
		

		__from_string dm this
	ends

	struct JSONException
		lpSmb		dptr ?
		pointIndex	dptr ?
	ends

	; %display_num sizeof.JSONNode, 10


	JSONRetCodes enum \
		SUCCESS,\
		UNUSED_TYPE,\
		UNKNOWN_TYPE,\
		INVALID_KEY,\
		FAILED

	include "JSONNode\JSONNode.fasm"
@endm
