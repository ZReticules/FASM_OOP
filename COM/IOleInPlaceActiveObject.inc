@module IOleInPlaceActiveObject

include "IOleWindow.inc"

IID_IOleInPlaceActiveObject @const GUID \
    0x00000117,0x0000,0x0000,<0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46>

struct IOleInPlaceActiveObject IOleWindow
    TranslateAccelerator     dm ?:virtual/std ; HRESULT TranslateAccelerator(this, LPMSG lpmsg)
    OnFrameWindowActivate    dm ?:virtual/std ; HRESULT OnFrameWindowActivate(this, BOOL fActivate)
    OnDocWindowActivate      dm ?:virtual/std ; HRESULT OnDocWindowActivate(this, BOOL fActivate)
    ResizeBorder             dm ?:virtual/std ; HRESULT ResizeBorder(this, LPCRECT prcBorder, 
                                            ; IOleInPlaceUIWindow *pUIWindow, BOOL fFrameWindow)
    EnableModeless           dm ?:virtual/std ; HRESULT EnableModeless(this, BOOL fEnable)
ends

@endm
