importlib setupapi,\
	SetupDiGetClassDevsA,\
	SetupDiEnumDeviceInfo,\
	SetupDiDestroyDeviceInfoList,\
	SetupDiOpenDevRegKey,\
	SetupDiGetDeviceRegistryPropertyA
	; SetupDiEnumDeviceInterfaces,\
	; SetupDiGetDeviceInterfaceDetailA

importlib advapi32,\
	RegQueryValueExA,\
	RegCloseKey

importlib msvcrt,\
	setlocale

importlib kernel32,\
	CloseHandle

importlib user32,\
	RegisterDeviceNotificationA,\
	UnregisterDeviceNotification

proc_noprologue

@const_align equ 16
COMInfo.DevinterfaceComportGuid @const GUID 86E0D1E0h, 8089h, 11D0h, <9Ch, 0E4h, 8h, 0h, 3Eh, 30h, 1Fh, 73h>
restore @const_align

proc COMInfo.make c uses pbx psi, this
	virtObj .this:arg COMInfo at pbx from @arg1
	@call [SetupDiGetClassDevsA](COMInfo.DevinterfaceComportGuid,\
		NULL, NULL, DIGCF_PRESENT or DIGCF_DEVICEINTERFACE)
	cmp pax, INVALID_HANDLE_VALUE
	jne .noGetClassErr
		mov pax, 0
		jmp .return
	.noGetClassErr:
	mov [.this.hDevInfoSet], pax
	mov psi, -1
	.next:
		inc esi
		@call [SetupDiEnumDeviceInfo]([.this.hDevInfoSet], psi, addr .this.devInfo)
	test pax, pax
	jnz .next
	mov pax, 1
	mov [.this.countPorts], si
	.return: ret
endp

proc COMInfo.getPortNameLen c uses pbx psi, this
	virtObj .this:arg COMInfo at pbx from @arg1
	hDeviceKey_r equ psi
	locals
		dwDataSize 	dd ?
		dwType 		dd ?
	endl
	@call [SetupDiOpenDevRegKey]([.this.hDevInfoSet], addr .this.devInfo,\
            DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE)
	test eax, eax
	mov hDeviceKey_r, pax
	mov eax, 0
		jz .return
	@call [RegQueryValueExA](hDeviceKey_r, "PortName", NULL,\
		addr dwType, NULL, addr dwDataSize)
	@call [RegCloseKey](hDeviceKey_r)
	mov eax, [dwDataSize]
	dec eax
	.return: ret

	restore hDeviceKey_r
endp

proc COMInfo.getPortNameChars c uses pbx psi pdi, this, strLp, size
	virtObj .this:arg COMInfo at pbx from @arg1
	_strLp 			equ psi
	hDeviceKey_r 	equ pdi
	@larg _strLp, @arg2
	@larg pax, @arg3
	locals
		dwDataSize 	dd ?
		dwType 		dd ?
	endl

	mov [dwDataSize], eax
	@call [SetupDiOpenDevRegKey]([.this.hDevInfoSet], addr .this.devInfo,\
            DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE)
	test eax, eax
	mov hDeviceKey_r, pax
	mov eax, 0
		jz .return
	@call [RegQueryValueExA](hDeviceKey_r, "PortName", NULL,\
			addr dwType, _strLp, addr dwDataSize)
	@call [RegCloseKey](hDeviceKey_r)
	mov eax, [dwDataSize]
	dec eax
	.return: ret

	restore _strLp, hDeviceKey_r
endp

