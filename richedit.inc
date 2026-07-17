@module RichEdit

include "DialogForm.inc"
include "equates/richedit.inc"

importlib riched20,\
	CreateTextServicesA="CreateTextServices"

importlib msftedit,\
	CreateTextServicesW="CreateTextServices"

struct __RICHEDIT EDIT
	setBgColor 		dm this:inline
	setTextColor 	dm this:inline
	enableStyle 	dm this:inline
	disableStyle 	dm this:inline
ends

struct RICHEDITW __RICHEDIT
	const CType equ MSFTEDIT_CLASS
ends

struct RICHEDITA __RICHEDIT
	const CType equ RICHEDIT_CLASSW
ends

local _used

if used RICHEDITW.used
_used = CreateTextServicesW
end if

if used RICHEDITA.used
_used = CreateTextServicesA
end if

ES_EX_ALLOWEOL_CR              = 0x00000001
ES_EX_ALLOWEOL_LF              = 0x00000002
ES_EX_ALLOWEOL_ALL             = (ES_EX_ALLOWEOL_CR or ES_EX_ALLOWEOL_LF)

ES_EX_CONVERT_EOL_ON_PASTE     = 0x00000004

ES_EX_ZOOMABLE                 = 0x00000010

include "richedit.asm"

@endm
