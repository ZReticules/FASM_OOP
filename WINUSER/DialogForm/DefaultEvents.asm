importlib user32,\
	SetWindowPos

macro @on_colors{
	WM_CTLCOLORDLG  		event DIALOGFORM_WM_CTLCOLORDLG
	WM_CTLCOLOREDIT  		event DIALOGFORM_WM_CTLCOLOR
	WM_CTLCOLORLISTBOX  	event DIALOGFORM_WM_CTLCOLOR
	WM_CTLCOLORSCROLLBAR  	event DIALOGFORM_WM_CTLCOLOR
	WM_CTLCOLORSTATIC  		event DIALOGFORM_WM_CTLCOLOR
}

macro @on_scaling{
	WM_SIZE event DIALOGFORM_WM_SIZE
}

proc DIALOGFORM_WM_CTLCOLOR uses pbx psi, formLp, paramsLp
	virtObj .params:arg params at pbx from @arg2
	@call [GetWindowPtrA]([.params.lparam], GWL_USERDATA)
	test eax, eax
	jz .noVal
		virtObj .control:arg _DLG_SPECIAL_CONTROL at psi from pax
		@call [SetBkColor]([.params.wparam], [.control.bkColor])
		@call [SetTextColor]([.params.wparam], [.control.txColor])
		mov pax, [.control.bgColorBrush]
	.noVal:
	ret
endp

proc DIALOGFORM_WM_CTLCOLORDLG, formLp, paramsLp
	@larg pcx, @arg1
	mov pax, [pcx + DIALOGFORM.bgColorBrush]
	ret
endp

struct DIALOGFORM.DOUBLE_POINT
	x 		dq ?
	y 		dq ?
ends

proc DIALOGFORM.EnumChildsProc uses pbx, hWnd, lParam
	@sarg @arg1, @arg2
	@larg pbx, @arg2
	virtual at pbx
		.old POINT
		.new POINT
	end virtual
	local NewSize:RECT
	@call [GetWindowPtrA]([hWnd], GWL_USERDATA)
	test eax, eax
		jz .return
	virtObj .cntr:arg CONTROL at pcx from pax
	cmp [.cntr.ID], 0
		jz .return
	mov eax, [.cntr.baseRect.left]
	imul eax, [.new.x]
	xor edx, edx
	div [.old.x]
	movd xmm0, eax
	mov eax, [.cntr.baseRect.top]
	imul eax, [.new.y]
	xor edx, edx
	div [.old.y]
	movd xmm1, eax
	mov eax, [.cntr.baseRect.right]
	imul eax, [.new.x]
	xor edx, edx
	div [.old.x]
	movd xmm2, eax
	mov eax, [.cntr.baseRect.bottom]
	imul eax, [.new.y]
	xor edx, edx
	div [.old.y]
	movd xmm3, eax
	; @call [SetWindowPos]([hWnd], HWND_TOP, xmm0, edx, eax, ecx, 0x0020 or 0x400)
	@call [MoveWindow]([hWnd], xmm0, xmm1, xmm2, xmm3, 1)
	; mov pax, [lpCntrl]
	; movzx pax, [.cntr.ID]
	; @call c [printf]("%d\n", pax)
	.return: 
	mov eax, 1
	ret
endp

proc DIALOGFORM_WM_SIZE uses pbx, lpForm, lpParams
	virtObj .this:arg DIALOGFORM at pbx from @arg1
	local NewRect:RECT, Coeffs:DIALOGFORM.DOUBLE_POINT
	@call [GetClientRect]([.this.hWnd], addr NewRect)
	movq xmm0, qword[.this.baseRect.right]
	movq qword[NewRect.left], xmm0
	@call [EnumChildWindows]([.this.hWnd], DIALOGFORM.EnumChildsProc, addr NewRect)
	@call [InvalidateRect]([.this.hWnd], NULL, 0)
	xor eax, eax
	ret
endp
