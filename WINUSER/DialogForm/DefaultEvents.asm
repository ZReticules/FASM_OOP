importlib user32,\
	SetWindowPos,\
	UpdateWindow,\
	MoveWindow,\
	EnumChildWindows,\
	DrawTextA,\
	FillRect,\
	DrawFocusRect,\
	InvalidateRect 

importlib gdi32,\
	SetBkColor,\
	SetTextColor,\
	DeleteObject,\
	PatBlt,\
	SelectObject,\
	RoundRect,\
	SetBkMode,\
	SetDCBrushColor,\
	GetObjectA,\
	GetStockObject,\
	CreateRoundRectRgn

importlib uxtheme,\
	DrawThemeBackground,\
	DrawThemeParentBackground

; macro @on_colors{
; 	WM_CTLCOLORDLG  		event DIALOGFORM_WM_CTLCOLORDLG
; 	WM_CTLCOLOREDIT  		event DIALOGFORM_WM_CTLCOLOR
; 	WM_CTLCOLORLISTBOX  	event DIALOGFORM_WM_CTLCOLOR
; 	WM_CTLCOLORBTN 			event DIALOGFORM_WM_CTLCOLOR
; 	; ; WM_CTLCOLORSCROLLBAR  	event DIALOGFORM_WM_CTLCOLOR
; 	WM_CTLCOLORSTATIC  		event DIALOGFORM_WM_CTLCOLOR
; }

macro @on_scaling {
	WM_SIZE event DIALOGFORM_WM_SIZE
} 

macro @on_scaling_redraw {
	WM_EXITSIZEMOVE event DIALOGFORM_WM_EXITSIZEMOVE
}

macro @set_min_max_sizes x=0, y=0, cx=0x7FFFFFFF, cy=0x7FFFFFFF{
	WM_GETMINMAXINFO event DIALOGFORM_WM_GETMINMAXINFO
	RECT x, y, cx, cy
}

.proc_frame_mode_static

; .proc DIALOGFORM_WM_CTLCOLOR uses pbx psi, lpForm, lpParams, lpEventData
; 	@sarg @arg1
; 	virtObj .params params at pbx from @arg2
; 	; $call c [puts]("lol")
; 	$call [GetWindowPtrA]([.params.lParam], GWL_USERDATA)
; 	; int3
; 	test eax, eax
; 	jz .noVal
; 		virtObj .control DLG_SPECIAL_CONTROL at psi from pax
; 		$call [SetBkColor]([.params.wParam], [.control.bkColor])
; 		$call [SetTextColor]([.params.wParam], [.control.txColor])
; 		mov pax, [.control.bgColorBrush]
; 		test pax, pax
; 			jnz .noVal
; 		virtObj .form DIALOGFORM at psi from [lpForm]
; 		mov pax, [.form.hWnd]
; 	.noVal:
; 	ret
; endp

; .proc ret_white, lpForm, lpParams, lpEventData
; 	$call c [printf]("lol")
; 	$call [GetStockObject](BLACK_BRUSH)
; 	ret
; endp

; .proc DIALOGFORM_WM_CTLCOLORDLG, lpForm, lpParams, lpEventData
; 	; int3
; 	@larg pcx, @arg1
; 	mov pax, [pcx + DIALOGFORM.bgColorBrush]
; 	ret
; endp

.proc stdcall DIALOGFORM_WM_GETMINMAXINFO_(.p_form, .p_params, .p_eventData)
	@sarg @arg3
	@larg pdx, @arg2
	virtObj .minMaxInfo MINMAXINFO at pax from [pdx + params.lParam]
	$call CNV|fill(addr .minMaxInfo.ptMinTrackSize, @arg3, sizeof.RECT)
	$return 0
.endp

.proc stdcall DIALOGFORM_WM_GETMINMAXINFO(.p_form:P_DIALOGFORM, .p_params, .p_eventData)
	@sarg @arg1, @arg2, @arg3
	virtObj .form DIALOGFORM at pcx from @arg1
	; $call .form.sData.mapDialog::mapRect(@arg3)
	$call [MapDialogRect]([.form.hWnd], @arg3)
	mov pax, [.p_eventData]
	mov pointer[pax - pointer.size], DIALOGFORM_WM_GETMINMAXINFO_
	$call DIALOGFORM_WM_GETMINMAXINFO_([.p_form], [.p_params], pax)
	$return 0
.endp

