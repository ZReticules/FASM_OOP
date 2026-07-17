define EM_SETCUEBANNER 0x1501

define EM_GETCUEBANNER 0x1502

struct EDIT CONTROL
	const LIMIT_TEXT = 65535

	setReadOnly			dm this:inline
	getLimitText		dm this:inline
	setLimitText		dm this:inline
	getPlaceholder		dm this:inline
	setPlaceholder		dm this:inline
	getSelected			dm this:inline
	getSelectedW		dm this:inline
	setSelected			dm this:inline
	setSelectedW		dm this:inline
	replaceSelected		dm this:inline
	replaceSelectedW	dm this:inline
	clearUndo 			dm this:inline
	addText				dm this
	addTextW			dm this

	const CType equ "Edit"
ends

include "EDIT.asm"
