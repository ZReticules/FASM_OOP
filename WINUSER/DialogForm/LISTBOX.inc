DL_BEGINDRAG 	= WM_USER + 133
DL_DRAGGING  	= WM_USER + 134
DL_DROPPED   	= WM_USER + 135
DL_CANCELDRAG	= WM_USER + 136

LB_GETLISTBOXINFO = 0x01B2

LBS_COMBOBOX = 0x8000

struct LISTBOX CONTROL
	const CType equ "Listbox"
	
	addItem 			dm this:inline
	deleteItem 			dm this:inline
	getCount 			dm this:inline
	getSelected 		dm this:inline
	setSelected 		dm this:inline
	isItemSelected 		dm this:inline
	getItemTextLen 		dm this:inline
	getItemText 		dm this:inline
	getItemTextLenW 	dm this:inline
	getItemTextW 		dm this:inline
	getItemData 		dm this:inline
	setItemData 		dm this:inline
	getTop 				dm this:inline
	setTop 				dm this:inline
	clear 				dm this:inline

	addFile 			dm this:inline
	addFileW 			dm this:inline
	addItemW 			dm this:inline
	addDir 				dm this:inline
	addDirW 			dm this:inline
	findStartWith 		dm this:inline
	findStartWithW 		dm this:inline
	findItem 			dm this:inline
	findItemW 			dm this:inline
	getAnchorIndex 		dm this:inline
	getCaretIndex 		dm this:inline
	getHorizontalExtent dm this:inline
	getItemHeight 		dm this:inline
	getItemRect 		dm this:inline
	itemsPerColumn 		dm this:inline
	getLocale 			dm this:inline
	selectedCount 		dm this:inline
	getSelectedItems 	dm this:inline
	initStorage 		dm this:inline
	insertItem 			dm this:inline
	insertItemW 		dm this:inline
	indexFromPoint 		dm this:inline
	resetContent 		dm this:inline
	findAndSelect 		dm this:inline
	findAndSelectW 		dm this:inline
	setRangeSelected 	dm this:inline
	selectRange 		dm this:inline
	setAnchorIndex 		dm this:inline
	setCaretIndex 		dm this:inline
	setColumnWidth 		dm this:inline
	setCount 			dm this:inline
	setHorizontalExtent dm this:inline
	setItemHeight 		dm this:inline
	setLocale 			dm this:inline
	setSelectedState 	dm this:inline
	setTabStops 		dm this:inline

	getItemString 		dm this
	getItemWString 		dm this
ends

macro LISTBOX.addItem this, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_ADDSTRING, NULL, p_cstr)
}

macro LISTBOX.deleteItem this, index{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_DELETESTRING, index, NULL)
}

macro LISTBOX.getCount this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETCOUNT, NULL, NULL)
}

macro LISTBOX.getSelected this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETCURSEL, NULL, NULL)
}

macro LISTBOX.setSelected this, index=-1{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETCURSEL, index, NULL)
}

macro LISTBOX.isItemSelected this, index{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETSEL, index, NULL)
}

macro LISTBOX.getItemTextLen this, index{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETTEXTLEN, index, NULL)
}

macro LISTBOX.getItemText this, index, lpBuf{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETTEXT, index, lpBuf)
}

macro LISTBOX.getItemTextLenW this, index{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + LISTBOX.hWnd], LB_GETTEXTLEN, index, NULL)
}

macro LISTBOX.getItemTextW this, index, lpBuf{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + LISTBOX.hWnd], LB_GETTEXT, index, lpBuf)
}

macro LISTBOX.getItemData this, index{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETITEMDATA, index, NULL)
}

macro LISTBOX.setItemData this, index, _data{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETITEMDATA, index, _data)
}

macro LISTBOX.getTop this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETTOPINDEX, NULL, NULL)
}

macro LISTBOX.setTop this, idx{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETTOPINDEX, idx, NULL)
}

macro LISTBOX.clear this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_RESETCONTENT, NULL, NULL)
}

macro LISTBOX.addFile this, p_cstrfname{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_ADDFILE, NULL, p_cstrfname)
}

macro LISTBOX.addFileW this, p_cwstrfname{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + LISTBOX.hWnd], LB_ADDFILE, NULL, p_cwstrfname)
}

macro LISTBOX.addItemW this, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + LISTBOX.hWnd], LB_ADDSTRING, NULL, p_cwstr)
}

macro LISTBOX.addDir this, flags, p_cstrpath{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_DIR, flags, p_cstrpath)
}

macro LISTBOX.addDirW this, flags, p_cwstrpath{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + LISTBOX.hWnd], LB_DIR, flags, p_cwstrpath)
}

macro LISTBOX.findStartWith this, idx, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_FINDSTRING, idx, p_cstr)
}

macro LISTBOX.findStartWithW this, idx, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + LISTBOX.hWnd], LB_FINDSTRING, idx, p_cwstr)
}

