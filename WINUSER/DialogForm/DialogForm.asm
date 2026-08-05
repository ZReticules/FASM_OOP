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

.proc_frame_mode_static

; .proc DIALOGFORM.start, this, parent
; 	virtObj .this DIALOGFORM
; 	mov [this], rcx
; 	mov [parent], rdx
; 	mov [.this.__close], DIALOGFORM_WM_CLOSE
; 	$call [GetModuleHandleA](0)
; 	mov rcx, [this]
; 	$call [DialogBoxIndirectParamA](rax, [.this.hDialogTemplate], [parent], [.this.lpDialogFunc], rcx)
; 	ret
; .endp

macro DIALOGFORM.start this, parent=NULL{
	local _this
	inlineObj _this, this, pcx
	mov [_this + DIALOGFORM.__close], DIALOGFORM_WM_CLOSE
	$call [DialogBoxIndirectParamA]([WND.hModule], [_this + DIALOGFORM.hDialogTemplate], parent, DLG.DialogProc, addr _this)
}

macro DIALOGFORM.setIcon this, hIcon{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DIALOGFORM.hWnd], WM_SETICON, ICON_BIG, hIcon)
}

macro DIALOGFORM.close this, result=NULL{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DIALOGFORM.hWnd], WM_CLOSE, result, 0)
}

.proc cdecl DIALOGFORM.startNM(.pthis, .parent) uses pbx
	virtObj .this DIALOGFORM at pbx from @arg1
	xor eax, eax
	cmp [.this.hWnd], 0
	jne .alreadyExists
		mov [.this.__close], DIALOGFORM_WM_CLOSE_NOMODAL
		$call [CreateDialogIndirectParamA]([WND.hModule], [.this.hDialogTemplate], @arg2, DLG.DialogProc, pbx)
		mov [.this.hWnd], pax
	.alreadyExists:
	ret
.endp


.proc stdcall DIALOGFORM_WM_CLOSE(.lpForm, .lpParam, .exitVal) uses pbx
	@sarg @arg3
	virtObj .form DIALOGFORM at pbx from @arg1
    ; $call [CloseThemeData]([.form.hTheme])
	$call .form.sData.mapDialog::unmake()
	$call [EndDialog]([.form.hWnd], [.exitVal])
	mov [.form.hWnd], 0
	ret
.endp

.proc stdcall DIALOGFORM_WM_CLOSE_NOMODAL(.lpForm, .lpParam, .exitVal) uses pbx
	virtObj .form DIALOGFORM at pbx from @arg1
    ; $call [CloseThemeData]([.form.hTheme])
	$call .form.sData.mapDialog::unmake()
	$call [DestroyWindow]([.form.hWnd])
	mov [.form.hWnd], 0
	$call [PostQuitMessage](0)
	ret
.endp

; .proc DIALOGFORM_WM_CTLCOLORDLG, formLp, paramsLp
; 	virtObj .params params
; 	$call [GetWindowLongPtrA]([.params.lparam], GWL_USERDATA)
; 	test rax, rax
; 	jz .noVal
; 		virtObj .control CONTROL at rax
; 		mov rax, [.control.bgColorBrush]
; 	.noVal:
; 	ret
; .endp

; .proc cdecl DIALOGFORM.setBgColor c, this, colorref
; 	virtObj .this DIALOGFORM at pcx from @arg1
; 	@sarg @arg1
; 	@larg pdx, @arg2
; 	cmp [.this.bgColorBrush], NULL
; 	je .emptyColor
; 		@sarg @arg2
; 		$call [DeleteObject]([.this.bgColorBrush])
; 		mov pdx, [colorref]
; 	.emptyColor:
; 	$call [CreateSolidBrush](pdx)
; 	mov pcx, [this]
; 	mov [.this.bgColorBrush], pax
; 	ret
; .endp

.proc cdecl DIALOGFORM.setCaptionColor(.pthis, .colorref)
	virtObj .this DIALOGFORM at pcx from @arg1
	@sarg @arg2
	$call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.CAPTION_COLOR, &.colorref, 4)
	ret
.endp

.proc cdecl DIALOGFORM.setTextColor(.pthis, .colorref)
	virtObj .this DIALOGFORM at pcx from @arg1
	@sarg @arg2
	$call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.TEXT_COLOR, &.colorref, 4)
	ret
.endp

.proc cdecl DIALOGFORM.setBorderColor(.pthis, .colorref)
	virtObj .this DIALOGFORM at pcx from @arg1
	@sarg @arg2
	$call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.BORDER_COLOR, &.colorref, 4)
	ret
.endp

