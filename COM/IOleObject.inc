@module IOleObject

include "IUnknown.inc"

OLEIVERB_PRIMARY            = 0
OLEIVERB_SHOW               = -1
OLEIVERB_OPEN               = -2
OLEIVERB_HIDE               = -3
OLEIVERB_UIACTIVATE         = -4
OLEIVERB_INPLACEACTIVATE    = -5
OLEIVERB_DISCARDUNDOSTATE   = -6

struct IOleObject IUnknown
    SetClientSite    	dm ?:virtual/std ; (this, pClientSite:IOleClientSite*)
    GetClientSite    	dm ?:virtual/std ; (this, ppClientSite:IOleClientSite**)
    SetHostNames     	dm ?:virtual/std ; (this, szContainerApp:LPCWSTR, szContainerObj:LPCWSTR)
    Close            	dm ?:virtual/std ; (this, dwSaveOption:DWORD)
    SetMoniker       	dm ?:virtual/std ; (this, dwWhichMoniker:DWORD, pmk:IMoniker*)
    GetMoniker       	dm ?:virtual/std ; (this, dwAssign:DWORD, dwWhichMoniker:DWORD, ppmk:IMoniker**)
    InitFromData     	dm ?:virtual/std ; (this, pDataObject:IDataObject*, fCreation:BOOL, dwReserved:DWORD)
    GetClipboardData	dm ?:virtual/std ; (this, dwReserved:DWORD, ppDataObject:IDataObject**)
    DoVerb           	dm ?:virtual/std ; (this, iVerb:LONG, lpmsg:LPMSG, pActiveSite:IOleClientSite*, lindex:LONG, hwndParent:HWND, lprcPosRect:LPRECT)
    EnumVerbs        	dm ?:virtual/std ; (this, ppEnumOleVerb:IEnumOLEVERB**)
    Update           	dm ?:virtual/std ; (this)
    IsUpToDate       	dm ?:virtual/std ; (this)
    GetUserClassID  	dm ?:virtual/std ; (this, pClsid:CLSID*)
    GetUserType      	dm ?:virtual/std ; (this, dwFormOfType:DWORD, pszUserType:LPWSTR*)
    SetExtent        	dm ?:virtual/std ; (this, dwDrawAspect:DWORD, psizel:SIZEL*)
    GetExtent        	dm ?:virtual/std ; (this, dwDrawAspect:DWORD, psizel:SIZEL*)
    Advise           	dm ?:virtual/std ; (this, pAdvSink:IAdviseSink*, pdwConnection:DWORD*)
    Unadvise         	dm ?:virtual/std ; (this, dwConnection:DWORD)
    EnumAdvise       	dm ?:virtual/std ; (this, ppEnumAdvise:IEnumSTATDATA**)
    GetMiscStatus    	dm ?:virtual/std ; (this, dwAspect:DWORD, pdwStatus:DWORD*)
    SetColorScheme   	dm ?:virtual/std ; (this, pLogpal:LOGPALETTE*)
ends

IID_IOleObject @const GUID 0x00000112,0x0000,0x0000,<0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46>

@endm
