@module COM
	
	importlib ole32,\
	       CoInitialize,\
	       CoUninitialize,\
	       CoCreateInstance

	include "GUID.inc"

	struct IUnknown @interface
		const @abstract equ 1 

	    QueryInterface	dm ?:virtual/std ; HRESULT QueryInterface(this, REFIID riid, void **ppvObject)
	    AddRef        	dm ?:virtual/std ; ULONG   AddRef(this)
	    Release       	dm ?:virtual/std ; ULONG   Release(this)
	ends

	IID_IUnknown @const GUID 0x00000000,0x0000,0x0000,<0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46>
	
	struct DECIMAL
	    wReserved dw ?
	    union
	        struct
	            scale db ?
	            sign db ?
	        ends
	        signscale dw ?
	    ends
	    Hi32 dd ?
	    union
	        struct
	            Lo32 dd ?
	            Mid32 dd ?
	        ends
	        Lo64 dq ?
	    ends
	ends
	 
	struct VARIANT
	    union
	        struct
	            vt dw ?
	            wReserved rw 3
	            union
	                llVal dq ?
	                lVal  dd ?
	                iVal  dw ?
	                bVal  db ?
	            ends
	        ends
	        decVal DECIMAL
	    ends
	ends

	S_OK                    = 0
	VARIANT_TRUE            = 0x0000FFFF
	VARIANT_FALSE			= 0

	VT_EMPTY            = 0x0000 ; не инициализирован
	VT_NULL             = 0x0001 ; SQL-style NULL
	VT_I2               = 0x0002 ; short (16-бит int)
	VT_I4               = 0x0003 ; long (32-бит int)
	VT_R4               = 0x0004 ; float
	VT_R8               = 0x0005 ; double
	VT_CY               = 0x0006 ; currency
	VT_DATE             = 0x0007 ; date/time (OLE Automation)
	VT_BSTR             = 0x0008 ; BSTR-строка
	VT_DISPATCH         = 0x0009 ; IDispatch*
	VT_ERROR            = 0x000A ; SCODE
	VT_BOOL             = 0x000B ; VARIANT_BOOL (-1=true, 0=false)
	VT_VARIANT          = 0x000C ; вложенный VARIANT
	VT_UNKNOWN          = 0x000D ; IUnknown*
	VT_DECIMAL          = 0x000E ; Decimal (16 байт, редко используется)
	; 0x000F зарезервирован

	VT_I1               = 0x0010 ; char / signed byte
	VT_UI1              = 0x0011 ; unsigned byte
	VT_UI2              = 0x0012 ; unsigned short
	VT_UI4              = 0x0013 ; unsigned long
	VT_I8               = 0x0014 ; 64-бит signed int (LARGE_INTEGER)
	VT_UI8              = 0x0015 ; 64-бит unsigned int (ULARGE_INTEGER)
	VT_INT              = 0x0016 ; platform-specific int
	VT_UINT             = 0x0017 ; platform-specific unsigned int
	VT_VOID             = 0x0018 ; аналог void
	VT_HRESULT          = 0x0019 ; HRESULT
	VT_PTR              = 0x001A ; pointer
	VT_SAFEARRAY        = 0x001B ; SAFEARRAY*
	VT_CARRAY           = 0x001C ; C-style array
	VT_USERDEFINED      = 0x001D ; пользовательский тип
	VT_LPSTR            = 0x001E ; ANSI string (char*)
	VT_LPWSTR           = 0x001F ; Unicode string (wchar_t*)

	CLSCTX enum \
		INPROC_SERVER = 0x1,\
		INPROC_HANDLER = 0x2,\
		LOCAL_SERVER = 0x4,\
		INPROC_SERVER16 = 0x8,\
		REMOTE_SERVER = 0x10,\
		INPROC_HANDLER16 = 0x20,\
		RESERVED1 = 0x40,\
		RESERVED2 = 0x80,\
		RESERVED3 = 0x100,\
		RESERVED4 = 0x200,\
		NO_CODE_DOWNLOAD = 0x400,\
		RESERVED5 = 0x800,\
		NO_CUSTOM_MARSHAL = 0x1000,\
		ENABLE_CODE_DOWNLOAD = 0x2000,\
		NO_FAILURE_LOG = 0x4000,\
		DISABLE_AAA = 0x8000,\
		ENABLE_AAA = 0x10000,\
		FROM_DEFAULT_CONTEXT = 0x20000,\
		ACTIVATE_X86_SERVER = 0x40000,\
		ACTIVATE_32_BIT_SERVER,\
		ACTIVATE_64_BIT_SERVER = 0x80000,\
		ENABLE_CLOAKING = 0x100000,\
		APPCONTAINER = 0x400000,\
		ACTIVATE_AAA_AS_IU = 0x800000,\
		RESERVED6 = 0x1000000,\
		ACTIVATE_ARM32_SERVER = 0x2000000,\
		ALLOW_LOWER_TRUST_REGISTRATION = 0x4000000,\
		PS_DLL = 0x80000000

	typedef CLSCTX, dptr
	; @at_start @call [CoInitialize](0)
	
	; @at_end int3 
	; @at_end	@call [CoUninitialize](0)

@endm
