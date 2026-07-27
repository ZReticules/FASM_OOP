define EM_SETCUEBANNER 0x1501

define EM_GETCUEBANNER 0x1502

struct EDIT CONTROL
	const LIMIT_TEXT = 65535

	setReadOnly				dm this:inline
	getLimitText			dm this:inline
	setLimitText			dm this:inline
	getPlaceholder			dm this:inline
	setPlaceholder			dm this:inline
	getSelected				dm this:inline
	getSelectedW			dm this:inline
	setSelected				dm this:inline
	setSelectedW			dm this:inline
	replaceSelected			dm this:inline
	replaceSelectedW		dm this:inline
	clearUndo 				dm this:inline

	canUndo 				dm this:inline
	charFromPos 			dm this:inline
	fmtLines 				dm this:inline
	getFirstVisibleLine 	dm this:inline
	getTextHandle 			dm this:inline
	getTimeStatus 			dm this:inline
	getLineChars 			dm this:inline
	getLineCharsW 			dm this:inline
	lineCount 				dm this:inline
	getMargins 				dm this:inline
	getModify 				dm this:inline
	getPaswordChar 			dm this:inline
	getPaswordCharW 		dm this:inline
	getTextRect 			dm this:inline
	getThumb 				dm this:inline
	getWordBreakProc 		dm this:inline
	hideBalloonTip 			dm this:inline
	lineIdxFromCharIdx 		dm this:inline
	lineIdxFromCharIdxW 	dm this:inline
	charIdxFromLineIdx 		dm this:inline
	charIdxFromLineIdxW 	dm this:inline
	lineLengthFromCharIdx 	dm this:inline
	lineLengthFromCharIdxW 	dm this:inline
	lineScroll 				dm this:inline
	posFromChar 			dm this:inline
	scroll 					dm this:inline
	scrollToCaret 			dm this:inline
	scrollToCaret 			dm this:inline
	setTextHandle 			dm this:inline
	setTimeStatus 			dm this:inline
	setMargins 				dm this:inline
	setModify 				dm this:inline
	setPasswordChar 		dm this:inline
	setTextRect 			dm this:inline
	setTextRectNP 			dm this:inline
	setTabstops 			dm this:inline
	setWordBreakProc 		dm this:inline
	showBalloonTip 			dm this:inline
	undo 					dm this:inline
	
	addText					dm this
	addTextW				dm this

	getLineLength			dm this
	getLineLengthW			dm this
	getLine					dm this
	getLineW				dm this
	getLineString			dm this
	getLineWString			dm this

	const CType equ "Edit"
ends

struct EDITBALLOONTIP
	cbStruct 	rd 1
	pszTitle 	rptr 1
	pszText 	rptr 1
	ttiIcon 	rd 1
ends

macro EDIT.setReadOnly this, isReadOnly{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETREADONLY, isReadOnly, NULL)
}

macro EDIT.getLimitText this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETLIMITTEXT, NULL, NULL)
}

macro EDIT.setLimitText this, charCount{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETLIMITTEXT, charCount, NULL)
}

macro EDIT.getPlaceholder this, p_cstr, bufSize{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETCUEBANNER, p_cstr, bufSize)
}

macro EDIT.setPlaceholder this, showFocus, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETCUEBANNER, showFocus, p_cwstr)
}

macro EDIT.getSelected this, p_start, p_end{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETSEL, p_start, p_end)
}

macro EDIT.getSelectedW this, p_start, p_end{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + EDIT.hWnd], EM_GETSEL, p_start, p_end)
}

macro EDIT.setSelected this, start, end{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETSEL, start, end)
}

macro EDIT.setSelectedW this, start, end{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + EDIT.hWnd], EM_SETSEL, start, end)
}

macro EDIT.replaceSelected this, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_REPLACESEL, NULL, p_cstr)
}

macro EDIT.replaceSelectedW this, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + EDIT.hWnd], EM_REPLACESEL, NULL, p_cwstr)
}

macro EDIT.clearUndo this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_EMPTYUNDOBUFFER, NULL, NULL)
}

macro EDIT.canUndo this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_CANUNDO, NULL, NULL)
}

