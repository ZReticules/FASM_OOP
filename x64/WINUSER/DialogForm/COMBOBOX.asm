proc_noprologue

proc COMBOBOX.initSubControl uses rbx, this
	virtObj .this:arg COMBOBOX at rbx
	locals 
		cbInfo COMBOBOXINFO
	endl
	mov rbx, rcx
	@call [SendMessageA]([.this.hWnd], CB_GETCOMBOBOXINFO, NULL, addr cbInfo)
	mov rax, [cbInfo.hwndItem]
	mov [.this.cEdit.hWnd], rax
	@call [SetWindowLongPtrA]([cbInfo.hwndItem], GWL_USERDATA, addr .this.cEdit)
	mov rax, [cbInfo.hwndList]
	mov [.this.cListBox.hWnd], rax
	@jret [SetWindowLongPtrA]([cbInfo.hwndList], GWL_USERDATA, addr .this.cListBox)
endp

proc_resprologue