@module COMIface
	struct DCB
		DCBlength 	dd ?
		BaudRate 	dd ?
		flags 		dd ?
		; DWORD fBinary:1; // binary mode, no EOF check
		; DWORD fParity:1; // enable parity checking
		; DWORD fOutxCtsFlow:1; // CTS output flow control
		; DWORD fOutxDsrFlow:1; // DSR output flow control
		; DWORD fDtrControl:2; // DTR flow control type
		; DWORD fDsrSensitivity:1; // DSR sensitivity
		; DWORD fTXContinueOnXoff:1; // XOFF continues Tx
		; DWORD fOutX:1; // XON/XOFF out flow control
		; DWORD fInX:1; // XON/XOFF in flow control
		; DWORD fErrorChar:1; // enable error replacement
		; DWORD fNull:1; // enable null stripping
		; DWORD fRtsControl:2; // RTS flow control
		; DWORD fAbortOnError:1; // abort reads/writes on error
		; DWORD fDummy2:17; // reserved
		wReserved	dw ?
		XonLim		dw ?
		XoffLim		dw ?
		ByteSize	db ?
		Parity		db ?
		StopBits	db ?
		XonChar		db ?
		XoffChar	db ?
		ErrorChar	db ?
		EofChar		db ?
		EvtChar		db ?
		wReserved1	dw ?
	ends

	struct COMSTAT
	  	flags 		dd ?
		; DWORD fCtsHold : 1;
		; DWORD fDsrHold : 1;
		; DWORD fRlsdHold : 1;
		; DWORD fXoffHold : 1;
		; DWORD fXoffSent : 1;
		; DWORD fEof : 1;
		; DWORD fTxim : 1;
		; DWORD fReserved : 25;
		cbInQue		dd ?
		cbOutQue	dd ?
	ends

	struct COMMTIMEOUTS 
		ReadIntervalTimeout			dd ?
		ReadTotalTimeoutMultiplier	dd ?
		ReadTotalTimeoutConstant	dd ?
		WriteTotalTimeoutMultiplier	dd ?
		WriteTotalTimeoutConstant	dd ?
	ends

	struct OVERLAPPED
		Internal			dptr ?
		InternalHigh		dptr ?
		union 
			struct
				Offset		dd ?
				OffsetHigh	dd ?
			ends
			Pointer 		dptr ?
		ends
		hEvent 				dptr 0
	ends

	struct COMIface
		; portNum 	dd ?
		handle 		dptr ?
		; dcb 		DCB
		comstat		COMSTAT
		timeouts 	COMMTIMEOUTS
		; open 		dm this
		; openN		dm this
		openA			dm this
		close 			dm this:inline

		getStat 		dm this:inline
		reset 			dm this:inline
		getParams		dm this:inline
		setParams 		dm this:inline
		setTimeouts 	dm this:inline
		setup 			dm this:inline
		setMask 		dm this:inline

		waitEvent 		dm this:inline

		read 			dm this
		getReadedCount	dm this
		write 			dm this
	ends

	include "COMIFace.fasm"
@endm
