define CB_GETCOMBOBOXINFO 164h

macro COMBOBOX.addItem this, lpString{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_ADDSTRING, NULL, lpString)
}

macro COMBOBOX.delItem this, idItem{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_DELETESTRING, idItem, NULL)
}

macro COMBOBOX.setSelected this, idItem{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_SETCURSEL, idItem, NULL)
}

macro COMBOBOX.getSelected this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETCURSEL, NULL, NULL)
}

macro COMBOBOX.setItemData this, idItem, dataValue{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_SETITEMDATA, idItem, dataValue)
}

macro COMBOBOX.getItemData this, idItem{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETITEMDATA, idItem, NULL)
}

macro COMBOBOX.clear this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_RESETCONTENT, NULL, NULL)
}

macro COMBOBOX.findItem this, idBefore, strLp{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_FINDSTRINGEXACT, idBefore, strLp)
}

macro COMBOBOX.getCount this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETCOUNT, NULL, NULL)
}

macro COMBOBOX.findStartWith this, idBefore, strLp{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_FINDSTRING, idBefore, strLp)
}

macro COMBOBOX.getItemTextLen this, idItem{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETLBTEXTLEN, idItem, NULL)
}

macro COMBOBOX.getItemText this, idItem, lpString{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETLBTEXT, idItem, lpString)
}

.proc_frame_mode_static

.proc cdecl COMBOBOX.getItemString(.pthis, .idItem, .lpString) uses pbx pdi psi
	virtObj .this COMBOBOX at pbx from @arg1
	@larg psi, @arg2
	virtObj .strDest String at pdi from @arg3

	$call [SendMessageA]([.this.hWnd], CB_GETLBTEXTLEN, psi, NULL)
	mov [.strDest.len], eax
	$call .strDest::realloc(addr pax + 1)::getLpChars()
	$call [SendMessageA]([.this.hWnd], CB_GETLBTEXT, psi, pax)
	ret
.endp

.proc cdecl COMBOBOX.initEdit(.pthis) uses pbx
	virtObj .this COMBOBOX at pbx from @arg1

	.local .cbInfo:COMBOBOXINFO
	$call [SendMessageA]([.this.hWnd], CB_GETCOMBOBOXINFO, NULL, &.cbInfo)
	mov pax, [.cbInfo.hwndItem]
	mov [.this.cEdit.hWnd], pax
	mov [.this.cEdit.ID], 0
	$call [SetWindowPtrA]([.cbInfo.hwndItem], GWL_USERDATA, &.this.cEdit)
	$call [GetClientRect]([.cbInfo.hwndItem], &.this.cEdit.baseRect)
	$call CNV|fill(&.this.cEdit.baseRect, &.this.baseRect, sizeof.RECT)
	ret
.endp

.proc cdecl COMBOBOX.initListBox(.pthis) uses pbx
	virtObj .this COMBOBOX at pbx from @arg1

	.local .cbInfo:COMBOBOXINFO
	$call [SendMessageA]([.this.hWnd], CB_GETCOMBOBOXINFO, NULL, &.cbInfo)
	mov pax, [.cbInfo.hwndList]
	mov [.this.cListBox.hWnd], pax
	mov [.this.cListBox.ID], 0
	$call [SetWindowPtrA]([.cbInfo.hwndList], GWL_USERDATA, &.this.cListBox)
	$call CNV|fill(&.this.cListBox.sData.baseRect, &.this.sData.baseRect, sizeof.RECT)
	ret
.endp

.proc cdecl COMBOBOX.initSubControl(.pthis) uses pbx
	virtObj .this COMBOBOX at pbx from @arg1

	.local .cbInfo:COMBOBOXINFO
	$call [SendMessageA]([.this.hWnd], CB_GETCOMBOBOXINFO, NULL, &.cbInfo)
	mov pax, [.cbInfo.hwndItem]
	mov [.this.cEdit.hWnd], pax
	mov pax, [.this.hWnd]
	mov [.this.cEdit.ID], 0
	$call [SetWindowPtrA]([.cbInfo.hwndItem], GWL_USERDATA, &.this.cEdit)
	$call [GetClientRect]([.cbInfo.hwndItem], &.this.cEdit.baseRect)
	$call CNV|fill(&.this.cEdit.baseRect, &.this.baseRect, sizeof.RECT)
	mov pax, [.cbInfo.hwndList]
	mov [.this.cListBox.hWnd], pax
	mov [.this.cListBox.ID], 0
	$call [SetWindowPtrA]([.cbInfo.hwndList], GWL_USERDATA, &.this.cListBox)
	$call CNV|fill(&.this.cListBox.baseRect, &.this.baseRect, sizeof.RECT)
	ret
.endp

.proc_frame_mode_previous
