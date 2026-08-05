BCM_FIRST           	= 0x1600
BCM_GETIDEALSIZE    	= BCM_FIRST + 0x1
BCM_SETIMAGELIST    	= BCM_FIRST + 0x2
BCM_GETIMAGELIST    	= BCM_FIRST + 0x3
BCM_SETTEXTMARGIN   	= BCM_FIRST + 0x4
BCM_GETTEXTMARGIN   	= BCM_FIRST + 0x5
BCM_SETDROPDOWNSTATE	= BCM_FIRST + 0x6
BCM_SETSPLITINFO    	= BCM_FIRST + 0x7
BCM_GETSPLITINFO    	= BCM_FIRST + 0x8
BCM_SETNOTE         	= BCM_FIRST + 0x9
BCM_GETNOTE         	= BCM_FIRST + 0xA
BCM_GETNOTELENGTH   	= BCM_FIRST + 0xB
BCM_SETSHIELD       	= BCM_FIRST + 0xC

BCN_FIRST	= -1250
BCN_LAST 	= 1350

BCN_HOTITEMCHANGE    	= BCN_FIRST + 0x1
BCN_DROPDOWN         	= BCN_FIRST + 0x2
NM_GETCUSTOMSPLITRECT	= BCN_FIRST + 0x3

struct BUTTON CONTROL
	const CType equ "Button"
	
	getIdealSize		dm this:inline
	getImageList		dm this:inline
	getNote				dm this:inline
	getNoteLength		dm this:inline
	getNoteLengthW		dm this:inline
	getSplitInfo		dm this:inline
	getTextMargin		dm this:inline
	setDropDownState	dm this:inline
	setImageList		dm this:inline
	getNote				dm this:inline
	setShield			dm this:inline
	setSplitInfo		dm this:inline
	setTextMargin		dm this:inline
	click				dm this:inline
	getCheck			dm this:inline
	getImage			dm this:inline
	getState			dm this:inline
	setCheck			dm this:inline
	setDontClick		dm this:inline
	setImage			dm this:inline
	setState			dm this:inline
	setStyle			dm this:inline
ends

struct BUTTON_IMAGELIST
	himl 	rptr 1
	margin 	RECT
	uAlign 	rd 1
ends

struct BUTTON_SPLITINFO
	mask 		rd 1
	himlGlyph	rptr 1
	uSplitStyle	rd 1
	size 		SIZE
ends

struct NMCUSTOMSPLITRECTINFO
	hdr			NMHDR
	rcClient	RECT
	rcButton	RECT
	rcSplit		RECT
ends

macro BUTTON.getIdealSize this, p_size{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BCM_GETIDEALSIZE, NULL, p_size)
}

macro BUTTON.getImageList this, p_imglist{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BCM_GETIMAGELIST, NULL, p_imglist)
}

macro BUTTON.getNote this, size, p_cwstrbuf{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BCM_GETNOTE, size, p_cwstrbuf)
}

macro BUTTON.getNoteLength this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BCM_GETNOTELENGTH, NULL, NULL)
}

macro BUTTON.getNoteLengthW this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + BUTTON.hWnd], BCM_GETNOTELENGTH, NULL, NULL)
}

macro BUTTON.getSplitInfo this, p_splitinfo{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BCM_GETSPLITINFO, NULL, p_splitinfo)
}

macro BUTTON.getTextMargin this, p_rect{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BCM_GETTEXTMARGIN, NULL, p_rect)
}

macro BUTTON.setDropDownState this, isPushed{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BCM_SETDROPDOWNSTATE, isPushed, NULL)
}

macro BUTTON.setImageList this, p_imglist{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BCM_SETIMAGELIST, NULL, p_imglist)
}

macro BUTTON.getNote this, size, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BCM_SETNOTE, size, p_cwstr)
}

macro BUTTON.setShield this, needDraw{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BCM_SETSHIELD, NULL, needDraw)
}

macro BUTTON.setSplitInfo this, p_splitinfo{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BCM_SETSPLITINFO, NULL, p_splitinfo)
}

macro BUTTON.setTextMargin this, p_rect{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BCM_SETTEXTMARGIN, NULL, p_rect)
}

macro BUTTON.click this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BM_CLICK, NULL, NULL)
}

macro BUTTON.getCheck this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BM_GETCHECK, NULL, NULL)
}

macro BUTTON.getImage this, iType{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BM_GETIMAGE, iType, NULL)
}

macro BUTTON.getState this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BM_GETSTATE, NULL, NULL)
}

macro BUTTON.setCheck this, checkState{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BM_SETCHECK, checkState, NULL)
}

macro BUTTON.setDontClick this, isDontClick{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BM_SETDONTCLICK, isDontClick, NULL)
}

macro BUTTON.setImage this, iType, hImg{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BM_SETIMAGE, iType, hImg)
}

macro BUTTON.setState this, isHighlight{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BM_SETSTATE, isHighlight, NULL)
}

macro BUTTON.setStyle this, style, needRedraw{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + BUTTON.hWnd], BM_SETSTYLE, style, needRedraw)
}

.proc_frame_mode_static

.proc cdecl BUTTON.getNoteWString(.pthis, .lpWString) uses pbx pdi
	@sarg @arg2
	virtObj .this:arg BUTTON at pbx from @arg1
	virtObj .string:arg WString at pdi from @arg2
	$call [.string.len] = .this::getNoteLength()
	$call .string::realloc([.string.len])::getLpWChars()
	mov edx, [.string.len]
	inc edx
	$call .this::getNote(pdx, pax)
	$return [.string.len]
.endp

.proc cdecl BUTTON.setNoteWString(.pthis, .lpWString) uses pbx pdi
	@sarg @arg2
	virtObj .this:arg BUTTON at pbx from @arg1
	virtObj .string:arg WString at pdx from @arg2
	$call .string::getLpWChars()
	$call .this::setNote(pax)
	ret
.endp

.proc_frame_mode_previous