; for EDIT: low part - x, high part - y
; for RICHEDIT: low part - x, high part - y
macro EDIT.charFromPos this, pos{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_CHARFROMPOS, NULL, pos)
}

; no support for RICHEDIT
macro EDIT.fmtLines this, needSoftBreak{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_FMTLINES, needSoftBreak, NULL)
}

macro EDIT.getFirstVisibleLine this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_FMTLINES, NULL, NULL)
}

; no support for RICHEDIT
macro EDIT.getTextHandle this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETHANDLE, NULL, NULL)
}

; no support for RICHEDIT
macro EDIT.getTimeStatus this, sType{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETIMESTATUS, sType, NULL)
}

macro EDIT.getLineChars this, idx, p_buf{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETLINE, idx, p_buf)
}

macro EDIT.getLineCharsW this, idx, p_wbuf{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + EDIT.hWnd], EM_GETLINE, idx, p_wbuf)
}

macro EDIT.lineCount this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETLINECOUNT, NULL, NULL)
}

; no support for RICHEDIT
macro EDIT.getMargins this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETMARGINS, NULL, NULL)
}

macro EDIT.getModify this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETMODIFY, NULL, NULL)
}

macro EDIT.getPaswordChar this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETPASWORDCHAR, NULL, NULL)
}

macro EDIT.getPaswordCharW this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + EDIT.hWnd], EM_GETPASWORDCHAR, NULL, NULL)
}

macro EDIT.getTextRect this, p_rect{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETRECT, NULL, p_rect)
}

macro EDIT.getThumb this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETTHUMB, NULL, NULL)
}

macro EDIT.getWordBreakProc this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_GETWORDBREAKPROC, NULL, NULL)
}

macro EDIT.hideBalloonTip this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_HIDEBALLOONTIP, NULL, NULL)
}

macro EDIT.lineIdxFromCharIdx this, idx{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_LINEFROMCHAR, idx, NULL)
}

macro EDIT.lineIdxFromCharIdxW this, idx{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + EDIT.hWnd], EM_LINEFROMCHAR, idx, NULL)
}

macro EDIT.charIdxFromLineIdx this, idx{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_LINEINDEX, idx, NULL)
}

macro EDIT.charIdxFromLineIdxW this, idx{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + EDIT.hWnd], EM_LINEINDEX, idx, NULL)
}

macro EDIT.lineLengthFromCharIdx this, charIdx{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_LINELENGTH, charIdx, NULL)
}

macro EDIT.lineLengthFromCharIdxW this, charIdx{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + EDIT.hWnd], EM_LINELENGTH, charIdx, NULL)
}

macro EDIT.lineScroll this, horzCount, vertCount{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_LINESCROLL, horzCount, vertCount)
}

macro EDIT.posFromChar this, arg1, arg2=NULL{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_POSFROMCHAR, arg1, arg2)
}

macro EDIT.scroll this, action{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SCROLL, action, NULL)
}

macro EDIT.scrollToCaret this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SCROLLCARET, NULL, NULL)
}

; no support for REICHEDIT
macro EDIT.setTextHandle this, hText{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETHANDLE, hText, NULL)
}

macro EDIT.setTimeStatus this, sType, sData{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETHANDLE, sType, sData)
}

macro EDIT.setMargins this, mFlags, val{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETMARGINS, mFlags, val)
}

macro EDIT.setModify this, isModify{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETMODIFY, isModify, NULL)
}

macro EDIT.setPasswordChar this, psChar{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETPASSWORDCHAR, psChar, NULL)
}

macro EDIT.setTextRect this, isRel, p_rect{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETRECT, isRel, p_rect)
}

macro EDIT.setTextRectNP this, isRel, p_rect{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETRECTNP, isRel, p_rect)
}

macro EDIT.setTabstops this, nStops=0, p_tabPositions=NULL{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETTABSTOPS, nStops, p_tabPositions)
}

macro EDIT.setWordBreakProc this, p_proc{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SETTABSTOPS, NULL, p_proc)
}

macro EDIT.showBalloonTip this, p_editBaloonTip{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_SHOWBALLOONTIP, NULL, p_editBaloonTip)
}

