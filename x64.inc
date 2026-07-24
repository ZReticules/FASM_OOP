__architecture fix x64
NONE fix

include "PreprocFunc.inc"
include "Constructions/std-types-consts.inc"
include "Constructions/High-level.inc"
include "Constructions/Low-level.inc"
include "ARCHITECTURE_SPECIFICS/x64/CONSTANTS.inc"
include "ARCHITECTURE_SPECIFICS/x64/CALLING_CONVENTIONS.inc"
include "ARCHITECTURE_SPECIFICS/x64/MULTIARCH_MACROS.inc"
include "ARCHITECTURE_SPECIFICS/x64/new_call64.inc"
include "ARCHITECTURE_SPECIFICS/x64/new_proc64.inc"

match , NO_STDHEAD {rept 0{}rept 1{
	include "STDHEAD.inc"
}

; include "win64a.inc"
include "std_redefs/redef_struct.inc"
include 'macro/proc64.inc'
include 'macro/com64.inc'
include 'macro/import64.inc'
include 'macro/export.inc'
include 'macro/resource.inc'
include 'macro/if.inc'

struc TCHAR [val] { 
	common 
	match any, val \{ 
		. db val 
	\}
   	match , val \{ 
   		. db ? 
	\} 
}
sizeof.TCHAR = 1
alignof.TCHAR = 1

include 'equates/kernel64.inc'
include 'equates/user64.inc'
include 'equates/gdi64.inc'
include 'equates/comctl64.inc'
include 'equates/comdlg64.inc'
include 'equates/shell64.inc'

macro api [name] { 
	if used name
	    label name dword at name#A
   end if 
}

include "std_redefs/redef_proc.inc"
include "new_proc.inc"

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