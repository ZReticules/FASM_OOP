if used COMBOBOX.addItem
	COMBOBOX.addItem:;, this, lpString
		virtObj .this:arg COMBOBOX
		mov r9, rdx
		xor r8d, r8d
		mov edx, CB_ADDSTRING
		mov rcx, [.this.hWnd]
		jmp [SendMessageA];([.this.hWnd], CB_ADDSTRING, NULL, r9)
end if

if used COMBOBOX.setSelected
	COMBOBOX.setSelected:;, this, idItem
		virtObj .this:arg COMBOBOX
		xor r9d, r9d
		mov r8, rdx
		mov edx, CB_SETCURSEL
		mov rcx, [.this.hWnd]
		jmp [SendMessageA];([.this.hWnd], CB_SETCURSEL, r8, NULL)
end if

if used COMBOBOX.getSelected
	COMBOBOX.getSelected:;, this
		virtObj .this:arg COMBOBOX
		xor r9d, r9d
		mov r8d, r9d
		mov edx, CB_GETCURSEL
		mov rcx, [.this.hWnd]
		jmp [SendMessageA];([.this.hWnd], CB_GETCURSEL, NULL, NULL)
end if

if used COMBOBOX.setItemData
	COMBOBOX.setItemData:;, this, idItem, dataValue
		virtObj .this:arg COMBOBOX
		mov r9, r8
		mov r8, rdx
		mov edx, CB_SETITEMDATA
		mov rcx, [.this.hWnd]
		jmp [SendMessageA];([.this.hWnd], CB_SETITEMDATA, r8, r9)
end if

if used COMBOBOX.getItemData
	COMBOBOX.getItemData:;, this, idItem
	virtObj .this:arg COMBOBOX
	xor r9d, r9d
	mov r8, rdx
	mov edx, CB_GETITEMDATA
	mov rcx, [.this.hWnd]
	jmp [SendMessageA];([.this.hWnd], CB_GETITEMDATA, r8, NULL)
end if

if used COMBOBOX.clear
	COMBOBOX.clear:;, this
		virtObj .this:arg COMBOBOX
		xor r9d, r9d
		mov r8d, r9d
		mov edx, CB_RESETCONTENT
		mov rcx, [.this.hWnd]
		jmp [SendMessageA];([.this.hWnd], CB_RESETCONTENT, NULL, NULL)
end if

if used COMBOBOX.findItem
	COMBOBOX.findItem:;, this, idBefore, strLp
		virtObj .this:arg COMBOBOX
		mov r9, r8
		mov r8, rdx
		mov edx, CB_FINDSTRING
		mov rcx, [.this.hWnd]
		jmp [SendMessageA];([.this.hWnd], CB_FINDSTRING, r8, r9)
end if

if used COMBOBOX.getItemTextLen
	COMBOBOX.getItemTextLen:;, this, idItem
		virtObj .this:arg COMBOBOX
		xor r8d, r8d
		mov r8, rdx
		mov edx, CB_GETLBTEXT
		mov rcx, [.this.hWnd]
		jmp [SendMessageA];([.this.hWnd], CB_GETLBTEXTLEN, r8, NULL)
end if

if used COMBOBOX.getItemText
	COMBOBOX.getItemText, this, idItem, lpString
	virtObj .this:arg COMBOBOX
	mov r9, r8
	mov r8, rdx
	mov edx, CB_GETLBTEXT
	mov rcx, [.this.hWnd]
	jmp [SendMessageA];([.this.hWnd], CB_GETLBTEXT, r8, r9)
end if

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
	lea r8, [.this.cListBox]
	mov edx, GWL_USERDATA
	mov rcx, [cbInfo.hwndList]
	pop rbx
	leave
	jmp [SetWindowLongPtrA];([cbInfo.hwndList], GWL_USERDATA, addr .this.cListBox)
endp