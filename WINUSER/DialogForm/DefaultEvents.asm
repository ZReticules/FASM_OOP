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

macro @set_min_max_sizes x=0, y=0, cx=0x7FFFFFFF, cy=0x7FFFFFFF{
	WM_GETMINMAXINFO event DIALOGFORM_WM_GETMINMAXINFO
	minMaxRect RECT x, y, cx, cy
}

proc_noprologue

; proc DIALOGFORM_WM_CTLCOLOR uses pbx psi, lpForm, lpParams, lpEventData
; 	@sarg @arg1
; 	virtObj .params:arg params at pbx from @arg2
; 	; @call c [puts]("lol")
; 	@call [GetWindowPtrA]([.params.lParam], GWL_USERDATA)
; 	; int3
; 	test eax, eax
; 	jz .noVal
; 		virtObj .control:arg DLG_SPECIAL_CONTROL at psi from pax
; 		@call [SetBkColor]([.params.wParam], [.control.bkColor])
; 		@call [SetTextColor]([.params.wParam], [.control.txColor])
; 		mov pax, [.control.bgColorBrush]
; 		test pax, pax
; 			jnz .noVal
; 		virtObj .form:arg DIALOGFORM at psi from [lpForm]
; 		mov pax, [.form.hWnd]
; 	.noVal:
; 	ret
; endp

; proc ret_white, lpForm, lpParams, lpEventData
; 	@call c [printf]("lol")
; 	@call [GetStockObject](BLACK_BRUSH)
; 	ret
; endp

; proc DIALOGFORM_WM_CTLCOLORDLG, lpForm, lpParams, lpEventData
; 	; int3
; 	@larg pcx, @arg1
; 	mov pax, [pcx + DIALOGFORM.bgColorBrush]
; 	ret
; endp

proc DIALOGFORM_WM_GETMINMAXINFO, lpForm, lpParams, lpEventData
	@sarg @arg3
	@larg pdx, @arg2
	virtObj .minMaxInfo MINMAXINFO at pax from [pdx + params.lParam]
	@call CNV::fill(addr .minMaxInfo.ptMinTrackSize, @arg3, sizeof.RECT)
	mov eax, 0
	ret
endp

proc DIALOGFORM_WM_SIZE uses pbx, lpForm, lpParams, lpEventData
	virtObj .form:arg DIALOGFORM at pbx from @arg1
	; local NewRect:RECT
	; @call [GetClientRect]([.form.hWnd], addr NewRect)
	; ; movq xmm0, qword[.form.baseRect.right]
	; ; movq qword[NewRect.left], xmm0
	; @call CNV::fill(addr NewRect.left, addr .form.sData.baseRect.right, sizeof.POINT)
	; ; int3
	; @call [EnumChildWindows]([.form.hWnd], DIALOGFORM.EnumChildsProc, addr NewRect)
	@call .form->scaleChilds()
	@call [InvalidateRect]([.form.hWnd], NULL, 1)
	; @call [UpdateWindow]([.form.hWnd])
	xor eax, eax
	ret
endp

; macro @on_ctlbtncolor{
; 	NM_CUSTOMDRAW notify COLORED_BUTTON_NM_CUSTOMDRAW
; }

