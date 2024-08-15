proc EDIT.addText uses ebx, this, strLp
	virtObj .this:arg EDIT at ebx
	local _start:DWORD,_end:DWORD
	mov ebx, [this]
	@call [SendMessageA]([.this.hWnd], EM_GETSEL, addr _start, addr _end)
	@call .this->getTextLen()
	@call [SendMessageA]([.this.hWnd], EM_SETSEL, eax, eax)
	@call [SendMessageA]([.this.hWnd], EM_REPLACESEL, 0, [strLp])
	@call [SendMessageA]([.this.hWnd], EM_SETSEL, [_start], [_end])
	ret
endp