macro EDIT.undo this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + EDIT.hWnd], EM_UNDO, NULL, NULL)
}

.proc_frame_mode_static

.proc cdecl EDIT.addText(.pthis:P_EDIT, .p_cstr:POINTER) uses pbx
	virtObj .this EDIT at pbx from @arg1
	@sarg @arg2

	.local .start_:DWORD, .end_:DWORD
	$call .this::getSelected(&.start_, &.end_)
	$call .this::getTextLen()
	$call .this::setSelected(eax, eax)
	$call .this::replaceSelected([.p_cstr])
	$call .this::setSelected([.start_], [.end_])
	ret
.endp

.proc cdecl EDIT.addTextW(.pthis:P_EDIT, .p_cwstr:POINTER) uses pbx
	virtObj .this EDIT at pbx from @arg1
	@sarg @arg2

	.local .start:DWORD, .end:DWORD
	$call .this::getSelectedW(&.start, &.end)
	$call .this::getTextLenW()
	$call .this::setSelectedW(eax, eax)
	$call .this::replaceSelectedW([.p_cwstr])
	$call .this::setSelectedW([.start], [.end])
	ret
.endp

.proc cdecl EDIT.getLineLength(.pthis:P_EDIT, .idx:DWORD) uses pbx
	virtObj .this EDIT at pbx from @arg1
	$call .this::charIdxFromLineIdx(@arg2)
	$call .this::lineLengthFromCharIdx(pax)
	ret
.endp

.proc cdecl EDIT.getLineLengthW(.pthis:P_EDIT, .idx:DWORD) uses pbx
	virtObj .this EDIT at pbx from @arg1
	$call .this::charIdxFromLineIdxW(@arg2)
	$call .this::lineLengthFromCharIdxW(pax)
	ret
.endp

; cBytes includes NULL-terminator
.proc cdecl EDIT.getLine(.pthis:P_EDIT, .idx:DWORD, .cBytes:WORD, .p_buf:POINTER) uses pbx
	virtObj .this EDIT at pbx from @arg1
	@sarg @arg2, @arg4
	@larg pdx, @arg3, pax, @arg4
	dec edx
	mov word[pax], dx
	$call .this::getLineChars([.idx], pax)
	mov pdx, [.p_buf]
	mov byte[pdx + pax], 0
	ret
.endp

; cWords includes NULL-terminator
.proc cdecl EDIT.getLineW(.pthis:P_EDIT, .idx:DWORD, .cWords:WORD, .p_buf:POINTER) uses pbx
	virtObj .this EDIT at pbx from @arg1
	@sarg @arg2, @arg4
	@larg pdx, @arg3, pax, @arg4
	dec edx
	mov word[pax], dx
	$call .this::getLineCharsW([.idx], pax)
	mov pdx, [.p_buf]
	mov word[pdx + pax * 2], 0
	ret
.endp

.proc cdecl EDIT.getLineString(.pthis:P_EDIT, .idx:DWORD, .p_dest:P_String) uses pbx pdi
	@sarg @arg2
	virtObj .this EDIT at pbx from @arg1
	virtObj .dest String at pdi from @arg3
	.local .lineLen:DWORD
	$call .this::charIdxFromLineIdx(@arg2)
	$call [.lineLen] = .this::lineLengthFromCharIdx(pax)
	$call .dest::realloc([.lineLen])::getLpChars()
	inc [.lineLen]
	$call [.dest.len] = .this::getLine([.idx], [.lineLen], pax)
	ret
.endp

.proc cdecl EDIT.getLineWString(.pthis:P_EDIT, .idx:DWORD, .p_dest:P_WString) uses pbx pdi
	@sarg @arg2
	virtObj .this EDIT at pbx from @arg1
	virtObj .dest WString at pdi from @arg3
	.local .lineLen:DWORD
	$call .this::charIdxFromLineIdxW(@arg2)
	$call [.lineLen] = .this::lineLengthFromCharIdxW(pax)
	$call .dest::realloc([.lineLen])::getLpWChars()
	inc [.lineLen]
	$call [.dest.len] = .this::getLineW([.idx], [.lineLen], pax)
	ret
.endp

.proc_frame_mode_previous

