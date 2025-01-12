
; macro GraphicsObjUtils.create_BMP this, x, y{
; }
; 4 аргумента - compatible bitmap для первого аргумента
; 3 - для hdc самого обьекта
macro GraphicsObjUtils.create_BMP this, args&{
	match =4, __argscount__\{
		@call [CreateCompatibleBitmap](args)
	\}
	match =3, __argscount__\{
		local _this
		inlineObj _this, this, rcx
		@call [CreateCompatibleBitmap]([_this+Graphics.hDC], args)
	\}
}

macro GraphicsObjUtils.create_pen this, style, width, colorref{
	@call [CreatePen](style, width, colorref)
}

macro GraphicsObjUtils.create_sBrush this, colorref{
	@call [CreateSolidBrush](colorref)
}

macro GraphicsObjUtils.create_pBrush this, hBMP{
	@call [CreatePatternBrush](hBMP)
}

macro GraphicsObjUtils.create_hBrush this, iHatch, colorref{
	@call [CreateHatchBrush](iHatch, colorref)
}