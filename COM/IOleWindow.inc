@module IOleWindow

include "IUnknown.inc"

IID_IOleWindow @const GUID \
    0x00000114,0x0000,0x0000,<0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46>

struct IOleWindow IUnknown
    GetWindow               dm ?:virtual/std ; HRESULT GetWindow(this, HWND *phwnd)
    ContextSensitiveHelp    dm ?:virtual/std ; HRESULT ContextSensitiveHelp(this, BOOL fEnterMode)
ends

@endm
