@module IShellItem
	include "COM.inc"

	struct IShellItem IUnknown
	    BindToHandler      dm ?:virtual/std ; HRESULT BindToHandler(this, IBindCtx *pbc, REFGUID bhid, REFIID riid, void **ppv)
	    GetParent          dm ?:virtual/std ; HRESULT GetParent(this, IShellItem **ppsi)
	    GetDisplayName     dm ?:virtual/std ; HRESULT GetDisplayName(this, SIGDN sigdnName, LPWSTR *ppszName)
	    GetAttributes      dm ?:virtual/std ; HRESULT GetAttributes(this, SFGAOF sfgaoMask, SFGAOF *psfgaoAttribs)
	    Compare            dm ?:virtual/std ; HRESULT Compare(this, IShellItem *psi, SICHINTF hint, int *piOrder)
	ends

	SIGDN enum \
		NORMALDISPLAY = 0,\
		PARENTRELATIVEPARSING = 0x80018001,\
		DESKTOPABSOLUTEPARSING = 0x80028000,\
		PARENTRELATIVEEDITING = 0x80031001,\
		DESKTOPABSOLUTEEDITING = 0x8004c000,\
		FILESYSPATH = 0x80058000,\
		URL = 0x80068000,\
		PARENTRELATIVEFORADDRESSBAR = 0x8007c001,\
		PARENTRELATIVE = 0x80080001,\
		PARENTRELATIVEFORUI = 0x80094001

	IID_IShellItem @const GUID 0x43826D1E,0xE718,0x42EE,<0xBC,0x55,0xA1,0xE2,0x61,0xC3,0x7B,0xFE>
@endm
