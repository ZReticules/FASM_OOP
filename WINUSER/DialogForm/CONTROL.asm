importlib user32,\
	HideCaret

macro CONTROL.hideCur this{
	local _this
	inlineObj _this, this, pcx
	@call [HideCaret]([_this+CONTROL.hWnd])
}

macro CONTROL.setTheme this, wstrLp{
	local _this
	inlineObj _this, this, pcx
	@call [SetWindowTheme]([_this+CONTROL.hWnd], wstrLp, 0)
}