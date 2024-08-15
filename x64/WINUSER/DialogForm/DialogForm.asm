importlib user32,\
	DialogBoxIndirectParamA,\
	EndDialog,\
	GetWindowTextLengthA,\
	GetWindowTextA,\
	GetDlgItem,\
	SetWindowTextA,\
	ComboBox_AddStringA,\
	SendMessageA,\
	CreateDialogIndirectParamA,\
	DestroyWindow,\
	PostQuitMessage,\
	GetMessageA,\
	IsDialogMessageA,\
	SetWindowLongPtrA,\
	GetWindowLongPtrA,\
	IsWindow,\
	TranslateMessage,\
	DispatchMessageA

importlib kernel32,\
	GetModuleHandleA

importlib gdi32,\
	CreateSolidBrush,\
	DeleteObject,\
	SetBkColor,\
	SetTextColor

importlib dwmapi,\
	DwmSetWindowAttribute
	
importlib uxtheme,\
	SetWindowTheme

proc_noprologue

proc DIALOGFORM.start, this, parent
	virtObj .this:arg DIALOGFORM
	mov [this], rcx
	mov [parent], rdx
	@call [GetModuleHandleA](0)
	mov rcx, [this]
	@call [DialogBoxIndirectParamA](rax, [.this.hDialogTemplate], [parent], [.this.lpDialogFunc], rcx)
	ret
endp

proc DIALOGFORM.startNM uses rbx, this, parent
	virtObj .this:arg DIALOGFORM at rbx
	mov rbx, rcx
	xor eax, eax
	cmp [.this.hWnd], 0
	jne .alreadyExists
		mov [.this.hWndParent], rdx
		@call [GetModuleHandleA](0)
		mov [.this.WM_CLOSE], DIALOGFORM_WM_CLOSE_nomodal
		@call [CreateDialogIndirectParamA](rax, [.this.hDialogTemplate], [.this.hWndParent], [.this.lpDialogFunc], rbx)
		mov [.this.hWnd], rax
	.alreadyExists:
	ret
endp

if used DIALOGFORM_WM_CLOSE;this, paramsLp
	DIALOGFORM_WM_CLOSE:
		virtObj .this:arg DIALOGFORM
		@jret [EndDialog]([.this.hWnd], NULL)
end if

proc DIALOGFORM_WM_CLOSE_nomodal, lpForm, lpParam
	virtObj .form:arg DIALOGFORM
	mov [lpForm], rcx
	@call [DestroyWindow]([.form.hWnd])
	mov rcx, [lpForm]
	mov [.form.hWnd], 0
	@jret [PostQuitMessage](0)
endp

proc DIALOGFORM_WM_CTLCOLOR uses rbx rsi, formLp, paramsLp
	virtObj .params:arg params at rbx
	mov rbx, rdx
	@call [GetWindowLongPtrA]([.params.lparam], GWL_USERDATA)
	test rax, rax
	jz .noVal
		virtObj .control:arg _DLG_SPECIAL_CONTROL at rsi
		mov rsi, rax
		@call [SetBkColor]([.params.wparam], [.control.bkColor])
		@call [SetTextColor]([.params.wparam], [.control.txColor])
		mov rax, [.control.bgColorBrush]
	.noVal:
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

proc DIALOGFORM.setBgColor, this, colorref
	virtObj .this:arg DIALOGFORM
	mov [this], rcx
	cmp [.this.bgColorBrush], NULL
	je .emptyColor
		mov [colorref], rdx
		@call [DeleteObject]([.this.bgColorBrush])
		mov rdx, [colorref]
	.emptyColor:
	@call [CreateSolidBrush](rdx)
	mov rcx, [this]
	mov [.this.bgColorBrush], rax
	ret
endp

proc DIALOGFORM.setCaptionColor, this, Colorref
	virtObj .this:arg DIALOGFORM
	local Color:DWORD
	mov [Color], edx
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.CAPTION_COLOR, addr Color, 4)
	ret
endp

proc DIALOGFORM.setTextColor, this, Colorref
	virtObj .this:arg DIALOGFORM
	mov [Colorref], rdx
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.TEXT_COLOR, addr Colorref, 4)
	ret
endp

proc DIALOGFORM.setBorderColor, this, Colorref
	virtObj .this:arg DIALOGFORM
	mov [Colorref], rdx
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.BORDER_COLOR, addr Colorref, 4)
	ret
endp

proc DIALOGFORM.setCornerType, this, RectType
	virtObj .this:arg DIALOGFORM
	mov [RectType], rdx
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.WINDOW_CORNER_PREFERENCE, addr RectType, 4)
	ret
endp

proc DIALOGFORM.dispatchMessages uses rbx, mainHandle
	virtObj .this:arg DIALOGFORM at rbx
	locals 
		msg MSG
	endl
	mov rbx, rcx
	@call [GetMessageA](addr msg, NULL, 0, 0)
	test eax, eax
	jnz .noEnd
		mov rcx, 1
		mov rdx, [.this.hWnd]
		cmp [msg.hwnd], rdx
		cmovne rax, rcx
		jmp .return
	.noEnd:
	@call [GetActiveWindow]()
	@call [IsDialogMessageA](rax, addr msg)
	test rax, rax
	jnz .return
		@call [TranslateMessage](addr msg)
		@call [DispatchMessageA](addr msg)
		mov rax, 1
	.return:ret
endp

proc_resprologue