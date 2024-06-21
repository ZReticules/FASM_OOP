if used EDIT.setReadOnly
	EDIT.setReadOnly:;, this, isReadOnly
		virtObj .this:arg EDIT
		mov r8, rdx
		mov edx, EM_SETREADONLY
		mov rcx, [.this.hWnd]
		jmp [SendMessageA];([.this.hWnd], EM_SETREADONLY, r8)
end if

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
	mov r9d, [_end]
	mov r8d, [_start]
	mov edx, EM_SETSEL
	mov rdx, [.this.hWnd]
	add rsp, 30h
	pop rbx
	jmp [SendMessageA];([.this.hWnd], EM_SETSEL, [_start], [_end])
endp
proc_resprologue