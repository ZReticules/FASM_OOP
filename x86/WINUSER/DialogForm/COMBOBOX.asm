proc COMBOBOX.initSubControl uses ebx, this
	virtObj .this:arg COMBOBOX at ebx
	locals 
		cbInfo COMBOBOXINFO
	endl
	mov ebx, [this]
	@call [SendMessageA]([.this.hWnd], CB_GETCOMBOBOXINFO, NULL, addr cbInfo)
	mov eax, [cbInfo.hwndItem]
	mov [.this.cEdit.hWnd], eax
	@call [SetWindowLongPtrA]([cbInfo.hwndItem], GWL_USERDATA, addr .this.cEdit)
	mov eax, [cbInfo.hwndList]
	mov [.this.cListBox.hWnd], eax
	@call [SetWindowLongPtrA]([cbInfo.hwndList], GWL_USERDATA, addr .this.cListBox)
	ret
endp