importlib user32,\
	DialogBoxIndirectParamA,\
	EndDialog,\
	GetWindowTextLengthA,\
	GetWindowTextA,\
	GetDlgItem,\
	SetWindowTextA,\
	SendMessageA,\
	CreateDialogIndirectParamA,\
	DestroyWindow,\
	PostQuitMessage,\
	GetMessageA,\
	IsDialogMessageA,\
	TranslateMessage,\
	DispatchMessageA,\
	EnableWindow,\
	ShowWindow

importlib gdi32,\
	CreateSolidBrush,\
	DeleteObject,\
	SetBkColor,\
	SetTextColor

importlib dwmapi,\
	DwmSetWindowAttribute
	
importlib uxtheme,\
	SetWindowTheme,\
    CloseThemeData

proc_noprologue

; proc DIALOGFORM.start, this, parent
; 	virtObj .this:arg DIALOGFORM
; 	mov [this], rcx
; 	mov [parent], rdx
; 	mov [.this.__close], DIALOGFORM_WM_CLOSE
; 	@call [GetModuleHandleA](0)
; 	mov rcx, [this]
; 	@call [DialogBoxIndirectParamA](rax, [.this.hDialogTemplate], [parent], [.this.lpDialogFunc], rcx)
; 	ret
; endp


macro DIALOGFORM.invalidate this{
	local _this
	inlineObj _this, this, pcx
	@call [InvalidateRect]([_this+DIALOGFORM.hWnd], NULL, 1)
}

macro DIALOGFORM.getTextLen this{
	local _this
	inlineObj _this, this, pcx
	@call [GetWindowTextLengthA]([_this+DIALOGFORM.hWnd])
}

macro DIALOGFORM.getText this, lpString, nMaxCount{
	local _this
	inlineObj _this, this, pcx
	@call [GetWindowTextA]([_this+DIALOGFORM.hWnd], lpString, nMaxCount)
}

macro DIALOGFORM.setText this, lpString{
	local _this
	inlineObj _this, this, pcx
	@call [SetWindowTextA]([_this+DIALOGFORM.hWnd], lpString)
}

macro DIALOGFORM.setIcon this, hIcon{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+DIALOGFORM.hWnd], WM_SETICON, ICON_BIG, hIcon)
}

macro DIALOGFORM.close this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+DIALOGFORM.hWnd], WM_CLOSE, 0, 0)
}

macro DIALOGFORM.setVisible this, bState{
	local _this
	inlineObj _this, this, pcx
	@call [ShowWindow]([_this+DIALOGFORM.hWnd], bState)
}

macro DIALOGFORM.setEnabled this, bState{
	local _this
	inlineObj _this, this, pcx
	@call [EnableWindow]([_this+DIALOGFORM.hWnd], bState)
}

macro DIALOGFORM.setFocus this{
	local _this
	inlineObj _this, this, pcx
	@call [SetFocus]([_this+DIALOGFORM.hWnd])
}

macro DIALOGFORM.start this, parent=NULL{
	local _this
	inlineObj _this, this, pcx
	mov [_this + DIALOGFORM.__close], DIALOGFORM_WM_CLOSE
	@call [DialogBoxIndirectParamA]([WND.hModule], [_this + DIALOGFORM.hDialogTemplate], parent, [_this + DIALOGFORM.lpDialogFunc], addr _this)
}

proc DIALOGFORM.startNM c uses pbx, this, parent
	virtObj .this:arg DIALOGFORM at pbx from @arg1
	xor eax, eax
	cmp [.this.hWnd], 0
	jne .alreadyExists
		mov [.this.__close], DIALOGFORM_WM_CLOSE_NOMODAL
		@call [CreateDialogIndirectParamA]([WND.hModule], [.this.hDialogTemplate], @arg2, [.this.lpDialogFunc], pbx)
		mov [.this.hWnd], pax
	.alreadyExists:
	ret
endp


