MCM_FIRST 			= 0x1000
MCM_GETCURSEL     	= MCM_FIRST + 1
MCM_SETCURSEL     	= MCM_FIRST + 2
MCM_GETMAXSELCOUNT	= MCM_FIRST + 3
MCM_SETMAXSELCOUNT	= MCM_FIRST + 4
MCM_GETSELRANGE   	= MCM_FIRST + 5
MCM_SETSELRANGE   	= MCM_FIRST + 6
MCM_GETMONTHRANGE 	= MCM_FIRST + 7
MCM_SETDAYSTATE   	= MCM_FIRST + 8
MCM_GETMINREQRECT 	= MCM_FIRST + 9
MCM_SETCOLOR      	= MCM_FIRST + 10
MCM_GETCOLOR      	= MCM_FIRST + 11

MCSC_BACKGROUND   = 0
MCSC_TEXT         = 1   
MCSC_TITLEBK      = 2
MCSC_TITLETEXT    = 3
MCSC_MONTHBK      = 4
MCSC_TRAILINGTEXT = 5

MCM_SETTODAY	= MCM_FIRST + 12
MCM_GETTODAY	= MCM_FIRST + 13
MCM_HITTEST 	= MCM_FIRST + 14

struct MCHITTESTINFO
    cbSize	rd 1
    pt		POINT

    uHit	rd 1
    st		SYSTEMTIME
    rc		RECT
    iOffset	rd 1
    iRow	rd 1
    iCol	rd 1
ends

MCHT_TITLE           	= 0x00010000
MCHT_CALENDAR        	= 0x00020000
MCHT_TODAYLINK       	= 0x00030000

MCHT_CALENDARCONTROL 	= 0x00100000

MCHT_NEXT            	= 0x01000000
MCHT_PREV            	= 0x02000000

MCHT_NOWHERE         	= 0x00000000

MCHT_TITLEBK         	= MCHT_TITLE
MCHT_TITLEMONTH      	= MCHT_TITLE or 0x0001
MCHT_TITLEYEAR       	= MCHT_TITLE or 0x0002
MCHT_TITLEBTNNEXT    	= MCHT_TITLE or MCHT_NEXT or 0x0003
MCHT_TITLEBTNPREV    	= MCHT_TITLE or MCHT_PREV or 0x0003

MCHT_CALENDARBK      	= MCHT_CALENDAR
MCHT_CALENDARDATE    	= MCHT_CALENDAR or 0x0001
MCHT_CALENDARDATENEXT	= MCHT_CALENDARDATE or MCHT_NEXT
MCHT_CALENDARDATEPREV	= MCHT_CALENDARDATE or MCHT_PREV
MCHT_CALENDARDAY     	= MCHT_CALENDAR or 0x0002
MCHT_CALENDARWEEKNUM 	= MCHT_CALENDAR or 0x0003
MCHT_CALENDARDATEMIN 	= MCHT_CALENDAR or 0x0004
MCHT_CALENDARDATEMAX 	= MCHT_CALENDAR or 0x0005

MCM_SETFIRSTDAYOFWEEK 	= MCM_FIRST + 15
MCM_GETFIRSTDAYOFWEEK 	= MCM_FIRST + 16
MCM_GETRANGE 			= MCM_FIRST + 17
MCM_SETRANGE 			= MCM_FIRST + 18
MCM_GETMONTHDELTA 		= MCM_FIRST + 19
MCM_SETMONTHDELTA 		= MCM_FIRST + 20
MCM_GETMAXTODAYWIDTH 	= MCM_FIRST + 21
MCM_SETUNICODEFORMAT	= CCM_SETUNICODEFORMAT
MCM_GETUNICODEFORMAT	= CCM_GETUNICODEFORMAT

MCMV_MONTH  	= 0
MCMV_YEAR   	= 1
MCMV_DECADE 	= 2
MCMV_CENTURY	= 3
MCMV_MAX    	= MCMV_CENTURY

MCM_GETCURRENTVIEW 		= MCM_FIRST + 22
MCM_GETCALENDARCOUNT 	= MCM_FIRST + 23

MCGIP_CALENDARCONTRO	= 0
MCGIP_NEXT          	= 1
MCGIP_PREV          	= 2
MCGIP_FOOTER        	= 3
MCGIP_CALENDAR      	= 4
MCGIP_CALENDARHEADER	= 5
MCGIP_CALENDARBODY  	= 6
MCGIP_CALENDARROW   	= 7
MCGIP_CALENDARCELL  	= 8

MCGIF_DATE = 0x00000001
MCGIF_RECT = 0x00000002
MCGIF_NAME = 0x00000004

struct MCGRIDINFO
    cbSize		rd 1
    dwPart		rd 1
    dwFlags		rd 1
    iCalendar	rd 1
    iRow		rd 1
    iCol		rd 1
    bSelected	rb 1
    stStart		SYSTEMTIME
    stEnd		SYSTEMTIME
    rc			RECT
    pszName		rptr 1
    cchName		rptr 1
ends

MCM_GETCALENDARGRIDINFO = MCM_FIRST + 24
MCM_GETCALID 			= MCM_FIRST + 27
MCM_SETCALID 			= MCM_FIRST + 28

MCM_SIZERECTTOMIN 		= MCM_FIRST + 29
MCM_SETCALENDARBORDER 	= MCM_FIRST + 30
MCM_GETCALENDARBORDER 	= MCM_FIRST + 31
MCM_SETCURRENTVIEW 		= MCM_FIRST + 32

