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
	ShowWindow,\
	SetFocus

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

macro DIALOGFORM.start this, parent=NULL{
	local _this
	inlineObj _this, this, pcx
	mov [_this + DIALOGFORM.__close], DIALOGFORM_WM_CLOSE
	@call [DialogBoxIndirectParamA]([WND.hModule], [_this + DIALOGFORM.hDialogTemplate], parent, DLG.DialogProc, addr _this)
}

macro DIALOGFORM.setIcon this, hIcon{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + DIALOGFORM.hWnd], WM_SETICON, ICON_BIG, hIcon)
}

macro DIALOGFORM.close this, result=NULL{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + DIALOGFORM.hWnd], WM_CLOSE, result, 0)
}

proc DIALOGFORM.startNM c uses pbx, this, parent
	virtObj .this:arg DIALOGFORM at pbx from @arg1
	xor eax, eax
	cmp [.this.hWnd], 0
	jne .alreadyExists
		mov [.this.__close], DIALOGFORM_WM_CLOSE_NOMODAL
		@call [CreateDialogIndirectParamA]([WND.hModule], [.this.hDialogTemplate], @arg2, DLG.DialogProc, pbx)
		mov [.this.hWnd], pax
	.alreadyExists:
	ret
endp


proc DIALOGFORM_WM_CLOSE uses pbx, lpForm, lpParam, exitVal
	@sarg @arg3
	virtObj .form:arg DIALOGFORM at pbx from @arg1
    ; @call [CloseThemeData]([.form.hTheme])
	@call .form.sData.mapDialog->unmake()
	@call [EndDialog]([.form.hWnd], [exitVal])
	mov [.form.hWnd], 0
	ret
endp

proc DIALOGFORM_WM_CLOSE_NOMODAL uses pbx, lpForm, lpParam, exitVal
	virtObj .form:arg DIALOGFORM at pbx from @arg1
    ; @call [CloseThemeData]([.form.hTheme])
	@call .form.sData.mapDialog->unmake()
	@call [DestroyWindow]([.form.hWnd])
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

; proc DIALOGFORM.setBgColor c, this, colorref
; 	virtObj .this:arg DIALOGFORM at pcx from @arg1
; 	@sarg @arg1
; 	@larg pdx, @arg2
; 	cmp [.this.bgColorBrush], NULL
; 	je .emptyColor
; 		@sarg @arg2
; 		@call [DeleteObject]([.this.bgColorBrush])
; 		mov pdx, [colorref]
; 	.emptyColor:
; 	@call [CreateSolidBrush](pdx)
; 	mov pcx, [this]
; 	mov [.this.bgColorBrush], pax
; 	ret
; endp

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

; proc DIALOGFORM.unsetBgColor c, this
; 	virtObj .this:arg DIALOGFORM at pcx from @arg1
; 	xor eax, eax
; 	xchg pax, [.this.bgColorBrush]
; 	@call [DeleteObject](pax)
; 	ret
; endp

