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

if false_TLS._initSize | false_TLS._exit <> false_TLS._end
	TLS_AddCallback TLS.startInit
end if

macro @init_tls{
	match =@init_tls, @init_tls\{
		local _used
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
			false_TLS._initSize = false_TLS._start - false_TLS._exit
		end virtual
		
		proc_noprologue
		if false_TLS._exit <> false_TLS._end
			align 8
			ThreadCount dptr 0
		end if
		if false_TLS._initSize | false_TLS._exit <> false_TLS._end
			proc TLS.startInit, handle, reason, reserved

				@sarg @arg1, @arg2
				if false_TLS._exit <> false_TLS._end
					switch @arg2
						case TLS_THREAD_DETACH
							lock dec [ThreadCount]
							jmp end_case
						case TLS_THREAD_ATTACH
							lock inc [ThreadCount]
					end_switch
				end if
				cmp @arg2, TLS_PROCESS_ATTACH
				jne TLS.startInit.return
					if false_TLS._exit <> false_TLS._end
						lock inc [ThreadCount]
					end if
					irpv mac, TLS_Marcoses\\{
						match macr, mac\\\{
							macr
						\\\}
						; match name, \\\`mac\\\{display name\\\}
					\\}
				TLS.startInit.return: ret
			endp
		end if
		if false_TLS._exit <> false_TLS._end
			proc TLS.exitInit, handle, reason, reserved
				@sarg @arg1, @arg2
				
				cmp @arg2, TLS_PROCESS_DETACH
					je TLS.exitInit.do_exit
				cmp @arg2, TLS_THREAD_DETACH
					jne TLS.exitInit.return
				cmp [ThreadCount], 0
					je TLS.exitInit.do_exit
				ret
				TLS.exitInit.do_exit:

					irpv mac, TLS_Marcoses_end\\{
						\\reverse
						match macr, mac\\\{
							macr
						\\\}
						; match name, \\\`mac\\\{display name\\\}
					\\}
				TLS.exitInit.return: ret
			endp
			TLS_AddCallback TLS.exitInit
		end if
		
		proc_resprologue
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