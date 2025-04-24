proc_noprologue

macro EDIT.setReadOnly this, isReadOnly{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + EDIT.hWnd], EM_SETREADONLY, isReadOnly, 0)
}

macro EDIT.getLimitText this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + EDIT.hWnd], EM_GETLIMITTEXT, 0, 0)
}

macro EDIT.setLimitText this, charCount{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + EDIT.hWnd], EM_SETLIMITTEXT, charCount, 0)
}

macro EDIT.getPlaceholder this, lpStr, bufSize{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + EDIT.hWnd], EM_GETCUEBANNER, lpStr, bufSize)
}

macro EDIT.setPlaceholder this, lpWstr, showFocus = 1{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + EDIT.hWnd], EM_SETCUEBANNER, showFocus, lpWstr)
}

proc EDIT.addText c uses pbx, this, strLp
	virtObj .this:arg EDIT at pbx from @arg1
	@sarg @arg2

	local _start:DWORD,_end:DWORD
	@call [SendMessageA]([.this.hWnd], EM_GETSEL, addr _start, addr _end)
	@call .this->getTextLen()
	@call [SendMessageA]([.this.hWnd], EM_SETSEL, eax, eax)
	@call [SendMessageA]([.this.hWnd], EM_REPLACESEL, 0, [strLp])
	@call [SendMessageA]([.this.hWnd], EM_SETSEL, [_start], [_end])
	ret
endp

proc_resprologue