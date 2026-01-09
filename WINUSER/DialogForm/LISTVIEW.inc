@module LISTVIEW

LVS_EX_GRIDLINES              = 0x00000001
LVS_EX_SUBITEMIMAGES          = 0x00000002
LVS_EX_CHECKBOXES             = 0x00000004
LVS_EX_TRACKSELECT            = 0x00000008
LVS_EX_HEADERDRAGDROP         = 0x00000010
LVS_EX_FULLROWSELECT          = 0x00000020
LVS_EX_ONECLICKACTIVATE       = 0x00000040
LVS_EX_TWOCLICKACTIVATE       = 0x00000080
LVS_EX_FLATSB                 = 0x00000100
LVS_EX_REGIONAL               = 0x00000200
LVS_EX_INFOTIP                = 0x00000400
LVS_EX_UNDERLINEHOT           = 0x00000800
LVS_EX_UNDERLINECOLD          = 0x00001000
LVS_EX_MULTIWORKAREAS         = 0x00002000
LVS_EX_LABELTIP               = 0x00004000
LVS_EX_BORDERSELECT           = 0x00008000
LVS_EX_DOUBLEBUFFER           = 0x00010000
LVS_EX_HIDELABELS             = 0x00020000
LVS_EX_SINGLEROW              = 0x00040000
LVS_EX_SNAPTOGRID             = 0x00080000
LVS_EX_SIMPLESELECT           = 0x00100000

LVS_EX_JUSTIFYCOLUMNS         = 0x00200000
LVS_EX_TRANSPARENTBKGND       = 0x00400000
LVS_EX_TRANSPARENTSHADOWTEXT  = 0x00800000
LVS_EX_AUTOAUTOARRANGE        = 0x01000000
LVS_EX_HEADERINALLVIEWS       = 0x02000000
LVS_EX_AUTOCHECKSELECT        = 0x08000000
LVS_EX_AUTOSIZECOLUMNS        = 0x10000000
LVS_EX_COLUMNSNAPPOINTS       = 0x40000000
LVS_EX_COLUMNOVERFLOW         = 0x80000000

LVM_SETUNICODEFORMAT = 0x2005
LVM_GETHEADER = LVM_FIRST + 31

struct LISTVIEW CONTROL
	const CType equ "SysListView32"

	insertColumn 		dm this:inline ; (this, index, column: P_LV_COLUMN)
	setColumn 			dm this:inline
	setColumnW 			dm this:inline
	insertItem 			dm this:inline ; (this, item: P_LVITEM)
	getItem 			dm this:inline
	getItemW 			dm this:inline
	setItemText			dm this:inline ; (this, index, item: P_LVITEM)
	setItemTextW		dm this:inline
	setItem				dm this:inline
	setColumnWidth		dm this:inline ; (this, index, width)
	deleteAllItems		dm this:inline ; (this)
	getHeader 			dm this:inline
	getExtendedStyle	dm this:inline
	getItemsCount		dm this:inline
	setBkColor 			dm this:inline ; (this, color: COLORREF)
	setTextColor		dm this:inline
	setTextBkColor		dm this:inline
	setUnicodeFormat 	dm this:inline ; (this, enabled: BOOL)
	setExtendedStyle	dm this:inline ; (this, mask: [DWORD_FLAGS | 0], style: [DWORD_FLAGS])
ends

struct NMLISTVIEW
	hdr 		NMHDR
	iItem		rd 1
	iSubItem	rd 1
	uNewState	rd 1
	uOldState	rd 1
	uChanged	rd 1
	match =x86, __architecture\{
    	ptAction 	POINT
	\}
	match =x64, __architecture\{
		struct
    		ptAction 	POINT
    		rd 1
    	ends
	\}
    lParam 		rptr 1
ends

struct LVITEM
	mask		rd 1;
	iItem		rd 1;
	iSubItem	rd 1;
	state		rd 1;
	stateMask	rptr 1;
	pszText 	rptr 1;
	cchTextMax	rd 1;
	iImage		rd 1;
	lParam 		rptr 1;
	iIndent		rd 1;
	iGroupId	rd 1;
	cColumns	rptr 1;
	puColumns 	rptr 1;
	piColFmt 	rptr 1;
	iGroup 		rd 1;
ends

struct LVCOLUMN
	mask 		rd 1
	fmt			rd 1;
	cx			rptr 1;
	pszText 	rptr 1;
	cchTextMax	rd 1;
	iSubItem	rd 1;
	iImage		rd 1;
	iOrder		rd 1;
	cxMin		rd 1;
	cxDefault	rd 1;
	cxIdeal		rptr 1;
ends

include "LISTVIEW.fasm"

@endm
