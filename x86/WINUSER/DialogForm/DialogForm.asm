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
	SetWindowLongA,\
	GetWindowLongA,\
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

proc DIALOGFORM.start, this, parent
	virtObj .this:arg DIALOGFORM
	@call [GetModuleHandleA](0)
	mov ecx, [this]
	@call [DialogBoxIndirectParamA](eax, [.this.hDialogTemplate], [parent], [.this.lpDialogFunc], ecx)
	ret
endp

proc DIALOGFORM.startNM uses ebx, this, parent
	virtObj .this:arg DIALOGFORM at ebx
	mov ebx, [this]
	xor eax, eax
	cmp [.this.hWnd], 0
	jne .alreadyExists
		mov edx, [parent]
		mov [.this.hWndParent], edx
		@call [GetModuleHandleA](0)
		mov [.this.WM_CLOSE], DIALOGFORM_WM_CLOSE_nomodal
		@call [CreateDialogIndirectParamA](eax, [.this.hDialogTemplate], [.this.hWndParent], [.this.lpDialogFunc], ebx)
		mov [.this.hWnd], eax
	.alreadyExists:
	ret
endp

proc DIALOGFORM_WM_CLOSE
	_static_args .lpForm, .lpParams
	virtObj .form:arg DIALOGFORM
	mov ecx, [.lpForm]
	@call [EndDialog]([.form.hWnd], NULL)
	ret args_space
endp

proc DIALOGFORM_WM_CLOSE_nomodal, lpForm, lpParams
	virtObj .form:arg DIALOGFORM
	mov ecx, [lpForm]
	@call [DestroyWindow]([.form.hWnd])
	mov ecx, [lpForm]
	mov [.form.hWnd], 0
	@call [PostQuitMessage](0)
	ret
endp

proc DIALOGFORM_WM_CTLCOLOR uses ebx esi, formLp, lpParams
	virtObj .params:arg params at ebx
	mov ebx, [lpParams]
	@call [GetWindowLongA]([.params.lparam], GWL_USERDATA)
	test eax, eax
	jz .noVal
		virtObj .control:arg _DLG_SPECIAL_CONTROL at esi
		mov esi, eax
		@call [SetBkColor]([.params.wparam], [.control.bkColor])
		@call [SetTextColor]([.params.wparam], [.control.txColor])
		mov eax, [.control.bgColorBrush]
	.noVal:
	ret
endp

; proc DIALOGFORM_WM_CTLCOLORDLG, formLp, lpParams
; 	virtObj .params:arg params
; 	@call [GetWindowLongA]([.params.lparam], GWL_USERDATA)
; 	test eax, eax
; 	jz .noVal
; 		virtObj .control:arg CONTROL at eax
; 		mov eax, [.control.bgColorBrush]
; 	.noVal:
; 	ret
; endp

proc DIALOGFORM.setBgColor, this, colorref
	virtObj .this:arg DIALOGFORM
	mov ecx, [this]
	cmp [.this.bgColorBrush], NULL
	je .emptyColor
		@call [DeleteObject]([.this.bgColorBrush])
	.emptyColor:
	@call [CreateSolidBrush]([colorref])
	mov ecx, [this]
	mov [.this.bgColorBrush], eax
	ret
endp

proc DIALOGFORM.setCaptionColor, this, Colorref
	virtObj .this:arg DIALOGFORM
	mov ecx, [this]
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.CAPTION_COLOR, addr Colorref, 4)
	ret
endp

proc DIALOGFORM.setTextColor, this, Colorref
	virtObj .this:arg DIALOGFORM
	mov ecx, [this]
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.TEXT_COLOR, addr Colorref, 4)
	ret
endp

proc DIALOGFORM.setBorderColor, this, Colorref
	virtObj .this:arg DIALOGFORM
	mov ecx, [this]
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.BORDER_COLOR, addr Colorref, 4)
	ret
endp

proc DIALOGFORM.setCornerType, this, RectType
	virtObj .this:arg DIALOGFORM
	mov ecx, [this]
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.WINDOW_CORNER_PREFERENCE, addr RectType, 4)
	ret
endp

proc DIALOGFORM.dispatchMessages uses ebx, this
	virtObj .this:arg DIALOGFORM at ebx
	locals 
		msg MSG
	endl
	mov ebx, [this]
	@call [GetMessageA](addr msg, NULL, 0, 0)
	test eax, eax
	jnz .noEnd
		mov ecx, 1
		mov edx, [.this.hWnd]
		cmp [msg.hwnd], edx
		cmovne eax, ecx
		jmp .return
	.noEnd:
	@call [GetActiveWindow]()
	@call [IsDialogMessageA](eax, addr msg)
	test eax, eax
	jnz .return
		@call [TranslateMessage](addr msg)
		@call [DispatchMessageA](addr msg)
		mov eax, 1
	.return:ret
endp