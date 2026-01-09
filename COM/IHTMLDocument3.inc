@module IHTMLDocument3

include "IHTMLDocument.inc"

IID_IHTMLDocument3 @const GUID \
    0x3050F485,0x98B5,0x11CF,<0xBB,0x82,0x00,0xAA,0x00,0xBD,0xCE,0x0B>

struct IHTMLDocument3 IDispatch

    releaseCapture            dm ?:virtual/std ; HRESULT releaseCapture(this)

    recalc                    dm ?:virtual/std ; HRESULT recalc(this, VARIANT_BOOL fForce)

    createTextNode            dm ?:virtual/std ; HRESULT createTextNode(this, BSTR text, IHTMLDOMNode **newTextNode)

    get_documentElement       dm ?:virtual/std ; HRESULT get_documentElement(this, IHTMLElement **p)

    get_uniqueID              dm ?:virtual/std ; HRESULT get_uniqueID(this, BSTR *p)

    attachEvent               dm ?:virtual/std ; HRESULT attachEvent(this, BSTR event, IDispatch *pDisp, VARIANT_BOOL *pfResult)

    detachEvent               dm ?:virtual/std ; HRESULT detachEvent(this, BSTR event, IDispatch *pDisp)

    put_onrowsdelete          dm ?:virtual/std ; HRESULT put_onrowsdelete(this, VARIANT v)
    get_onrowsdelete          dm ?:virtual/std ; HRESULT get_onrowsdelete(this, VARIANT *p)

    put_onrowsinserted        dm ?:virtual/std ; HRESULT put_onrowsinserted(this, VARIANT v)
    get_onrowsinserted        dm ?:virtual/std ; HRESULT get_onrowsinserted(this, VARIANT *p)

    put_oncellchange          dm ?:virtual/std ; HRESULT put_oncellchange(this, VARIANT v)
    get_oncellchange          dm ?:virtual/std ; HRESULT get_oncellchange(this, VARIANT *p)

    put_ondatasetchanged      dm ?:virtual/std ; HRESULT put_ondatasetchanged(this, VARIANT v)
    get_ondatasetchanged      dm ?:virtual/std ; HRESULT get_ondatasetchanged(this, VARIANT *p)

    put_ondataavailable       dm ?:virtual/std ; HRESULT put_ondataavailable(this, VARIANT v)
    get_ondataavailable       dm ?:virtual/std ; HRESULT get_ondataavailable(this, VARIANT *p)

    put_ondatasetcomplete     dm ?:virtual/std ; HRESULT put_ondatasetcomplete(this, VARIANT v)
    get_ondatasetcomplete     dm ?:virtual/std ; HRESULT get_ondatasetcomplete(this, VARIANT *p)

    put_onpropertychange      dm ?:virtual/std ; HRESULT put_onpropertychange(this, VARIANT v)
    get_onpropertychange      dm ?:virtual/std ; HRESULT get_onpropertychange(this, VARIANT *p)

    put_dir                   dm ?:virtual/std ; HRESULT put_dir(this, BSTR v)
    get_dir                   dm ?:virtual/std ; HRESULT get_dir(this, BSTR *p)

    put_oncontextmenu         dm ?:virtual/std ; HRESULT put_oncontextmenu(this, VARIANT v)
    get_oncontextmenu         dm ?:virtual/std ; HRESULT get_oncontextmenu(this, VARIANT *p)

    put_onstop                dm ?:virtual/std ; HRESULT put_onstop(this, VARIANT v)
    get_onstop                dm ?:virtual/std ; HRESULT get_onstop(this, VARIANT *p)

    createDocumentFragment    dm ?:virtual/std ; HRESULT createDocumentFragment(this, IHTMLDocument2 **pNewDoc)

    get_parentDocument        dm ?:virtual/std ; HRESULT get_parentDocument(this, IHTMLDocument2 **p)

    put_enableDownload        dm ?:virtual/std ; HRESULT put_enableDownload(this, VARIANT_BOOL v)
    get_enableDownload        dm ?:virtual/std ; HRESULT get_enableDownload(this, VARIANT_BOOL *p)

    put_baseUrl               dm ?:virtual/std ; HRESULT put_baseUrl(this, BSTR v)
    get_baseUrl               dm ?:virtual/std ; HRESULT get_baseUrl(this, BSTR *p)

    get_childNodes            dm ?:virtual/std ; HRESULT get_childNodes(this, IDispatch **p)

    put_inheritStyleSheets    dm ?:virtual/std ; HRESULT put_inheritStyleSheets(this, VARIANT_BOOL v)
    get_inheritStyleSheets    dm ?:virtual/std ; HRESULT get_inheritStyleSheets(this, VARIANT_BOOL *p)

    put_onbeforeeditfocus     dm ?:virtual/std ; HRESULT put_onbeforeeditfocus(this, VARIANT v)
    get_onbeforeeditfocus     dm ?:virtual/std ; HRESULT get_onbeforeeditfocus(this, VARIANT *p)

    getElementsByName         dm ?:virtual/std ; HRESULT getElementsByName(this, BSTR v, IHTMLElementCollection **pelColl)

    getElementById            dm ?:virtual/std ; HRESULT getElementById(this, BSTR v, IHTMLElement **pel)

    getElementsByTagName      dm ?:virtual/std ; HRESULT getElementsByTagName(this, BSTR v, IHTMLElementCollection **pelColl)

ends


@endm
