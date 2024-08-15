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

postpone {
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