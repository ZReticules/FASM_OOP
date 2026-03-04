__architecture fix x64
NONE fix

include "PreprocFunc.inc"
include "Constructions\High-level.inc"
include "Constructions\Low-level.inc"
include "ARCHITECTURE_SPECIFICS/x64/CONSTANTS.inc"
include "ARCHITECTURE_SPECIFICS/x64/CALLING_CONVENTIONS.inc"
include "ARCHITECTURE_SPECIFICS/x64/MULTIARCH_MACROS.inc"
include "ARCHITECTURE_SPECIFICS/x64/new_call64.inc"
include "ARCHITECTURE_SPECIFICS/x64/new_proc64.inc"

match , NO_STDHEAD {rept 0{}rept 1{
	include "STDHEAD.inc"
}

include "win64a.inc"
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