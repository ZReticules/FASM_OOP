@module TREEVIEW

	struct TREEVIEW CONTROL
		const CType 	equ "SysTreeView32"
		addItem 		dm this:inline
		addItemW		dm this:inline
		setBgColor 		override this:inline
		setTextColor 	dm this:inline	
	ends

	struct NMTVITEMCHANGE
		hdr 		NMHDR
		uChanged	dd ?
		hItem 		dd ?
		uStateNew	dd ?
		uStateOld 	dd ?
		lParam 		dd ?
	ends

	struct TVITEMA
		mask			dd ?;
		hItem 			dd ?
		state			dd ?;
		stateMask		dd ?;
		pszText			dd ?
		cchTextMax		dd ?
		iImage			dd ?
		iSelectedImage	dd ?
		cChildren		dd ?
		lParam 			dd ?
	ends
	
	struct NMTREEVIEWA
		hdr 	NMHDR
		action 	dd ?
		itemOld	TVITEMA
		itemNew	TVITEMA
		ptDrag	POINT   
	ends

	include "TREEVIEW.asm"

@endm