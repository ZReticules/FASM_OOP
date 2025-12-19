@module Winuser
	
	include "String.inc"


	; NM_OUTOFMEMORY     	= (NM_FIRST-1)
	; NM_CLICK           	= (NM_FIRST-2)     
	; NM_DBLCLK          	= (NM_FIRST-3)
	; NM_RETURN          	= (NM_FIRST-4)
	; NM_RCLICK          	= (NM_FIRST-5)     
	; NM_RDBLCLK         	= (NM_FIRST-6)
	; NM_SETFOCUS        	= (NM_FIRST-7)
	; NM_KILLFOCUS       	= (NM_FIRST-8)
	; NM_CUSTOMDRAW      	= (NM_FIRST-12)
	; NM_HOVER           	= (NM_FIRST-13)
	; NM_NCHITTEST       	= (NM_FIRST-14)    
	; NM_KEYDOWN         	= (NM_FIRST-15)    
	; NM_RELEASEDCAPTURE 	= (NM_FIRST-16)
	; NM_SETCURSOR       	= (NM_FIRST-17)    
	; NM_CHAR            	= (NM_FIRST-18)    
	; NM_TOOLTIPSCREATED 	= (NM_FIRST-19)    
	; NM_LDOWN           	= (NM_FIRST-20)
	; NM_RDOWN           	= (NM_FIRST-21)
	; NM_THEMECHANGED    	= (NM_FIRST-22)

	; define NM_FONTCHANGED          (NM_FIRST-23)
	; define NM_CUSTOMTEXT           (NM_FIRST-24)    
	; define NM_TVSTATEIMAGECHANGING (NM_FIRST-24)    

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

	importlib comctl32,\
		TaskDialog

	importlib msvcrt,\
		free

	define GA_PARENT	1
	define GA_ROOT		2
	define GA_ROOTOWNER	3

	define NULL 0
	NONE fix

	;button styles
		define BS_SPLITBUTTON          0x0000000C
		define BS_DEFSPLITBUTTON       0x0000000D
		define BS_COMMANDLINK          0x0000000E
		define BS_DEFCOMMANDLINK       0x0000000F

	; MSGBOX Styles
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

		define GWL_EXSTYLE		-20
		define GWL_HINSTANCE	-6
		define GWL_ID			-12
		define GWL_STYLE		-16
		define GWL_USERDATA		-21
		define GWL_WNDPROC		-4

		define HWND_BROADCAST  	0xffff

	;WS_EX
		define WS_EX_COMPOSITED 33554432

	struct WND
		msgBox 					dm this:inline
		msgBoxW					dm this:inline
		setTimer				dm this:inline
		killTimer				dm this:inline
		taskDlg 				dm this:inline
		deleteStockIcon			dm this:inline
		dragFilesCount			dm this:inline
		isThemeLight			dm this

		repaintWindow			dm this
		; repaintWindowWithBack	dm this
		getStockIcon			dm this
		dragFileNameString		dm this
		
		const darkThemeColor 	equ 0x333333
		const darkThemeCFD		equ L "DarkMode_CFD"
		const darkThemeExplorer	equ L "DarkMode_Explorer"
		
		db ?
	ends

	; DarkThemeCFD = $			
	; 	du "DarkMode_CFD", 0
	; DarkThemeExplorer = $
	; 	du "DarkMode_Explorer", 0

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

	TD_WARNING_ICON 	= 65535
	TD_ERROR_ICON 		= 65534
	TD_INFORMATION_ICON = 65533
	TD_SHIELD_ICON 		= 65532

	TDCBF_OK_BUTTON 	= 0x1
	TDCBF_YES_BUTTON 	= 0x2
	TDCBF_NO_BUTTON 	= 0x4
	TDCBF_CANCEL_BUTTON = 0x8
	TDCBF_RETRY_BUTTON 	= 0x10
	TDCBF_CLOSE_BUTTON 	= 0x20

	include "Graphics\Graphics.inc"
	include "WINUSER\CUSTOMDRAW.inc"
	include "WINUSER\Winuser.fasm"
	include "WINUSER\MsgBox.inc"
	include "WINUSER\StockIcons.inc"
	include "WINUSER\ColorsHook\ColorsHook.inc"
	include "WINUSER\MapDialog.inc"

	struct MENUINFO
		cbSize			dd ?
		fMask			dd ?
		dwStyle			dd ?
		cyMax			dd ?
		hbrBack 		dptr ?
		dwContextHelpID	dptr ?
	  	dwMenuData 		dptr ?
	ends

	define MIM_APPLYTOSUBMENUS 0x80000000
	define MIM_BACKGROUND 0x00000002
	define MIM_HELPID 0x00000004
	define MIM_MAXHEIGHT 0x00000001
	define MIM_MENUDATA 0x00000008
	define MIM_STYLE 0x00000010

	define MNS_AUTODISMISS 0x10000000
	define MNS_CHECKORBMP 0x04000000
	define MNS_DRAGDROP 0x20000000
	define MNS_MODELESS 0x40000000
	define MNS_NOCHECK 0x80000000
	define MNS_NOTIFYBYPOS 0x08000000

	struct MENUBARINFO
		cbSize 		dd ?;
		match =x64, __architecture\{
			struct
				rcBar 		RECT;
				dd ?
			ends
		rept 0\{\}rept 1\{
			struct
				rcBar 		RECT;
			ends
		\}
		hMenu		dptr ?;
		hwndMenu	dptr ?;
		dwFlags 	dptr ?
	ends

@endm
