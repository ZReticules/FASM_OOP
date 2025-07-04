@module IFileDialog
	include "IModalWindow.inc"

	struct IFileDialog IModalWindow
	    SetFileTypes      	dm ?:virtual/std ; HRESULT SetFileTypes(this, UINT cFileTypes, const COMDLG_FILTERSPEC *rgFilterSpec)
	    SetFileTypeIndex  	dm ?:virtual/std ; HRESULT SetFileTypeIndex(this, UINT iFileType)
	    GetFileTypeIndex  	dm ?:virtual/std ; HRESULT GetFileTypeIndex(this, UINT *piFileType)
	    Advise            	dm ?:virtual/std ; HRESULT Advise(this, IFileDialogEvents *pfde, DWORD *pdwCookie)
	    Unadvise          	dm ?:virtual/std ; HRESULT Unadvise(this, DWORD dwCookie)
	    SetOptions        	dm ?:virtual/std ; HRESULT SetOptions(this, FILEOPENDIALOGOPTIONS fos)
	    GetOptions        	dm ?:virtual/std ; HRESULT GetOptions(this, FILEOPENDIALOGOPTIONS *pfos)
	    SetDefaultFolder  	dm ?:virtual/std ; HRESULT SetDefaultFolder(this, IShellItem *psi)
	    SetFolder         	dm ?:virtual/std ; HRESULT SetFolder(this, IShellItem *psi)
	    GetFolder         	dm ?:virtual/std ; HRESULT GetFolder(this, IShellItem **ppsi)
	    GetCurrentSelection dm ?:virtual/std ; HRESULT GetCurrentSelection(this, IShellItem **ppsi)
	    SetFileName       	dm ?:virtual/std ; HRESULT SetFileName(this, LPCWSTR pszName)
	    GetFileName       	dm ?:virtual/std ; HRESULT GetFileName(this, LPWSTR *pszName)
	    SetTitle          	dm ?:virtual/std ; HRESULT SetTitle(this, LPCWSTR pszTitle)
	    SetOkButtonLabel  	dm ?:virtual/std ; HRESULT SetOkButtonLabel(this, LPCWSTR pszText)
	    SetFileNameLabel  	dm ?:virtual/std ; HRESULT SetFileNameLabel(this, LPCWSTR pszLabel)
	    GetResult         	dm ?:virtual/std ; HRESULT GetResult(this, IShellItem **ppsi)
	    AddPlace          	dm ?:virtual/std ; HRESULT AddPlace(this, IShellItem *psi, FDAP fdap)
	    SetDefaultExtension dm ?:virtual/std ; HRESULT SetDefaultExtension(this, LPCWSTR pszDefaultExtension)
	    Close             	dm ?:virtual/std ; HRESULT Close(this, HRESULT hr)
	    SetClientGuid     	dm ?:virtual/std ; HRESULT SetClientGuid(this, REFGUID guid)
	    ClearClientData   	dm ?:virtual/std ; HRESULT ClearClientData(this)
	    SetFilter         	dm ?:virtual/std ; HRESULT SetFilter(this, IShellItemFilter *pFilter)
	ends
	
	IID_IFileDialog        @const GUID 0x42F85136,0xDB7E,0x439C,<0x85,0xF1,0xE4,0x07,0x5D,0x13,0x5F,0xC8>
@endm
