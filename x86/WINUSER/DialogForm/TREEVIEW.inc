define TVM_SETBKCOLOR 	4381
define TVM_SETTEXTCOLOR	4382

define TVS_EX_NOSINGLECOLLAPSE	1
define TVS_EX_MULTISELECT		2
define TVS_EX_NOINDENTSTATE 	8

struct TREEVIEW CONTROL
	const CType 	equ "SysTreeView32"
	insertItem 		TV_INSERTSTRUCT NULL, TVI_LAST, <TVIF_TEXT, ?, ?, ?, ?, ?, ?, ?, ?, ?>
	addItem 		dm this:inline
	setBgColor 		override this:inline
	setTextColor 	dm this:inline	
ends

macro TREEVIEW.addItem this, pszText, hInsertAfter, hParent{
	local _this
	inlineObj _this, this, ecx
	match any, pszText\{
		; int3
		fillParam edx, pszText
		mov [_this+TREEVIEW.insertItem.item.pszText], edx
		; @call c [printf](pszText)
	\}
	match any, hInsertAfter\{
		fillParam edx, hInsertAfter
		mov [_this+TREEVIEW.insertItem.hInsertAfter], edx
	\}
	match any, hParent\{
		fillParam edx, hParent
		mov [_this+TREEVIEW.insertItem.hParent], edx
	\}
	@call [SendMessageA]([_this+TREEVIEW.hWnd], TVM_INSERTITEM, NULL, addr _this+TREEVIEW.insertItem)
}

macro TREEVIEW.setBgColor this, colorref{
	local _this
	inlineObj _this, this, ecx
	@call [SendMessageA]([_this+TREEVIEW.hWnd], TVM_SETBKCOLOR, 0, colorref)
}

macro TREEVIEW.setTextColor this, colorref{
	local _this
	inlineObj _this, this, ecx
	@call [SendMessageA]([_this+TREEVIEW.hWnd], TVM_SETTEXTCOLOR, 0, colorref)
}