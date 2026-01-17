@module IUserNotification

include "IUnknown.inc"

NIIF_NONE 				= 0x00000000
NIIF_INFO 				= 0x00000001
NIIF_WARNING 			= 0x00000002
NIIF_ERROR 				= 0x00000003
NIIF_USER 				= 0x00000004
NIIF_NOSOUND 			= 0x00000010
NIIF_LARGE_ICON 		= 0x00000010
NIIF_RESPECT_QUIET_TIME = 0x00000080
NIIF_ICON_MASK 			= 0x0000000F

struct IUserNotification IUnknown
    SetBalloonInfo  dm ?:virtual/std ; HRESULT SetBalloonInfo(this, LPCWSTR pszTitle, LPCWSTR pszText, DWORD dwInfoFlags)
    SetBalloonRetry dm ?:virtual/std ; HRESULT SetBalloonRetry(this, DWORD dwShowTime, DWORD dwInterval, DWORD cRetryCount)
    SetIconInfo     dm ?:virtual/std ; HRESULT SetIconInfo(this, HICON hIcon, LPCWSTR pszToolTip)
    Show            dm ?:virtual/std ; HRESULT Show(this, IQueryContinue *pqc, DWORD dwContinuePollInterval)
ends

IID_IUserNotification   @const GUID 0xBA9711BA,0x5893,0x4787,<0xA7,0xE1,0x41,0x27,0x71,0x51,0x55,0x0B>
CLSID_UserNotification  @const GUID 0x0010890E,0x8789,0x413C,<0xAD,0xBC,0x48,0xF5,0xB5,0x11,0xB3,0xAF>

@endm