struct NMSELCHANGE
    nmhdr		NMHDR
    stSelStart	SYSTEMTIME
    stSelEnd	SYSTEMTIME
ends

MCN_SELCHANGE = MCN_FIRST - 3

struct NMDAYSTATE
    nmhdr		NMHDR
    stStart		SYSTEMTIME
    cDayState	rd 1
    prgDayState	rptr 1
ends

MCN_GETDAYSTATE	= MCN_FIRST - 1

typedef NMSELECT, NMSELCHANGE

MCN_SELECT = MCN_FIRST

struct NMVIEWCHANGE
    nmhdr		NMHDR
    dwOldView	rd 1
    dwNewView	rd 1
ends

MCN_VIEWCHANGE = MCN_FIRST - 4

MCS_DAYSTATE     	= 0x0001
MCS_MULTISELECT  	= 0x0002
MCS_WEEKNUMBERS  	= 0x0004
MCS_NOTODAYCIRCLE	= 0x0008
MCS_NOTODAY      	= 0x0010

MCS_NOTRAILINGDATES 	= 0x0040
MCS_SHORTDAYSOFWEEK 	= 0x0080
MCS_NOSELCHANGEONNAV	= 0x0100


struct MONTH_CALENDAR CONTROL
	const CType equ "SysMonthCal32"
ends

macro MONTH_CALENDAR.getBorder this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETCALENDARBORDER, NULL, NULL)
}

macro MONTH_CALENDAR.getCalendarCount this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETCALENDARCOUNT, NULL, NULL)
}

macro MONTH_CALENDAR.getCalendarInfo this, p_info{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETCALENDARINFO, NULL, p_info)
}

macro MONTH_CALENDAR.getCALID this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETCALID, NULL, NULL)
}

macro MONTH_CALENDAR.getColor this, colType{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETCOLOR, colType, NULL)
}

macro MONTH_CALENDAR.getCurrentView this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETCURRENTVIEW, NULL, NULL)
}

macro MONTH_CALENDAR.getCurSel this, p_systime{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETCURSEL, NULL, p_systime)
}

macro MONTH_CALENDAR.getFirstDayOfWeek this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETFIRSTDAYOFWEEK, NULL, NULL)
}

macro MONTH_CALENDAR.getMaxSelCount this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETMAXSELCOUNT, NULL, NULL)
}

macro MONTH_CALENDAR.getMaxTodayWidth this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETMAXTODAYWIDTH, NULL, NULL)
}

macro MONTH_CALENDAR.getMinReqRect this, p_rect{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETMINREQRECT, NULL, p_rect)
}

macro MONTH_CALENDAR.getMonthDelta this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETMONTHDELTA, NULL, NULL)
}

macro MONTH_CALENDAR.getMonthRange this, gmr, p_systemtime_2{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETMONTHRANGE, gmr, p_systemtime_2)
}

macro MONTH_CALENDAR.getRange this, p_systemtime_2{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETRANGE, NULL, p_systemtime_2)
}

macro MONTH_CALENDAR.getSelRange this, p_systemtime_2{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETSELRANGE, NULL, p_systemtime_2)
}

macro MONTH_CALENDAR.getToday this, p_systemtime{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETTODAY, NULL, p_systemtime)
}

macro MONTH_CALENDAR.getUnicodeFormat this{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETUNICODEFORMAT, NULL, NULL)
}

macro MONTH_CALENDAR.hitTest this, p_hitinfo{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_HITTEST, NULL, p_hitinfo)
}

macro MONTH_CALENDAR.setBorder this, needSet=0, borderSize=0{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SETCALENDARBORDER, needSet, borderSize)
}

macro MONTH_CALENDAR.setCALID this, cid{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SETCALID, cid, NULL)
}

macro MONTH_CALENDAR.setColor this, colType, colorref{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SETCOLOR, colType, colorref)
}

macro MONTH_CALENDAR.setCurrentView this, view{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SETCURRENTVIEW, NULL, view)
}

macro MONTH_CALENDAR.setCurSel this, p_systime{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SETCURSEL, NULL, p_systime)
}

macro MONTH_CALENDAR.setDayState this, cnt, mdaystate_arr{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SETDAYSTATE, cnt, mdaystate_arr)
}

macro MONTH_CALENDAR.setFirstDayOfWeek this, weekDay{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SETFIRSTDAYOFWEEK, NULL, weekDay)
}

macro MONTH_CALENDAR.setMaxSelCount this, cnt{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SETMAXSELCOUNT, cnt, NULL)
}

macro MONTH_CALENDAR.setMonthDelta this, mcount{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SETMONTHDELTA, mcount, NULL)
}

macro MONTH_CALENDAR.setRange this, flags, p_systemtime_2{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SETRANGE, flags, p_systemtime_2)
}

macro MONTH_CALENDAR.setSelRange this, p_systemtime_2{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SETSELRANGE, NULL, p_systemtime_2)
}

macro MONTH_CALENDAR.setToday this, p_systemtime_2{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SETTODAY, NULL, p_systemtime_2)
}

macro MONTH_CALENDAR.setUnicodeFormat this, isUnicode{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_GETUNICODEFORMAT, isUnicode, NULL)
}

macro MONTH_CALENDAR.sizeRectToMin this, p_rect{
	local _this
	inlineObj _this, this, pcx
	$call [SendMessageA]([_this + MONTH_CALENDAR.hWnd], MCM_SIZERECTTOMIN, NULL, p_rect)
}