@module CONTROL

	include "control_specialfields.inc"

	struct ScaleMode
		x 	db ?
		y 	db ?
		rx 	db ?
		ry 	db ? 
	ends

	struct DLG_CONTROL_SERVICE_DATA
		baseRect		RECT
		lpEvents		dptr ?
		lpNotifications	dptr ?
		lpHooks 		dptr ?
		scaleMode		ScaleMode
	ends

	struct CONTROL WINAPI_WINDOW
		ID				dd ?
		sData 			DLG_CONTROL_SERVICE_DATA
		
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