proc COMInfo.getPortNameString c uses pbx psi pdi, _this, lpString
	@virtObj this:arg COMInfo at pbx from @arg1
	@virtObj strDest:arg String at pdi from @arg2
	
	hDeviceKey_r equ psi
	locals
		dwDataSize dd ?
		dwType dd ?
	endl

	@call [SetupDiOpenDevRegKey]([this.hDevInfoSet], addr this.devInfo,\
            DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE)
	test eax, eax
	mov hDeviceKey_r, pax
	mov eax, 0
		jz .return
	@call [RegQueryValueExA](hDeviceKey_r, "PortName", NULL,\
			addr dwType, NULL, addr dwDataSize)
	@call strDest->realloc([dwDataSize])->getLpChars()
	@call [RegQueryValueExA](hDeviceKey_r, "PortName", NULL,\
			addr dwType, pax, addr dwDataSize)

	@call [RegCloseKey](hDeviceKey_r)
	mov eax, [dwDataSize]
	dec eax
	mov [string.len], eax
	.return: ret

	restore hDeviceKey_r
endp

proc COMInfo.getPortInfoLen c, this, typeInfo
	virtObj .this:arg COMInfo at pcx from @arg1
	locals
		dwDataSize 	dd ?
		dwType 		dd ?
	endl
	@call [SetupDiGetDeviceRegistryPropertyA]([.this.hDevInfoSet], addr .this.devInfo,\
	 	@arg2, addr dwType, NULL, 0, addr dwDataSize)
	mov eax, [dwDataSize]
	dec eax
	.return: ret
endp

proc COMInfo.getPortInfoChars c uses psi pdi, this, strLp, maxLen, typeInfo
	virtObj .this:arg COMInfo at pcx from @arg1
	@sarg @arg4
	_strLp 		equ psi
	_maxLen 	equ pdi
	@larg _strLp, @arg2, \
		_maxLen, @arg3
	locals
		dwDataSize 	dd ?
		dwType 		dd ?
	endl
	@call [SetupDiGetDeviceRegistryPropertyA]([.this.hDevInfoSet], addr .this.devInfo,\
	 	[typeInfo], addr dwType, _strLp, _maxLen, addr dwDataSize)
	mov eax, [dwDataSize]
	dec eax
	.return: ret

	restore _strLp, _maxLen
endp

proc COMInfo.getPortInfoString c uses psi pdi, _this, lpString, typeInfo
	@virtObj this:arg COMInfo at psi from @arg1
	@virtObj string:arg String at pdi from @arg2
	@sarg @arg3

	locals
		dwDataSize 	dd ?
		dwType 		dd ?
	endl
	@call [SetupDiGetDeviceRegistryPropertyA]([this.hDevInfoSet], addr this.devInfo,\
	 		@arg3, addr dwType, NULL, 0, addr dwDataSize)
	@call string->realloc([dwDataSize])->getLpChars()
	@call [SetupDiGetDeviceRegistryPropertyA]([this.hDevInfoSet], addr this.devInfo,\
	 		[typeInfo], addr dwType, pax, [dwDataSize], addr dwDataSize)
	mov eax, [dwDataSize]
	dec eax
	mov [string.len], eax
	.return: ret
endp

proc COMInfo.registerNotify c, handle, type
	@sarg @arg1, @arg2
	local notifyFilter:DEV_BROADCAST_DEVICEINTERFACE_A
	mov [notifyFilter.dbcc_size], sizeof.DEV_BROADCAST_DEVICEINTERFACE_A
	mov [notifyFilter.dbcc_devicetype], DBT_DEVTYP_DEVICEINTERFACE
	@call CNV::fill(addr notifyFilter.dbcc_classguid, addr COMInfo.DevinterfaceComportGuid, sizeof.GUID)
	@call [RegisterDeviceNotificationA]([handle], addr notifyFilter, [type])
	ret
endp

macro COMInfo.unregisterNotify hNotify{
	@call [UnregisterDeviceNotification](hNotify)
}

