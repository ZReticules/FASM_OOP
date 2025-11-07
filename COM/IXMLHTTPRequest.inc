@module IXMLHTTPRequest

include "IDispatch.inc"

; ------------------------------------------------------------
; IXMLHTTPRequest (MSXML 6.0) — dual-интерфейс на базе IDispatch
; ProgID: "MSXML2.XMLHTTP.6.0"
; CLSID : {88D96A0A-F192-11D4-A65F-0040963251E5}
; IID   : {ED8C108D-4349-11D2-91A4-00C04F7969E8}
; ------------------------------------------------------------

struct IXMLHTTPRequest IDispatch
    ; HRESULT open(this, BSTR method, BSTR url, VARIANT async, VARIANT user, VARIANT password)
    open_               	dm ?:virtual/std

    ; HRESULT setRequestHeader(this, BSTR header, BSTR value)
    setRequestHeader    	dm ?:virtual/std

    ; HRESULT getResponseHeader(this, BSTR header, BSTR* value)
    getResponseHeader   	dm ?:virtual/std

    ; HRESULT getAllResponseHeaders(this, BSTR* headers)
    getAllResponseHeaders 	dm ?:virtual/std

    ; HRESULT send(this, VARIANT body) ; body: VT_EMPTY / VT_BSTR / VT_ARRAY|VT_UI1 / IStream и т.п.
    send_               	dm ?:virtual/std

    ; HRESULT abort(this)
    abort               	dm ?:virtual/std

    ; -------- свойства / геттеры --------

    ; HRESULT get_status(this, LONG* pStatus)
    get_status          	dm ?:virtual/std

    ; HRESULT get_statusText(this, BSTR* pStatusText)
    get_statusText      	dm ?:virtual/std

    ; HRESULT get_responseXML(this, IDispatch** ppDOM) ; обычно IXMLDOMDocument*
    get_responseXML     	dm ?:virtual/std

    ; HRESULT get_responseText(this, BSTR* pText)
    get_responseText    	dm ?:virtual/std

    ; HRESULT get_responseBody(this, VARIANT* pBody) ; SAFEARRAY(VT_UI1)
    get_responseBody    	dm ?:virtual/std

    ; HRESULT get_responseStream(this, VARIANT* pStream) ; IStream в VARIANT’е
    get_responseStream  	dm ?:virtual/std

    ; HRESULT get_readyState(this, LONG* pState)
    get_readyState      	dm ?:virtual/std

    ; -------- onreadystatechange (обработчик события) --------
    ; HRESULT put_onreadystatechange(this, IDispatch* pDisp)
    put_onreadystatechange 	dm ?:virtual/std

    open 					dm this:inline
    send 					dm this:inline
ends

match =x86, __architecture\{
	macro IXMLHTTPRequest.open this, method, url, asyncT=VT_BOOL, asyncV=VARIANT_FALSE, userT=VT_ERROR, userV=DISP_E_PARAMNOTFOUND, passwordT=VT_ERROR, passwordV=DISP_E_PARAMNOTFOUND\\{
		; \\local ..this, _this
		; inlineObj _this, this, pcx
		; virtObj ..this IXMLHTTPRequest at _this
		@call IXMLHTTPRequest::>open_(this, method, url, qword asyncT, qword asyncV, qword userT, qword userV, qword passwordT, qword passwordV)
	\\}
	macro IXMLHTTPRequest.send this, bodyT=VT_ERROR, bodyV=DISP_E_PARAMNOTFOUND\\{
		; \\local ..this, _this
		; inlineObj _this, this, pcx
		; virtObj ..this IXMLHTTPRequest at _this
		@call IXMLHTTPRequest::>send_(this, qword bodyT, qword bodyV)
	\\}
\}
match =x64, __architecture\{
	macro IXMLHTTPRequest.open this, method, url, asyncT=VT_BOOL, asyncV=VARIANT_FALSE, userT=VT_ERROR, userV=DISP_E_PARAMNOTFOUND, passwordT=VT_ERROR, passwordV=DISP_E_PARAMNOTFOUND\\{
		; \\local ..this, _this
		; inlineObj _this, this, pcx
		; virtObj ..this IXMLHTTPRequest at _this
		@call IXMLHTTPRequest::>open_(this, method, url, <va_list asyncT, asyncV>, <va_list userT, userV>, <va_list passwordT, passwordV>)
	\\}
	macro IXMLHTTPRequest.send this, bodyT=VT_ERROR, bodyV=DISP_E_PARAMNOTFOUND\\{
		; \\local ..this, _this
		; inlineObj _this, this, pcx
		; virtObj ..this IXMLHTTPRequest at _this
		@call IXMLHTTPRequest::>send_(this, <va_list bodyT, bodyV>)
	\\}
\}

; GUID’ы

IID_IXMLHTTPRequest  @const GUID 0xED8C108D, 0x4349, 0x11D2, <0x91, 0xA4, 0x00, 0xC0, 0x4F, 0x79, 0x69, 0xE8>   ; IXMLHTTPRequest
CLSID_XMLHTTP60      @const GUID 0x88D96A0A, 0xF192, 0x11D4, <0xA6, 0x5F, 0x00, 0x40, 0x96, 0x32, 0x51, 0xE5>   ; MSXML2.XMLHTTP.6.0


@endm
