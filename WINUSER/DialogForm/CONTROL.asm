importlib user32,\
	HideCaret

macro CONTROL.hideCur this{
	local _this
	inlineObj _this, this, pcx
	$call [HideCaret]([_this+CONTROL.hWnd])
}

macro CONTROL.setTheme this, wstrLp{
	local _this
	inlineObj _this, this, pcx
	$call [SetWindowTheme]([_this+CONTROL.hWnd], wstrLp, 0)
}

macro CONTROL.getBaseRect this, dest{
	local _this, _dest
	inlineObj _dest, dest, pdx
	inlineObj _this, this, pcx
	movups xmm0, xword[_this + CONTROL.sData.baseRect]
	movups xword[_dest], xmm0
}

macro CONTROL.setBaseRect this, src{
	local _this, _src
	inlineObj _src, src, pdx
	inlineObj _this, this, pcx
	movups xmm0, xword[_src]
	movups xword[_this + CONTROL.sData.baseRect], xmm0
}

macro CONTROL.getScaleMode this{
	local _this
	inlineObj _this, this, pcx
	mov eax, dword[_this + CONTROL.sData.scaleMode]
}

macro CONTROL.setScaleMode this, src{
	local _this
	@loadGPR pdx, src
	inlineObj _this, this, pcx
	mov dword[_this + CONTROL.sData.scaleMode], edx
}