importlib user32,\
	GetWindowTextLengthA,\
	GetWindowTextA,\
	SetWindowTextA,\
	GetWindowTextLengthW,\
	GetWindowTextW,\
	SetWindowTextW,\
	SendMessageA,\
	EnableWindow,\
	ShowWindow,\
	SetFocus


struct WINAPI_WINDOW
	hWnd HWND

	setText 	dm this:inline
	getTextLen 	dm this:inline
	getText 	dm this:inline
	setTextW 	dm this:inline
	getTextLenW dm this:inline
	getTextW 	dm this:inline
	setEnabled	dm this:inline
	setVisible	dm this:inline
	setFocus	dm this:inline
	invalidate 	dm this:inline
	setRedraw	dm this:inline

	getString	dm this
	setString 	dm this
	getWString	dm this
	setWString 	dm this
ends

macro WINAPI_WINDOW.invalidate this, lpRect=NULL, bErase=0{
	local _this
	inlineObj _this, this, pcx
	$call [InvalidateRect]([_this + WINAPI_WINDOW.hWnd], lpRect, bErase)
}

macro WINAPI_WINDOW.getTextLen this{
	local _this
	inlineObj _this, this, pcx
	$call [GetWindowTextLengthA]([_this + WINAPI_WINDOW.hWnd])
}

macro WINAPI_WINDOW.getText this, lpCstr, nMaxCount{
	local _this
	inlineObj _this, this, pcx
	$call [GetWindowTextA]([_this + WINAPI_WINDOW.hWnd], lpCstr, nMaxCount)
}

macro WINAPI_WINDOW.setText this, lpCstr{
	local _this
	inlineObj _this, this, pcx
	$call [SetWindowTextA]([_this + WINAPI_WINDOW.hWnd], lpCstr)
}

macro WINAPI_WINDOW.getTextLenW this{
	local _this
	inlineObj _this, this, pcx
	$call [GetWindowTextLengthW]([_this + WINAPI_WINDOW.hWnd])
}

macro WINAPI_WINDOW.getTextW this, lpWstr, nMaxCount{
	local _this
	inlineObj _this, this, pcx
	$call [GetWindowTextW]([_this + WINAPI_WINDOW.hWnd], lpWstr, nMaxCount)
}

macro WINAPI_WINDOW.setTextW this, lpWstr{
	local _this
	inlineObj _this, this, pcx
	$call [SetWindowTextW]([_this + WINAPI_WINDOW.hWnd], lpWstr)
}

macro WINAPI_WINDOW.setVisible this, bState{
	local _this
	inlineObj _this, this, pcx
	$call [ShowWindow]([_this + WINAPI_WINDOW.hWnd], bState)
}

macro WINAPI_WINDOW.setEnabled this, bState{
	local _this
	inlineObj _this, this, pcx
	$call [EnableWindow]([_this + WINAPI_WINDOW.hWnd], bState)
}

macro WINAPI_WINDOW.setFocus this{
	local _this
	inlineObj _this, this, pcx
	$call [SetFocus]([_this + WINAPI_WINDOW.hWnd])
}

macro WINAPI_WINDOW.setRedraw this, needRedraw{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + WINAPI_WINDOW.hWnd], WM_SETREDRAW, needRedraw, NULL)
}

.proc_frame_mode_static

.proc cdecl WINAPI_WINDOW.getString(.pthis, .lpString) uses pbx pdi
	@sarg @arg2
	virtObj .this WINAPI_WINDOW at pbx from @arg1
	virtObj .string String at pdi from @arg2
	$call [.string.len] = [GetWindowTextLengthA]([.this.hWnd])
	$call .string::realloc([.string.len])::getLpChars()
	mov edx, [.string.len]
	inc edx
	$call [GetWindowTextA]([.this.hWnd], pax, pdx)
	$return [.string.len]
.endp

.proc cdecl WINAPI_WINDOW.setString(.pthis, .lpString) uses pbx pdi
	@sarg @arg2
	virtObj .this:arg WINAPI_WINDOW at pbx from @arg1
	virtObj .string:arg String at pdx from @arg2
	$call .string::getLpChars()
	$call [SetWindowTextA]([.this.hWnd], pax)
	ret
.endp

.proc cdecl WINAPI_WINDOW.getWString(.pthis, .lpWString) uses pbx pdi
	@sarg @arg2
	virtObj .this:arg WINAPI_WINDOW at pbx from @arg1
	virtObj .string:arg WString at pdi from @arg2
	$call [.string.len] = [GetWindowTextLengthW]([.this.hWnd])
	$call .string::realloc([.string.len])::getLpWChars()
	mov edx, [.string.len]
	inc edx
	$call [GetWindowTextW]([.this.hWnd], pax, pdx)
	$return [.string.len]
.endp

.proc cdecl WINAPI_WINDOW.setWString(.pthis, .lpWString) uses pbx pdi
	@sarg @arg2
	virtObj .this:arg WINAPI_WINDOW at pbx from @arg1
	virtObj .string:arg WString at pdx from @arg2
	$call .string::getLpWChars()
	$call [SetWindowTextW]([.this.hWnd], pax)
	ret
.endp

.proc_frame_mode_previous
