@module COM
	
importlib ole32,\
       CoInitialize,\
       CoUninitialize,\
       CoCreateInstance

include "GUID.inc"

struct COM
	New dm this:inline
	rb 1
ends

.proc cdecl COM.__New(.pp_this, src, size)
	@sarg @arg1, @arg2, @arg3
	.local .p_this:POINTER
	$call [p_this] = CNV|alloc([size])
	$call CNV|fill([p_this], [src], [size])
	mov pax, [p_this]
	mov pcx, [.pp_this]
	mov pointer[pcx], pax
	mov eax, S_OK
	ret
.endp

macro COM.New lpDest, Type\{
	match =1, Type\#.@WIN_COM\\{
	    $call [CoCreateInstance](CLSID_\#Type, NULL,\
			CLSCTX.INPROC_HANDLER or CLSCTX.INPROC_SERVER or CLSCTX.LOCAL_SERVER,\
			IID_\#Type, lpDest)
	rept 0\\{\\} rept 1\\{
		$call COM.__New(lpDest, const Type, sizeof.\#Type)
	\\}
\}

typedef LCID, dd
typedef SCODE, dd

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


struct VARIANT_NAME_4
    pvRecord rptr 1;
    pRecInfo rptr 1;
ends

struct VARIANT_NAME_5
    parray rptr 1;
    pvData rptr 1;
ends

struct VARIANT_NAME_6
    pvData 		rptr 1;
    pvReserved 	rptr 1;
ends

struct VARIANT
    union
        struct
            vt rw 1
            wReserved rw 3
            union
                llVal 				rq 1
                parray 				rptr 1
                bstrVal				rptr 1
                pdispVal 			rptr 1
                lVal  				rd 1
                iVal  				rw 1
                bVal  				rb 1
                __VARIANT_NAME_4	VARIANT_NAME_4 
                __VARIANT_NAME_5	VARIANT_NAME_5 
                __VARIANT_NAME_6	VARIANT_NAME_6
            ends
        ends
        decVal DECIMAL
    ends
ends

struct EXCEPINFO
	wCode				rw 1
	wReserved			rw 1
	bstrSource			rptr 1 ; BSTR
	bstrDescription		rptr 1 ; BSTR
	bstrHelpFile		rptr 1 ; BSTR
	dwHelpContext		rd 1
	pvReserved			rptr 1;
	pfnDeferredFillIn	rptr 1;
	scode 				SCODE ?
ends

S_OK 	= 0
S_FALSE = 1

E_FAIL 			= 0x80004005
E_NOTIMPL 		= 0x80004001
E_ABORT 		= 0x80004004
E_UNEXPECTED 	= 0x8000FFFF
E_NOINTERFACE 	= 0x80004002
E_INVALIDARG 	= 0x80070057
E_OUTOFMEMORY 	= 0x8007000E
E_POINTER 		= 0x80004003
E_HANDLE 		= 0x80070006
E_ACCESSDENIED 	= 0x80070005

VS_E_PACKAGENOTLOADED 		= 0x8004D104
VS_E_PROJECTNOTLOADED 		= 0x8004D108
VS_E_WIZARDBACKBUTTONPRESS 	= 0x8004D10B
VS_E_INCOMPATIBLEDOCDATA 	= 0x8004D106
VS_E_PROJECTMIGRATIONFAILED = 0x8004D10E
VS_E_CANCELVSLAUNCH 		= 0x8004D10C
VS_E_WINDOWLOCKED 			= 0x8004D111
VS_E_SPECIFY_OTHER_SILO 	= 0x8004D112

OLECMDERR_E_FIRST 			= 0x80040100
OLECMDERR_E_NOTSUPPORTED 	= 0x80040100
OLECMDERR_E_DISABLED 		= 0x80040101
OLECMDERR_E_NOHELP 			= 0x80040102
OLECMDERR_E_CANCELED 		= 0x80040103
OLECMDERR_E_UNKNOWNGROUP 	= 0x80040104

VSM_ENTERMODAL 			= 0x00000200
VSM_EXITMODAL 			= 0x00000201
VSM_TOOLWINDOWCREATED 	= 0x00000202

VARIANT_TRUE            = 0x0000FFFF
VARIANT_FALSE			= 0

VT_EMPTY            = 0x0000 ; No value
VT_NULL             = 0x0001 ; SQL-style Null
VT_I2               = 0x0002 ; 16-bit signed int
VT_I4               = 0x0003 ; 32-bit signed int
VT_R4               = 0x0004 ; 32-bit float
VT_R8               = 0x0005 ; 64-bit float (double)
VT_CY               = 0x0006 ; Currency (scaled int64)
VT_DATE             = 0x0007 ; DATE (double days since 1899-12-30)
VT_BSTR             = 0x0008 ; OLE Automation string
VT_DISPATCH         = 0x0009 ; IDispatch*
VT_ERROR            = 0x000A ; SCODE
VT_BOOL             = 0x000B ; VARIANT_BOOL (TRUE=-1, FALSE=0)
VT_VARIANT          = 0x000C ; VARIANT (by ref)
VT_UNKNOWN          = 0x000D ; IUnknown*
VT_DECIMAL          = 0x000E ; Decimal (96-bit)

; 0x000F зарезервирован

VT_I1               = 0x0010 ; 8-bit signed int
VT_UI1              = 0x0011 ; 8-bit unsigned int
VT_UI2              = 0x0012 ; 16-bit unsigned int
VT_UI4              = 0x0013 ; 32-bit unsigned int
VT_I8               = 0x0014 ; 64-bit signed int
VT_UI8              = 0x0015 ; 64-bit unsigned int
VT_INT              = 0x0016 ; machine-dependent int
VT_UINT             = 0x0017 ; machine-dependent unsigned int
VT_VOID             = 0x0018 ; C style void
VT_HRESULT          = 0x0019 ; Standard HRESULT
VT_PTR              = 0x001A ; Pointer type
VT_SAFEARRAY        = 0x001B ; SAFEARRAY*
VT_CARRAY           = 0x001C ; C-style array
VT_USERDEFINED      = 0x001D ; User defined type
VT_LPSTR            = 0x001E ; LPSTR (ANSI)
VT_LPWSTR           = 0x001F ; LPWSTR (Unicode)

; зарезервировано: 0x0020 – 0x003F

VT_RECORD           = 0x0024 ; user defined record
VT_INT_PTR          = 0x0025 ; signed machine int pointer
VT_UINT_PTR         = 0x0026 ; unsigned machine int pointer

; специальные "доп. типы"
VT_FILETIME         = 0x0040 ; FILETIME
VT_BLOB             = 0x0041 ; Length-prefixed bytes
VT_STREAM           = 0x0042 ; IStream*
VT_STORAGE          = 0x0043 ; IStorage*
VT_STREAMED_OBJECT  = 0x0044 ; Streamed IUnknown*
VT_STORED_OBJECT    = 0x0045 ; Stored IUnknown*
VT_BLOB_OBJECT      = 0x0046 ; Blobbed IUnknown*
VT_CF               = 0x0047 ; Clipboard format
VT_CLSID            = 0x0048 ; CLSID
VT_VERSIONED_STREAM = 0x0049 ; Versioned stream

; зарезервировано 0x004A–0x0050

VT_BSTR_BLOB        = 0x0FFF ; Reserved (BSTR as blob)

; by-ref модификаторы
VT_VECTOR           = 0x1000 ; simple counted array
VT_ARRAY            = 0x2000 ; SAFEARRAY*
VT_BYREF            = 0x4000 ; pointer to value

VT_RESERVED         = 0x8000
VT_ILLEGAL          = 0xFFFF
VT_ILLEGALMASKED    = 0x0FFF
VT_TYPEMASK         = 0x0FFF

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
; @at_start $call [CoInitialize](0)

; @at_end int3 
; @at_end	$call [CoUninitialize](0)

IID_NULL @const GUID 0x00000000,0x0000,0x0000,<0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00>

@endm