.proc cdecl DIALOGFORM.setCornerType(.pthis, .rectType)
	virtObj .this DIALOGFORM at pcx from @arg1
	@sarg @arg2
	$call [DwmSetWindowAttribute]([.this.hWnd], DWMWA.WINDOW_CORNER_PREFERENCE, &.rectType, 4)
	ret
.endp

.proc cdecl DIALOGFORM.dispatchMessages(.pthis) uses pbx
	virtObj .this DIALOGFORM at pbx from @arg1
	.local .msg:MSG
	$call [GetMessageA](&.msg, NULL, 0, 0)
	test eax, eax
	jnz .noEnd
		mov ecx, 1
		mov pdx, [.this.hWnd]
		cmp [.msg.hwnd], pdx
		cmovne eax, ecx
		jmp .return
	.noEnd:
	$call [GetActiveWindow]()
	mov ecx, eax
	$call [IsDialogMessageA](ecx, &.msg)
	test eax, eax
	jnz .return
		$call [TranslateMessage](&.msg)
		$call [DispatchMessageA](&.msg)
		mov eax, 1
	.return: ret
.endp

struct DIALOGFORM.ScaleContext
	base	RECT 
	union
		client RECT
		struct
			offset	POINT
			new		SIZE
		ends
	ends
	union
		defaultGeometry RECT
		struct
			defaultPos 	POINT
			defaultSize	SIZE
		ends
	ends
ends

.proc cdecl DIALOGFORM.ScaleChildsEnumProc(.p_cntrl, .lParam) uses pbx psi pdi
	virtObj .cntrl CONTROL at psi from @arg1
	virtObj .ctx DIALOGFORM.ScaleContext at pbx from @arg2
	
	.local .preparedRect:RECT
	lea pdi, [.preparedRect]

	cmp [.cntrl.ID], 0
		jz .return

	mov eax, [.cntrl.sData.baseRect.left]
	imul eax, [.ctx.defaultSize.cx]
	xor edx, edx
	div dword[.ctx.base.right]
	sub eax, [.ctx.defaultGeometry.left]
	mov [.preparedRect.left], eax
	
	mov eax, [.cntrl.sData.baseRect.right]
	imul eax, [.ctx.defaultSize.cx]
	xor edx, edx
	div dword[.ctx.base.right]
	sub eax, [.ctx.defaultGeometry.left]
	mov [.preparedRect.right], eax

	mov eax, [.cntrl.sData.baseRect.top]
	imul eax, [.ctx.defaultSize.cy]
	xor edx, edx
	div dword[.ctx.base.bottom]
	sub eax, [.ctx.defaultGeometry.top]
	mov [.preparedRect.top], eax

	mov eax, [.cntrl.sData.baseRect.bottom]
	imul eax, [.ctx.defaultSize.cy]
	xor edx, edx
	div dword[.ctx.base.bottom]
	sub eax, [.ctx.defaultGeometry.top]
	mov [.preparedRect.bottom], eax


	.scaleMthods @const dptr .justScale,\ 
		.saveStartPos,\ 
		.saveEndDist,\ 
		.saveCenterDist,\
		.saveSizeAtLeft,\	; _c[x/y]_sticker only 
		.saveSizeAtRight,\	; _[x/y]_sticker only
		.centerScale		; _c[x/y]_sticker only
							; only if _[x/y]_sticker = 0
							; align control at center in autoscale
	cmp [.cntrl.ID], 0
		jz .return
	cmp dword[.cntrl.sData.scaleMode], -1
		je .return

	mov ecx, SIZE.cx
	mov eax, [.preparedRect.left]
	movzx edx, [.cntrl.sData.scaleMode.x]
	$call dword xmm0 = c pointer[.scaleMthods + pdx * pointer.size]()
	
	; mov ecx, SIZE.cx
	mov eax, [.preparedRect.right]
	movzx edx, [.cntrl.sData.scaleMode.rx]
	$call dword xmm2 = c pointer[.scaleMthods + pdx * pointer.size]()

	mov ecx, SIZE.cy
	mov eax, [.preparedRect.top]
	movzx edx, [.cntrl.sData.scaleMode.y]
	$call dword xmm1 = c pointer[.scaleMthods + pdx * pointer.size]()

	; mov ecx, SIZE.cy
	mov eax, [.preparedRect.bottom]
	movzx edx, [.cntrl.sData.scaleMode.ry]
	$call dword xmm3 = c pointer[.scaleMthods + pdx * pointer.size]()

	psubd xmm2, xmm0
	psubd xmm3, xmm1
	
	movq xmm4, qword[.ctx.offset]
	paddd xmm0, xmm4
	psrldq xmm4, 4
	paddd xmm1, xmm4

	cmp [.cntrl.sData.defaultGeometry.left], -1
	jne @f
		movd [.cntrl.sData.defaultPos.x], xmm0
		movd [.cntrl.sData.defaultPos.y], xmm1
		movd [.cntrl.sData.defaultSize.cx], xmm2
		movd [.cntrl.sData.defaultSize.cy], xmm3
	@@:
	$call [MoveWindow]([.cntrl.hWnd], dword xmm0, dword xmm1, dword xmm2, dword xmm3, 1)
	; $call c [printf]("%x\n", dword[.cntr.sData.scaleMode])
	.return: $return esp

	.proc cdecl .justScale
		virtObj .ctx DIALOGFORM.ScaleContext at pbx
		imul eax, [.ctx.new + pcx]
		xor edx, edx
		div dword[.ctx.defaultSize + pcx]
		ret
	.endp

	.proc cdecl .saveStartPos
		ret
	.endp

	.proc cdecl .saveEndDist
		virtObj .ctx DIALOGFORM.ScaleContext at pbx
		sub eax, [.ctx.defaultSize + pcx]
		add eax, [.ctx.new + pcx]
		ret
	.endp

	.proc cdecl .saveCenterDist
		virtObj .ctx DIALOGFORM.ScaleContext at pbx
		mov edx, [.ctx.defaultSize + pcx]
		shr edx, 1
		sub eax, edx
		mov edx, [.ctx.new + pcx]
		shr edx, 1
		add eax, edx
		ret
	.endp

	.proc cdecl .saveSizeAtLeft
		virtObj .preparedRect RECT at pdi
		sub eax, [.preparedRect.left + pcx]
		movd edx, xmm0
		jecxz @f
			movd edx, xmm1
		@@:
		add eax, edx
		ret
	.endp

	.proc cdecl .saveSizeAtRight
		virtObj .cntr CONTROL at psi
		virtObj .preparedRect RECT at pdi
		mov eax, [.preparedRect.right + pcx]
		movzx edx, [.cntr.sData.scaleMode.rx + pcx]
		$call c pointer[DIALOGFORM.ScaleChildsEnumProc.scaleMthods + pdx * pointer.size]()
		sub eax, [.preparedRect.right + pcx]
		add eax, [.preparedRect.left + pcx]
		ret
	.endp

	.proc cdecl .centerScale
		virtObj .ctx DIALOGFORM.ScaleContext at pbx
		virtObj .preparedRect RECT at pdi
		mov edx, [.preparedRect.left + pcx]
		sub edx, eax
		movd xmm3, edx

		imul eax, [.ctx.new + pcx]
		xor edx, edx
		div dword[.ctx.defaultSize + pcx]

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
		ret
	.endp
