@module IHTMLDOMNode

include "IDispatch.inc"

IID_IHTMLDOMNode @const GUID \
    0x3050F5DA,0x98B5,0x11CF,<0xBB,0x82,0x00,0xAA,0x00,0xBD,0xCE,0x0B>

struct IHTMLDOMNode IDispatch
	get_nodeType            dm ?:virtual/std ; HRESULT get_nodeType(this, long *p)
	get_parentNode          dm ?:virtual/std ; HRESULT get_parentNode(this, IHTMLDOMNode **p)
	hasChildNodes           dm ?:virtual/std ; HRESULT hasChildNodes(this, VARIANT_BOOL *fChildren)
	get_childNodes          dm ?:virtual/std ; HRESULT get_childNodes(this, IDispatch **p)
	get_attributes          dm ?:virtual/std ; HRESULT get_attributes(this, IDispatch **p)
	insertBefore            dm ?:virtual/std ; HRESULT insertBefore(this, IHTMLDOMNode *newChild, VARIANT refChild, IHTMLDOMNode **node)
	removeChild             dm ?:virtual/std ; HRESULT removeChild(this, IHTMLDOMNode *oldChild, IHTMLDOMNode **node)
	replaceChild            dm ?:virtual/std ; HRESULT replaceChild(this, IHTMLDOMNode *newChild, IHTMLDOMNode *oldChild, IHTMLDOMNode **node)
	cloneNode               dm ?:virtual/std ; HRESULT cloneNode(this, VARIANT_BOOL fDeep, IHTMLDOMNode **clonedNode)
	removeNode              dm ?:virtual/std ; HRESULT removeNode(this, VARIANT_BOOL fDeep, IHTMLDOMNode **removed)
	swapNode                dm ?:virtual/std ; HRESULT swapNode(this, IHTMLDOMNode *otherNode, IHTMLDOMNode **swappedNode)
	replaceNode             dm ?:virtual/std ; HRESULT replaceNode(this, IHTMLDOMNode *replacement, IHTMLDOMNode **replaced)
	appendChild             dm ?:virtual/std ; HRESULT appendChild(this, IHTMLDOMNode *newChild, IHTMLDOMNode **node)
	get_nodeName            dm ?:virtual/std ; HRESULT get_nodeName(this, BSTR *p)
	put_nodeValue           dm ?:virtual/std ; HRESULT put_nodeValue(this, VARIANT v)
    get_nodeValue           dm ?:virtual/std ; HRESULT get_nodeValue(this, VARIANT *p)
	get_firstChild          dm ?:virtual/std ; HRESULT get_firstChild(this, IHTMLDOMNode **p)
	get_lastChild           dm ?:virtual/std ; HRESULT get_lastChild(this, IHTMLDOMNode **p)
	get_previousSibling     dm ?:virtual/std ; HRESULT get_previousSibling(this, IHTMLDOMNode **p)
	get_nextSibling         dm ?:virtual/std ; HRESULT get_nextSibling(this, IHTMLDOMNode **p)
ends

@endm
