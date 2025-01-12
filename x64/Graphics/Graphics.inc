@module Graphics

	struct Graphics
		const BMP 	= (Graphics.__hBMP_prev - Graphics.__hArr) / 8
		const Pen 	= (Graphics.__hPen_prev - Graphics.__hArr) / 8
		const Brush = (Graphics.__hBrush_prev - Graphics.__hArr) / 8
		const Rgn 	= (Graphics.__hRgn_prev - Graphics.__hArr) / 8
		const Font 	= (Graphics.__hFont_prev - Graphics.__hArr) / 8

		hDC				dq ?
		union
			struct
				__hBMP_prev		dq 0
				__hPen_prev		dq 0
				__hBrush_prev	dq 0
				__hRgn_prev		dq 0
				__hFont_prev	dq 0
			ends
			__hArr		dq 5 dup(?)
		ends
		
		createCompatible 	dm this
		destroy 			dm this
		selectObject 		dm this
		create				dm this;:inline
		rectangle 			dm this:inline
		fillRect			dm this:inline
		frameRect			dm this:inline
		ellipse 			dm this:inline
		roundRect 			dm this:inline
		drawText  			dm this:inline
		drawTextW  			dm this:inline
		setBkColor 			dm this:inline
		setTextColor 		dm this:inline
		setBkMode 			dm this:inline
		getBkColor 			dm this:inline
		getTextColor 		dm this:inline
		getBkMode 			dm this:inline
		createObjects		dm this:inline
		selectObjects 		dm this:inline
		destroyObject		dm this:inline
		freeObject			dm this:inline
	ends

	struct XFORM 
		eM11	dd ?;
		eM12	dd ?;
		eM21	dd ?;
		eM22	dd ?;
		eDx		dd ?;
		eDy		dd ?;
	ends

	define GM_COMPATIBLE 	1
	define GM_ADVANCED 		2

	include "Graphics.asm"

@endm