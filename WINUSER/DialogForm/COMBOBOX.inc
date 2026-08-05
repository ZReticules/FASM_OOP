CBM_FIRST 			= 0x1700
CB_SETMINVISIBLE 	= CBM_FIRST + 1
CB_GETMINVISIBLE 	= CBM_FIRST + 2
CB_SETCUEBANNER 	= CBM_FIRST + 3
CB_GETCUEBANNER 	= CBM_FIRST + 4

struct COMBOBOX CONTROL
	struct 
		cEdit 			EDIT
		cListBox		LISTBOX
	ends
	
	addItem 			dm this:inline
	delItem  			dm this:inline
	getCount  			dm this:inline
	setSelected 		dm this:inline
	getSelected 		dm this:inline
	setItemData			dm this:inline
	getItemData			dm this:inline
	clear				dm this:inline
	findItem 			dm this:inline
	findItemW 			dm this:inline
	findStartWith		dm this:inline
	findStartWithW		dm this:inline
	getItemTextLen		dm this:inline
	getItemTextLenW		dm this:inline
	getItemText			dm this:inline
	getItemTextW		dm this:inline

	addDir 				dm this:inline
	addDirW 			dm this:inline
	getPlaceholder 		dm this:inline
	setPlaceholder 		dm this:inline
	getDroppedRect 		dm this:inline
	getDroppedState 	dm this:inline
	getDroppedWidth 	dm this:inline
	getSelected 		dm this:inline
	getSelectedW 		dm this:inline
	setSelected 		dm this:inline
	setSelectedW 		dm this:inline
	isUiExtended 		dm this:inline
	getHorizontalExtent dm this:inline
	getItemHeight 		dm this:inline
	getLocale 			dm this:inline
	getMinVisible 		dm this:inline
	setMinVisible 		dm this:inline
	getTop 				dm this:inline
	setTop 				dm this:inline
	initStorage 		dm this:inline
	insertItem 			dm this:inline
	insertItemW 		dm this:inline
	setTextLimit 		dm this:inline
	findAndSelect 		dm this:inline
	findAndSelectW 		dm this:inline
	setDroppedWidth 	dm this:inline
	setUiExtended 		dm this:inline
	setHorizontalExtent dm this:inline
	setItemHeight 		dm this:inline
	setLocale 			dm this:inline
	setDropped 			dm this:inline

	getItemString		dm this
	getItemWString		dm this

	; setItemText			dm this
	initSubControl		dm this
	initListBox			dm this
	initEdit			dm this
	
	const CType equ "ComboBox"
ends

struct COMBOBOXINFO 
	cbSize		dd sizeof.COMBOBOXINFO;
	rcItem 		RECT
	rcButton 	RECT
	stateButton	dd ?
	hwndCombo	dptr ?
	hwndItem	dptr ?
	hwndList	dptr ?
ends

CB_GETCOMBOBOXINFO = 164h

macro COMBOBOX.addItem this, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_ADDSTRING, NULL, p_cstr)
}

macro COMBOBOX.addItem this, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_ADDSTRING, NULL, p_cwstr)
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

macro COMBOBOX.findItem this, idBefore, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_FINDSTRINGEXACT, idBefore, p_cstr)
}

macro COMBOBOX.findItemW this, idBefore, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_FINDSTRINGEXACT, idBefore, p_cwstr)
}

macro COMBOBOX.getCount this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETCOUNT, NULL, NULL)
}

macro COMBOBOX.findStartWith this, idBefore, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_FINDSTRING, idBefore, p_cstr)
}

macro COMBOBOX.findStartWithW this, idBefore, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_FINDSTRING, idBefore, p_cwstr)
}

macro COMBOBOX.getItemTextLen this, idItem{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETLBTEXTLEN, idItem, NULL)
}

macro COMBOBOX.getItemTextLenW this, idItem{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_GETLBTEXTLEN, idItem, NULL)
}

macro COMBOBOX.getItemText this, idItem, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETLBTEXT, idItem, p_cstr)
}

macro COMBOBOX.getItemTextW this, idItem, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_GETLBTEXT, idItem, p_cwstr)
}

macro COMBOBOX.addDir this, flags, p_cstrpath{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_DIR, flags, p_cstrpath)
}

macro COMBOBOX.addDirW this, flags, p_cwstrpath{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_DIR, flags, p_cwstrpath)
}

