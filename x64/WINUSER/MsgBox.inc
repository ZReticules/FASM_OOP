importlib user32,\
	MessageBoxIndirectA

struct MsgBoxParams
	cbSize	     		dq sizeof.MsgBoxParams
	hwndOwner	    	dq NULL
	hInstance	    	dq NULL
	lpszText	    	dq ?
	lpszCaption	    	dq NULL
	dwStyle	     		dq MB_OK
	lpszIcon	     	dq NULL
	dwContextHelpId    	dq NULL
	lpfnMsgBoxCallback 	dq ?
	dwLanguageId	    dq ?
	show				dm this:inline
ends

struc MsgBox lpszText*, lpszCaption=NULL, dwStyle=MB_OK, lpszIcon=NULL , hwndOwner=NULL, lpfnMsgBoxCallback=NONE {
	local any, _lpszText, _lpszCaption
	_lpszText equ lpszText
	_lpszCaption equ lpszCaption
	param@parser any, _lpszText
	param@parser any, _lpszCaption
	. MsgBoxParams sizeof.MsgBoxParams, hwndOwner, NONE, _lpszText, _lpszCaption, dwStyle, lpszIcon, NONE, lpfnMsgBoxCallback
}

macro MsgBox lpszText*, lpszCaption=NULL, dwStyle=MB_OK, hwndOwner=NULL, lpfnMsgBoxCallback=NULL {
	local any, _lpszText, _lpszCaption
	_lpszText equ lpszText
	_lpszCaption equ lpszCaption
	param@parser any, _lpszText
	param@parser any, _lpszCaption
	MsgBoxParams sizeof.MsgBoxParams, hwndOwner, NONE, _lpszText, _lpszCaption, dwStyle, NONE, NONE, lpfnMsgBoxCallback
}

macro MsgBoxParams.show this{
	@call [MessageBoxIndirectA](this)
}