proc DIALOGFORM_WM_CLOSE uses pbx, lpForm, lpParam, exitVal
	virtObj .form:arg DIALOGFORM at pbx from @arg1
    @call [CloseThemeData]([.form.hTheme])
	@call [EndDialog]([.form.hWnd], [exitVal])
	mov pcx, [lpForm]
	mov [.form.hWnd], 0
	ret
endp

proc DIALOGFORM_WM_CLOSE_NOMODAL, lpForm, lpParam, exitVal
	virtObj .form:arg DIALOGFORM at pcx from @arg1
	@sarg @arg1
    @call [CloseThemeData]([.form.hTheme])
	@call [DestroyWindow]([.form.hWnd])
	mov pcx, [lpForm]
	mov [.form.hWnd], 0
	@call [PostQuitMessage](0)
	ret
endp

; proc DIALOGFORM_WM_CTLCOLORDLG, formLp, paramsLp
; 	virtObj .params:arg params
; 	@call [GetWindowLongPtrA]([.params.lparam], GWL_USERDATA)
; 	test rax, rax
; 	jz .noVal
; 		virtObj .control:arg CONTROL at rax
; 		mov rax, [.control.bgColorBrush]
; 	.noVal:
; 	ret
; endp

proc DIALOGFORM.setBgColor c, this, colorref
	virtObj .this:arg DIALOGFORM at pcx from @arg1
	@sarg @arg1
	@larg pdx, @arg2
	cmp [.this.bgColorBrush], NULL
	je .emptyColor
		@sarg @arg2
		@call [DeleteObject]([.this.bgColorBrush])
		mov pdx, [colorref]
	.emptyColor:
	@call [CreateSolidBrush](pdx)
	mov pcx, [this]
	mov [.this.bgColorBrush], pax
	ret
endp

proc DIALOGFORM.setCaptionColor c, this, Colorref
	virtObj .this:arg DIALOGFORM at pcx from @arg1
	@sarg @arg2
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.CAPTION_COLOR, addr Colorref, 4)
	ret
endp

proc DIALOGFORM.setTextColor c, this, Colorref
	virtObj .this:arg DIALOGFORM at pcx from @arg1
	@sarg @arg2
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.TEXT_COLOR, addr Colorref, 4)
	ret
endp

proc DIALOGFORM.setBorderColor c, this, Colorref
	virtObj .this:arg DIALOGFORM at pcx from @arg1
	@sarg @arg2
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.BORDER_COLOR, addr Colorref, 4)
	ret
endp

proc DIALOGFORM.setCornerType c, this, RectType
	virtObj .this:arg DIALOGFORM at pcx from @arg1
	@sarg @arg2
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.WINDOW_CORNER_PREFERENCE, addr RectType, 4)
	ret
endp

proc DIALOGFORM.dispatchMessages c uses pbx, this
	virtObj .this:arg DIALOGFORM at pbx from @arg1
	locals 
		msg MSG
	endl
	@call [GetMessageA](addr msg, NULL, 0, 0)
	test eax, eax
	jnz .noEnd
		mov ecx, 1
		mov pdx, [.this.hWnd]
		cmp [msg.hwnd], pdx
		cmovne eax, ecx
		jmp .return
	.noEnd:
	@call [GetActiveWindow]()
	mov ecx, eax
	@call [IsDialogMessageA](ecx, addr msg)
	test eax, eax
	jnz .return
		@call [TranslateMessage](addr msg)
		@call [DispatchMessageA](addr msg)
		mov eax, 1
	.return: ret
endp

; proc DIALOGFORM.getSize, this, lpSizeFunc
; 	virtObj .this:arg DIALOGFORM
; 	local winRect:RECT
; 	mov r8, rdx
; 	@call r8([.this.hWnd], addr winRect)
; 	mov rax, [winRect.right]
; 	sub rax, [winRect.left]
; 	mov rdx, [winRect.bottom]
; 	sub rdx, [winRect.top]
; 	cdq
; 	ret
; endp

; proc DIALOGFORM.getSize
; 	virtObj .this:arg DIALOGFORM
; 	@call [GetClientRect]
; endp

proc_resprologue
