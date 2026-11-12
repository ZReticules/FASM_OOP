@module IUserNotification2

include "IUnknown.inc"
include "IUserNotificationCallback.inc"

if ~definite NIIF_NONE
	NIIF_NONE 				= 0x00000000
	NIIF_INFO 				= 0x00000001
	NIIF_WARNING 			= 0x00000002
	NIIF_ERROR 				= 0x00000003
	NIIF_USER 				= 0x00000004
	NIIF_NOSOUND 			= 0x00000010
	NIIF_LARGE_ICON 		= 0x00000010
	NIIF_RESPECT_QUIET_TIME = 0x00000080
	NIIF_ICON_MASK 			= 0x0000000F
end if

struct IUserNotification2 IUnknown
    SetBalloonInfo  dm ?:virtual/std ; HRESULT SetBalloonInfo(this, LPCWSTR pszTitle, LPCWSTR pszText, DWORD dwInfoFlags)
    SetBalloonRetry dm ?:virtual/std ; HRESULT SetBalloonRetry(this, DWORD dwShowTime, DWORD dwInterval, DWORD cRetryCount)
    SetIconInfo     dm ?:virtual/std ; HRESULT SetIconInfo(this, HICON hIcon, LPCWSTR pszToolTip)
    Show            dm ?:virtual/std ; HRESULT Show(this, IUserNotificationCallback *pSink, DWORD dwContinuePollInterval, IUserNotificationCallback *pSink)
    PlaySound       dm ?:virtual/std ; HRESULT PlaySound(this, LPCWSTR pszSound)
ends

IID_IUserNotification2  @const GUID 0x215913CC,0x57EB,0x4FAB,<0xAB,0x5A,0xE5,0xFA,0x7B,0xEA,0x2A,0x6C>

if ~definite CLSID_IUserNotification
	CLSID_IUserNotification  @const GUID 0x0010890E,0x8789,0x413C,<0xAD,0xBC,0x48,0xF5,0xB5,0x11,0xB3,0xAF>
end if

if used CLSID_IUserNotification2
    CLSID_IUserNotification2 = CLSID_IUserNotification
end if

@endm