macro LISTBOX.findItem this, idx, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_FINDSTRINGEXACT, idx, p_cstr)
}

macro LISTBOX.findItemW this, idx, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + LISTBOX.hWnd], LB_FINDSTRINGEXACT, idx, p_cwstr)
}

macro LISTBOX.getAnchorIndex this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETANCHORINDEX, NULL, NULL)
}

macro LISTBOX.getCaretIndex this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETCARETINDEX, NULL, NULL)
}

macro LISTBOX.getHorizontalExtent this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETHORIZONTALEXTENT, NULL, NULL)
}

macro LISTBOX.getItemHeight this, idx=0{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETITEMHEIGHT, idx, NULL)
}

macro LISTBOX.getItemRect this, idx, p_rect{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETITEMRECT, idx, p_rect)
}

macro LISTBOX.itemsPerColumn this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETLISTBOXINFO, NULL, NULL)
}

macro LISTBOX.getLocale this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETLOCALE, NULL, NULL)
}

macro LISTBOX.selectedCount this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETSELCOUNT, NULL, NULL)
}

macro LISTBOX.getSelectedItems this, cnt, p_indexes{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_GETSELITEMS, cnt, p_indexes)
}

macro LISTBOX.initStorage this, cnt, memAmount{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_INITSTORAGE, cnt, memAmount)
}

macro LISTBOX.insertItem this, idx, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_INSERTSTRING, idx, p_cstr)
}

macro LISTBOX.insertItemW this, idx, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + LISTBOX.hWnd], LB_INSERTSTRING, idx, p_cwstr)
}

; hiword - y-coordinate
; loword - x-coordinate
macro LISTBOX.indexFromPoint this, dwcoords{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_ITEMFROMPOINT, NULL, dwcoords)
}

macro LISTBOX.resetContent this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_RESETCONTENT, NULL, NULL)
}

macro LISTBOX.findAndSelect this, idxStart, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SELECTSTRING, idxStart, p_cstr)
}

macro LISTBOX.findAndSelectW this, idxStart, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + LISTBOX.hWnd], LB_SELECTSTRING, idxStart, p_cwstr)
}

; hiword - ends
; loword - start
macro LISTBOX.setRangeSelected this, isSelected, dwrange{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + LISTBOX.hWnd], LB_SELITEMRANGE, isSelected, dwrange)
}

; start < end - select range
; start >= end - deselect range
macro LISTBOX.selectRange this, start, end{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + LISTBOX.hWnd], LB_SELITEMRANGEEX, idxStart, p_cwstr)
}

macro LISTBOX.setAnchorIndex this, idx{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETANCHORINDEX, idx, NULL)
}

macro LISTBOX.setCaretIndex this, idx, partiallyVisible=0{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETCARETINDEX, idx, partiallyVisible)
}

macro LISTBOX.setColumnWidth this, width{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETCOLUMNWIDTH, width, NULL)
}

macro LISTBOX.setCount this, count_{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETCOUNT, count_, NULL)
}

macro LISTBOX.setHorizontalExtent this, newExtent{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETHORIZONTALEXTENT, newExtent, NULL)
}

macro LISTBOX.setItemHeight this, idx, height{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETITEMHEIGHT, idx, height)
}

macro LISTBOX.setLocale this, locale{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETITEMHEIGHT, locale, NULL)
}

macro LISTBOX.setSelectedState this, isSelected, idx=-1{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETSEL, isSelected, idx)
}

macro LISTBOX.setTabStops this, nStops=0, p_tabPositions=NULL{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + LISTBOX.hWnd], LB_SETTABSTOPS, nStops, p_tabPositions)
}

.proc_frame_mode_static

.proc cdecl LISTBOX.getItemString(.pthis:P_LISTBOX, .idx:DWORD, .p_dest:P_String) uses pbx pdi
	virtObj .this LISTBOX at pbx from @arg1
	@sarg @arg2
	virtObj .dest String at pdi from @arg3
	$call .this::getItemTextLen(@arg2)
	cmp eax, LB_ERR
		je .return

	$call .dest::realloc(pax)::getLpChars()
	$call [.dest.len] = .this::getItemText([.idx], pax)
	.return: ret
.endp

.proc cdecl LISTBOX.getItemWString(.pthis:P_LISTBOX, .idx:DWORD, .p_dest:P_WString) uses pbx pdi
	virtObj .this LISTBOX at pbx from @arg1
	@sarg @arg2
	virtObj .dest WString at pdi from @arg3
	$call .this::getItemTextLenW(@arg2)
	cmp eax, LB_ERR
		je .return
		
	$call .dest::realloc(pax)::getLpWChars()
	$call [.dest.len] = .this::getItemTextW([.idx], pax)
	.return: ret
.endp

.proc_frame_mode_previous
