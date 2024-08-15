proc_noprologue

proc EDIT.addText uses rbx, this, strLp
	virtObj .this:arg EDIT at rbx
	local _start:DWORD,_end:DWORD
	mov rbx, rcx
	mov [strLp], rdx
	@call [SendMessageA]([.this.hWnd], EM_GETSEL, addr _start, addr _end)
	@call .this->getTextLen()
	@call [SendMessageA]([.this.hWnd], EM_SETSEL, rax, rax)
	@call [SendMessageA]([.this.hWnd], EM_REPLACESEL, 0, [strLp])
	@jret [SendMessageA]([.this.hWnd], EM_SETSEL, [_start], [_end])
endp

proc_resprologue