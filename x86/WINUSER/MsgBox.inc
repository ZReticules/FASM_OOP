importlib user32,\
	MessageBoxIndirectA

struct MsgBoxParams
	cbSize	     		dd sizeof.MsgBoxParams
	hwndOwner	    	dd NULL
	hInstance	    	dd NULL
	lpszText	    	dd ?
	lpszCaption	    	dd NULL
	dwStyle	     		dd MB_OK
	lpszIcon	     	dd NULL
	dwContextHelpId    	dd NULL
	lpfnMsgBoxCallback 	dd ?
	dwLanguageId	    dd ?
	show				dm this:inline
ends

struc MsgBox lpszText*, lpszCaption=0, dwStyle=MB_OK, lpszIcon=NULL , hwndOwner=NULL, lpfnMsgBoxCallback=NONE {
	local any, _lpszText, _lpszCaption
	_lpszText equ lpszText
	_lpszCaption equ lpszCaption
	param@parser any, _lpszText
	param@parser any, _lpszCaption
	. MsgBoxParams sizeof.MsgBoxParams, hwndOwner, NONE, _lpszText, _lpszCaption, dwStyle, lpszIcon, NONE, lpfnMsgBoxCallback
}

macro MsgBox lpszText*, lpszCaption=NULL, dwStyle=MB_OK, hwndOwner=NULL, lpfnMsgBoxCallback=NULL {
	_lpszText equ lpszText
	_lpszCaption equ lpszCaption
	param@parser any, _lpszText
	param@parser any, _lpszCaption
	MsgBoxParams sizeof.MsgBoxParams, hwndOwner, NONE, _lpszText, _lpszCaption, dwStyle, NONE, NONE, lpfnMsgBoxCallback
}

macro MsgBoxParams.show this{
	@call [MessageBoxIndirectA](this)
}