.proc stdcall DIALOGFORM_WM_SIZE(.p_form, .p_params, .p_eventData) uses pbx
	virtObj .form DIALOGFORM at pbx from @arg1
	; local NewRect:RECT
	; $call [GetClientRect]([.form.hWnd], addr NewRect)
	; ; movq xmm0, qword[.form.baseRect.right]
	; ; movq qword[NewRect.left], xmm0
	; $call CNV|fill(addr NewRect.left, addr .form.sData.baseRect.right, sizeof.POINT)
	; ; int3
	; $call [EnumChildWindows]([.form.hWnd], DIALOGFORM.EnumChildsProc, addr NewRect)
	$call .form::scaleChilds()
	; $call [InvalidateRect]([.form.hWnd], NULL, 1)
	; $call [UpdateWindow]([.form.hWnd])
	$call [EnumChildWindows]([.form.hWnd], .invalidate_childs, NULL)
	$return 0

	.proc stdcall .invalidate_childs(.hWnd, .lParam)
		@sarg @arg1
		$call DLG|getPtr(@arg1)
		test eax, eax
			jz .return
		test [pax + CONTROL.sData.flags], CONTROL.UNVALIDATABLE
			jnz .return
		$call [InvalidateRect]([.hWnd], NULL, 1)
		.return: $return esp
	.endp
.endp

.proc stdcall DIALOGFORM_WM_EXITSIZEMOVE(.p_form, .p_params, .p_eventData)
	$call DIALOGFORM|invalidate(@arg1, NULL, 1)
	ret
.endp

; .proc stdcall DIALOGFORM_WM_EXITSIZEMOVE uses pbx, lpForm, lpParams, lpEventData
; 	virtObj .form DIALOGFORM at pbx from @arg1
; 	$call [InvalidateRect]([.form.hWnd], NULL, 1)
; 	$call [EnumChildWindows]([.form.hWnd], .invalidate_childs, NULL)
; 	ret
; 	.proc .invalidate_childs, hWnd, lParam
; 		@sarg @arg1
; 		$call DLG|getPtr(@arg1)
; 		test eax, eax
; 			jz .return
; 		test [pax + CONTROL.sData.flags], CONTROL.UNVALIDATABLE
; 			jnz .return
; 		$call [InvalidateRect]([hWnd], NULL, 1)
; 		.return: 
; 			mov eax, esp
; 			ret
; 	endp
; endp

; macro @on_ctlbtncolor{
; 	NM_CUSTOMDRAW notify COLORED_BUTTON_NM_CUSTOMDRAW
; }

