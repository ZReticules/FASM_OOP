@module IDocHostUIHandler

include "IUnknown.inc"
include "IDispatch.inc"

struct IDocHostUIHandler IUnknown
    ShowContextMenu         dm ?:virtual/std ; (this, dwID:DWORD, ppt:POINT*, pcmdtReserved:IUnknown*, pdispReserved:IDispatch*)
    GetHostInfo             dm ?:virtual/std ; (this, pInfo:DOCHOSTUIINFO*)
    ShowUI                  dm ?:virtual/std ; (this, dwID:DWORD, pActiveObject:IOleInPlaceActiveObject*, pCommandTarget:IOleCommandTarget*, pFrame:IOleInPlaceFrame*, pDoc:IOleInPlaceUIWindow*)
    HideUI                  dm ?:virtual/std ; (this)
    UpdateUI                dm ?:virtual/std ; (this)
    EnableModeless          dm ?:virtual/std ; (this, fEnable:BOOL)
    OnDocWindowActivate     dm ?:virtual/std ; (this, fActivate:BOOL)
    OnFrameWindowActivate   dm ?:virtual/std ; (this, fActivate:BOOL)
    ResizeBorder            dm ?:virtual/std ; (this, prcBorder:RECT*, pUIWindow:IOleInPlaceUIWindow*, fFrameWindow:BOOL)
    TranslateAccelerator    dm ?:virtual/std ; (this, lpMsg:MSG*, pguidCmdGroup:GUID*, nCmdID:DWORD)
    GetOptionKeyPath        dm ?:virtual/std ; (this, pchKey:LPWSTR*, dw:DWORD)
    GetDropTarget           dm ?:virtual/std ; (this, pDropTarget:IDropTarget*, ppDropTarget:IDropTarget**)
    GetExternal             dm ?:virtual/std ; (this, ppDispatch:IDispatch**)
    TranslateUrl            dm ?:virtual/std ; (this, dwTranslate:DWORD, pchURLIn:LPWSTR, ppchURLOut:LPWSTR*)
    FilterDataObject        dm ?:virtual/std ; (this, pDO:IDataObject*, ppDORet:IDataObject**)
ends

IID_IDocHostUIHandler @const GUID 0xBD3F23C0,0xD43E,0x11CF,<0x89,0x3B,0x00,0xAA,0x00,0xBD,0xCE,0x1A>

if used IDocHostUIHandler_s.used
    struct IDocHostUIHandler_s IUnknown_s
        QueryInterface          override this

        ShowContextMenu         dm this:virtual/std
        GetHostInfo             dm this:virtual/std
        ShowUI                  dm this:virtual/std
        HideUI                  dm this:virtual/std
        UpdateUI                dm this:virtual/std
        EnableModeless          dm this:virtual/std
        OnDocWindowActivate     dm this:virtual/std
        OnFrameWindowActivate   dm this:virtual/std
        ResizeBorder            dm this:virtual/std
        TranslateAccelerator    dm this:virtual/std
        GetOptionKeyPath        dm this:virtual/std
        GetDropTarget           dm this:virtual/std
        GetExternal             dm this:virtual/std
        TranslateUrl            dm this:virtual/std
        FilterDataObject        dm this:virtual/std
    ends

    include "IDocHostUIHandler.fasm"
end if

@endm