.endp

; .proc DIALOGFORM.scaleToRect(.hWndParent:HWND, .p_rect:POINTER, .idStart:DWORD, .idEnd:DWORD) uses pbx pdi
; 	xor edi, edi
; 	@block
; 		$call [GetDlgItem]([.this.hWnd], addr pdi + 1)
; 		test eax, eax
; 			jz @fb

; 		$call DLG|getPtr(pax)
; 		test pax, pax
; 			jz @fb

; 		virtObj .cntrl CONTROL at pbx from pax

; 		$call std DIALOGFORM.ScaleChildsEnumProc([.cntrl.hWnd], &.sizesRect)
; 	@endb 	< inc edi >,\
; 			< cmp edi, [.this.idCount] >,\
; 			< jb @sb >
; 	ret
; .endp

.proc cdecl DIALOGFORM.scaleChilds(.pthis) uses pbx pdi ;psi
	virtObj .this DIALOGFORM at pbx from @arg1
	.local .scaleContext:DIALOGFORM.ScaleContext
	; .local .sizesRect:RECT, .defaultSize:SIZE
	$call [GetClientRect]([.this.hWnd], &.scaleContext.client)
	movups xmm0, xword[.this.sData.baseRect]
	movups xmm1, xword[.scaleContext.client]
	movq xmm2, qword[.this.sData.defaultSize]
	
	movups xmm3, xmm0
	pslldq xmm3, 8
	psubd xmm0, xmm3

	movups xmm3, xmm1
	pslldq xmm3, 8
	psubd xmm1, xmm3

	pslldq xmm2, 8

	movups xword[.scaleContext.base], xmm0
	movups xword[.scaleContext.client], xmm1
	movups xword[.scaleContext.defaultGeometry], xmm2
	
	xor edi, edi
	@@:
		$call [GetDlgItem]([.this.hWnd], addr pdi + 1)
		$call DLG|getPtr(pax)
		add edi, [pax + CONTROL.sData.childrenCount]
		$call c DIALOGFORM.ScaleChildsEnumProc(pax, &.scaleContext)
		inc edi
	cmp edi, [.this.idCount]
	jb @b
	; $call [EnumChildWindows]([.this.hWnd], DIALOGFORM.ScaleChildsEnumProc, &.sizesRect)
	ret
