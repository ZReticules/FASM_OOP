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

proc DIALOGFORM.EnumChildsProc uses pbx psi, hWnd, lParam
	@sarg @arg1, @arg2
	virtObj .point:arg DIALOGFORM.DOUBLE_POINT at pbx from @arg2
	; @call c [puts]("lol")
	local NewSize:RECT
	@call [GetWindowPtrA]([hWnd], GWL_USERDATA)
	virtObj .cntr:arg CONTROL at pax
	cvtsi2sd xmm0, [.cntr.baseRect.left]
	cvtsi2sd xmm1, [.cntr.baseRect.right]
	cvtsi2sd xmm2, [.cntr.baseRect.bottom]
	cvtsi2sd xmm3, [.cntr.baseRect.top]
	movq xmm4, [.point.x]
	movq xmm5, [.point.y]
	mulsd xmm0, xmm4
	mulsd xmm1, xmm4
	mulsd xmm2, xmm5
	mulsd xmm3, xmm5
	cvtsd2si eax, xmm0
	movd xmm0, eax
	cvtsd2si eax, xmm1
	cvtsd2si ecx, xmm2
	cvtsd2si edx, xmm3
	; cvtsd2si xmm0, xmm3
	; int3
	mov ebx, eax
	movd esi, xmm0
	; @call [SetWindowPos]([hWnd], HWND_TOP, float xmm0, edx, eax, ecx, 0x0020)
	@call [MoveWindow]([hWnd], xmm0, edx, eax, ecx, 1)
	mov eax, 1
	ret
endp

proc DIALOGFORM_WM_SIZE uses pbx, lpForm, lpParams
	virtObj .this:arg DIALOGFORM at pbx from @arg1
	; @call c [printf]("lol")
	local NewRect:RECT, Coeffs:DIALOGFORM.DOUBLE_POINT
	@call [GetClientRect]([.this.hWnd], addr NewRect)
	; @call c [printf]("%d:%d\n", [NewRect.right], [NewRect.bottom])
	cvtsi2sd xmm0, [NewRect.right]
	cvtsi2sd xmm1, [.this.baseRect.right]
	cvtsi2sd xmm2, [NewRect.bottom]
	cvtsi2sd xmm3, [.this.baseRect.bottom]
	divsd xmm0, xmm1
	divsd xmm2, xmm3
	movq [Coeffs.x], xmm0
	movq [Coeffs.y], xmm2
	; @call CNV:fill(addr .this.baseRect, addr NewRect, sizeof.RECT)
	; mov pax, [.this.hWnd]
	; mov [Coeffs.hParent], pax
	@call [EnumChildWindows]([.this.hWnd], DIALOGFORM.EnumChildsProc, addr Coeffs)
	@call [InvalidateRect]([.this.hWnd], NULL, 0)
	; @call c [puts]("kek")
	xor eax, eax
	ret
endp
