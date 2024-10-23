struct _IMAGE_TLS_DIRECTORY64
	StartAddressOfRawData 	dq ?
	EndAddressOfRawData 	dq ?
	AddressOfIndex 			dq ?
	AddressOfCallBacks 		dq ?
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

macro TLS_AddMacro [args]{
	; common
	TLS_Marcoses equ args
}

postpone {
	local ._start, ._end, ._size, TLS._init
	virtual at $
		._start:
		irpv mac, TLS_Marcoses\{
			match macr, mac\\{
				macr
			\\}
		\}
		._end:
		._size = ._start - ._end
	end virtual
	if ._size
		proc_noprologue
		proc TLS._init
			irpv mac, TLS_Marcoses\{
				match macr, mac\\{
					macr
				\\}
			\}
			ret
		endp
		proc_resprologue
		TLS_AddCallback TLS._init
	end if
	if Has_Tls
		; section ".TLS" readable writeable
		data 9
			_IMAGE_TLS_DIRECTORY64 0, 0, tls_index, tls_callbacks, 0, 0
			tls_index   	dq 0
			tls_callbacks:	
				irpv thiscallback, TLS_Callbacks\{
					if defined thiscallback
						dq thiscallback
					end if
				\}
				dq 0
		end data
	end if
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