macro COMBOBOX.getPlaceholder this, p_cwstr, bufSize{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_GETCUEBANNER, p_cwstr, bufSize)
}

macro COMBOBOX.setPlaceholder this, showFocus, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_SETCUEBANNER, showFocus, p_cwstr)
}

macro COMBOBOX.getDroppedRect this, p_rect{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETDROPPEDCONTROLRECT, NULL, p_rect)
}

macro COMBOBOX.getDroppedState this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETDROPPEDSTATE, NULL, NULL)
}

macro COMBOBOX.getDroppedWidth this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETDROPPEDWIDTH, NULL, NULL)
}

macro COMBOBOX.getSelected this, p_start, p_end{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETEDITSEL, p_start, p_end)
}

macro COMBOBOX.getSelectedW this, p_start, p_end{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_GETEDITSEL, p_start, p_end)
}

; loword - start, hiword - end
macro COMBOBOX.setSelected this, pos{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_SETEDITSEL, NULL, pos)
}

; loword - start, hiword - end
macro COMBOBOX.setSelectedW this, pos{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_SETEDITSEL, NULL, pos)
}

macro COMBOBOX.isUiExtended this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETEXTENDEDUI, NULL, NULL)
}

macro COMBOBOX.getHorizontalExtent this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETHORIZONTALEXTENT, NULL, NULL)
}

macro COMBOBOX.getItemHeight this, idx=-1{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETITEMHEIGHT, idx, NULL)
}

macro COMBOBOX.getLocale this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETLOCALE, NULL, NULL)
}

macro COMBOBOX.getMinVisible this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETMINVISIBLE, NULL, NULL)
}

macro COMBOBOX.setMinVisible this, minVisible{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_SETMINVISIBLE, minVisible, NULL)
}

macro COMBOBOX.getTop this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETTOPINDEX, NULL, NULL)
}

macro COMBOBOX.setTop this, idx{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_SETTOPINDEX, idx, NULL)
}

macro COMBOBOX.initStorage this, cnt, memAmount{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_INITSTORAGE, cnt, memAmount)
}

macro COMBOBOX.insertItem this, idx, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_INSERTSTRING, idx, p_cstr)
}

macro COMBOBOX.insertItemW this, idx, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_INSERTSTRING, idx, p_cwstr)
}

macro COMBOBOX.setTextLimit this, limit{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_LIMITTEXT, limit, NULL)
}

macro COMBOBOX.findAndSelect this, idxStart, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_SELECTSTRING, idxStart, p_cstr)
}

macro COMBOBOX.findAndSelectW this, idxStart, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + COMBOBOX.hWnd], CB_SELECTSTRING, idxStart, p_cwstr)
}

macro COMBOBOX.setDroppedWidth this, width{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_SETDROPPEDWIDTH, width, NULL)
}

macro COMBOBOX.setUiExtended this, isExtended{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_GETEXTENDEDUI, isExtended, NULL)
}

macro COMBOBOX.setHorizontalExtent this, width{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_SETHORIZONTALEXTENT, width, NULL)
}

macro COMBOBOX.setItemHeight this, idx, height{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_SETITEMHEIGHT, idx, height)
}

macro COMBOBOX.setLocale this, locale{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_SETITEMHEIGHT, locale, NULL)
}

macro COMBOBOX.setDropped this, isDropped{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + COMBOBOX.hWnd], CB_SHOWDROPDOWN, isDropped, NULL)
}
 
.proc_frame_mode_static

.proc cdecl COMBOBOX.getItemString(.pthis, .idx, .p_dest) uses pbx pdi
	virtObj .this COMBOBOX at pbx from @arg1
	@sarg @arg2
	virtObj .dest String at pdi from @arg3
	$call .this::getItemTextLen(@arg2)
	cmp eax, CB_ERR
		je .return

	$call .dest::realloc(pax)::getLpChars()
	$call [.dest.len] = .this::getItemText([.idx], pax)
	.return:  ret
.endp

.proc cdecl COMBOBOX.getItemWString(.pthis, .idx, .p_dest) uses pbx pdi
	virtObj .this COMBOBOX at pbx from @arg1
	@sarg @arg2
	virtObj .dest WString at pdi from @arg3
	$call .this::getItemTextLenW(@arg2)
	cmp eax, CB_ERR
		je .return
		
	$call .dest::realloc(pax)::getLpWChars()
	$call [.dest.len] = .this::getItemTextW([.idx], pax)
	.return: ret
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
