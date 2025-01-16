proc_noprologue

proc COMBOBOX.initSubControl c uses pbx, this
	virtObj .this:arg COMBOBOX at pbx from @arg1
	locals 
		cbInfo COMBOBOXINFO
	endl
	@call [SendMessageA]([.this.hWnd], CB_GETCOMBOBOXINFO, NULL, addr cbInfo)
	mov pax, [cbInfo.hwndItem]
	mov [.this.cEdit.hWnd], pax
	@call [SetWindowLongPtrA]([cbInfo.hwndItem], GWL_USERDATA, addr .this.cEdit)
	mov pax, [cbInfo.hwndList]
	mov [.this.cListBox.hWnd], pax
	@call [SetWindowLongPtrA]([cbInfo.hwndList], GWL_USERDATA, addr .this.cListBox)
	ret
endp

proc_resprologue