.endp

.proc stdcall DIALOGFORM.layoutPosChanged(.p_form, .p_params, .p_control, .p_eventData) uses pbx pdi psi pbp
	@larg pcx, @arg1, pdx, @arg2
	mov pbp, [pcx + DIALOGFORM.hWnd]
	virtObj .winPos WINDOWPOS at pdx from [pdx + Params.lParam]

	virtObj .cntrl CONTROL at pbx from @arg3

	.local .scaleContext:DIALOGFORM.ScaleContext
	; $call CNV|fill(&.scaleContext.clientRect, [.params.lParam], sizeof.RECT)
	movups xmm0, xword[.winPos.x]
	movups xmm1, xword[.cntrl.sData.baseRect]
	movups xmm2, xword[.cntrl.sData.defaultGeometry]
	
	movaps xmm4, xmm1
	pslldq xmm4, 8
	psubd xmm1, xmm4

	movups xword[.scaleContext.client], xmm0
	movups xword[.scaleContext.base], xmm1
	movups xword[.scaleContext.defaultGeometry], xmm2

	mov edi, [.cntrl.ID]
	mov esi, [.cntrl.sData.childrenCount]
	test esi, esi
		jz .return

	add esi, edi
	@@:
		$call [GetDlgItem](pbp, addr pdi + 1)
		$call DLG|getPtr(pax)
		add edi, [pax + CONTROL.sData.childrenCount]
		$call c DIALOGFORM.ScaleChildsEnumProc(pax, &.scaleContext)
		inc edi
	cmp edi, esi
	jb @b
	.return: ret
.endp

macro DIALOGFORM.getBaseRect this, dest{
	local _this, _dest
	inlineObj _dest, dest, pdx
	inlineObj _this, this, pcx
	movups xmm0, xword[_this + DIALOGFORM.sData.baseRect]
	movups xword[_dest], xmm0
}

macro DIALOGFORM.setBaseRect this, src{
	local _this, _src
	inlineObj _src, src, pdx
	inlineObj _this, this, pcx
	movups xmm0, xword[_src]
	movups xword[_this + DIALOGFORM.sData.baseRect], xmm0
}

; .proc DIALOGFORM.__ScaleChildRectsEnumProc uses pbx psi, hWnd, .lParam
; 	@sarg @arg1, @arg2
; 	@larg pbx, @arg2

; 	virtual at pbx
; 		.old POINT
; 		.new POINT
; 	end virtual
	
; 	local .retFromScale:POINTER

; 	$call [GetWindowPtrA]([hWnd], GWL_USERDATA)
; 	test eax, eax
; 		jz .return

; 	virtObj .cntr CONTROL at psi from pax
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
		
; 	$call [MoveWindow]([hWnd], xmm0, xmm1, xmm2, xmm3, 1)
; 	.return: 
; 	mov eax, 1
; 	ret
; .endp

; .proc DIALOGFORM.__scaleChildRects c uses pbx, this
; 	virtObj .this DIALOGFORM at pbx from @arg1
; 	local sizesRect:RECT
; 	$call [GetClientRect]([.this.hWnd], addr sizesRect)
; 	; movq xmm0, qword[.this.baseRect.right]
; 	; movq qword[sizesRect.left], xmm0
; 	; $call CNV|fill(addr sizesRect.left, addr .this.sData.baseRect.right, sizeof.POINT)
; 	; $call [EnumChildWindows]([.this.hWnd], DIALOGFORM.__ScaleChildRectsEnumProc, addr sizesRect)
; 	; $call CNV|fill(addr .this.sData.baseRect.right, addr sizesRect.right, sizeof.POINT)
; 	$call CNV|fill(addr .this.sData.defaultSize, addr sizesRect.right, sizeof.POINT)
; 	ret
; .endp

; .proc DIALOGFORM.getSize, this, lpSizeFunc
; 	virtObj .this DIALOGFORM
; 	local winRect:RECT
; 	mov r8, rdx
; 	$call r8([.this.hWnd], addr winRect)
; 	mov rax, [winRect.right]
; 	sub rax, [winRect.left]
; 	mov rdx, [winRect.bottom]
; 	sub rdx, [winRect.top]
; 	cdq
; 	ret
; .endp

; .proc DIALOGFORM.getSize
; 	virtObj .this DIALOGFORM
; 	$call [GetClientRect]
; .endp

; macro DIALOGFORM.setText this, lpCstr{
; 	local _this
; 	inlineObj _this, this, pcx
; 	$call [SetWindowTextA]([_this + DIALOGFORM.hWnd], lpCstr)
; }


.proc_frame_mode_previous
