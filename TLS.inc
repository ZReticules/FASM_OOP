struct _IMAGE_TLS_DIRECTORY
	StartAddressOfRawData 	dptr ?
	EndAddressOfRawData 	dptr ?
	AddressOfIndex 			dptr ?
	AddressOfCallBacks 		dptr ?
	SizeOfZeroFill			dd ?
	Characteristics			dd ?
ends

Has_Tls = 0

virtual at 0
	TLS_Callbacks::
end virtual

macro TLS_AddCallback [args]{
	common
	virtual TLS_Callbacks
		dptr args
	end virtual
	Has_Tls = 1
}

importlib kernel32,\
	GetCurrentThreadId,\
	TlsAlloc,\
	TlsFree,\
	TlsGetValue,\
	TlsSetValue

TLS_PROCESS_DETACH 	= DLL_PROCESS_DETACH
TLS_PROCESS_ATTACH 	= DLL_PROCESS_ATTACH
TLS_THREAD_ATTACH 	= DLL_THREAD_ATTACH
TLS_THREAD_DETACH 	= DLL_THREAD_DETACH
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

macro @at_start [args]{
	TLS_Marcoses equ args
}

macro @at_end [args]{
	TLS_Marcoses_end equ args
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

if false_TLS._size
	TLS_AddCallback TLS.startAndEndInit
end if

macro @init_tls{
	match =@init_tls, @init_tls\{
		local ._start, ._end, ._size, _used
		virtual at $
			proc false_TLS.init, handle, reason, reserved
			false_TLS._start:
			irpv mac, TLS_Marcoses\\{
				match macr, mac\\\{
					macr
				\\\}
			\\}
			false_TLS._exit:
			irpv mac, TLS_Marcoses_end\\{
				\\reverse
				match macr, mac\\\{
					macr
				\\\}
				; match name, \\\`mac\\\{display name\\\}
			\\}
			false_TLS._end:
			endp
			_used = false_TLS.init
			false_TLS._size = false_TLS._start - false_TLS._end
		end virtual
		if false_TLS._size
			proc_noprologue
			proc TLS.startAndEndInit, handle, reason, reserved
				@sarg @arg1, @arg2
				cmp @arg2, TLS_PROCESS_ATTACH
				jne TLS.startAndEndInit.no_start_tls
					irpv mac, TLS_Marcoses\\{
						match macr, mac\\\{
							macr
						\\\}
						; match name, \\\`mac\\\{display name\\\}
					\\}

				if false_TLS._exit <> false_TLS._end
					jmp TLS.startAndEndInit.return
				end if
				TLS.startAndEndInit.no_start_tls:
				if false_TLS._exit <> false_TLS._end
					cmp @arg2, TLS_PROCESS_DETACH
					jne TLS.startAndEndInit.return
						irpv mac, TLS_Marcoses_end\\{
							\\reverse
							match macr, mac\\\{
								macr
							\\\}
							; match name, \\\`mac\\\{display name\\\}
						\\}
				end if
				TLS.startAndEndInit.return: ret
			endp
			proc_resprologue
		end if
		if Has_Tls
		; display "lol"
			; section ".tls" readable writeable
			align 16
			data 9
				_TLS_DIR _IMAGE_TLS_DIRECTORY 0, 0, tls_index, tls_callbacks, 0, 0
				tls_index   	dptr 0
				tls_callbacks:	
					local byte_count, _callback
					virtual TLS_Callbacks
						byte_count = $
					end virtual
					repeat byte_count / pointer.size
						load _callback pointer from TLS_Callbacks: (% - 1) * pointer.size
						dptr _callback
					end repeat
					dptr 0
			end data
		end if
		define @init_tls
	\}
}

total_postpone equ @init_tls

; postpone {
; 	@init_tls
; }