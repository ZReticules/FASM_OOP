macro LISTBOX.addItem this, lpCstr{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_ADDSTRING, NULL, "ASCII")
}

macro LISTBOX.deleteItem this, index{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_DELETESTRING, index, NULL)
}

macro LISTBOX.getCount this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETCOUNT, index, NULL)
}

macro LISTBOX.getSelected this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETCURSEL, NULL, NULL)
}

macro LISTBOX.setSelected this, index=-1{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETCURSEL, index, NULL)
}

macro LISTBOX.isSelected this, index{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETSEL, index, NULL)
}

macro LISTBOX.getItemTextLen this, index{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETTEXTLEN, index, NULL)
}

macro LISTBOX.getItemText this, index, lpBuf{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETTEXT, index, lpBuf)
}

macro LISTBOX.getItemData this, index{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETITEMDATA, index, NULL)
}

macro LISTBOX.setItemData this, index, _data{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETITEMDATA, index, _data)
}

macro LISTBOX.getTop this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETTOPINDEX, NULL, NULL)
}

macro LISTBOX.setTop this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETTOPINDEX, NULL, NULL)
}

macro LISTBOX.clear this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this + LISTBOX.hWnd], LB_RESETCONTENT, NULL, NULL)
}