define CB_GETCOMBOBOXINFO 164h

struct COMBOBOX CONTROL
	cEdit 			EDIT
	cListBox		LISTBOX
	addItem 		dm this:inline
	setSelected 	dm this:inline
	getSelected 	dm this:inline
	setItemData		dm this:inline
	getItemData		dm this:inline
	clear			dm this:inline
	findItem 		dm this:inline
	getItemTextLen	dm this:inline
	getItemText		dm this:inline
	; setItemText		dm this
	initSubControl	dm this
	const CType equ "ComboBox"
ends

macro COMBOBOX.addItem this, lpString{
	local _this
	inlineObj _this, this, rcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_ADDSTRING, NULL, lpString)
}

macro COMBOBOX.setSelected this, idItem{
	local _this
	inlineObj _this, this, rcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_SETCURSEL, idItem, NULL)
}

macro COMBOBOX.getSelected this{
	local _this
	inlineObj _this, this, rcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETCURSEL, NULL, NULL)
}

macro COMBOBOX.setItemData this, idItem, dataValue{
	local _this
	inlineObj _this, this, rcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_SETITEMDATA, idItem, dataValue)
}

macro COMBOBOX.getItemData this, idItem{
	local _this
	inlineObj _this, this, rcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETITEMDATA, idItem, NULL)
}

macro COMBOBOX.clear this{
	local _this
	inlineObj _this, this, rcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_RESETCONTENT, NULL, NULL)
}

macro COMBOBOX.findItem this, idBefore, strLp{
	local _this
	inlineObj _this, this, rcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_FINDSTRING, idBefore, strLp)
}

macro COMBOBOX.getItemTextLen this, idItem{
	local _this
	inlineObj _this, this, rcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETLBTEXTLEN, idItem, NULL)
}

macro COMBOBOX.getItemText this, idItem, lpString{
	local _this
	inlineObj _this, this, rcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETLBTEXT, idItem, lpString)
}

struct COMBOBOXINFO 
	cbSize		dd sizeof.COMBOBOXINFO;
	rcItem 		RECT
	rcButton 	RECT
	stateButton	dd ?
	hwndCombo	dq ?
	hwndItem	dq ?
	hwndList	dq ?
ends

include "COMBOBOX.asm"