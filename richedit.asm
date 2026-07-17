macro __RICHEDIT.setBgColor this, colorref{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + __RICHEDIT.hWnd], EM_SETBKGNDCOLOR, 0, colorref)
}

macro __RICHEDIT.setTextColor this, colorref{
	@fillGPR pdx, colorref
	local _this
	inlineObj _this, this, pcx
	local _cf
	procbuf_alloc _cf, sizeof.CHARFORMAT2A
	mov [_cf + CHARFORMAT2A.cbSize], sizeof.CHARFORMAT2A
	mov [_cf + CHARFORMAT2A.dwMask], CFM_COLOR
	mov [_cf + CHARFORMAT2A.crTextColor], edx
	mov [_cf + CHARFORMAT2A.dwEffects], 0
	$call [SendMessageA]([_this + __RICHEDIT.hWnd], EM_SETCHARFORMAT, SCF_ALL, &_cf)
	procbuf_free _cf
}
