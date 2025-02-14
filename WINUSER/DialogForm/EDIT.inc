@module EDIT

	define EM_SETCUEBANNER 0x1501

	define EM_GETCUEBANNER 0x1502

	struct EDIT _DLG_SPECIAL_CONTROL
		setReadOnly		dm this:inline
		getLimitText	dm this:inline
		setLimitText	dm this:inline
		getPlaceholder	dm this:inline
		setPlaceholder	dm this:inline
		addText			dm this

		const CType equ "Edit"
	ends

	include "EDIT.asm"

@endm
