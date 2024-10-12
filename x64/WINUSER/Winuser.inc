importlib user32,\
	MessageBoxA,\
	SetTimer,\
	KillTimer,\
	GetFocus,\
	ChangeWindowMessageFilterEx,\
	GetFocus,\
	GetActiveWindow,\
	SendMessageW,\
	EnumWindows,\
	GetParent,\
	GetAncestor,\
	GetDesktopWindow,\
	GetDC,\
	GetClientRect,\
	GetWindowRect,\
	ReleaseDC,\
	LoadImageA

importlib msvcrt,\
	free

define GA_PARENT	1
define GA_ROOT		2
define GA_ROOTOWNER	3

define NULL 0
NONE fix
	define BS_SPLITBUTTON          0x0000000C
	define BS_DEFSPLITBUTTON       0x0000000D
	define BS_COMMANDLINK          0x0000000E
	define BS_DEFCOMMANDLINK       0x0000000F

; uType
	define MB_OK	 				0x00000000
	define MB_OKCANCEL	 			0x00000001
	define MB_ABORTRETRYIGNORE		0x00000002
	define MB_YESNOCANCEL	 		0x00000003
	define MB_YESNO	 				0x00000004
	define MB_RETRYCANCEL	 		0x00000005
	define MB_CANCELTRYCONTINUE		0x00000006
	define MB_HELP	 				0x00004000

	define MB_ICONEXCLAMATION 		0x00000030
	define MB_ICONWARNING 			0x00000030
	define MB_ICONINFORMATION 		0x00000040
	define MB_ICONASTERISK 			0x00000040
	define MB_ICONQUESTION 			0x00000020
	define MB_ICONSTOP 				0x00000010
	define MB_ICONERROR 			0x00000010
	define MB_ICONHAND 				0x00000010

	define MB_DEFBUTTON1 			0x00000000
	define MB_DEFBUTTON2 			0x00000100
	define MB_DEFBUTTON3 			0x00000200
	define MB_DEFBUTTON4 			0x00000300

	define MB_APPLMODAL 			0x00000000
	define MB_SYSTEMMODAL 			0x00001000
	define MB_TASKMODAL 			0x00002000

	define MB_SETFOREGROUND 		0x00010000
	define MB_DEFAULT_DESKTOP_ONLY 	0x00020000
	define MB_RIGHT 				0x00080000
	define MB_RTLREADING 			0x00100000
	define MB_TOPMOST 				0x00040000
	define MB_SERVICE_NOTIFICATION 	0x00200000

; rax
	define IDOK 					1
	define IDCANCEL 				2
	define IDABORT 					3
	define IDRETRY 					4
	define IDIGNORE 				5
	define IDYES 					6
	define IDNO 					7
	define IDTRYAGAIN 				10
	define IDCONTINUE 				11

; icons
	define IDI_APPLICATION	32512
	define IDI_ERROR		32513
	define IDI_QUESTION		32514
	define IDI_WARNING		32515
	define IDI_INFORMATION	32516
	define IDI_WINLOGO		32517
	define IDI_SHIELD		32518

	define GWL_EXSTYLE		–20
	define GWL_HINSTANCE	–6
	define GWL_ID			-12
	define GWL_STYLE		-16
	define GWL_USERDATA		-21
	define GWL_WNDPROC		–4

	define HWND_BROADCAST  	0xffff

;WS_EX
	define WS_EX_COMPOSITED 33554432

struct WND
	msgBox 					dm this:inline
	setTimer				dm this:inline
	killTimer				dm this:inline
	getStockIcon			dm this
	const darkThemeColor 	= 0x2E2E2E
	const darkThemeCFD		= DarkThemeCFD
	const darkThemeExplorer	= DarkThemeExplorer
	db ?
ends

DarkThemeCFD = $			
	du "DarkMode_CFD", 0
DarkThemeExplorer = $
	du "DarkMode_Explorer", 0

DWMWA enum\
	NCRENDERING_ENABLED,\
	NCRENDERING_POLICY,\
	TRANSITIONS_FORCEDISABLED,\
	ALLOW_NCPAINT,\
	CAPTION_BUTTON_BOUNDS,\
	NONCLIENT_RTL_LAYOUT,\
	FORCE_ICONIC_REPRESENTATION,\
	FLIP3D_POLICY,\
	EXTENDED_FRAME_BOUNDS,\
	HAS_ICONIC_BITMAP,\
	DISALLOW_PEEK,\
	EXCLUDED_FROM_PEEK,\
	CLOAK,\
	CLOAKED,\
	FREEZE_REPRESENTATION,\
	PASSIVE_UPDATE_MODE,\
	USE_HOSTBACKDROPBRUSH,\
	USE_IMMERSIVE_DARK_MODE = 20,\
	WINDOW_CORNER_PREFERENCE = 33,\
	BORDER_COLOR,\
	CAPTION_COLOR,\
	TEXT_COLOR,\
	VISIBLE_FRAME_BORDER_THICKNESS,\
	SYSTEMBACKDROP_TYPE,\
	LAST

DWMWCP enum\ 
	DONOTROUND=1,\
	ROUND,\
	ROUNDSMALL

define MAX_PATH 260

macro WND.msgBox lpText, lpCaption = NULL, utype = MB_OK, hWnd = NULL{
	@call [MessageBoxA](hWnd, lpText, lpCaption, utype)
}

macro WND.setTimer uElapse, lpTimerFunc, nIDEvent=0, hWnd=NULL{
	@call [SetTimer](hWnd, nIDEvent, uElapse, lpTimerFunc)
}

macro WND.killTimer nIDEvent, hWnd=NULL{
	@call [KillTimer](hwnd, nIDEvent)
}

include_once "TOOLS\x64\WINUSER\MsgBox.inc"
include_once "TOOLS\x64\WINUSER\StockIcons.inc"
