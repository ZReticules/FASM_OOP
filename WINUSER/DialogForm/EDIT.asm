proc_noprologue

proc EDIT.addText c uses pbx, this, strLp
	virtObj .this:arg EDIT at pbx from @arg1
	@sarg @arg2

	local _start:DWORD,_end:DWORD
	@call [SendMessageA]([.this.hWnd], EM_GETSEL, addr _start, addr _end)
	@call .this->getTextLen()
	@call [SendMessageA]([.this.hWnd], EM_SETSEL, eax, eax)
	@call [SendMessageA]([.this.hWnd], EM_REPLACESEL, 0, [strLp])
	@jret [SendMessageA]([.this.hWnd], EM_SETSEL, [_start], [_end])
endp

proc_resprologue