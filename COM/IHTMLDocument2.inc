@module IHTMLDocument2

include "IDispatch.inc"

IID_IHTMLDocument2 @const GUID 0x332C4425,0x26CB,0x11D0,<0xB4,0x83,0x00,0xC0,0x4F,0xD9,0x01,0x19>
CLSID_HTMLDocument @const GUID 0x25336920,0x03F9,0x11CF,<0x8F,0xD0,0x00,0xAA,0x00,0x68,0x6F,0x13>

struct IHTMLDocument IDispatch
    get_Script              dm ?:virtual/std ; HRESULT get_Script(this, IDispatch **ppDisp)
ends

struct IHTMLDocument2 IHTMLDocument
    get_all                 dm ?:virtual/std ; HRESULT get_all(this, IHTMLElementCollection **ppColl)
    get_body                dm ?:virtual/std ; HRESULT get_body(this, IHTMLElement **ppElem)
    get_activeElement       dm ?:virtual/std ; HRESULT get_activeElement(this, IHTMLElement **ppElem)
    get_images              dm ?:virtual/std ; HRESULT get_images(this, IHTMLElementCollection **ppColl)
    get_applets             dm ?:virtual/std ; HRESULT get_applets(this, IHTMLElementCollection **ppColl)
    get_links               dm ?:virtual/std ; HRESULT get_links(this, IHTMLElementCollection **ppColl)
    get_forms               dm ?:virtual/std ; HRESULT get_forms(this, IHTMLElementCollection **ppColl)
    get_anchors             dm ?:virtual/std ; HRESULT get_anchors(this, IHTMLElementCollection **ppColl)

    put_title               dm ?:virtual/std ; HRESULT put_title(this, BSTR v)
    get_title               dm ?:virtual/std ; HRESULT get_title(this, BSTR *p)

    get_scripts             dm ?:virtual/std ; HRESULT get_scripts(this, IHTMLElementCollection **ppColl)

    put_designMode          dm ?:virtual/std ; HRESULT put_designMode(this, BSTR v)
    get_designMode          dm ?:virtual/std ; HRESULT get_designMode(this, BSTR *p)

    get_selection           dm ?:virtual/std ; HRESULT get_selection(this, IHTMLSelectionObject **ppSel)

    get_readyState          dm ?:virtual/std ; HRESULT get_readyState(this, BSTR *p)
    get_frames              dm ?:virtual/std ; HRESULT get_frames(this, IHTMLFramesCollection2 **ppFrames)
    get_embeds              dm ?:virtual/std ; HRESULT get_embeds(this, IHTMLElementCollection **ppColl)
    get_plugins             dm ?:virtual/std ; HRESULT get_plugins(this, IHTMLElementCollection **ppColl)

    put_alinkColor          dm ?:virtual/std ; HRESULT put_alinkColor(this, VARIANT v)
    get_alinkColor          dm ?:virtual/std ; HRESULT get_alinkColor(this, VARIANT *p)
    put_bgColor             dm ?:virtual/std ; HRESULT put_bgColor(this, VARIANT v)
    get_bgColor             dm ?:virtual/std ; HRESULT get_bgColor(this, VARIANT *p)
    put_fgColor             dm ?:virtual/std ; HRESULT put_fgColor(this, VARIANT v)
    get_fgColor             dm ?:virtual/std ; HRESULT get_fgColor(this, VARIANT *p)
    put_linkColor           dm ?:virtual/std ; HRESULT put_linkColor(this, VARIANT v)
    get_linkColor           dm ?:virtual/std ; HRESULT get_linkColor(this, VARIANT *p)
    put_vlinkColor          dm ?:virtual/std ; HRESULT put_vlinkColor(this, VARIANT v)
    get_vlinkColor          dm ?:virtual/std ; HRESULT get_vlinkColor(this, VARIANT *p)

    get_referrer            dm ?:virtual/std ; HRESULT get_referrer(this, BSTR *p)
    get_location            dm ?:virtual/std ; HRESULT get_location(this, IHTMLLocation **ppLoc)
    get_lastModified        dm ?:virtual/std ; HRESULT get_lastModified(this, BSTR *p)

    put_url                 dm ?:virtual/std ; HRESULT put_url(this, BSTR v)
    get_url                 dm ?:virtual/std ; HRESULT get_url(this, BSTR *p)

    put_domain              dm ?:virtual/std ; HRESULT put_domain(this, BSTR v)
    get_domain              dm ?:virtual/std ; HRESULT get_domain(this, BSTR *p)

    put_cookie              dm ?:virtual/std ; HRESULT put_cookie(this, BSTR v)
    get_cookie              dm ?:virtual/std ; HRESULT get_cookie(this, BSTR *p)

    put_expands             dm ?:virtual/std ; HRESULT put_expands(this, VARIANT_BOOL v)
    get_expands             dm ?:virtual/std ; HRESULT get_expands(this, VARIANT_BOOL *p)

    put_charset             dm ?:virtual/std ; HRESULT put_charset(this, BSTR v)
    get_charset             dm ?:virtual/std ; HRESULT get_charset(this, BSTR *p)

    put_defaultCharset      dm ?:virtual/std ; HRESULT put_defaultCharset(this, BSTR v)
    get_defaultCharset      dm ?:virtual/std ; HRESULT get_defaultCharset(this, BSTR *p)

    get_mimeType            dm ?:virtual/std ; HRESULT get_mimeType(this, BSTR *p)
    get_fileSize            dm ?:virtual/std ; HRESULT get_fileSize(this, BSTR *p)
    get_fileCreatedDate     dm ?:virtual/std ; HRESULT get_fileCreatedDate(this, BSTR *p)
    get_fileModifiedDate    dm ?:virtual/std ; HRESULT get_fileModifiedDate(this, BSTR *p)
    get_fileUpdatedDate     dm ?:virtual/std ; HRESULT get_fileUpdatedDate(this, BSTR *p)
    get_security            dm ?:virtual/std ; HRESULT get_security(this, BSTR *p)
    get_protocol            dm ?:virtual/std ; HRESULT get_protocol(this, BSTR *p)
    get_nameProp            dm ?:virtual/std ; HRESULT get_nameProp(this, BSTR *p)

    write                   dm ?:virtual/std ; HRESULT write(this, SAFEARRAY *psarray)
    writeln                 dm ?:virtual/std ; HRESULT writeln(this, SAFEARRAY *psarray)

    open                    dm ?:virtual/std ; HRESULT open(this, BSTR url, VARIANT name, VARIANT features, VARIANT replace, IDispatch **ppDisp)

    close                   dm ?:virtual/std ; HRESULT close(this)
    clear                   dm ?:virtual/std ; HRESULT clear(this)

    queryCommandSupported   dm ?:virtual/std ; HRESULT queryCommandSupported(this, BSTR cmdID, VARIANT_BOOL *pf)
    queryCommandEnabled     dm ?:virtual/std ; HRESULT queryCommandEnabled(this, BSTR cmdID, VARIANT_BOOL *pf)
    queryCommandState       dm ?:virtual/std ; HRESULT queryCommandState(this, BSTR cmdID, VARIANT_BOOL *pf)
    queryCommandIndeterm    dm ?:virtual/std ; HRESULT queryCommandIndeterm(this, BSTR cmdID, VARIANT_BOOL *pf)
    queryCommandText        dm ?:virtual/std ; HRESULT queryCommandText(this, BSTR cmdID, BSTR *pbstr)
    queryCommandValue       dm ?:virtual/std ; HRESULT queryCommandValue(this, BSTR cmdID, VARIANT *pvar)

    execCommand             dm ?:virtual/std ; HRESULT execCommand(this, BSTR cmdID, VARIANT_BOOL showUI, VARIANT value, VARIANT_BOOL *pf)
    execCommandShowHelp     dm ?:virtual/std ; HRESULT execCommandShowHelp(this, BSTR cmdID, VARIANT_BOOL *pf)

    createElement           dm ?:virtual/std ; HRESULT createElement(this, BSTR tag, IHTMLElement **ppElem)

    put_onhelp              dm ?:virtual/std ; HRESULT put_onhelp(this, VARIANT v)
    get_onhelp              dm ?:virtual/std ; HRESULT get_onhelp(this, VARIANT *p)
    put_onclick             dm ?:virtual/std ; HRESULT put_onclick(this, VARIANT v)
    get_onclick             dm ?:virtual/std ; HRESULT get_onclick(this, VARIANT *p)
    put_ondblclick          dm ?:virtual/std ; HRESULT put_ondblclick(this, VARIANT v)
    get_ondblclick          dm ?:virtual/std ; HRESULT get_ondblclick(this, VARIANT *p)
    put_onkeyup             dm ?:virtual/std ; HRESULT put_onkeyup(this, VARIANT v)
    get_onkeyup             dm ?:virtual/std ; HRESULT get_onkeyup(this, VARIANT *p)
    put_onkeydown           dm ?:virtual/std ; HRESULT put_onkeydown(this, VARIANT v)
    get_onkeydown           dm ?:virtual/std ; HRESULT get_onkeydown(this, VARIANT *p)
    put_onkeypress          dm ?:virtual/std ; HRESULT put_onkeypress(this, VARIANT v)
    get_onkeypress          dm ?:virtual/std ; HRESULT get_onkeypress(this, VARIANT *p)

    put_onmouseup           dm ?:virtual/std ; HRESULT put_onmouseup(this, VARIANT v)
    get_onmouseup           dm ?:virtual/std ; HRESULT get_onmouseup(this, VARIANT *p)
    put_onmousedown         dm ?:virtual/std ; HRESULT put_onmousedown(this, VARIANT v)
    get_onmousedown         dm ?:virtual/std ; HRESULT get_onmousedown(this, VARIANT *p)
    put_onmousemove         dm ?:virtual/std ; HRESULT put_onmousemove(this, VARIANT v)
    get_onmousemove         dm ?:virtual/std ; HRESULT get_onmousemove(this, VARIANT *p)
    put_onmouseout          dm ?:virtual/std ; HRESULT put_onmouseout(this, VARIANT v)
    get_onmouseout          dm ?:virtual/std ; HRESULT get_onmouseout(this, VARIANT *p)
    put_onmouseover         dm ?:virtual/std ; HRESULT put_onmouseover(this, VARIANT v)
    get_onmouseover         dm ?:virtual/std ; HRESULT get_onmouseover(this, VARIANT *p)

    put_onreadystatechange  dm ?:virtual/std ; HRESULT put_onreadystatechange(this, VARIANT v)
    get_onreadystatechange  dm ?:virtual/std ; HRESULT get_onreadystatechange(this, VARIANT *p)

    put_onafterupdate       dm ?:virtual/std ; HRESULT put_onafterupdate(this, VARIANT v)
    get_onafterupdate       dm ?:virtual/std ; HRESULT get_onafterupdate(this, VARIANT *p)
    put_onrowexit           dm ?:virtual/std ; HRESULT put_onrowexit(this, VARIANT v)
    get_onrowexit           dm ?:virtual/std ; HRESULT get_onrowexit(this, VARIANT *p)
    put_onrowenter          dm ?:virtual/std ; HRESULT put_onrowenter(this, VARIANT v)
    get_onrowenter          dm ?:virtual/std ; HRESULT get_onrowenter(this, VARIANT *p)
    put_ondragstart         dm ?:virtual/std ; HRESULT put_ondragstart(this, VARIANT v)
    get_ondragstart         dm ?:virtual/std ; HRESULT get_ondragstart(this, VARIANT *p)
    put_onselectstart       dm ?:virtual/std ; HRESULT put_onselectstart(this, VARIANT v)
    get_onselectstart       dm ?:virtual/std ; HRESULT get_onselectstart(this, VARIANT *p)

    elementFromPoint        dm ?:virtual/std ; HRESULT elementFromPoint(this, LONG x, LONG y, IHTMLElement **ppElem)

    get_parentWindow        dm ?:virtual/std ; HRESULT get_parentWindow(this, IHTMLWindow2 **ppWnd)
    get_styleSheets         dm ?:virtual/std ; HRESULT get_styleSheets(this, IHTMLStyleSheetsCollection **ppSheets)

    put_onbeforeupdate      dm ?:virtual/std ; HRESULT put_onbeforeupdate(this, VARIANT v)
    get_onbeforeupdate      dm ?:virtual/std ; HRESULT get_onbeforeupdate(this, VARIANT *p)
    put_onerrorupdate       dm ?:virtual/std ; HRESULT put_onerrorupdate(this, VARIANT v)
    get_onerrorupdate       dm ?:virtual/std ; HRESULT get_onerrorupdate(this, VARIANT *p)

    toString                dm ?:virtual/std ; HRESULT toString(this, BSTR *p)
    createStyleSheet        dm ?:virtual/std ; HRESULT createStyleSheet(this, BSTR href, LONG index, IHTMLStyleSheet **ppSheet)

ends

@endm
