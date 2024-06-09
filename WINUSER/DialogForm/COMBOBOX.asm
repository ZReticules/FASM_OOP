proc COMBOBOX.addItem, this, lpString
	virtObj .this:arg COMBOBOX
	mov r9, rdx
	@call [SendMessageA]([.this.hWnd], CB_ADDSTRING, NULL)
	ret
endp

proc COMBOBOX.setSelected, this, idItem
	virtObj .this:arg COMBOBOX
	mov r8, rdx
	@call [SendMessageA]([.this.hWnd], CB_SETCURSEL, r8, NULL)
	ret
endp

proc COMBOBOX.getSelected, this, idItem
	virtObj .this:arg COMBOBOX
	@call [SendMessageA]([.this.hWnd], CB_GETCURSEL, NULL, NULL)
	ret
endp

proc COMBOBOX.setItemData, this, idItem, dataValue
	virtObj .this:arg COMBOBOX at rcx
	mov r9, r8
	mov r8, rdx
	@call [SendMessageA]([.this.hWnd], CB_SETITEMDATA)
	ret
endp

proc COMBOBOX.getItemData, this, idItem
	virtObj .this:arg COMBOBOX at rcx
	mov r8, rdx
	@call [SendMessageA]([.this.hWnd], CB_GETITEMDATA, r8, NULL)
	ret
endp

proc COMBOBOX.clear, this
	virtObj .this:arg COMBOBOX at rcx
	@call [SendMessageA]([.this.hWnd], CB_RESETCONTENT, NULL, NULL)
	ret
endp

proc COMBOBOX.findItem, this, idBefore, strLp
	virtObj .this:arg COMBOBOX at rcx
	mov r9, r8
	mov r8, rdx
	@call [SendMessageA]([.this.hWnd], CB_FINDSTRING)
	ret
endp

proc COMBOBOX.getItemTextLen, this, idItem
	virtObj .this:arg COMBOBOX
	mov r8, rdx
	@call [SendMessageA]([.this.hWnd], CB_GETLBTEXTLEN, r8, NULL)
	ret
endp

proc COMBOBOX.getItemText, this, idItem, lpString
	virtObj .this:arg COMBOBOX
	mov r9, r8
	mov r8, rdx
	@call [SendMessageA]([.this.hWnd], CB_GETLBTEXT)
	ret
endp

; a = COMBOBOX.initSubControl
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
	@call [SetWindowLongPtrA]([cbInfo.hwndList], GWL_USERDATA, addr .this.cListBox)
	; @call [printf]("%d - %d", qword [cbInfo.hwndCombo], [.this.hWnd])
	ret
endp