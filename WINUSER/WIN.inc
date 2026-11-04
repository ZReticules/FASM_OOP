@module WIN

importlib kernel32,\
	AddAtomA,\
	DeleteAtom

importlib user32,\
	GetPropA,\
	SetPropA,\
	RemovePropA

struct WIN
	setPtr 		dm this:inline	; (handle, value) -> void
	getPtr 		dm this:inline	; (handle) -> ptr
	removePtr 	dm this:inline	; (handle) -> void
	db ?
ends

bss WIN.atom:POINTER
WIN.__usedProp = 0

macro WIN.at_start\{
	if used WIN.__usedProp
		$call [WIN.atom] = [AddAtomA](<const db "FASM_OOP_WinPtr", 0>)
	end if
\}

macro WIN.at_end\{
	if used WIN.__usedProp
		$call [DeleteAtom]([WIN.atom])
	end if
\}

@at_start WIN.at_start
@at_end WIN.at_end

macro WIN.setPtr handle, vPtr\{
	WIN.__useProp = WIN.__usedProp
	$call [SetPropA](handle, [WIN.atom], vPtr)
\}

macro WIN.getPtr handle\{
	WIN.__useProp = WIN.__usedProp
	$call [GetPropA](handle, [WIN.atom])
\}

macro WIN.removePtr handle\{
	WIN.__useProp = WIN.__usedProp
	$call [RemovePropA](handle, [WIN.atom])
\}

@endm
