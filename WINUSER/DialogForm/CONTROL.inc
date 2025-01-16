@module CONTROL

	struct CONTROL
		hWnd			dptr ?
		ID				dw ?
		baseRect		RECT ?, ?, ?, ?

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
		setBgColor		dm _DLG_SPECIAL_CONTROL.setBgColor
		
		bkColor 		dd 0xFFFFFF
		txColor			dd 0x000000
		bgColorBrush 	dptr 0
	ends

	proc _DLG_SPECIAL_CONTROL.setBgColor c, this, colorref
		virtObj .this:arg _DLG_SPECIAL_CONTROL at pcx from @arg1
		@sarg @arg1
		@larg pdx, @arg2
		cmp [.this.bgColorBrush], NULL
		je .emptyColor
			@sarg @arg2
			@call [DeleteObject]([.this.bgColorBrush])
			mov pdx, [colorref]
		.emptyColor:
		@call [CreateSolidBrush](pdx)
		mov pcx, [this]
		mov [.this.bgColorBrush], pax
		ret
	endp

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