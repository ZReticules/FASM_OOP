struct STATIC CONTROL
	const CType equ "Static"
	setImage 	dm this:inline
	getImage 	dm this:inline
	setIcon 	dm this:inline
	getIcon 	dm this:inline
ends

macro STATIC.setImage this, iType, hImage{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this+STATIC.hWnd], STM_SETIMAGE, imageType, hImage)
}

macro STATIC.getImage this, iType{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this+STATIC.hWnd], STM_GETIMAGE, imageType, NULL)
}

macro STATIC.setIcon this, iType, hIcon{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this+STATIC.hWnd], STM_SETIMAGE, hIcon, NULL)
}

macro STATIC.getIcon this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this+STATIC.hWnd], STM_GETIMAGE, NULL, NULL)
}
