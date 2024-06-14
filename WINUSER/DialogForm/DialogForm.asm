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

proc DIALOGFORM.start, this, parent
	virtObj .this:arg DIALOGFORM
	push rcx
	push rdx
	@call [GetModuleHandleA](0)
	pop r8
	mov rcx, [rsp]						;������������ �����
	push rcx
	mov r9, [.this.lpDialogFunc]
	mov rdx, [.this.hDialogTemplate]
	mov rcx, rax
	@call [DialogBoxIndirectParamA]()
	pop rcx								;������������ �������
	ret
endp

proc DIALOGFORM.startNM uses rbx r12, this, parent
	virtObj .this:arg DIALOGFORM at rbx
	.parentHWnd equ r12
	mov rbx, rcx
	xor rax, rax
	cmp [.this.hWnd], 0
	jne .alreadyExists
		mov .parentHWnd, rdx
		@call [GetModuleHandleA](0)
		mov [.this.WM_CLOSE], DIALOGFORM_WM_CLOSE_nomodal
		@call [CreateDialogIndirectParamA](rax, [.this.hDialogTemplate], r12, [.this.lpDialogFunc], rbx)
		mov [.this.hWnd], rax
		mov [.this.hWndParent], r12
	.alreadyExists:
	ret
endp

if used DIALOGFORM_WM_CLOSE;this, paramsLp
	DIALOGFORM_WM_CLOSE:
		virtObj .this:arg DIALOGFORM
		mov rcx, [.this.hWnd]
		xor rdx, rdx
		jmp [EndDialog]
end if

proc DIALOGFORM_WM_CLOSE_nomodal, formLp, lpParam
	virtObj .form:arg DIALOGFORM
	push rcx
	@call [DestroyWindow]([.form.hWnd])
	pop rcx
	mov [.form.hWnd], 0
	leave
	xor rcx, rcx
	jmp [PostQuitMessage]
endp

proc DIALOGFORM_WM_CTLCOLOR uses rbx r12, formLp, paramsLp
	virtObj .params:arg params at rbx
	mov rbx, rdx
	@call [GetWindowLongPtrA]([.params.lparam], GWL_USERDATA)
	test rax, rax
	jz .noVal
		virtObj .control:arg CONTROL at r12
		mov r12, rax
		@call [SetBkColor]([.params.wparam], [.control.bkColor])
		@call [SetTextColor]([.params.wparam], [.control.txColor])
		mov rax, [.control.bgColorBrush]
	.noVal:
	ret
endp

proc DIALOGFORM_WM_CTLCOLORDLG, formLp, paramsLp
	virtObj .params:arg params
	@call [GetWindowLongPtrA]([.params.lparam], GWL_USERDATA)
	test rax, rax
	jz .noVal
		virtObj .control:arg CONTROL at rax
		mov rax, [.control.bgColorBrush]
	.noVal:
	ret
endp

proc DIALOGFORM.getTextLen, this
	virtObj .this:arg DIALOGFORM
	@call [GetWindowTextLengthA]([.this.hWnd])
	ret
endp

proc DIALOGFORM.getText, this, lpString, nMaxCount
	virtObj .this:arg DIALOGFORM
	@call [GetWindowTextA]([.this.hWnd])
	ret
endp

proc DIALOGFORM.setText, this, lpString
	virtObj .this:arg CONTROL
	@call [SetWindowTextA]([.this.hWnd])
	ret
endp

proc DIALOGFORM.setIcon, this, hIcon
	virtObj .this:arg DIALOGFORM
	mov r9, rdx
	@call [SendMessageA]([.this.hWnd], WM_SETICON, ICON_BIG, r9)
	ret
endp

if used DIALOGFORM.close
	DIALOGFORM.close:
		virtObj .this:arg DIALOGFORM
		xor r9, r9
		mov r8, r9
		mov rdx, WM_CLOSE
		mov rcx, [.this.hWnd]
		jmp [SendMessageA]
end if

proc DIALOGFORM.setBgColor, this, colorref
	virtObj .this:arg DIALOGFORM
	push rcx
	cmp [.this.bgColorBrush], NULL
	je .emptyColor
		push rdx
		sub rsp, 20h
		@call [DeleteObject]([.this.bgColorBrush])
		add rsp, 20h
		pop rdx
	.emptyColor:
	sub rsp, 20h
	@call [CreateSolidBrush](rdx)
	add rsp, 20h
	pop rcx
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
	local Color:DWORD
	mov [Color], edx
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.TEXT_COLOR, addr Color, 4)
	ret
endp

proc DIALOGFORM.setBorderColor, this, Colorref
	virtObj .this:arg DIALOGFORM
	local Color:DWORD
	mov [Color], edx
	@call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.BORDER_COLOR, addr Color, 4)
	ret
