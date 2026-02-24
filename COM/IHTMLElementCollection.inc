@module IHTMLElementCollection

include "IDispatch.inc"

IID_IHTMLElementCollection @const GUID \
    0x3050F21F,0x98B5,0x11CF,<0xBB,0x82,0x00,0xAA,0x00,0xBD,0xCE,0x0B>

struct IHTMLElementCollection IDispatch
	toString		dm ?:virtual/std ; HRESULT toString(this, BSTR *String)
	put_length		dm ?:virtual/std ; HRESULT put_length(this, LONG v)
	get_length		dm ?:virtual/std ; HRESULT get_length(this, LONG *p)
	get__newEnum	dm ?:virtual/std ; HRESULT get__newEnum(this, IUnknown **ppEnum)
	item			dm ?:virtual/std ; HRESULT item(this, VARIANT name, VARIANT index, IDispatch **ppDisp)
	tags			dm ?:virtual/std ; HRESULT tags(this, VARIANT tagName, IDispatch **ppDisp)
ends

@endm
