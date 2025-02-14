@module COLORED_BUTTON

	struct COLORED_BUTTON BUTTON
		const CType 	equ "Button"

		setBgColor		dm this
		unsetBgColor	dm this

		txColor			dd 0
		bgColorBrush 	dptr 0

		NM_CUSTOMDRAW notify COLORED_BUTTON_NM_CUSTOMDRAW
	ends

	include "COLORED_BUTTON.asm"
@endm