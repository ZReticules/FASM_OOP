struct _IMAGE_TLS_DIRECTORY32
	StartAddressOfRawData 	dd ?
	EndAddressOfRawData 	dd ?
	AddressOfIndex 			dd ?
	AddressOfCallBacks 		dd ?
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
			_IMAGE_TLS_DIRECTORY32 0, 0, tls_index, tls_callbacks, 0, 0
			tls_index   	dd 0
			tls_callbacks:	
				irpv thiscallback, TLS_Callbacks\{
					if defined thiscallback
						dd thiscallback
					end if
				\}
				dd 0
		end data
	end if
}