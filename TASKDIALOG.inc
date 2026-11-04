@module TASKDIALOG

include "Winuser.inc"
include "WINUSER/WINAPI_WINDOW.inc"

struct __TEventsHeader
	count_ 	rb 1
	first 	rb 1
	events_ rw 0
ends

include "WINUSER/TaskDialog/task_special_fields.inc"

define TASKDIALOG_FIRST_BUTTON_ID 	12
define TASKDIALOG_FIRST_RADIO_ID 	0

define TD_SHIELD_ICON 		65532
define TD_INFORMATION_ICON 	65533
define TD_ERROR_ICON 		65534
define TD_WARNING_ICON 		65535

TDI enum HOLLOW=0,\
	SHEET=2,\
	EXPLORER_FOLDER_OPEN,\
	EXPLORER_FOLDER_FLAT=5,\
	EXPLORER_FOLDER_LEFT,\
	SEARCH=8,\
	EXPLORER_FOLDER_CLOSED=10,\
	EXPLORER_GAMES=14,\
	APPLICATION,\
	TRANSPARENT_SPACE,\
	EXPLORER_SEARCH,\
	TEXT_FILE,\
	LETTER,\
	PICTURE,\
	DIASHOW=103,\
    SHIELD_GREY = 65527,\
    SHIELD_OK,\
    SHIELD_ERROR,\
    SHIELD_WARNING,\
    SHIELD_BLUE,\
    SHIELD,\
    INFORMATION,\
    ERROR,\
    WARNING

define TDF_ENABLE_HYPERLINKS 			0x1
define TDF_USE_HICON_MAIN 				0x2
define TDF_USE_HICON_FOOTER 			0x4
define TDF_ALLOW_DIALOG_CANCELLATION 	0x8
define TDF_USE_COMMAND_LINKS 			0x10
define TDF_USE_COMMAND_LINKS_NO_ICON 	0x20
define TDF_EXPAND_FOOTER_AREA 			0x40
define TDF_EXPANDED_BY_DEFAULT 			0x80
define TDF_VERIFICATION_FLAG_CHECKED 	0x100
define TDF_SHOW_PROGRESS_BAR 			0x200
define TDF_SHOW_MARQUEE_PROGRESS_BAR 	0x400
define TDF_CALLBACK_TIMER 				0x800
define TDF_POSITION_RELATIVE_TO_WINDOW 	0x1000
define TDF_RTL_LAYOUT 					0x2000
define TDF_NO_DEFAULT_RADIO_BUTTON 		0x4000
define TDF_CAN_BE_MINIMIZED 			0x8000
define TDF_NO_SET_FOREGROUND 			0x00010000
define TDF_SIZE_TO_CONTENT 				0x01000000



define TDCBF_OK_BUTTON 		0x1
define TDCBF_YES_BUTTON 	0x2
define TDCBF_NO_BUTTON 		0x4
define TDCBF_CANCEL_BUTTON 	0x8
define TDCBF_RETRY_BUTTON 	0x10
define TDCBF_CLOSE_BUTTON 	0x20

TDM_FIRST = WM_USER + 101

TDM enum NAVIGATE_PAGE=TDM_FIRST,\
	CLICK_BUTTON,\
	SET_MARQUEE_PROGRESS_BAR,\
	SET_PROGRESS_BAR_STATE,\
	SET_PROGRESS_BAR_RANGE,\
	SET_PROGRESS_BAR_POS,\
	SET_PROGRESS_BAR_MARQUEE,\
	SET_ELEMENT_TEXT,\
	CLICK_RADIO_BUTTON=WM_USER+110,\
	ENABLE_BUTTON,\
	ENABLE_RADIO_BUTTON,\
	CLICK_VERIFICATION,\
	UPDATE_ELEMENT_TEXT,\
	SET_BUTTON_ELEVATION_REQUIRED_STATE,\
	UPDATE_ICON

TDM_LAST = TDM.UPDATE_ICON

TDN_FIRST = 0

