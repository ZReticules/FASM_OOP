importlib kernel32,\
	AddAtomA,\
	DeleteAtom

importlib user32,\
	GetPropA,\
	SetPropA,\
	RemovePropA

bss DLG.atom:POINTER
DLG.__usedProp = 0

macro DLG.at_start{
	if used DLG.__usedProp
		$call [DLG.atom] = [AddAtomA]("FASM_OOP_WinPtr")
	end if
}

macro DLG.at_end{
	if used DLG.__usedProp
		$call [DeleteAtom]([DLG.atom])
	end if
}

@at_start DLG.at_start
@at_end DLG.at_end

macro DLG.setPtr handle, vPtr{
	DLG.__useProp = DLG.__usedProp
	$call [SetPropA](handle, [DLG.atom], vPtr)
}

macro DLG.getPtr handle{
	DLG.__useProp = DLG.__usedProp
	$call [GetPropA](handle, [DLG.atom])
}

macro DLG.removePtr handle{
	DLG.__useProp = DLG.__usedProp
	$call [RemovePropA](handle, [DLG.atom])
}