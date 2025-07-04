@module IFileOpenDialog
	include "IFileDialog.inc"

	struct IFileOpenDialog IFileDialog
	    GetResults      	dm ?:virtual/std ; HRESULT GetResults(this, IShellItemArray **ppenum)
	    GetSelectedItems	dm ?:virtual/std ; HRESULT GetSelectedItems(this, IShellItemArray **ppsai)
	ends

	CLSID_FileOpenDialog  @const GUID 0xDC1C5A9C,0xE88A,0x4DDE,<0xA5,0xA1,0x60,0xF8,0x2A,0x20,0xAE,0xF7>
	IID_IFileOpenDialog   @const GUID 0xD57C7288,0xD4AD,0x4768,<0xBE,0x02,0x9D,0x96,0x95,0x32,0xD9,0x60>
@endm
