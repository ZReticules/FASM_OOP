@module CONTROL

include "control_specialfields.inc"

.alignmode_cdecl

struct DLG.ScaleMode
	x 	db ?
	y 	db ?
	rx 	db ?
	ry 	db ? 
ends

struc DLG.EvalScaleMode x, y, rx, ry\{
	. = x or y shl 8 or rx shl 16 or ry shl 24
\}

CONTROL.UNVALIDATABLE = 1

struct DLG.CONTROL_SERVICE_DATA
	baseRect		RECT
	lpEvents		rptr 1
	lpNotifications	rptr 1
	lpHooks 		rptr 1
	scaleMode		DLG.ScaleMode
	flags 			rd 1
	union
		defaultGeometry	RECT
		struct
			defaultPos 	POINT
			defaultSize SIZE
		ends
	ends
	scalerId 		rd 1
	childrenCount 	rd 1
ends

struct CONTROL WINAPI_WINDOW
	ID				rd 1
	sData 			DLG.CONTROL_SERVICE_DATA
	
	hideCur 		dm this:inline
	setTheme 		dm this:inline
	getClientSize	dm this:inline
	getWindowSize 	dm this:inline
	getBaseRect		dm this:inline
	setBaseRect		dm this:inline
	getScaleMode	dm this:inline
	setScaleMode	dm this:inline
ends

struct __LAYOUT_CONTROL CONTROL
	const CType equ "Static"
	WM_WINDOWPOSCHANGED hook NULL
ends

include "CONTROL.asm"

struct TABCONTROL CONTROL
	const CType equ "SysTabControl32"
ends

UDM_SETRANGE32 = 1135

struct UPDOWN CONTROL
	const CType equ "msctls_updown32"
ends

HDS_HORZ      	= 0x0000  ; Горизонтальный заголовок (по умолчанию)
HDS_BUTTONS   	= 0x0002  ; Кнопки вместо заголовков
HDS_HOTTRACK  	= 0x0004  ; Hot tracking (подсветка при наведении)
HDS_HIDDEN    	= 0x0008  ; Скрытый заголовок
HDS_DRAGDROP  	= 0x0040  ; Разрешить перетаскивание колонок
HDS_FULLDRAG  	= 0x0080  ; Показывать содержимое при перетаскивании
HDS_FILTERBAR 	= 0x0100  ; Добавить строку фильтрации
HDS_FLAT      	= 0x0200  ; Плоский стиль (Windows XP)
HDS_CHECKBOXES	= 0x0400  ; Чекбоксы в заголовках (Vista+)
HDS_NOSIZING  	= 0x0800  ; Запретить изменение размера
HDS_OVERFLOW  	= 0x1000  ; Кнопка переполнения (Vista+)

HDS_NONEFIXEDWIDTH	= 0x0010  ; Все колонки нефиксированной ширины
HDS_THICKPAD      	= 0x10000 ; Толстые отступы в XP/Vista

DDL_READWRITE  = 0x0000
DDL_READONLY   = 0x0001
DDL_HIDDEN     = 0x0002
DDL_SYSTEM     = 0x0004
DDL_DIRECTORY  = 0x0010
DDL_ARCHIVE    = 0x0020
DDL_POSTMSGS   = 0x2000
DDL_DRIVES     = 0x4000
DDL_EXCLUSIVE  = 0x8000

include "STATIC.inc"
include "BUTTON.inc"
include "EDIT.inc"
include "IPEDIT.inc"
include "LISTBOX.inc"
include "COMBOBOX.inc"
include "MONTH_CALENDAR.inc"
include "DATETIMEPICK.inc"
include "LISTVIEW.inc"

.alignmode_previous

@endm
