struct EDIT _DLG_SPECIAL_CONTROL
	setReadOnly	dm this:inline
	addText		dm this
	const CType equ "Edit"
ends

macro EDIT.setReadOnly this, isReadOnly{
	local _this
	inlineObj _this, this, ecx
	@call [SendMessageA]([_this+EDIT.hWnd], EM_SETREADONLY, isReadOnly)
}

include "EDIT.asm"