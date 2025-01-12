importlib user32,\
	MessageBoxIndirectA

struct MsgBoxParams
	cbSize	     		dptr ?
	hwndOwner	    	dptr ?
	hInstance	    	dptr ?
	lpszText	    	dptr ?
	lpszCaption	    	dptr ?
	dwStyle	     		dptr ?
	lpszIcon	     	dptr ?
	dwContextHelpId    	dptr ?
	lpfnMsgBoxCallback 	dptr ?
	dwLanguageId	    dptr ?
	show				dm this:inline
ends

struc MsgBox lpszText*, lpszCaption, [args] {;dwStyle=MB_OK, lpszIcon=NULL , hwndOwner=NULL, lpfnMsgBoxCallback=NONE
	common
	local a, _lpszCaption, any, _lpszText
	_lpszText equ lpszText
	a#_type equ MB_OK
	a#_hWnd equ NULL
	a#_icon equ NULL
	a#_callback equ NONE
	match name:val, lpszCaption\{
		_lpszCaption equ NULL
		restore a\#name
		a\#name equ val
	rept 0 \{ \} rept 1\{
		_lpszCaption equ lpszCaption
	\}
	forward
	match name:val, args\{
		restore a\#name
		a\#name equ val
	\}
	common
	_lpszCaption equ lpszCaption
	param@parser any, _lpszText
	param@parser any, _lpszCaption
	. MsgBoxParams sizeof.MsgBoxParams, a#_hWnd, NONE, _lpszText, _lpszCaption, a#_type, a#_icon, NONE, a#_callback
}

macro MsgBox lpszText*, lpszCaption=NULL, [args]{;dwStyle=MB_OK, hwndOwner=NULL, lpfnMsgBoxCallback=NULL 
	common
	local a, _lpszCaption, any, _lpszText
	_lpszText equ lpszText
	a#_type equ MB_OK
	a#_hWnd equ NULL
	a#_icon equ NULL
	a#_callback equ NONE
	match name:val, lpszCaption\{
		_lpszCaption equ NULL
		restore a\#name
		a\#name equ val
	rept 0 \{ \} rept 1\{
		_lpszCaption equ lpszCaption
	\}
	forward
	match name:val, args\{
		restore a\#name
		a\#name equ val
	\}
	common
	_lpszCaption equ lpszCaption
	param@parser any, _lpszText
	param@parser any, _lpszCaption
	MsgBoxParams sizeof.MsgBoxParams, a#_hWnd, NONE, _lpszText, _lpszCaption, a#_type, a#_icon, NONE, a#_callback
}

macro MsgBoxParams.show this{
	@call [MessageBoxIndirectA](this)
}