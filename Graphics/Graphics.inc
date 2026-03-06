@module Graphics

	struct Graphics
		const BMP 	= (Graphics.__hBMP_prev - Graphics.__hArr) / pointer.size
		const PEN 	= (Graphics.__hPen_prev - Graphics.__hArr) / pointer.size
		const BRUSH = (Graphics.__hBrush_prev - Graphics.__hArr) / pointer.size
		const RGN 	= (Graphics.__hRgn_prev - Graphics.__hArr) / pointer.size
		const FONT 	= (Graphics.__hFont_prev - Graphics.__hArr) / pointer.size

		hDC				dptr ?
		union
			struct
				__hBMP_prev		dptr 0
				__hPen_prev		dptr 0
				__hBrush_prev	dptr 0
				__hRgn_prev		dptr 0
				__hFont_prev	dptr 0
			ends
			__hArr		rptr 5
		ends
		
		make 				dm this:inline
		create				dm this
		createCompatible 	dm this
		unmake 				dm this
		selectObject 		dm this

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
		; createObjects		dm this:inline
		createCompatibleBMP	dm this ;(pthis:P_Graphics, hDC, cx, cy)
		createPen			dm this ;(pthis:P_Graphics, style, width, colorref)
		createSolidBrush	dm this ;(pthis:P_Graphics, colorref)
		createPatternBrush	dm this ;(pthis:P_Graphics, hBMP)
		createHatchBrush	dm this ;(pthis:P_Graphics, iHatch, colorref)
		; selectObjects 		dm this:inline
		destroyObject		dm this:inline
		freeObject			dm this:inline
		setDCBrushColor 	dm this:inline
		getDCBrushColor 	dm this:inline
		setDCPenColor 		dm this:inline
		getDCPenColor 		dm this:inline
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