endp

proc DIALOGFORM.setCornerType, this, _RectType
	virtObj .this:arg DIALOGFORM
	local RectType:DWORD
	mov [RectType], edx
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
	; @call [IsWindow]([msg.hwnd])
	; test rax, rax
	; 	jz .stdDispatch
	; @call [IsDialogMessageA]([msg.hwnd], addr msg)
	; test rax, rax
	; 	jnz .return
	.stdDispatch:
		@call [TranslateMessage](addr msg)
		@call [DispatchMessageA](addr msg)
		mov rax, 1
	.return:ret
endp

macro ShblDialog formType, _x = 0, _y = 0, _cx = 100, _cy = 50, _Text = "DialogForm", _style=WS_VISIBLE+WS_CAPTION+WS_SYSMENU+WS_MINIMIZEBOX+WS_MAXIMIZEBOX+DS_CENTER, _styleEx = NULL{
	; match formName formType, formInfo\{
		local controlCounter
		controlCounter = 0
		while $ mod 2 <> 0
			db 0
		end while
		formType\#_DlgTemplate DLGTEMPLATE \
			_style,\
			_styleEx,\
			formType\#.cdit,\
			_x,\
			_y,\
			_cx,\
			_cy
		dw 00h ; menu
		dw 00h ; header
		du _Text
		dw 0
		local isAlign							;������������
		isAlign = 1  							;���������� ��� ��������, ��������� �� ������������
		irpv cntrl, formType#@ControlStack\{
			while $ mod 4 <> 0
				db 0
				isAlign = 1
			end while
			if isAlign = 0 						;���� ������������ �� ����, ����������� ��� 4 �����
				dd 0
			end if
			isAlign = 0
			controlCounter = controlCounter + 1
			formType#.\#cntrl\#._id = controlCounter
			formType#.\#cntrl\#_DLGITEMTEMPLATE DLGITEMTEMPLATE \
				formType#.\#cntrl\#._style,\
				formType#.\#cntrl\#._styleEx,\
				formType#.\#cntrl\#._x,\
				formType#.\#cntrl\#._y,\
				formType#.\#cntrl\#._cx,\
				formType#.\#cntrl\#._cy,\
				formType#.\#cntrl\#._id
			match TName, cntrl\#.Type\\{
				du TName\\#.CType
				dw 00h
				du formType#.\#cntrl\#._Text
				dw 00h
			\\}
		\}
		formType#.cdit = controlCounter

		proc formType#_DialogProc uses rbx, hwnddlg, msg, wparam, lparam
			virtObj .this:arg formType at rbx
			mov [hwnddlg], rcx
			mov [msg], rdx
			mov [wparam], r8
			mov [lparam], r9
			xor eax, eax
			cmp [msg], WM_INITDIALOG
			jne .nextEvent
				mov rbx, [lparam]
				mov [.this.hWnd], rcx
				@call [SetWindowLongPtrA]([hwnddlg], GWL_USERDATA, [lparam])
		irpv cntrl, formType#@ControlStack\{
				mov [.this.\#cntrl\#.ID], formType#.\#cntrl\#._id
				@call [GetDlgItem]([.this.hWnd], word [.this.\#cntrl\#.ID])
				mov [.this.\#cntrl\#.hWnd], rax
				@call [SetWindowLongPtrA]([.this.\#cntrl\#.hWnd], GWL_USERDATA, addr .this.\#cntrl)
		\}
			.nextEvent:
			@call [GetWindowLongPtrA]([hwnddlg], GWL_USERDATA)
			mov rbx, rax
			xor eax, eax
		irpv evnt, formType#@EventStack\{
			cmp [msg], evnt
			jne .nextEvent\#evnt
				@call [.this.\#evnt](addr .this, addr hwnddlg)
				jmp .return
			.nextEvent\#evnt:
		\}
			cmp [msg], WM_COMMAND
			jne .NextEvent
		irpv cntrl, formType#@ControlStack\{
			
			match TName, cntrl\#.Type\\{
				irpv evnt, TName\\#@EventStack\\\{
				cmp [wparam], evnt shl 16 + formType#.\#cntrl\#._id
				jne .nextEvnt\#cntrl\\\#evnt
					@call [.this.\#cntrl\#.\\\#evnt](addr .this, addr hwnddlg, addr .this.\#cntrl)
					jmp .return
				.nextEvnt\#cntrl\\\#evnt:
				\\\}
			\\}
		\}
			.NextEvent:
			xor eax, eax
			.return:ret
		endp

		; formInfo NONE, formType\#.cdit, NONE, NONE, NONE, formType\#_DlgTemplate, formType\#_DialogProc, _initvals
	; \}
}