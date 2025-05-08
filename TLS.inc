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

importlib kernel32,\
	GetCurrentThreadId,\
	TlsAlloc,\
	TlsFree,\
	TlsGetValue,\
	TlsSetValue

TLS_THREAD_ATTACH equ 1
TLS_THREAD_DETACH equ 3
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

macro @init_tls{
	match =@init_tls, @init_tls\{
		local ._start, ._end, ._size, TLS._init, _used
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
			proc TLS._init, handle, reason, reserved
				@sarg @arg1, @arg2
				cmp @arg2, TLS_THREAD_ATTACH
				jne TLS._init.no_start_tls
					cmp [TLS._init.idProcess], 0
						jne TLS._init.return
					@call [GetCurrentThreadId]()
					mov [TLS._init.idProcess], pax
					irpv mac, TLS_Marcoses\\{
						match macr, mac\\\{
							macr
						\\\}
						; match name, \\\`mac\\\{display name\\\}
					\\}

				if false_TLS._exit <> false_TLS._end
					jmp TLS._init.return
				end if
				TLS._init.no_start_tls:
				if false_TLS._exit <> false_TLS._end
					cmp @arg2, TLS_THREAD_DETACH
					jne TLS._init.return
						@call [GetCurrentThreadId]()
						cmp [TLS._init.idProcess], pax
							jne TLS._init.return
						irpv mac, TLS_Marcoses_end\\{
							\\reverse
							match macr, mac\\\{
								macr
							\\\}
							; match name, \\\`mac\\\{display name\\\}
						\\}
				end if
				TLS._init.return: ret
				TLS._init.idProcess dptr 0
			endp
			proc_resprologue
			TLS_AddCallback TLS._init
		end if
		if Has_Tls
		; display "lol"
			; section ".tls" readable writeable
			align 16
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

total_postpone equ @init_tls

; postpone {
; 	@init_tls
; }