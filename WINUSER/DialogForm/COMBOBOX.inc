define CB_GETCOMBOBOXINFO 164h

struct COMBOBOX CONTROL
	cEdit 			EDIT
	cListBox		LISTBOX
	addItem 		dm this
	setSelected 	dm this
	getSelected 	dm this
	setItemData		dm this
	getItemData		dm this
	clear			dm this
	findItem 		dm this
	getItemTextLen	dm this
	getItemText		dm this
	; setItemText		dm this
	initSubControl	dm this
	const CType equ "ComboBox"
ends

struct COMBOBOXINFO 
  cbSize		dd sizeof.COMBOBOXINFO;
  rcItem 		RECT
  rcButton 		RECT
  stateButton	dd ?
  hwndCombo		dq ?
  hwndItem		dq ?
  hwndList		dq ?
ends

include "COMBOBOX.asm"