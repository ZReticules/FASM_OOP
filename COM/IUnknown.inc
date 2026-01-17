@module IUnknown

include "COM.inc"

struct IUnknown @interface
	const @abstract equ 1 
	const @WIN_COM equ 1

    QueryInterface	dm ?:virtual/std ; HRESULT QueryInterface(this, REFIID riid, void **ppvObject)
    AddRef        	dm ?:virtual/std ; ULONG   AddRef(this)
    Release       	dm ?:virtual/std ; ULONG   Release(this)
ends

IID_IUnknown @const GUID 0x00000000,0x0000,0x0000,<0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46>

if used IUnknown_s.used
	struct IUnknown_s IUnknown
		const @abstract equ 0
		const @WIN_COM equ 0
		
		QueryInterface 	override this
		AddRef 			override this
		Release 		override this

	    refCount 	dd 1
	ends

	include "IUnknown.fasm"
end if

@endm
