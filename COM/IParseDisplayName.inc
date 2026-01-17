@module IParseDisplayName

include "IUnknown.inc"

IID_IParseDisplayName @const GUID \
    0x0000011A,0x0000,0x0000,<0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46>

struct IParseDisplayName IUnknown
    ParseDisplayName dm ?:virtual/std ; HRESULT ParseDisplayName(this, IBindCtx *pbc, LPOLESTR pszDisplayName, 
                                    ; ULONG *pchEaten, IMoniker **ppmkOut)
ends

@endm
