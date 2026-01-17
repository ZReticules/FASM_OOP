@module IOleContainer

include "IParseDisplayName.inc"

struct IOleContainer IParseDisplayName
    EnumObjects		dm ?:virtual/std ; HRESULT EnumObjects(this, DWORD grfFlags, IEnumUnknown **ppenum)
    LockContainer	dm ?:virtual/std ; HRESULT LockContainer(this, BOOL fLock)
ends

@endm