proc DIALOGFORM.ScaleChildsEnumProc uses pbx psi, hWnd, lParam
	@sarg @arg1, @arg2
	@larg pbx, @arg2

	virtual at pbx
		.old POINT
		.new POINT
		.def POINT
	end virtual
	
	local retFromScale:POINTER, preparedRect:RECT

	; @call [GetWindowPtrA]([hWnd], GWL_USERDATA)
	@call DLG::getPtr([hWnd])
	test eax, eax
		jz .return
	
	virtObj .cntr:arg CONTROL at psi from pax


	cmp [.cntr.ID], 0
		jz .return

	mov ecx, POINT.x
	mov eax, [.cntr.sData.baseRect.left]
	imul eax, [.def + pcx]
	xor edx, edx
	div dword[.old + pcx]
	mov [preparedRect.left], eax
	
	; mov ecx, POINT.x
	mov eax, [.cntr.sData.baseRect.right]
	imul eax, [.def + pcx]
	xor edx, edx
	div dword[.old + pcx]
	mov [preparedRect.right], eax

	mov ecx, POINT.y
	mov eax, [.cntr.sData.baseRect.top]
	imul eax, [.def + pcx]
	xor edx, edx
	div dword[.old + pcx]
	mov [preparedRect.top], eax

	; mov ecx, POINT.y
	mov eax, [.cntr.sData.baseRect.bottom]
	imul eax, [.def + pcx]
	xor edx, edx
	div dword[.old + pcx]
	mov [preparedRect.bottom], eax


	.scaleMthods @const dptr .justScale,\ 
		.saveStartPos,\ 
		.saveEndDist,\ 
		.saveCenterDist,\
		.saveSizeAtLeft,\	; _c[x/y]_sticker only 
		.saveSizeAtRight,\	; _[x/y]_sticker only
		.centerScale		; _c[x/y]_sticker only
							; only if _[x/y]_sticker = 0
							; align control at center in autoscale
	cmp [.cntr.ID], 0
		jz .return
	cmp dword[.cntr.sData.scaleMode], -1
		je .return

	mov ecx, POINT.x
	mov eax, [preparedRect.left]
	mov [retFromScale], @f
	movzx edx, [.cntr.sData.scaleMode.x]
	jmp pointer[.scaleMthods + pdx * pointer.size]
	@@:
	movd xmm0, eax
	
	; mov ecx, POINT.x
	mov eax, [preparedRect.right]
	mov [retFromScale], @f
	movzx edx, [.cntr.sData.scaleMode.rx]
	jmp pointer[.scaleMthods + pdx * pointer.size]
	@@:
	movd xmm2, eax

	mov ecx, POINT.y
	mov eax, [preparedRect.top]
	mov [retFromScale], @f
	movzx edx, [.cntr.sData.scaleMode.y]
	jmp pointer[.scaleMthods + pdx * pointer.size]
	@@:
	movd xmm1, eax

	; mov ecx, POINT.y
	mov eax, [preparedRect.bottom]
	mov [retFromScale], @f
	movzx edx, [.cntr.sData.scaleMode.ry]
	jmp pointer[.scaleMthods + pdx * pointer.size]
	@@:
	movd xmm3, eax

	psubd xmm2, xmm0
	psubd xmm3, xmm1
	@call [MoveWindow]([hWnd], xmm0, xmm1, xmm2, xmm3, 1)
	; @call c [printf]("%x\n", dword[.cntr.sData.scaleMode])
	.return: 
	mov eax, 1
	ret

	.justScale:
		imul eax, [.new + pcx]
		xor edx, edx
		div dword[.def + pcx]
		jmp [retFromScale]

	.saveStartPos:
		jmp [retFromScale]

	.saveEndDist:
		sub eax, [.def + pcx]
		add eax, [.new + pcx]
		jmp [retFromScale]

	.saveCenterDist:
		mov edx, [.def + pcx]
		shr edx, 1
		sub eax, edx
		mov edx, [.new + pcx]
		shr edx, 1
		add eax, edx
		jmp [retFromScale]

	.saveSizeAtLeft:
		sub eax, [preparedRect.left + pcx]
		movd edx, xmm0
		jecxz @f
			movd edx, xmm1
		@@:
		add eax, edx
		jmp [retFromScale]

	.saveSizeAtRight:
		movptr xmm3, [retFromScale]
		mov eax, [preparedRect.right + pcx]
		mov [retFromScale], @f
		shr ecx, 2
		movzx edx, [.cntr.sData.scaleMode.rx + pcx]
		shl ecx, 2
		jmp pointer[.scaleMthods + pdx * pointer.size]
		@@:
		movptr [retFromScale], xmm3
		sub eax, [preparedRect.right + pcx]
		add eax, [preparedRect.left + pcx]
		jmp [retFromScale]

	.centerScale:
		mov edx, [preparedRect.left + pcx]
		sub edx, eax
		movd xmm3, edx

		imul eax, [.new + pcx]
		xor edx, edx
		div dword[.def + pcx]

		movd edx, xmm0
		jecxz @f
			movd edx, xmm1
		@@:
		sub edx, eax
		movd xmm4, edx
		psubd xmm3, xmm4
		movd edx, xmm3
		psrlq xmm3, 1
		paddd xmm0, xmm3
		jecxz @f
			psubd xmm0, xmm3
			psubd xmm1, xmm3
		@@:
		sar edx, 1
		adc edx, 0
		sub eax, edx
		; movd edx, xmm3
		; sub edx, eax
		; sar edx, 1
		; sub eax, edx
		; lea eax, [eax + edx * 2]
		jmp [retFromScale]
