proc EDIT.setReadOnly, this, isReadOnly
	virtObj .this:arg EDIT
	mov r8, rdx
	@call [SendMessageA]([.this.hWnd], EM_SETREADONLY)
	ret
endp