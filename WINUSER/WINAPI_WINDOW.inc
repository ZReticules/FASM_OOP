importlib user32,\
	GetWindowTextLengthA,\
	GetWindowTextA,\
	SendMessageA,\
	EnableWindow,\
	ShowWindow,\
	SetFocus


struct WINAPI_WINDOW
	hWnd dptr ?

	setText 	dm this:inline
	getTextLen 	dm this:inline
	getText 	dm this:inline
	setEnabled	dm this:inline
	setVisible	dm this:inline
	setFocus	dm this:inline
	invalidate 	dm this:inline
	setRedraw	dm this:inline

	getString	dm this
	setString 	dm this
ends

macro WINAPI_WINDOW.invalidate this, lpRect=NULL, bErase=0{
	local _this
	inlineObj _this, this, pcx
	@call [InvalidateRect]([_this + WINAPI_WINDOW.hWnd], lpRect, bErase)
}

macro WINAPI_WINDOW.getTextLen this{
	local _this
	inlineObj _this, this, pcx
	@call [GetWindowTextLengthA]([_this + WINAPI_WINDOW.hWnd])
}

macro WINAPI_WINDOW.getText this, lpCstr, nMaxCount{
	local _this
	inlineObj _this, this, pcx
	@call [GetWindowTextA]([_this + WINAPI_WINDOW.hWnd], lpCstr, nMaxCount)
}

macro WINAPI_WINDOW.setText this, lpCstr{
	local _this
	inlineObj _this, this, pcx
	@call [SetWindowTextA]([_this + WINAPI_WINDOW.hWnd], lpCstr)
}

macro WINAPI_WINDOW.setVisible this, bState{
	local _this
	inlineObj _this, this, pcx
	@call [ShowWindow]([_this + WINAPI_WINDOW.hWnd], bState)
}

macro WINAPI_WINDOW.setEnabled this, bState{
	local _this
	inlineObj _this, this, pcx
	@call [EnableWindow]([_this + WINAPI_WINDOW.hWnd], bState)
}

macro WINAPI_WINDOW.setFocus this{
	local _this
	inlineObj _this, this, pcx
	@call [SetFocus]([_this + WINAPI_WINDOW.hWnd])
}

macro WINAPI_WINDOW.setRedraw this, needRedraw{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + WINAPI_WINDOW.hWnd], WM_SETREDRAW, needRedraw, NULL)
}

proc_noprologue

proc WINAPI_WINDOW.getString c uses pbx pdi, this, lpString
	; int3
	@sarg @arg2
	virtObj .this:arg WINAPI_WINDOW at pbx from @arg1
	virtObj .string:arg String at pdi from @arg2
	@call [GetWindowTextLengthA]([.this.hWnd])
	mov [.string.len], eax
	@call .string->realloc([.string.len])->getLpChars()
	mov edx, [.string.len]
	inc edx
	@call [GetWindowTextA]([.this.hWnd], pax, pdx)
	mov eax, [.string.len]
	ret
endp

proc WINAPI_WINDOW.setString c uses pbx pdi, this, lpString
	; int3
	@sarg @arg2
	virtObj .this:arg WINAPI_WINDOW at pbx from @arg1
	virtObj .string:arg String at pdx from @arg2
	@call .string->getLpChars()
	@call [SetWindowTextA]([.this.hWnd], pax)
	ret
endp

proc_resprologue
