importlib setupapi,\
	SetupDiGetClassDevsA,\
	SetupDiEnumDeviceInfo,\
	SetupDiOpenDevRegKey,\
	SetupDiGetDeviceRegistryPropertyA
	; SetupDiEnumDeviceInterfaces,\
	; SetupDiGetDeviceInterfaceDetailA

importlib Advapi32,\
	RegQueryValueExA 

importlib msvcrt,\
	setlocale

GUID_DEVINTERFACE_COMPORT GUID 86E0D1E0h, 8089h, 11D0h, <9Ch, 0E4h, 8h, 0h, 3Eh, 30h, 1Fh, 73h>

proc COMInfo.init uses rbx r12, this
	virtObj .this:arg COMInfo at rbx
	mov rbx, rcx
	@call [SetupDiGetClassDevsA](addr GUID_DEVINTERFACE_COMPORT,\
		NULL, NULL, DIGCF_PRESENT or DIGCF_DEVICEINTERFACE)
	cmp rax, INVALID_HANDLE_VALUE
	jne .noGetClassErr
		mov rax, 0
		jmp .return
	.noGetClassErr:
	mov [.this.hDevInfoSet], rax
	mov r12, -1
	.next:
		inc r12
		@call [SetupDiEnumDeviceInfo]([.this.hDevInfoSet], r12, addr .this.devInfo)
		test rax, rax
	jnz .next
	mov rax, 1
	mov [.this.countPorts], r12w
	.return:ret
endp

proc COMInfo.choseId uses rbx r12, this, idPort
	virtObj .this:arg COMInfo at rbx
	mov rbx, rcx
	mov r12, rdx
	@call [SetupDiEnumDeviceInfo]([.this.hDevInfoSet], rdx, addr .this.devInfo)
	; @call [SetupDiEnumDeviceInterfaces]([.this.hDevInfoSet], addr .this.devInfo,\
	; 	addr .this.devInfo.ClassGuid, 0, addr .this.devIface)
	ret
endp

proc COMInfo.getPortName uses rbx r12 r13, this, strLp, size
	virtObj .this:arg COMInfo at rbx
	.strLp equ r12
	.hDeviceKey_r equ r13
	locals
		dwDataSize dd ?
		dwType dd ?
	endl
	mov rbx, rcx
	mov .strLp, rdx
	mov [dwDataSize], r8d
	@call [SetupDiOpenDevRegKey]([.this.hDevInfoSet], addr .this.devInfo,\
            DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE)
	test rax, rax
	mov .hDeviceKey_r, rax
	mov rax, 0
		jz .return
	@call [RegQueryValueExA](.hDeviceKey_r, "PortName", NULL,\
		addr dwType, .strLp, addr dwDataSize)
	mov eax, [dwDataSize]
	.return:ret
endp

proc COMInfo.getPortNameLen uses rbx r12, this
	virtObj .this:arg COMInfo at rbx
	.hDeviceKey_r equ r12
	locals
		dwDataSize dd ?
		dwType dd ?
	endl
	mov rbx, rcx
	@call [SetupDiOpenDevRegKey]([.this.hDevInfoSet], addr .this.devInfo,\
            DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE)
	test rax, rax
	mov .hDeviceKey_r, rax
	mov rax, 0
		jz .return
	@call [RegQueryValueExA](hDeviceKey_r, "PortName", NULL,\
		addr dwType, NULL, addr dwDataSize)
	mov eax, [dwDataSize]
	.return:ret
endp

proc COMInfo.getPortInfoLen, this, typeInfo
	virtObj .this:arg COMInfo
	locals
		dwDataSize dd ?
		dwType dd ?
	endl
	mov r8, rdx
	lea rdx, [.this.devInfo]
	mov rcx, [.this.hDevInfoSet]
	@call [SetupDiGetDeviceRegistryPropertyA](rcx, rdx,\
	 	r8, addr dwType, NULL, 0, addr dwDataSize)
	mov eax, [dwDataSize]
	.return:ret
