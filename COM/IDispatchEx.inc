@module IDispatchEx

include "IDispatch.inc"

struct IDispatchEx IDispatch
    GetDispID           	dm ?:virtual/std ; (this, bstrName:BSTR, grfdex:DWORD, pid:DISPID*)
    InvokeEx            	dm ?:virtual/std ; (this, id:DISPID, lcid:LCID, wFlags:WORD, 
                                         ; pdp:P_DISPPARAMS, pvarRes:P_VARIANT, 
                                         ; pei:P_EXCEPINFO, pspCaller:PSERVICE_PROVIDER)
    DeleteMemberByName  	dm ?:virtual/std ; (this, bstrName:BSTR, grfdex:DWORD)
    DeleteMemberByDispID 	dm ?:virtual/std ; (this, id:DISPID, grfdex:DWORD)
    GetMemberProperties 	dm ?:virtual/std ; (this, id:DISPID, grfdexFetch:DWORD, pgrfdex:P_DWORD)
    GetMemberName       	dm ?:virtual/std ; (this, id:DISPID, pbstrName:BSTR*)
    GetNextDispID       	dm ?:virtual/std ; (this, grfdex:DWORD, id:DISPID, pid:DISPID*)
    GetNameSpaceParent  	dm ?:virtual/std ; (this, ppunk:IUnknown**)
ends

IID_IDispatchEx @const GUID 0xA6EF9860,0xC720,0x11D0,<0x93,0x37,0x00,0xA0,0xC9,0x0D,0xCA,0xA9>

IDispatchEx.fdexNameCaseSensitive = 0x00000001
IDispatchEx.fdexNameEnsure = 0x00000002
IDispatchEx.fdexNameImplicit = 0x00000004
IDispatchEx.fdexNameCaseInsensitive = 0x00000008
IDispatchEx.fdexNameInternal = 0x00000010
IDispatchEx.fdexNameNoDynamicProperties = 0x00000020
IDispatchEx.fdexNameFromGet = 0x00000100
IDispatchEx.fdexNameFromSet = 0x00000200
IDispatchEx.fdexNameFromIDispatch = 0x00000400
IDispatchEx.fdexNameFromScript = 0x00000800
IDispatchEx.fdexNameFromServer = 0x00001000
IDispatchEx.fdexNameFromTopLevelWindow = 0x00002000
IDispatchEx.fdexNameFromNextLevel = 0x00004000
IDispatchEx.fdexNameFromPreviousLevel = 0x00008000
IDispatchEx.fdexNameFromParent = 0x00010000
IDispatchEx.fdexNameFromChild = 0x00020000
IDispatchEx.fdexNameFromSibling = 0x00040000
IDispatchEx.fdexNameFromAncestor = 0x00080000
IDispatchEx.fdexNameFromDescendant = 0x00100000
IDispatchEx.fdexNameFromDocument = 0x00200000
IDispatchEx.fdexNameFromFrame = 0x00400000
IDispatchEx.fdexNameFromPopup = 0x00800000
IDispatchEx.fdexNameFromDialog = 0x01000000
IDispatchEx.fdexNameFromMenu = 0x02000000
IDispatchEx.fdexNameFromToolbar = 0x04000000
IDispatchEx.fdexNameFromStatusbar = 0x08000000
IDispatchEx.fdexNameFromContext = 0x10000000
IDispatchEx.fdexNameFromAny = 0x20000000

@endm
