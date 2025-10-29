@module IRegExp2
	include "IDispatch.inc"

	struct IRegExp2 IDispatch
		; HRESULT get_Pattern(BSTR* pPattern)
	    getPattern      dm ?:virtual/std
		; HRESULT put_Pattern(BSTR pattern)
	    setPattern      dm ?:virtual/std
		; HRESULT get_IgnoreCase(VARIANT_BOOL* pVal)
	    getIgnoreCase   dm ?:virtual/std
		; HRESULT put_IgnoreCase(VARIANT_BOOL val)
	    setIgnoreCase   dm ?:virtual/std
		; HRESULT get_Global(VARIANT_BOOL* pVal)
	    getGlobal       dm ?:virtual/std
		; HRESULT put_Global(VARIANT_BOOL val)
	    setGlobal       dm ?:virtual/std
		; HRESULT get_Multiline(VARIANT_BOOL* pVal)
	    getMultiline    dm ?:virtual/std
		; HRESULT put_Multiline(VARIANT_BOOL val)
	    setMultiline    dm ?:virtual/std
		; HRESULT Execute(BSTR input, IDispatch** ppMatches)
	    Execute         dm ?:virtual/std
		; HRESULT Test(BSTR input, VARIANT_BOOL* pResult)
	    Test            dm ?:virtual/std
		; HRESULT Replace(BSTR input, VARIANT replace, BSTR* pResult)
		; x86 @call .re->Replace([testString], VT_BSTR, 0, [replString], 0, addr resultString)
		; x64 @call .re->Replace([testString], <va_list VT_BSTR, [replString]>, addr resultString)
	    Replace         dm ?:virtual/std
	    replace  		dm this:inline
	ends

	match =x86, __architecture\{
		macro IRegExp2.replace this, srcSysString, vtType, replVal, lpResult\\{
			; \\local ..this, _this
			; inlineObj _this, this, pcx
			; virtObj ..this IRegExp2 at _this
			; @call ..this->Replace(srcSysString, vtType, 0, replVal, 0, lpResult)
			@call IRegExp2::>Replace(this, srcSysString, vtType, 0, replVal, 0, lpResult)
		\\}
	\}
	match =x64, __architecture\{
		macro IRegExp2.replace this, srcSysString, vtType, replVal, lpResult\\{
			; \\local ..this, _this
			; inlineObj _this, this, pcx
			; virtObj ..this IRegExp2 at _this
			@call IRegExp2::>Replace(this, srcSysString, <va_list vtType, replVal>, lpResult)
		\\}
	\}

	struct IMatchCollection2 IDispatch
		; HRESULT get_Item(long index, IDispatch** ppMatch)
	    getItem         dm ?:virtual/std
		; HRESULT get_Count(long* pCount)
	    getCount        dm ?:virtual/std
		; HRESULT get__NewEnum(IUnknown** ppEnum)
	    getNewEnum      dm ?:virtual/std
	ends

	struct IMatch2 IDispatch
		; HRESULT get_Value(BSTR* pValue)
	    getValue        dm ?:virtual/std
		; HRESULT get_FirstIndex(long* pFirstIndex)
	    getFirstIndex   dm ?:virtual/std
		; HRESULT get_Length(long* pLength)
	    getLength       dm ?:virtual/std
		; HRESULT get_SubMatches(IDispatch** ppSubMatches)
	    getSubMatches   dm ?:virtual/std
	ends

	struct ISubMatches IDispatch
		; HRESULT get_Item(long index, VARIANT* pItem)
	    getItem         dm ?:virtual/std
		; HRESULT get_Count(long* pCount)
	    getCount        dm ?:virtual/std
		; HRESULT get__NewEnum(IUnknown** ppEnum)
	    getNewEnum      dm ?:virtual/std
	ends

	; GUID {3F4DACB0-160D-11D2-A8E9-00104B365C9F}
	IID_IRegExp2 			@const GUID 03F4DACB0h, 0160Dh, 011D2h, <0A8h, 0E9h, 000h, 010h, 04Bh, 036h, 05Ch, 09Fh>
	 
	; GUID {3F4DACA4-160D-11D2-A8E9-00104B365C9F}
	CLSID_IRegExp 			@const GUID 03F4DACA4h, 0160Dh, 011D2h, <0A8h, 0E9h, 000h, 010h, 04Bh, 036h, 05Ch, 09Fh>

	IID_IMatchCollection2  	@const GUID 03F1238AFh, 0160Dh, 011D2h, <08Eh,086h,000h,060h,097h,0C9h,0A2h,059h>
	IID_IMatch2            	@const GUID 03F1238B0h, 0160Dh, 011D2h, <08Eh,086h,000h,060h,097h,0C9h,0A2h,059h>
	IID_ISubMatches        	@const GUID 03F1238B1h, 0160Dh, 011D2h, <08Eh,086h,000h,060h,097h,0C9h,0A2h,059h>
@endm
