@module IUserNotificationCallback

include "IUnknown.inc"

struct IUserNotificationCallback IUnknown
    OnBalloonUserClick  dm ?:virtual/std ; HRESULT OnBalloonUserClick(this, POINT *pt)
    OnContextMenu       dm ?:virtual/std ; HRESULT OnContextMenu(this, POINT *pt)
    OnLeftClick         dm ?:virtual/std ; HRESULT OnLeftClick(this, POINT *pt)
ends

IID_IUserNotificationCallback @const GUID 0x19108294,0x0441,0x4AFF,<0x80,0x13,0xFA,0x0A,0x73,0x0B,0x0B,0xEA>

if used IUserNotificationCallback_s.used

struct IUserNotificationCallback_s IUnknown_s
    QueryInterface override this

    OnBalloonUserClick  dm this:virtual/std
    OnContextMenu       dm this:virtual/std
    OnLeftClick         dm this:virtual/std
ends

end if

include "IUserNotificationCallback.fasm"

@endm
