struct _IMAGE_TLS_DIRECTORY
	StartAddressOfRawData 	dptr ?
	EndAddressOfRawData 	dptr ?
	AddressOfIndex 			dptr ?
	AddressOfCallBacks 		dptr ?
	SizeOfZeroFill			dd ?
	Characteristics			dd ?
ends

Has_Tls = 0

macro TLS_AddCallback [args]{
	TLS_Callbacks equ args
	Has_Tls = args
	common
	Has_Tls = 1
}

; example

; if used CNV._heap
; 	proc_noprologue

; 	if used CNV._heap
; 		CNV._heap dq ?
; 	end if

; 	proc CNV._init
; 		if used CNV._heap
; 			@call [GetProcessHeap]()
; 			mov [CNV._heap], rax
; 		end if
; 		ret
; 	endp

; 	TLS_AddCallback CNV._init

; 	proc_resprologue
; end if

macro TLS_AddMacro [args]{
	; common
	TLS_Marcoses equ args
}

; example

; if used CNV._heap
; 	CNV._heap dptr ?
; end if

; macro CNV._initHeap{
; 	if used CNV._heap
; 		@call [GetProcessHeap]()
; 		mov [CNV._heap], pax
; 	end if
; }

; TLS_AddMacro CNV._initHeap

macro @init_tls{
	match =@init_tls, @init_tls\{
		local ._start, ._end, ._size, TLS._init
		virtual at $
			._start:
			irpv mac, TLS_Marcoses\\{
				match macr, mac\\\{
					macr
				\\\}
			\\}
			._end:
			._size = ._start - ._end
		end virtual
		if ._size
			proc_noprologue
			proc TLS._init, handle, reason, reserved
				irpv mac, TLS_Marcoses\\{
					match macr, mac\\\{
						macr
					\\\}
					; match name, \\\`mac\\\{display name\\\}
				\\}
				ret
			endp
			proc_resprologue
			TLS_AddCallback TLS._init
		end if
		if Has_Tls
		; display "lol"
			; section ".tls" readable writeable
			data 9
				_TLS_DIR _IMAGE_TLS_DIRECTORY 0, 0, tls_index, tls_callbacks, 0, 0
				tls_index   	dptr 0
				tls_callbacks:	
					irpv thiscallback, TLS_Callbacks\\{
						if defined thiscallback
							dptr thiscallback
						end if
					\\}
					dptr 0
			end data
		end if
		define @init_tls
	\}
}

postpone {
	@init_tls
}