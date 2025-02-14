proc_noprologue

proc COLORED_BUTTON_NM_CUSTOMDRAW uses pbx psi pdi, lpForm, lpNmhdr, lpControl
	mov eax, CDRF_DODEFAULT
	; int3
	virtObj .form:arg DIALOGFORM at pbx
	virtObj .nmhdr NMCUSTOMDRAW at pdi from @arg2
	virtObj .cntrl:arg COLORED_BUTTON at psi
	local hBrush:POINTER, hFont:dptr 0, textLen:POINTER, lpText:POINTER
	; int3
	cmp dword[.nmhdr.dwDrawStage], CDDS_PREPAINT
	jne .no_paint
		mov pbx, @arg1
		mov psi, @arg3
		mov edx, PBS_NORMAL
		mov eax, PBS_DEFAULTED_ANIMATING
        test [.nmhdr.uItemState], CDIS_FOCUS
        	cmovnz edx, eax
		mov eax, PBS_HOT
        test [.nmhdr.uItemState], CDIS_HOT
        	cmovnz edx, eax
        mov eax, PBS_PRESSED
        test [.nmhdr.uItemState], CDIS_SELECTED
        	cmovnz edx, eax
        mov eax, PBS_DISABLED
        test [.nmhdr.uItemState], CDIS_DISABLED
        	cmovnz edx, eax
        @call [DrawThemeBackground]([.form.hTheme], [.nmhdr.hdc], BP_PUSHBUTTON, pdx, addr .nmhdr.rc, NULL)
        @call [SendMessageA]([.cntrl.hWnd], WM_GETFONT, 0, 0)
        test eax, eax
        jz .no_font
        	@call [SelectObject]([.nmhdr.hdc], pax)
        	mov [hFont], pax
        .no_font:
        cmp [.cntrl.bgColorBrush], 0
        je .no_color
	        @call [SelectObject]([.nmhdr.hdc], [.cntrl.bgColorBrush])
	        mov [hBrush], pax
	        test eax, eax
	        jnz @f
	        	@call [GetLastError]()
	        	@call c [printf]("%dggg", eax)
	        @@:
	        @call [PatBlt]([.nmhdr.hdc],\ 
	            [.nmhdr.rc.left],\ 
	            [.nmhdr.rc.top],\
	            [.nmhdr.rc.right],\
	            [.nmhdr.rc.bottom],\
	            PATINVERT)
	        @call [SelectObject]([.nmhdr.hdc], [hBrush])
        .no_color:
        @call .cntrl->getTextLen()
        lea pax, [pax + 1]
        mov [textLen], pax
        @call CNV:alloc(pax)
        mov [lpText], pax
        @call .cntrl->getText([lpText], [textLen])
        @call [SetBkMode]([.nmhdr.hdc], TRANSPARENT)
        ; int3
        @call [SetTextColor]([.nmhdr.hdc], [.cntrl.txColor])
        @call [DrawTextA]([.nmhdr.hdc], [lpText], [textLen], addr .nmhdr.rc, DT_CENTER or DT_SINGLELINE or DT_VCENTER)
        @call CNV:free([lpText])
        cmp [hFont], 0
        jne .no_return_font
        	@call [SelectObject]([.nmhdr.hdc], [hFont])
        .no_return_font:
        @call [SetWindowPtrA]([.form.hWnd], 0, CDRF_SKIPDEFAULT)
		mov eax, 1
	.no_paint:
	ret
endp

proc COLORED_BUTTON.setBgColor c, this, colorref
	virtObj .this:arg COLORED_BUTTON at pcx from @arg1
	@sarg @arg1
	@larg pdx, @arg2
	cmp [.this.bgColorBrush], NULL
	je .emptyColor
		@sarg @arg2
		@call [DeleteObject]([.this.bgColorBrush])
		mov pdx, [colorref]
	.emptyColor:
	not pdx
	and pdx, 0xFFFFFF
	@call [CreateSolidBrush](pdx)
	mov pcx, [this]
	mov [.this.bgColorBrush], pax
	ret
endp

proc COLORED_BUTTON.unsetBgColor c, this
	virtObj .this:arg COLORED_BUTTON at pcx from @arg1
	xor eax, eax
	xchg pax, [.this.bgColorBrush]
	@call [DeleteObject](pax)
	ret
endp

proc_resprologue
