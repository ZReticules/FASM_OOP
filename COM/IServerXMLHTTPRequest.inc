@module IServerXMLHTTPRequest

include "IXMLHTTPRequest.inc"

; Базируется на IXMLHTTPRequest (IDispatch). Добавляет 4 метода.
; Версии: впервые в MSXML 3.0; есть в 6.0. 
; Реализация: msxml3.dll / msxml6.dll
; IID:
IID_IServerXMLHTTPRequest	@const GUID 0x2E9196BF,0x13BA,0x4DD4,<0x91,0xCA,0x6C,0x57,0x1F,0x28,0x14,0x95>

; CoClasses (для контекста):
; CLSID_ServerXMLHTTP      {AFBA6B42-5692-48EA-8141-DC517DCF0EF1}  (общий)
; CLSID_ServerXMLHTTP30    {AFB40FFD-B609-40A3-9828-F88BBE11E4E3}
; CLSID_ServerXMLHTTP40    {88D969C6-F192-11D4-A65F-0040963251E5}  (default IF: IServerXMLHTTPRequest2)
; CLSID_ServerXMLHTTP60    {88D96A0B-F192-11D4-A65F-0040963251E5}  (default IF: IServerXMLHTTPRequest2)
; ProgID для 6.0: "Msxml2.ServerXMLHTTP.6.0"
CLSID_ServerXMLHTTP60		@const GUID 0x88D96A0B,0xF192,0x11D4,<0xA6,0x5F,0x00,0x40,0x96,0x32,0x51,0xE5>

struct IServerXMLHTTPRequest IXMLHTTPRequest
    ; --- Расширения ServerXMLHTTP (добавляются к IXMLHTTPRequest) ---
    setTimeouts         dm ?:virtual/std ; HRESULT setTimeouts(this, LONG resolve, LONG connect, LONG send, LONG receive)
    waitForResponse_    dm ?:virtual/std ; HRESULT waitForResponse(this, VARIANT timeoutInSeconds, VARIANT_BOOL* isSuccessful)
    getOption           dm ?:virtual/std ; HRESULT getOption(this, SERVERXMLHTTP_OPTION option, VARIANT* value)
    setOption_          dm ?:virtual/std ; HRESULT setOption(this, SERVERXMLHTTP_OPTION option, VARIANT value)

    waitForResponse 	dm this:inline
    setOption 			dm this:inline
ends

match =x86, __architecture\{
	macro IServerXMLHTTPRequest.waitForResponse this, timeoutT, timeoutV, isSuccessful\\{
		; \\local ..this, _this
		; inlineObj _this, this, pcx
		; virtObj ..this IServerXMLHTTPRequest at _this
		$call waitForResponse_<IServerXMLHTTPRequest>(this, qword timeoutT, qword timeoutV, isSuccessful)
	\\}
	macro IServerXMLHTTPRequest.setOption this, option, valueT, valueV\\{
		; \\local ..this, _this
		; inlineObj _this, this, pcx
		; virtObj ..this IServerXMLHTTPRequest at _this
		$call setOption_<IServerXMLHTTPRequest>(this, option, qword valueT, qword valueV)
	\\}
\}
match =x64, __architecture\{
	macro IServerXMLHTTPRequest.waitForResponse this, timeoutT, timeoutV, isSuccessful\\{
		; \\local ..this, _this
		; inlineObj _this, this, pcx
		; virtObj ..this IServerXMLHTTPRequest at _this
		$call waitForResponse_<IServerXMLHTTPRequest>(this, <va_list timeoutT, timeoutV>, isSuccessful)
	\\}
	macro IServerXMLHTTPRequest.setOption this, option, valueT, valueV\\{
		; \\local ..this, _this
		; inlineObj _this, this, pcx
		; virtObj ..this IServerXMLHTTPRequest at _this
		$call setOption_<IServerXMLHTTPRequest>(this, option, <va_list valueT, valueV>)
	\\}
\}

; Перечисление опций (фрагмент):
SERVERXMLHTTP_OPTION enum \
    SXH_OPTION_URL=-1,\
    SXH_OPTION_URL_CODEPAGE,\
    SXH_OPTION_ESCAPE_PERCENT_IN_URL,\
    SXH_OPTION_IGNORE_SERVER_SSL_CERT_ERROR_FLAGS,\
    SXH_OPTION_SELECT_CLIENT_SSL_CERT

@endm
