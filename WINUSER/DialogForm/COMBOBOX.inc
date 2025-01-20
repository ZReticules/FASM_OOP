define CB_GETCOMBOBOXINFO 164h

struct COMBOBOX CONTROL
	cEdit 			EDIT
	cListBox		LISTBOX
	bgColorBrush	dptr ?
	addItem 		dm this:inline
	delItem  		dm this:inline
	getCount  		dm this:inline
	setSelected 	dm this:inline
	getSelected 	dm this:inline
	setItemData		dm this:inline
	getItemData		dm this:inline
	clear			dm this:inline
	findItem 		dm this:inline
	findStartWith	dm this:inline
	getItemTextLen	dm this:inline
	getItemText		dm this:inline
	; setItemText		dm this
	initSubControl	dm this
	initListBox		dm this
	initEdit		dm this
	const CType equ "ComboBox"
ends

macro COMBOBOX.addItem this, lpString{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_ADDSTRING, NULL, lpString)
}

macro COMBOBOX.delItem this, idItem{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_DELETESTRING, idItem, NULL)
}

macro COMBOBOX.setSelected this, idItem{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_SETCURSEL, idItem, NULL)
}

macro COMBOBOX.getSelected this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETCURSEL, NULL, NULL)
}

macro COMBOBOX.setItemData this, idItem, dataValue{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_SETITEMDATA, idItem, dataValue)
}

macro COMBOBOX.getItemData this, idItem{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETITEMDATA, idItem, NULL)
}

macro COMBOBOX.clear this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_RESETCONTENT, NULL, NULL)
}

macro COMBOBOX.findItem this, idBefore, strLp{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_FINDSTRINGEXACT, idBefore, strLp)
}

macro COMBOBOX.getCount this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETCOUNT, NULL, NULL)
}

macro COMBOBOX.findStartWith this, idBefore, strLp{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_FINDSTRING, idBefore, strLp)
}

macro COMBOBOX.getItemTextLen this, idItem{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETLBTEXTLEN, idItem, NULL)
}

macro COMBOBOX.getItemText this, idItem, lpString{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETLBTEXT, idItem, lpString)
}

struct COMBOBOXINFO 
	cbSize		dd sizeof.COMBOBOXINFO;
	rcItem 		RECT
	rcButton 	RECT
	stateButton	dd ?
	hwndCombo	dptr ?
	hwndItem	dptr ?
	hwndList	dptr ?
ends

include "COMBOBOX.asm"