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
	show				dm this
ends

struc MsgBox lpszText*, lpszCaption=NULL, dwStyle=MB_OK, lpszIcon=NULL , hwndOwner=NULL, lpfnMsgBoxCallback=NONE {
	local _lpszText, _lpszCaption
	if lpszText eqtype ""
		_lpszText = $
		db lpszText, 0
	else
		_lpszText = lpszText
	end if
	if lpszCaption eqtype ""
		_lpszCaption = $
		db lpszCaption, 0
	else
		_lpszCaption = lpszCaption
	end if
	. MsgBoxParams sizeof.MsgBoxParams, hwndOwner, NONE, _lpszText, _lpszCaption, dwStyle, lpszIcon, NONE, lpfnMsgBoxCallback
}

macro MsgBox lpszText*, lpszCaption=NULL, dwStyle=MB_OK, hwndOwner=NULL, lpfnMsgBoxCallback=NULL {
	local _lpszText, _lpszCaption
	if lpszText eqtype ""
		_lpszText = $
		db lpszText, 0
	else
		_lpszText = lpszText
	end if
	if lpszCaption eqtype ""
		_lpszCaption = $
		db lpszCaption, 0
	else
		_lpszCaption = lpszCaption
	end if
	MsgBoxParams sizeof.MsgBoxParams, hwndOwner, NONE, _lpszText, _lpszCaption, dwStyle, NONE, NONE, lpfnMsgBoxCallback
}

MsgBoxParams.show:
	jmp [MessageBoxIndirectA]