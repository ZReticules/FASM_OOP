IPN_FIRST			= -860
IPN_LAST 			= -879
IPN_FIELDCHANGED 	= IPN_FIRST - 0

struct IPEDIT CONTROL
	const CType equ "SysIPAddress32"

	clear 		dm this:inline
	getIP 		dm this:inline
	isBlank 	dm this:inline
	setIP 		dm this:inline
	setFocus 	dm this:inline
	setRange 	dm this:inline
ends

struct NMIPADDRESS
	hdr		NMHDR
	iField	rd 1
	iValue	rd 1
ends

macro IPEDIT.clear this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + IPEDIT.hWnd], IPM_CLEARADDRESS, NULL, NULL)
}

macro IPEDIT.getIP this, p_dwip{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + IPEDIT.hWnd], IPM_GETADDRESS, NULL, p_dwip)
}

macro IPEDIT.isBlank this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + IPEDIT.hWnd], IPM_ISBLANK, NULL, NULL)
}

macro IPEDIT.setIP this, dwip{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + IPEDIT.hWnd], IPM_SETADDRESS, NULL, dwip)
}

macro IPEDIT.setFocus this, idx{
	match , idx\{
		$call setFocus<WINAPI_WINDOW>(this)
	rept 0\{\}rept 1\{
		\local _this
		inlineObj _this, this, pcx
		$call [SendMessageA]([_this + IPEDIT.hWnd], IPM_SETFOCUS, idx, NULL)
	\}
}

macro IPEDIT.setRange this, idx, range{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + IPEDIT.hWnd], IPM_SETRANGE, idx, range)
}