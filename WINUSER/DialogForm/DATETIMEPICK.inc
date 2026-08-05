DTM_FIRST  	= 0x1000
DTN_FIRST 	= -740
DTN_LAST  	= -745
DTN_FIRST2 	= -753
DTN_LAST2  	= -799

DTM_GETSYSTEMTIME  	= DTM_FIRST + 1
DTM_SETSYSTEMTIME  	= DTM_FIRST + 2
DTM_GETRANGE 		= DTM_FIRST + 3
DTM_SETRANGE 		= DTM_FIRST + 4
DTM_SETFORMATA 		= DTM_FIRST + 5
DTM_SETFORMATW 		= DTM_FIRST + 50


DTM_SETMCCOLOR 	= DTM_FIRST + 6
DTM_GETMCCOLOR 	= DTM_FIRST + 7
DTM_GETMONTHCAL	= DTM_FIRST + 8
DTM_SETMCFONT  	= DTM_FIRST + 9
DTM_GETMCFONT  	= DTM_FIRST + 10

DTM_SETMCSTYLE   	= DTM_FIRST + 11
DTM_GETMCSTYLE   	= DTM_FIRST + 12
DTM_CLOSEMONTHCAL	= DTM_FIRST + 13

DTM_GETDATETIMEPICKERINFO	= DTM_FIRST + 14

struct DATETIMEPICKERINFO
    cbSize			rd 1
    rcCheck 		RECT
    stateCheck		rd 1
    rcButton		RECT
    stateButton		rd 1
    hwndEdit		HWND
    hwndUD			HWND
    hwndDropDown	HWND
ends

DTM_GETIDEALSIZE 			= DTM_FIRST + 15

DTS_UPDOWN          		= 0x0001
DTS_SHOWNONE        		= 0x0002
DTS_SHORTDATEFORMAT 		= 0x0000
DTS_LONGDATEFORMAT  		= 0x0004
DTS_SHORTDATECENTURYFORMAT 	= 0x000C
DTS_TIMEFORMAT      		= 0x0009
DTS_APPCANPARSE     		= 0x0010
DTS_RIGHTALIGN      		= 0x0020

DTN_DATETIMECHANGE = DTN_FIRST2 - 6

struct NMDATETIMECHANGE
    nmhdr 	NMHDR
    dwFlags rd 1
    st		SYSTEMTIME
ends

DTN_USERSTRINGA = DTN_FIRST2 - 5
DTN_USERSTRINGW = DTN_FIRST - 5

struct NMDATETIMESTRING
    nmhdr 			NMHDR
    pszUserString	rptr 1
    st				SYSTEMTIME
    dwFlags 		rd 1
ends

typedef NMDATETIMESTRINGA, NMDATETIMESTRING
typedef NMDATETIMESTRINGW, NMDATETIMESTRING


DTN_WMKEYDOWNA = DTN_FIRST2 - 4
DTN_WMKEYDOWNW = DTN_FIRST - 4

struct NMDATETIMEWMKEYDOWN
    nmhdr 		NMHDR
    nVirtKey	rd 1
    pszFormat	rptr 1
    st			SYSTEMTIME
ends

typedef NMDATETIMEWMKEYDOWNA, NMDATETIMEWMKEYDOWN
typedef NMDATETIMEWMKEYDOWNW, NMDATETIMEWMKEYDOWN


DTN_FORMATA = DTN_FIRST2 - 3
DTN_FORMATW = DTN_FIRST - 3

struct NMDATETIMEFORMATA
    nmhdr		NMHDR 
    pszFormat	rptr 1  
    pszDisplay	rptr 1 
    szDisplay 	rb 64
ends

struct NMDATETIMEFORMATW
    nmhdr		NMHDR 
    pszFormat	rptr 1  
    pszDisplay	rptr 1 
    szDisplay 	rw 64
ends

DTN_FORMATQUERYA = DTN_FIRST2 - 2
DTN_FORMATQUERYW = DTN_FIRST - 2

struct NMDATETIMEFORMATQUERY
    nmhdr		NMHDR 
    pszFormat	rptr 1  
    szMax 		SIZE
ends

typedef NMDATETIMEFORMATQUERYA, NMDATETIMEFORMATQUERY
typedef NMDATETIMEFORMATQUERYW, NMDATETIMEFORMATQUERY

DTN_DROPDOWN	= DTN_FIRST2 - 1
DTN_CLOSEUP     = DTN_FIRST2


GDTR_MIN = 0x0001
GDTR_MAX = 0x0002

GDT_ERROR	= -1
GDT_VALID	= 0
GDT_NONE 	= 1

MCSC_BACKGROUND  	= 0
MCSC_TEXT        	= 1
MCSC_TITLEBK     	= 2
MCSC_TITLETEXT   	= 3
MCSC_MONTHBK     	= 4
MCSC_TRAILINGTEXT	= 5

struct DATETIMEPICK CONTROL
	const CType equ "SysDateTimePick32"

	closeMonthCal 		dm this:inline
	closeMonthCal 		dm this:inline
	getIdealSize 		dm this:inline
	getMonthCalColor 	dm this:inline
	getMonthCalFont 	dm this:inline
	getMonthCalStyle 	dm this:inline
	getMonthCal 		dm this:inline
	getRange 			dm this:inline
	getSystemTime 		dm this:inline
	setFormat 			dm this:inline
	setFormatW 			dm this:inline
	setMonthCalColor 	dm this:inline
	setMonthCalFont 	dm this:inline
	setMonthCalStyle	dm this:inline
	setRange 			dm this:inline
	setSystemTime 		dm this:inline
ends

macro DATETIMEPICK.closeMonthCal this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_CLOSEMONTHCAL, NULL, NULL)
}

macro DATETIMEPICK.closeMonthCal this, p_info{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_GETDATETIMEPICKERINFO, NULL, p_info)
}

macro DATETIMEPICK.getIdealSize this, p_size{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_GETIDEALSIZE, NULL, p_size)
}

macro DATETIMEPICK.getMonthCalColor this, cType{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_GETMCCOLOR, cType, NULL)
}

macro DATETIMEPICK.getMonthCalFont this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_GETMCFONT, NULL, NULL)
}

macro DATETIMEPICK.getMonthCalStyle this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_GETMCSTYLE, NULL, NULL)
}

macro DATETIMEPICK.getMonthCal this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_GETMONTHCAL, NULL, NULL)
}

macro DATETIMEPICK.getRange this, p_systime_2{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_GETRANGE, NULL, p_systime_2)
}

macro DATETIMEPICK.getSystemTime this, p_systime{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_GETSYSTEMTIME, NULL, p_systime)
}

macro DATETIMEPICK.setFormat this, p_cstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_SETFORMATA, NULL, p_cstr)
}

macro DATETIMEPICK.setFormatW this, p_cwstr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageW]([_this + DATETIMEPICK.hWnd], DTM_SETFORMATW, NULL, p_cwstr)
}

macro DATETIMEPICK.setMonthCalColor this, cType, val{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_SETMCCOLOR, cType, val)
}

macro DATETIMEPICK.setMonthCalFont this, val, needRedraw=1{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_SETMCFONT, val, needRedraw)
}

macro DATETIMEPICK.setMonthCalStyle this, val{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_SETMCSTYLE, NULL, val)
}

macro DATETIMEPICK.setRange this, gdtr_, p_systime_2{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_SETRANGE, gdtr_, p_systime_2)
}

macro DATETIMEPICK.setSystemTime this, gdt_, p_systime{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + DATETIMEPICK.hWnd], DTM_SETSYSTEMTIME, gdt_, p_systime)
}