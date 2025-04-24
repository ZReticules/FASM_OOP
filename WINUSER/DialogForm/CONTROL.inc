@module CONTROL

	include "control_specialfields.inc"

	struct DLG_CONTROL_SPECTIAL_DATA
		baseRect		RECT ?, ?, ?, ?
		lpEvents		dptr ?
		lpNotifications	dptr ?
		lpHooks 		dptr ?
	ends

	struct CONTROL
		hWnd			dptr ?
		ID				dw ?
		sData 			DLG_CONTROL_SPECTIAL_DATA
		
		getTextLen 		dm DIALOGFORM.getTextLen:inline
		getText 		dm DIALOGFORM.getText:inline
		setText 		dm DIALOGFORM.setText:inline
		setEnabled		dm DIALOGFORM.setEnabled:inline
		setVisible		dm DIALOGFORM.setVisible:inline
		setFocus		dm DIALOGFORM.setFocus:inline
		getString		dm DIALOGFORM.getString
		hideCur 		dm this:inline
		setTheme 		dm this:inline
		getClientSize	dm this:inline
		getWindowSize 	dm this:inline
	ends

	include "CONTROL.asm"

	struct BUTTON CONTROL
		const CType equ "Button"
	ends

	struct LISTBOX CONTROL
		const CType equ "Listbox"
	ends

	struct TABCONTROL CONTROL
		const CType equ "SysTabControl32"
	ends

	UDM_SETRANGE32 = 1135

	struct UPDOWN CONTROL
		const CType equ "msctls_updown32"
	ends

	include "STATIC.inc"
	include "EDIT.inc"
	include "COMBOBOX.inc"

@endm