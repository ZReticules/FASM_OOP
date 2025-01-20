proc_noprologue

proc COMBOBOX.initEdit c uses pbx, this
	virtObj .this:arg COMBOBOX at pbx from @arg1
	locals 
		cbInfo COMBOBOXINFO
	endl
	@call [SendMessageA]([.this.hWnd], CB_GETCOMBOBOXINFO, NULL, addr cbInfo)
	mov pax, [cbInfo.hwndItem]
	mov [.this.cEdit.hWnd], pax
	mov [.this.cEdit.ID], 0
	@call [SetWindowPtrA]([cbInfo.hwndItem], GWL_USERDATA, addr .this.cEdit)
	@call [GetClientRect]([cbInfo.hwndItem], addr .this.cEdit.baseRect)
	@call CNV:fill(addr .this.cEdit.baseRect, addr .this.baseRect, sizeof.RECT)
	ret
endp

proc COMBOBOX.initListBox c uses pbx, this
	virtObj .this:arg COMBOBOX at pbx from @arg1
	locals 
		cbInfo COMBOBOXINFO
	endl
	@call [SendMessageA]([.this.hWnd], CB_GETCOMBOBOXINFO, NULL, addr cbInfo)
	mov pax, [cbInfo.hwndList]
	mov [.this.cListBox.hWnd], pax
	mov [.this.cListBox.ID], 0
	@call [SetWindowPtrA]([cbInfo.hwndList], GWL_USERDATA, addr .this.cListBox)
	@call CNV:fill(addr .this.cListBox.baseRect, addr .this.baseRect, sizeof.RECT)
	ret
endp

proc COMBOBOX.initSubControl c uses pbx, this
	virtObj .this:arg COMBOBOX at pbx from @arg1
	locals 
		cbInfo COMBOBOXINFO
	endl
	@call [SendMessageA]([.this.hWnd], CB_GETCOMBOBOXINFO, NULL, addr cbInfo)
	mov pax, [cbInfo.hwndItem]
	mov [.this.cEdit.hWnd], pax
	mov pax, [.this.hWnd]
	cmp pax, [cbInfo.hwndItem]
	jne @f
		@call c [printf]("lol\n")
	@@:
	mov [.this.cEdit.ID], 0
	@call [SetWindowPtrA]([cbInfo.hwndItem], GWL_USERDATA, addr .this.cEdit)
	@call [GetClientRect]([cbInfo.hwndItem], addr .this.cEdit.baseRect)
	@call CNV:fill(addr .this.cEdit.baseRect, addr .this.baseRect, sizeof.RECT)
	mov pax, [cbInfo.hwndList]
	mov [.this.cListBox.hWnd], pax
	mov [.this.cListBox.ID], 0
	@call [SetWindowPtrA]([cbInfo.hwndList], GWL_USERDATA, addr .this.cListBox)
	@call CNV:fill(addr .this.cListBox.baseRect, addr .this.baseRect, sizeof.RECT)
	ret
endp

proc_resprologue
