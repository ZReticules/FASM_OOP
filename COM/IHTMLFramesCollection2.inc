@module IHTMLFramesCollection2

include "IDispatch.inc"

struct IHTMLFramesCollection2 IDispatch
    item		dm ?:virtual/std ; HRESULT item(this, VARIANT* pvarIndex, VARIANT* pvarResult)
    get_length	dm ?:virtual/std ; HRESULT get_length(this, long* plength)
ends

IID_IHTMLFramesCollection2 @const GUID 0x332C4425,0x26CB,0x11D0,<0xB4,0x83,0x00,0xC0,0x4F,0xD9,0x01,0x19>

@endm
