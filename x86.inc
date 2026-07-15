__architecture fix x86
NONE fix

include "PreprocFunc.inc"
include "Constructions/High-level.inc"
include "Constructions/Low-level.inc"
include "ARCHITECTURE_SPECIFICS/x86/CONSTANTS.inc"
include "ARCHITECTURE_SPECIFICS/x86/CALLING_CONVENTIONS.inc"
include "ARCHITECTURE_SPECIFICS/x86/MULTIARCH_MACROS.inc"
include "ARCHITECTURE_SPECIFICS/x86/new_call32.inc"
include "ARCHITECTURE_SPECIFICS/x86/new_proc32.inc"

match , NO_STDHEAD {rept 0{}rept 1{
	include "STDHEAD.inc"
}

; include "win32a.inc"
include "std_redefs/redef_struct.inc"
include 'macro/proc32.inc'
include 'macro/com32.inc'
include 'macro/import32.inc'
include 'macro/export.inc'
include 'macro/resource.inc'

struc TCHAR [val] { 
	common 
	match any, val \{ 
		. db val \}
   	match , val \{ 
   		. db ? 
	\} 
}
sizeof.TCHAR = 1
alignof.TCHAR = 1

include 'equates/kernel32.inc'
include 'equates/user32.inc'
include 'equates/gdi32.inc'
include 'equates/comctl32.inc'
include 'equates/comdlg32.inc'
include 'equates/shell32.inc'
include 'equates/wsock32.inc'

macro api [name] { 
	if used name
	    label name dword at name#A
   end if 
}

include "std_redefs/redef_proc.inc"
include "std_redefs/new_proc.inc"

include "FASM_OOP/FASM_OOP.inc"
include "importlib.inc"
include "TLS.inc"

include "cstr/cstr.inc"

macro @arch_include fname, ext=".asm"{
	match _arch, __architecture\{
		match _str, fname \# \`_arch \# ext\\{
			include _str
		\\}
	\}
}
include "CNV/CNV.inc"