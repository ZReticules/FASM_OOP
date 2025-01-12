
define TVM_SETBKCOLOR 	4381
define TVM_SETTEXTCOLOR	4382

TVN_ITEMCHANGED = 0x5e

define TVS_EX_NOSINGLECOLLAPSE	1
define TVS_EX_MULTISELECT		2
define TVS_EX_NOINDENTSTATE 	8

macro TREEVIEW.addItem this, lpTV_INSERTSTRUCT, pszText{
	local _this
	inlineObj _this, this, ecx
	match any, pszText\{
		inlineObj _insert, lpTV_INSERTSTRUCT, edx
		; int3
		fillParam eax, pszText
		mov [_insert+TV_INSERTSTRUCT.item.pszText], eax
		; @call c [printf](pszText)
	\}
	@call [SendMessageA]([_this+TREEVIEW.hWnd], TVM_INSERTITEM, NULL, lpTV_INSERTSTRUCT)
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