TDN enum CREATED, \
	NAVIGATED, \
	BUTTON_CLICKED, \
	HYPERLINK_CLICKED, \
	TIMER, \
	DESTROYED, \
	RADIO_BUTTON_CLICKED, \
	DIALOG_CONSTRUCTED, \
	VERIFICATION_CLICKED, \
	HELP, \
	EXPANDO_BUTTON_CLICKED

TDN_LAST = TDN.EXPANDO_BUTTON_CLICKED

TDE enum CONTENT, \
	EXPANDED_INFORMATION, \
	FOOTER, \
	MAIN_INSTRUCTION,\
	HEADER=TDE.MAIN_INSTRUCTION

TDIE enum ICON_MAIN,\
    ICON_FOOTER

PBST enum NORMAL=1,\
	ERROR,\
	PAUSED

struct TASKDIALOGCONFIG
	cbSize 					dd ?
	hwndParent				dptr ?
	hInstance				dptr ?
	dwFlags					dd ?
	dwCommonButtons			dd ?
	pszWindowTitle			dptr ?
	union
		hMainIcon			dptr ?
		pszMainIcon			dptr ?
	ends
	pszMainInstruction		dptr ?
	pszContent				dptr ?
	cButtons 				dd ?
	pButtons				dptr ?
	nDefaultButton			dd ?
	cRadioButtons 			dd ?
	pRadioButtons			dptr ?
	nDefaultRadioButton		dd ?
	pszVerificationText		dptr ?
	pszExpandedInformation	dptr ?
	pszExpandedControlText	dptr ?
	pszCollapsedControlText	dptr ?
	union
		hFooterIcon			dptr ?
		pszFooterIcon		dptr ?
	ends
	pszFooter				dptr ?
	pfCallback				dptr ?
	lpCallbackData			dptr ?
	cxWidth 				dd ?
ends

struct TASKDIALOG_BUTTON
	nButtonID 		dd ?
	pszButtonText 	dptr ?
ends

struct TDResult
	lastBtn		dd ?
	lastRad		dd ?
	isChecked 	dd ?
ends

struct TASK_SERVICEDATA
	p_tevents 	dptr ?
	p_hooks		dptr ?
ends

struct TASKDIALOG_TEMPLATE
	const TFLAGS 			= 0
	const TCOMMON_BUTTONS 	= 0
	const TICON 			= NULL
	const TICON_FOOTER 		= NULL

	const TTITLE_PTR 				= NULL
	const TCONTENT_PTR 				= NULL
	const TCHECKBOX_PTR 			= NULL
	const TEXPANDED_CONTROL_PTR 	= NULL
	const TCOLLAPSED_CONTROL_PTR	= NULL
	const TEXPANDED_PTR 			= NULL
	const THEADER_PTR 				= NULL
	const TFOOTER_PTR 				= NULL

	const TDEF_PUSH_BUTTON 	= NULL
	const TDEF_RADIO_BUTTON = NULL

	const TWIDTH = 0
	
	__sData 	TASK_SERVICEDATA
	p_config 	P_TASKDIALOGCONFIG
ends

struct TASKDIALOG WINAPI_WINDOW
	const TFLAGS 			= 0
	const TCOMMON_BUTTONS 	= 0
	const TICON 			= NULL
	const TICON_FOOTER 		= NULL

	const TDEF_PUSH_BUTTON 	= NULL
	const TDEF_RADIO_BUTTON = NULL

	const TTITLE_PTR 				= NULL
	const TCONTENT_PTR 				= NULL
	const TCHECKBOX_PTR 			= NULL
	const TEXPANDED_CONTROL_PTR 	= NULL
	const TCOLLAPSED_CONTROL_PTR	= NULL
	const TEXPANDED_PTR 			= NULL
	const THEADER_PTR 				= NULL
	const TFOOTER_PTR 				= NULL

	p_template 	rptr 1
	result 		TDResult

	run 					dm this
	navigate 				dm this

	clickPushButton			dm this:inline
	clickRadioButton 		dm this:inline
	enablePushButton 		dm this:inline
	enableRadioButton 		dm this:inline
	
	setCheckBoxState 		dm this:inline
	setAdminShieldButton	dm this:inline
	setContent				dm this:inline
	updateContent			dm this:inline
	setIsBarMarquee			dm this:inline
	configureMarqueeBar		dm this:inline
	setBarPos 				dm this:inline
	setBarRange 			dm this:inline
	setBarState 			dm this:inline
	updateIcon 				dm this:inline
