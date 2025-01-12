define MAX_PATH 260

if used WND._hModule
	WND._hModule dq ?
end if

macro WND.moduleInit{
	if used WND._hModule
		@call [GetModuleHandleA](0)
		mov [WND._hModule], rax
	end if
}

TLS_AddMacro WND.moduleInit

macro WND.msgBox lpText, lpCaption = NULL, utype = MB_OK, hWnd = NULL{
	@call [MessageBoxA](hWnd, lpText, lpCaption, utype)
}

macro WND.setTimer uElapse, lpTimerFunc, nIDEvent=0, hWnd=NULL{
	@call [SetTimer](hWnd, nIDEvent, uElapse, lpTimerFunc)
}

macro WND.killTimer nIDEvent, hWnd=NULL{
	@call [KillTimer](hWnd, nIDEvent)
}