macro COMInfo.getPortInfo this, [args]{
	common
	match =4, __argscount__\{
		@call c COMInfo.getPortInfoChars(this, args)
	rept 0\{\} rept 1\{
		@call c COMInfo.getPortInfoString(this, args)
	\}
}

macro COMInfo.getPortName this, [args]{
	common
	match =3, __argscount__\{
		@call c COMInfo.getPortNameChars(this, args)
	rept 0\{\} rept 1\{
		@call c COMInfo.getPortNameString(this, args)
	\}
}

macro COMInfo.choseId this, idPort{
	local _this
	inlineObj _this, this, pcx
	@call [SetupDiEnumDeviceInfo]([_this + COMInfo.hDevInfoSet], idPort, addr _this + COMInfo.devInfo)
}

macro COMInfo.unmake this{
	local _this
	inlineObj _this, this, pcx
	@call [SetupDiDestroyDeviceInfoList]([_this + COMInfo.hDevInfoSet])
}

proc_resprologue

; proc GetCommInfo uses r12 r13 r14 r15 rsi
; 	hDevInfoSet_r 	equ r12
; 	hDeviceKey_r 	equ r14
; 	COMNameStr		equ r15
; 	COMTypeStr 		equ rsi
; 	locals 
; 		stackFrame 	dq ?
; 		stackFrame2 dq ?
; 		devInfo 	SP_DEVINFO_DATA
; 		dwType 		dq 0
; 		dwDataSize 	dq 0
; 		dwRetSize	dq 0
; 	endl
; 	@call [setlocale](0, ".1251")
; 	@call [SetupDiGetClassDevsA](addr GUID_DEVINTERFACE_COMPORT,\
; 		NULL, NULL, DIGCF_PRESENT or DIGCF_DEVICEINTERFACE)
; 	cmp rax, INVALID_HANDLE_VALUE
; 	jne .noGetClassErr
; 		mov rax, 0
; 		jmp .return
; 	.noGetClassErr:
; 	mov hDevInfoSet_r, rax
; 	mov r13, -1
; 	.getInfoLoop:
; 		mov [stackFrame], rsp
; 		inc r13
; 		@call [SetupDiEnumDeviceInfo](hDevInfoSet_r, r13, addr devInfo)
; 		test rax, rax
; 			jz .break
; 		@call [SetupDiOpenDevRegKey](hDevInfoSet_r, addr devInfo,\
;             DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE)
; 		cmp rax, INVALID_HANDLE_VALUE
; 		je .invalidHandle
; 			mov hDeviceKey_r, rax
; 			@call [SetupDiGetDeviceRegistryPropertyA](hDevInfoSet_r, addr devInfo,\
; 			 	SPDRP_DEVICEDESC, addr dwType, NULL, 0, addr dwDataSize)
; 			cmp [dwType], REG_SZ
; 				jne .continue
; 			; inc [dwDataSize]
; 			; push [dwDataSize]
; 			stackAlloc COMTypeStr, [dwDataSize]
; 			mov byte[COMTypeStr], 0
; 			mov [stackFrame2], rsp
; 			and rsp, -16
; 			; dec [dwDataSize]
; 			@call [SetupDiGetDeviceRegistryPropertyA](hDevInfoSet_r, addr devInfo,\
; 			 	SPDRP_DEVICEDESC, addr dwType, COMTypeStr, [dwDataSize], addr dwDataSize)
; 			@call [RegQueryValueExA](hDeviceKey_r, "PortName", NULL,\
; 				addr dwType, NULL, addr dwDataSize)
; 			test rax, rax
; 				jnz .continue
; 			cmp [dwType], REG_SZ
; 				jne .continue
; 			; inc [dwDataSize]
; 			mov rsp, [stackFrame2]
; 			stackAlloc COMNameStr, [dwDataSize]
; 			mov byte[COMNameStr], 0
; 			and rsp, -16
; 			mov rax, [dwDataSize]
; 			; dec rax
; 			mov [dwRetSize], rax
; 			@call [RegQueryValueExA](hDeviceKey_r, "PortName", NULL,\
; 				addr dwType, COMNameStr, addr dwRetSize)
; 			mov rax, [dwDataSize]
; 			cmp [dwRetSize], rax
; 				ja .continue
; 			mov byte[COMTypeStr-1], '|'
; 			@call [puts](COMNameStr)
; 		.invalidHandle:
; 		.continue:
; 		mov rsp, [stackFrame]
; 	jmp .getInfoLoop
; 	.break:
; 	.return:ret
; endp