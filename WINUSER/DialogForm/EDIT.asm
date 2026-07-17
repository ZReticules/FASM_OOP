.proc_frame_mode_static

macro EDIT.setReadOnly this, isReadOnly{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETREADONLY, isReadOnly, 0)
}

macro EDIT.getLimitText this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETLIMITTEXT, 0, 0)
}

macro EDIT.setLimitText this, charCount{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETLIMITTEXT, charCount, 0)
}

macro EDIT.getPlaceholder this, lpStr, bufSize{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETCUEBANNER, lpStr, bufSize)
}

macro EDIT.setPlaceholder this, lpWstr, showFocus = 1{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETCUEBANNER, showFocus, lpWstr)
}

macro EDIT.getSelected this, lpStart, lpEnd{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETSEL, lpStart, lpEnd)
}

macro EDIT.getSelectedW this, lpStart, lpEnd{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + EDIT.hWnd], EM_GETSEL, lpStart, lpEnd)
}

macro EDIT.setSelected this, start, end{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETSEL, start, end)
}

macro EDIT.setSelectedW this, start, end{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + EDIT.hWnd], EM_SETSEL, start, end)
}

macro EDIT.replaceSelected this, lpStr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_REPLACESEL, 0, lpStr)
}

macro EDIT.replaceSelectedW this, lpWstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + EDIT.hWnd], EM_REPLACESEL, 0, lpWstr)
}

macro EDIT.clearUndo this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_EMPTYUNDOBUFFER, 0, NULL)
}

.proc cdecl EDIT.addText(.pthis, .lpStr) uses pbx
	virtObj .this EDIT at pbx from @arg1
	@sarg @arg2

	.local .start_:DWORD, .end_:DWORD
	$call .this::getSelected(&.start_, &.end_)
	$call .this::getTextLen()
	$call .this::setSelected(eax, eax)
	$call .this::replaceSelected([.lpStr])
	$call .this::setSelected([.start_], [.end_])
	ret
.endp

.proc cdecl EDIT.addTextW(.pthis, .lpWstr) uses pbx
	virtObj .this EDIT at pbx from @arg1
	@sarg @arg2

	.local .start:DWORD, .end:DWORD
	$call .this::getSelectedW(&.start, &.end)
	$call .this::getTextLenW()
	$call .this::setSelectedW(eax, eax)
	$call .this::replaceSelectedW([.lpWstr])
	$call .this::setSelectedW([.start], [.end])
	ret
.endp

.proc_frame_mode_previous
