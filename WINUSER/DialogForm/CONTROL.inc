@module CONTROL

include "control_specialfields.inc"

struct@aligner equ c_aligner

struct ScaleMode
	x 	db ?
	y 	db ?
	rx 	db ?
	ry 	db ? 
ends

CONTROL.UNVALIDATABLE = 1

struct DLG_CONTROL_SERVICE_DATA
	baseRect		RECT
	lpEvents		rptr 1
	lpNotifications	rptr 1
	lpHooks 		rptr 1
	scaleMode		ScaleMode
	flags 			rd 1
ends

struct CONTROL WINAPI_WINDOW
	ID				rd 1
	sData 			DLG_CONTROL_SERVICE_DATA
	
	hideCur 		dm this:inline
	setTheme 		dm this:inline
	getClientSize	dm this:inline
	getWindowSize 	dm this:inline
ends

include "CONTROL.asm"

struct BUTTON CONTROL
	const CType equ "Button"
ends

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

include "STATIC.inc"
include "EDIT.inc"
include "LISTBOX.inc"
include "COMBOBOX.inc"
include "LISTVIEW.inc"

restore struct@aligner

@endm
