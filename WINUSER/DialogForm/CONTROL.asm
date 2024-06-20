importlib user32,\
	HideCaret

if used CONTROL.hideCur
	CONTROL.hideCur:;, this
		virtObj .this:arg CONTROL
		mov rcx, [.this.hWnd]
		jmp [HideCaret];([.this.hWnd])
end if

if used CONTROL.setTheme
	CONTROL.setTheme:;, this, wstrLp
		virtObj .this:arg CONTROL
		xor r8, r8
		mov rcx, [.this.hWnd]
		jmp [SetWindowTheme];([.this.hWnd], rdx, 0)
end if