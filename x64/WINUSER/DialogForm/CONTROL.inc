@module CONTROL

	struct CONTROL
		hWnd			dq ?
		ID				dw ?
		bgColorBrush 	dq 0

		setBgColor		dm DIALOGFORM.setBgColor
		getTextLen 		dm DIALOGFORM.getTextLen:inline
		getText 		dm DIALOGFORM.getText:inline
		setText 		dm DIALOGFORM.setText:inline
		setEnable		dm DIALOGFORM.setEnable:inline
		setVisible		dm DIALOGFORM.setVisible:inline
		setFocus		dm DIALOGFORM.setFocus:inline
		hideCur 		dm this:inline
		setTheme 		dm this:inline
		getClientSize	dm this:inline
		getWindowSize 	dm this:inline
	ends

	include "CONTROL.asm"

	struct _DLG_SPECIAL_CONTROL CONTROL
		bkColor 		dd 0xFFFFFF
		txColor			dd 0x000000
	ends

	struct BUTTON CONTROL
		const CType equ "Button"
	ends

	struct LISTBOX _DLG_SPECIAL_CONTROL
		const CType equ "Listbox"
	ends

	struct TABCONTROL CONTROL
		const CType equ "SysTabControl32"
	ends

	include "STATIC.inc"
	include "EDIT.inc"
	include "COMBOBOX.inc"

@endm