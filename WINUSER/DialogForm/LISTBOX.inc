struct LISTBOX CONTROL
	const CType equ "Listbox"
	
	addItem			dm this:inline
	deleteItem		dm this:inline
	getCount		dm this:inline
	getSelected		dm this:inline
	setSelected		dm this:inline
	isSelected		dm this:inline
	getItemTextLen	dm this:inline
	getItemText		dm this:inline
	getItemData		dm this:inline
	setItemData		dm this:inline
	getTop			dm this:inline
	setTop			dm this:inline
	clear			dm this:inline
ends

include "LISTBOX.asm"
