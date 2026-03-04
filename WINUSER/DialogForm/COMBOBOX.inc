struct COMBOBOX CONTROL
	struct 
		cEdit 			EDIT
		cListBox		LISTBOX
	ends
	
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
	getItemString	dm this

	; setItemText		dm this
	initSubControl	dm this
	initListBox		dm this
	initEdit		dm this
	
	const CType equ "ComboBox"
ends

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