endp

proc DIALOGFORM.scaleChilds c uses pbx, this
	virtObj .this:arg DIALOGFORM at pbx from @arg1
	local sizesRect:RECT, defaultSize:SIZE
	@call [GetClientRect]([.this.hWnd], addr sizesRect)
	@call CNV::fill(addr sizesRect.left, addr .this.sData.baseRect.right, sizeof.POINT)
	@call CNV::fill(addr defaultSize, addr .this.sData.defaultSize, sizeof.POINT)
	@call [EnumChildWindows]([.this.hWnd], DIALOGFORM.ScaleChildsEnumProc, addr sizesRect)
	ret
endp

; proc DIALOGFORM.__ScaleChildRectsEnumProc uses pbx psi, hWnd, lParam
; 	@sarg @arg1, @arg2
; 	@larg pbx, @arg2

; 	virtual at pbx
; 		.old POINT
; 		.new POINT
; 	end virtual
	
; 	local retFromScale:POINTER

; 	@call [GetWindowPtrA]([hWnd], GWL_USERDATA)
; 	test eax, eax
; 		jz .return

; 	virtObj .cntr:arg CONTROL at psi from pax
; 	cmp [.cntr.ID], 0
; 		jz .return

; 	mov ecx, POINT.x
; 	mov eax, [.cntr.sData.baseRect.left]
; 	imul eax, [.new + pcx]
; 	xor edx, edx
; 	div dword[.old + pcx]
; 	movd xmm0, eax
	
; 	; mov ecx, POINT.x
; 	mov eax, [.cntr.sData.baseRect.right]
; 	imul eax, [.new + pcx]
; 	xor edx, edx
; 	div dword[.old + pcx]
; 	movd xmm2, eax

; 	mov ecx, POINT.y
; 	mov eax, [.cntr.sData.baseRect.top]
; 	imul eax, [.new + pcx]
; 	xor edx, edx
; 	div dword[.old + pcx]
; 	movd xmm1, eax

; 	; mov ecx, POINT.y
; 	mov eax, [.cntr.sData.baseRect.bottom]
; 	imul eax, [.new + pcx]
; 	xor edx, edx
; 	div dword[.old + pcx]
; 	movd xmm3, eax

; 	movd [.cntr.sData.baseRect.left], xmm0
; 	movd [.cntr.sData.baseRect.top], xmm1
; 	movd [.cntr.sData.baseRect.right], xmm2
; 	movd [.cntr.sData.baseRect.bottom], xmm3
; 	psubd xmm2, xmm0
; 	psubd xmm3, xmm1

; 	cmp dword[.cntr.sData.scaleMode], -1
; 		je .return
		
; 	@call [MoveWindow]([hWnd], xmm0, xmm1, xmm2, xmm3, 1)
; 	.return: 
; 	mov eax, 1
; 	ret
; endp

; proc DIALOGFORM.__scaleChildRects c uses pbx, this
; 	virtObj .this:arg DIALOGFORM at pbx from @arg1
; 	local sizesRect:RECT
; 	@call [GetClientRect]([.this.hWnd], addr sizesRect)
; 	; movq xmm0, qword[.this.baseRect.right]
; 	; movq qword[sizesRect.left], xmm0
; 	; @call CNV::fill(addr sizesRect.left, addr .this.sData.baseRect.right, sizeof.POINT)
; 	; @call [EnumChildWindows]([.this.hWnd], DIALOGFORM.__ScaleChildRectsEnumProc, addr sizesRect)
; 	; @call CNV::fill(addr .this.sData.baseRect.right, addr sizesRect.right, sizeof.POINT)
; 	@call CNV::fill(addr .this.sData.defaultSize, addr sizesRect.right, sizeof.POINT)
; 	ret
; endp

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

; macro DIALOGFORM.setText this, lpCstr{
; 	local _this
; 	inlineObj _this, this, pcx
; 	@call [SetWindowTextA]([_this + DIALOGFORM.hWnd], lpCstr)
; }


proc_resprologue
