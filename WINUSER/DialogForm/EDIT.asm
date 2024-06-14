proc EDIT.setReadOnly, this, isReadOnly
	virtObj .this:arg EDIT
	mov r8, rdx
	@call [SendMessageA]([.this.hWnd], EM_SETREADONLY)
	ret
endp

proc EDIT.addText uses rbx, this
	virtObj .this:arg EDIT at rbx
	locals 
		_start dd ?
		_end dd ?
		strLp dq ?
	endl
	mov rbx, rcx
	mov [strLp], rdx
	@call [SendMessageA]([.this.hWnd], EM_GETSEL, addr _start, addr _end)
	@call .this->getTextLen()
	@call [SendMessageA]([.this.hWnd], EM_SETSEL, rax, rax)
	@call [SendMessageA]([.this.hWnd], EM_REPLACESEL, 0, [strLp])
	@call [SendMessageA]([.this.hWnd], EM_SETSEL, [_start], [_end])
	ret
endp