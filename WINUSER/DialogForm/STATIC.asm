macro STATIC.setImage this, hBMP, imageType = IMAGE_BITMAP{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+STATIC.hWnd], STM_SETIMAGE, imageType, hBMP)
}