__architecture fix x86
NONE fix

include "PreprocFunc.inc"
include "Constructions\High-level.inc"
include "Constructions\Low-level.inc"
include "ARCHITECTURE_SPECIFICS/x86/CONSTANTS.inc"
include "ARCHITECTURE_SPECIFICS/x86/CALLING_CONVENTIONS.inc"
include "ARCHITECTURE_SPECIFICS/x86/MULTIARCH_MACROS.inc"
include "ARCHITECTURE_SPECIFICS/x86/new_call32.inc"
include "ARCHITECTURE_SPECIFICS/x86/new_proc32.inc"

match , NO_STDHEAD {rept 0{}rept 1{
	include "STDHEAD.inc"
}

include "win32a.inc"
include "redef_std.inc"
include "new_proc.inc"

include "FASM_OOP\FASM_OOP.inc"
include "importlib.inc"
include "TLS.inc"

include "cstr\cstr.inc"

macro @arch_include fname, ext=".asm"{
	match _arch, __architecture\{
		match _str, fname \# \`_arch \# ext\\{
			include _str
		\\}
	\}
}
include "CNV\CNV.inc"