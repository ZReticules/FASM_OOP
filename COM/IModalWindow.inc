@module IModalWindow
	include "COM.inc"

	struct IModalWindow IUnknown
	    Show dm ?:virtual/std ; HRESULT Show(this, HWND hwndOwner)
	ends

	IID_IModalWindow @const GUID 0xB4DB1657,0x70D7,0x485E,<0x8E,0x3E,0x6F,0xCB,0x5A,0x5C,0x18,0xE0>
@endm
