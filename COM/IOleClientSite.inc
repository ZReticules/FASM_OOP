@module IOleClientSite

include "IUnknown.inc"

struct IOleClientSite IUnknown
    SaveObject            	dm ?:virtual/std ; (this)
    GetMoniker            	dm ?:virtual/std ; (this, dwAssign:DWORD, dwWhichMoniker:DWORD, ppmk:IMoniker**)
    GetContainer          	dm ?:virtual/std ; (this, ppContainer:IOleContainer**)
    ShowObject            	dm ?:virtual/std ; (this)
    OnShowWindow          	dm ?:virtual/std ; (this, fShow:BOOL)
    RequestNewObjectLayout	dm ?:virtual/std ; (this)
ends

IID_IOleClientSite @const GUID 0x00000118,0x0000,0x0000,<0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46>

if used IOleClientSite_s.used
    struct IOleClientSite_s IUnknown_s
        QueryInterface          override this

        SaveObject              dm this:virtual/std
        GetMoniker              dm this:virtual/std
        GetContainer            dm this:virtual/std
        ShowObject              dm this:virtual/std
        OnShowWindow            dm this:virtual/std
        RequestNewObjectLayout  dm this:virtual/std
    ends

    include "IOleClientSite.fasm"
end if

@endm