; proc COLORED_BUTTON_NM_CUSTOMDRAW uses pbx psi pdi, lpForm, lpNmhdr, lpControl, lpEventData
; 	mov eax, CDRF_DODEFAULT
; 	; int3
; 	virtObj .form:arg DIALOGFORM at pbx
; 	virtObj .nmhdr NMCUSTOMDRAW at pdi from @arg2
; 	virtObj .cntrl:arg BUTTON at psi
; 	local hBrush:POINTER, hFont:dptr 0, textLen:POINTER, lpText:POINTER, logBrush:LOGBRUSH, hRgn:POINTER
; 	; int3
; 	cmp dword[.nmhdr.dwDrawStage], CDDS_PREPAINT
; 	jne .no_prepaint
; 		mov pbx, @arg1
;         @call [SetWindowPtrA]([.form.hWnd], 0, CDRF_NOTIFYPOSTPAINT or CDRF_NOTIFYPOSTERASE)
;         mov eax, 1
; 	.no_prepaint:
; 	; cmp dword[.nmhdr.dwDrawStage], CDDS_POSTERASE
; 	; jne .no_erase
; 	; 	@call [GetStockObject](WHITE_BRUSH)
; 	; 	@call [FillRect]([.nmhdr.hdc], addr .nmhdr.rc, pax)
; 	; .no_erase:
; 	cmp dword[.nmhdr.dwDrawStage], CDDS_POSTPAINT
; 	jne .no_paint
; 		mov pbx, @arg1
; 		mov psi, @arg3
; 		; inc [.nmhdr.rc.left]
; 		; inc [.nmhdr.rc.top]
; 		; @call [CreateRoundRectRgn]([.nmhdr.rc.left], [.nmhdr.rc.top], [.nmhdr.rc.right], [.nmhdr.rc.bottom], 5, 5)
; 		; dec [.nmhdr.rc.left]
; 		; dec [.nmhdr.rc.top]
; 		; @call [SelectObject]([.nmhdr.hdc], pax)
; 		; mov [hRgn], pax
; 		; @call [DrawThemeParentBackground]([.form.hTheme], [.nmhdr.hdc], addr .nmhdr.rc)
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
;         ; @call [DrawThemeBackground]([.form.hTheme], [.nmhdr.hdc], [.nmhdr.dwItemSpec], PBS_HOT, addr .nmhdr.rc, NULL)
; 		; @call [SendMessageA]([.cntrl.hWnd], WM_PRINTCLIENT, [.nmhdr.hdc], PRF_CLIENT)
;         cmp [.cntrl.bgColorBrush], 0
;         je .no_color
; 	        ; @call [PatBlt]([.nmhdr.hdc],\ 
; 	        ;     [.nmhdr.rc.left],\ 
; 	        ;     [.nmhdr.rc.top],\
; 	        ;     [.nmhdr.rc.right],\
; 	        ;     [.nmhdr.rc.bottom],\
; 	        ;     DSTINVERT)
; 	        @call [GetObjectA]([.cntrl.bgColorBrush], sizeof.LOGBRUSH, addr logBrush)
; 	        mov eax, [logBrush.lbColor]
; 	        not eax
; 	        and eax, 0xFFFFFF
; 	        @call [CreateSolidBrush](pax)
; 	        @call [SelectObject]([.nmhdr.hdc], pax)
; 	        mov [hBrush], pax
; 	        @call [PatBlt]([.nmhdr.hdc],\ 
; 	            [.nmhdr.rc.left],\ 
; 	            [.nmhdr.rc.top],\
; 	            [.nmhdr.rc.right],\
; 	            [.nmhdr.rc.bottom],\
; 	            PATINVERT)
; 	        @call [SelectObject]([.nmhdr.hdc], [hBrush])
; 	        @call [DeleteObject](pax)
; 	        ; @call [SelectObject]([.nmhdr.hdc], [hRgn])
; 	        ; @call [DeleteObject](pax)
; 	        ; test [.nmhdr.uItemState], CDIS_FOCUS
; 	        ; jz @f
; 	        ; 	@call [DrawFocusRect]([.nmhdr.hdc], addr .nmhdr.rc)
; 	        ; @@:
;         .no_color:
;         ; @call [SendMessageA]([.cntrl.hWnd], WM_GETFONT, 0, 0)
;         ; test eax, eax
;         ; jz .no_font
;         ; 	@call [SelectObject]([.nmhdr.hdc], pax)
;         ; 	mov [hFont], pax
;         ; .no_font:
;         ; @call .cntrl->getTextLen()
;         ; lea pax, [pax + 1]
;         ; mov [textLen], pax
;         ; @call CNV::alloc(pax)
;         ; mov [lpText], pax
;         ; @call .cntrl->getText([lpText], [textLen])
;         ; @call [SetBkMode]([.nmhdr.hdc], TRANSPARENT)
;         ; @call [SetTextColor]([.nmhdr.hdc], [.cntrl.txColor])
;         ; @call [DrawTextA]([.nmhdr.hdc], [lpText], [textLen], addr .nmhdr.rc, DT_CENTER or DT_SINGLELINE or DT_VCENTER)
;         ; @call CNV::free([lpText])
;         ; cmp [hFont], 0
;         ; jne .no_return_font
;         ; 	@call [SelectObject]([.nmhdr.hdc], [hFont])
;         ; .no_return_font:
;         ; test [.nmhdr.uItemState], CDIS_FOCUS
;         ; jz @f
;         ; 	@call [DrawFocusRect]([.nmhdr.hdc], addr .nmhdr.rc)
;         ; @@:
;         ; @call [SetWindowPtrA]([.form.hWnd], 0, CDRF_DOERASE)
; 		mov eax, 1
; 	.no_paint:
; 	ret
; endp

proc_resprologue
