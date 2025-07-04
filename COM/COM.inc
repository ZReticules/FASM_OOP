@module COM
	
	importlib ole32,\
	       CoInitialize,\
	       CoUninitialize,\
	       CoCreateInstance

	include "GUID.inc"

	struct IUnknown @interface
		const @imported equ 1 

	    QueryInterface	dm ?:virtual/std ; HRESULT QueryInterface(this, REFIID riid, void **ppvObject)
	    AddRef        	dm ?:virtual/std ; ULONG   AddRef(this)
	    Release       	dm ?:virtual/std ; ULONG   Release(this)
	ends

	IID_IUnknown @const GUID 0x00000000,0x0000,0x0000,<0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46>

	; @at_start @call [CoInitialize](0)
	
	; @at_end int3 
	; @at_end	@call [CoUninitialize](0)

@endm
