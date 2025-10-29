@module IDispatch
	include "COM.inc"

	DISP_E_PARAMNOTFOUND = -2147352572

	struct IDispatch IUnknown
	    GetTypeInfoCount	dm ?:virtual/std
	    GetTypeInfo     	dm ?:virtual/std
	    GetIDsOfNames   	dm ?:virtual/std
	    Invoke         		dm ?:virtual/std
	ends
	
	IID_IModalWindow @const GUID 0x00020400,0x0000,0x0000,<0x46,0x00,0x00,0x00,0x00,0x00,0x00,0x00>
@endm