endp

proc COMInfo.getPortInfo uses rbx r12 r13 r14, this, typeInfo, strLp, maxLen
	virtObj .this:arg COMInfo
	.strLp 		equ r12
	.maxLen 	equ r13
	locals
		dwDataSize dd ?
		dwType dd ?
	endl
	mov .maxLen, r9
	mov .strLp, r8
	mov r8, rdx
	lea rdx, [.this.devInfo]
	mov rcx, [.this.hDevInfoSet]
	@call [SetupDiGetDeviceRegistryPropertyA](rcx, rdx,\
	 	r8, addr dwType, .strLp, .maxLen, addr dwDataSize)
	mov eax, [dwDataSize]
	.return:ret
endp

proc GetCommInfo uses r12 r13 r14 r15 rsi
	hDevInfoSet_r 	equ r12
	hDeviceKey_r 	equ r14
	COMNameStr		equ r15
	COMTypeStr 		equ rsi
	locals 
		stackFrame 	dq ?
		stackFrame2 dq ?
		devInfo 	SP_DEVINFO_DATA
		dwType 		dq 0
		dwDataSize 	dq 0
		dwRetSize	dq 0
	endl
	@call [setlocale](0, ".1251")
	@call [SetupDiGetClassDevsA](addr GUID_DEVINTERFACE_COMPORT,\
		NULL, NULL, DIGCF_PRESENT or DIGCF_DEVICEINTERFACE)
	cmp rax, INVALID_HANDLE_VALUE
	jne .noGetClassErr
		mov rax, 0
		jmp .return
	.noGetClassErr:
	mov hDevInfoSet_r, rax
	mov r13, -1
	.getInfoLoop:
		mov [stackFrame], rsp
		inc r13
		@call [SetupDiEnumDeviceInfo](hDevInfoSet_r, r13, addr devInfo)
		test rax, rax
			jz .break
		@call [SetupDiOpenDevRegKey](hDevInfoSet_r, addr devInfo,\
            DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE)
		cmp rax, INVALID_HANDLE_VALUE
		je .invalidHandle
			mov hDeviceKey_r, rax
			@call [SetupDiGetDeviceRegistryPropertyA](hDevInfoSet_r, addr devInfo,\
			 	SPDRP_DEVICEDESC, addr dwType, NULL, 0, addr dwDataSize)
			cmp [dwType], REG_SZ
				jne .continue
			; inc [dwDataSize]
			; push [dwDataSize]
			stackAlloc COMTypeStr, [dwDataSize]
			mov byte[COMTypeStr], 0
			mov [stackFrame2], rsp
			and rsp, -16
			; dec [dwDataSize]
			@call [SetupDiGetDeviceRegistryPropertyA](hDevInfoSet_r, addr devInfo,\
			 	SPDRP_DEVICEDESC, addr dwType, COMTypeStr, [dwDataSize], addr dwDataSize)
			@call [RegQueryValueExA](hDeviceKey_r, "PortName", NULL,\
				addr dwType, NULL, addr dwDataSize)
			test rax, rax
				jnz .continue
			cmp [dwType], REG_SZ
				jne .continue
			; inc [dwDataSize]
			mov rsp, [stackFrame2]
			stackAlloc COMNameStr, [dwDataSize]
			mov byte[COMNameStr], 0
			and rsp, -16
			mov rax, [dwDataSize]
			; dec rax
			mov [dwRetSize], rax
			@call [RegQueryValueExA](hDeviceKey_r, "PortName", NULL,\
				addr dwType, COMNameStr, addr dwRetSize)
			mov rax, [dwDataSize]
			cmp [dwRetSize], rax
				ja .continue
			mov byte[COMTypeStr-1], '|'
			@call [puts](COMNameStr)
		.invalidHandle:
		.continue:
		mov rsp, [stackFrame]
	jmp .getInfoLoop
	.break:
	.return:ret
endp