ends

; struct TASKDIALOG WINAPI_WINDOW
; 	const TFLAGS 			= 0
; 	const TCOMMON_BUTTONS 	= 0
; 	const TICON 			= NULL
; 	const TICON_FOOTER 		= NULL

; 	const TTITLE_PTR 				= NULL
; 	const TCONTENT_PTR 				= NULL
; 	const TCHECKBOX_PTR 			= NULL
; 	const TEXPANDED_CONTROL_PTR 	= NULL
; 	const TCOLLAPSED_CONTROL_PTR	= NULL
; 	const TEXPANDED_PTR 			= NULL
; 	const THEADER_PTR 				= NULL
; 	const TFOOTER_PTR 				= NULL

; 	const TDEF_PUSH_BUTTON 	= NULL
; 	const TDEF_RADIO_BUTTON = NULL
	
; 	sData 	TASK_SERVICEDATA
; 	result 	TDResult

; 	run 					dm this

; 	clickPushButton			dm this:inline
; 	clickRadioButton 		dm this:inline
; 	enablePushButton 		dm this:inline
; 	enableRadioButton 		dm this:inline
	
; 	setCheckBoxState 		dm this:inline
; 	setAdminShieldButton	dm this:inline
; 	setContent				dm this:inline
; 	updateContent			dm this:inline
; 	setIsBarMarquee			dm this:inline
; 	configureMarqueeBar		dm this:inline
; 	setBarPos 				dm this:inline
; 	setBarRange 			dm this:inline
; 	setBarState 			dm this:inline
; 	updateIcon 				dm this:inline
; ends

include "WINUSER/TaskDialog/TASKDIALOG.fasm"

macro @MakeTaskConfig name\{
	if name\#.hooks.counter > 0
		TASKDIALOG.useHooks
	end if
	if name\#.tevents.counter > 0
		TASKDIALOG.useEvents
	end if
	name\#.CONFIG @const TASKDIALOGCONFIG sizeof.TASKDIALOGCONFIG,\
		NULL, \
		NULL, \
		name\#.TFLAGS, \
		name\#.TCOMMON_BUTTONS,\
		name\#.TTITLE_PTR,\
		name\#.TICON,\
		name\#.THEADER_PTR,\
		name\#.TCONTENT_PTR,\
		name\#.TPUSH_BTN_COUNT, name\#.pushBtnArray, name\#.TDEF_PUSH_BUTTON,\
		name\#.TRADIO_BTN_COUNT, name\#.radioBtnArray, name\#.TDEF_RADIO_BUTTON,\
		name\#.TCHECKBOX_PTR,\
		name\#.TEXPANDED_PTR, name\#.TEXPANDED_CONTROL_PTR, \
		name\#.TCOLLAPSED_CONTROL_PTR,\
		name\#.TICON_FOOTER,\
		name\#.TFOOTER_PTR,\
		NULL,\
		NULL,\
		name\#.TWIDTH
\}

macro tasktempl taskTempl&\{
	\local ..anon
	make@tasktempl ..anon, taskTempl
\}

struc tasktempl taskTempl&\{
	make@tasktempl ., taskTempl
\}

macro make@tasktempl name, taskTempl&\{
	match taskType _initvals, taskTempl\\{
		name taskType <taskType\\#.tevents, taskType\\#.hooks>, taskType\\#.CONFIG, _initvals
	rept 0\\{\\}match taskType, taskTempl\\{
		name taskTempl <taskType\\#.tevents, taskType\\#.hooks>, taskType\\#.CONFIG
	\\}
\}

@endm
