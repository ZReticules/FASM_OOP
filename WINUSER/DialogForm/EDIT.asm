if used EDIT.setReadOnly
	EDIT.setReadOnly:;, this, isReadOnly
		virtObj .this:arg EDIT
		mov r8, rdx
		mov edx, EM_SETREADONLY
		mov rcx, [.this.hWnd]
		jmp [SendMessageA];([.this.hWnd], EM_SETREADONLY, r8)
end if

proc EDIT.addText uses rbx, this
	virtObj .this:arg EDIT at rbx
	locals 
		_start dd ?
		_end dd ?
		strLp dq ?
	endl
	mov rbx, rcx
	frame
	mov [strLp], rdx
	@call [SendMessageA]([.this.hWnd], EM_GETSEL, addr _start, addr _end)
	@call .this->getTextLen()
	@call [SendMessageA]([.this.hWnd], EM_SETSEL, rax, rax)
	@call [SendMessageA]([.this.hWnd], EM_REPLACESEL, 0, [strLp])
	endf
	mov r9d, [_end]
	mov r8d, [_start]
	mov edx, EM_SETSEL
	mov rdx, [.this.hWnd]
	pop rbx
	leave
	jmp [SendMessageA];([.this.hWnd], EM_SETSEL, [_start], [_end])
endp