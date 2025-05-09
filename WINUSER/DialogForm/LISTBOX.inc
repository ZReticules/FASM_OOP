struct LISTBOX CONTROL
	const CType equ "Listbox"
	
	addItem			dm this
	deleteItem		dm this
	getCount		dm this
	getSelected		dm this
	setSelected		dm this
	isSelected		dm this
	getItemTextLen	dm this
	getItemText		dm this
	getItemData		dm this
	setItemData		dm this
	getTop			dm this
	setTop			dm this
	clear			dm this
ends

include "LISTBOX.asm"