; .proc COLORED_BUTTON_NM_CUSTOMDRAW uses pbx psi pdi, lpForm, lpNmhdr, lpControl, lpEventData
; 	mov eax, CDRF_DODEFAULT
; 	; int3
; 	virtObj .form DIALOGFORM at pbx
; 	virtObj .nmhdr NMCUSTOMDRAW at pdi from @arg2
; 	virtObj .cntrl BUTTON at psi
; 	local hBrush:POINTER, hFont:dptr 0, textLen:POINTER, lpText:POINTER, logBrush:LOGBRUSH, hRgn:POINTER
; 	; int3
; 	cmp dword[.nmhdr.dwDrawStage], CDDS_PREPAINT
; 	jne .no_prepaint
; 		mov pbx, @arg1
;         $call [SetWindowPtrA]([.form.hWnd], 0, CDRF_NOTIFYPOSTPAINT or CDRF_NOTIFYPOSTERASE)
;         mov eax, 1
; 	.no_prepaint:
; 	; cmp dword[.nmhdr.dwDrawStage], CDDS_POSTERASE
; 	; jne .no_erase
; 	; 	$call [GetStockObject](WHITE_BRUSH)
; 	; 	$call [FillRect]([.nmhdr.hdc], addr .nmhdr.rc, pax)
; 	; .no_erase:
; 	cmp dword[.nmhdr.dwDrawStage], CDDS_POSTPAINT
; 	jne .no_paint
; 		mov pbx, @arg1
; 		mov psi, @arg3
; 		; inc [.nmhdr.rc.left]
; 		; inc [.nmhdr.rc.top]
; 		; $call [CreateRoundRectRgn]([.nmhdr.rc.left], [.nmhdr.rc.top], [.nmhdr.rc.right], [.nmhdr.rc.bottom], 5, 5)
; 		; dec [.nmhdr.rc.left]
; 		; dec [.nmhdr.rc.top]
; 		; $call [SelectObject]([.nmhdr.hdc], pax)
; 		; mov [hRgn], pax
; 		; $call [DrawThemeParentBackground]([.form.hTheme], [.nmhdr.hdc], addr .nmhdr.rc)
; 		; mov edx, PBS_NORMAL
;         ; test [.nmhdr.uItemState], CDIS_FOCUS
;         ; jz @f
;         ; 	or edx, PBS_DEFAULTED_ANIMATING
;         ; 	; or edx, PBS_HOT
;         ; @@:
;         ; test [.nmhdr.uItemState], CDIS_HOT
;         ; jz @f
;         ; 	or edx, PBS_HOT
;         ; @@:
;         ; test [.nmhdr.uItemState], CDIS_SELECTED
;         ; jz @f
;         ; 	or edx, PBS_PRESSED
;         ; @@:
;         ; test [.nmhdr.uItemState], CDIS_DISABLED
;         ; jz @f
;         ; 	or edx, PBS_DISABLED
;         ; @@:
;         ; $call [DrawThemeBackground]([.form.hTheme], [.nmhdr.hdc], [.nmhdr.dwItemSpec], PBS_HOT, addr .nmhdr.rc, NULL)
; 		; $call [SendMessageA]([.cntrl.hWnd], WM_PRINTCLIENT, [.nmhdr.hdc], PRF_CLIENT)
;         cmp [.cntrl.bgColorBrush], 0
;         je .no_color
; 	        ; $call [PatBlt]([.nmhdr.hdc],\ 
; 	        ;     [.nmhdr.rc.left],\ 
; 	        ;     [.nmhdr.rc.top],\
; 	        ;     [.nmhdr.rc.right],\
; 	        ;     [.nmhdr.rc.bottom],\
; 	        ;     DSTINVERT)
; 	        $call [GetObjectA]([.cntrl.bgColorBrush], sizeof.LOGBRUSH, addr logBrush)
; 	        mov eax, [logBrush.lbColor]
; 	        not eax
; 	        and eax, 0xFFFFFF
; 	        $call [CreateSolidBrush](pax)
; 	        $call [SelectObject]([.nmhdr.hdc], pax)
; 	        mov [hBrush], pax
; 	        $call [PatBlt]([.nmhdr.hdc],\ 
; 	            [.nmhdr.rc.left],\ 
; 	            [.nmhdr.rc.top],\
; 	            [.nmhdr.rc.right],\
; 	            [.nmhdr.rc.bottom],\
; 	            PATINVERT)
; 	        $call [SelectObject]([.nmhdr.hdc], [hBrush])
; 	        $call [DeleteObject](pax)
; 	        ; $call [SelectObject]([.nmhdr.hdc], [hRgn])
; 	        ; $call [DeleteObject](pax)
; 	        ; test [.nmhdr.uItemState], CDIS_FOCUS
; 	        ; jz @f
; 	        ; 	$call [DrawFocusRect]([.nmhdr.hdc], addr .nmhdr.rc)
; 	        ; @@:
;         .no_color:
;         ; $call [SendMessageA]([.cntrl.hWnd], WM_GETFONT, 0, 0)
;         ; test eax, eax
;         ; jz .no_font
;         ; 	$call [SelectObject]([.nmhdr.hdc], pax)
;         ; 	mov [hFont], pax
;         ; .no_font:
;         ; $call .cntrl::getTextLen()
;         ; lea pax, [pax + 1]
;         ; mov [textLen], pax
;         ; $call CNV|alloc(pax)
;         ; mov [lpText], pax
;         ; $call .cntrl::getText([lpText], [textLen])
;         ; $call [SetBkMode]([.nmhdr.hdc], TRANSPARENT)
;         ; $call [SetTextColor]([.nmhdr.hdc], [.cntrl.txColor])
;         ; $call [DrawTextA]([.nmhdr.hdc], [lpText], [textLen], addr .nmhdr.rc, DT_CENTER or DT_SINGLELINE or DT_VCENTER)
;         ; $call CNV|free([lpText])
;         ; cmp [hFont], 0
;         ; jne .no_return_font
;         ; 	$call [SelectObject]([.nmhdr.hdc], [hFont])
;         ; .no_return_font:
;         ; test [.nmhdr.uItemState], CDIS_FOCUS
;         ; jz @f
;         ; 	$call [DrawFocusRect]([.nmhdr.hdc], addr .nmhdr.rc)
;         ; @@:
;         ; $call [SetWindowPtrA]([.form.hWnd], 0, CDRF_DOERASE)
; 		mov eax, 1
; 	.no_paint:
; 	ret
; endp

.proc_frame_mode_previous
