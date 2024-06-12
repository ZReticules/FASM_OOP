importlib user32,\
	HideCaret

proc CONTROL.hideCur, this
	virtObj .this:arg CONTROL at rcx
	@call [HideCaret]([.this.hWnd])
	ret
endp

proc CONTROL.setTheme, this, wstrLp
	virtObj .this:arg CONTROL
	@call [SetWindowTheme]([.this.hWnd], rdx, 0)
	ret
endp