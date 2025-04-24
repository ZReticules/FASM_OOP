define CB_GETCOMBOBOXINFO 164h

macro COMBOBOX.addItem this, lpString{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_ADDSTRING, NULL, lpString)
}

macro COMBOBOX.delItem this, idItem{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_DELETESTRING, idItem, NULL)
}

macro COMBOBOX.setSelected this, idItem{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_SETCURSEL, idItem, NULL)
}

macro COMBOBOX.getSelected this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETCURSEL, NULL, NULL)
}

macro COMBOBOX.setItemData this, idItem, dataValue{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_SETITEMDATA, idItem, dataValue)
}

macro COMBOBOX.getItemData this, idItem{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETITEMDATA, idItem, NULL)
}

macro COMBOBOX.clear this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_RESETCONTENT, NULL, NULL)
}

macro COMBOBOX.findItem this, idBefore, strLp{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_FINDSTRINGEXACT, idBefore, strLp)
}

macro COMBOBOX.getCount this{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETCOUNT, NULL, NULL)
}

macro COMBOBOX.findStartWith this, idBefore, strLp{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_FINDSTRING, idBefore, strLp)
}

macro COMBOBOX.getItemTextLen this, idItem{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETLBTEXTLEN, idItem, NULL)
}

macro COMBOBOX.getItemText this, idItem, lpString{
	local _this
	inlineObj _this, this, pcx
	@call [SendMessageA]([_this+COMBOBOX.hWnd], CB_GETLBTEXT, idItem, lpString)
}

proc_noprologue

proc COMBOBOX.initEdit c uses pbx, this
	virtObj .this:arg COMBOBOX at pbx from @arg1
	locals 
		cbInfo COMBOBOXINFO
	endl
	@call [SendMessageA]([.this.hWnd], CB_GETCOMBOBOXINFO, NULL, addr cbInfo)
	mov pax, [cbInfo.hwndItem]
	mov [.this.cEdit.hWnd], pax
	mov [.this.cEdit.ID], 0
	@call [SetWindowPtrA]([cbInfo.hwndItem], GWL_USERDATA, addr .this.cEdit)
	@call [GetClientRect]([cbInfo.hwndItem], addr .this.cEdit.baseRect)
	@call CNV::fill(addr .this.cEdit.baseRect, addr .this.baseRect, sizeof.RECT)
	ret
endp

proc COMBOBOX.initListBox c uses pbx, this
	virtObj .this:arg COMBOBOX at pbx from @arg1
	locals 
		cbInfo COMBOBOXINFO
	endl
	@call [SendMessageA]([.this.hWnd], CB_GETCOMBOBOXINFO, NULL, addr cbInfo)
	mov pax, [cbInfo.hwndList]
	mov [.this.cListBox.hWnd], pax
	mov [.this.cListBox.ID], 0
	@call [SetWindowPtrA]([cbInfo.hwndList], GWL_USERDATA, addr .this.cListBox)
	@call CNV::fill(addr .this.cListBox.sData.baseRect, addr .this.sData.baseRect, sizeof.RECT)
	ret
endp

proc COMBOBOX.initSubControl c uses pbx, this
	virtObj .this:arg COMBOBOX at pbx from @arg1
	locals 
		cbInfo COMBOBOXINFO
	endl
	@call [SendMessageA]([.this.hWnd], CB_GETCOMBOBOXINFO, NULL, addr cbInfo)
	mov pax, [cbInfo.hwndItem]
	mov [.this.cEdit.hWnd], pax
	mov pax, [.this.hWnd]
	cmp pax, [cbInfo.hwndItem]
	jne @f
		@call c [printf]("lol\n")
	@@:
	mov [.this.cEdit.ID], 0
	@call [SetWindowPtrA]([cbInfo.hwndItem], GWL_USERDATA, addr .this.cEdit)
	@call [GetClientRect]([cbInfo.hwndItem], addr .this.cEdit.baseRect)
	@call CNV::fill(addr .this.cEdit.baseRect, addr .this.baseRect, sizeof.RECT)
	mov pax, [cbInfo.hwndList]
	mov [.this.cListBox.hWnd], pax
	mov [.this.cListBox.ID], 0
	@call [SetWindowPtrA]([cbInfo.hwndList], GWL_USERDATA, addr .this.cListBox)
	@call CNV::fill(addr .this.cListBox.baseRect, addr .this.baseRect, sizeof.RECT)
	ret
endp

proc_resprologue
