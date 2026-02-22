@module Except
	importlib kernel32,\
		AddVectoredExceptionHandler,\
		RemoveVectoredExceptionHandler,\
		TlsAlloc,\
		TlsFree,\
		TlsGetValue,\
		TlsSetValue,\
		SetThreadErrorMode,\
		RaiseException

	EXCEPTION_MAXIMUM_PARAMETERS    =  15

	;// Возвращаемые из обработчиков значения
	EXCEPTION_CONTINUE_EXECUTION    =  0
	EXCEPTION_CONTINUE_SEARCH       =  1
	EXCEPTION_NESTED_EXCEPTION      =  2
	EXCEPTION_COLLIDED_UNWIND       =  3

	; некоторые коды исключений
	EXCEPTION_DBG_RIP                   = 0x40010007
	EXCEPTION_GUARD_PAGE                = 0x80000001
	EXCEPTION_DATATYPE_MISALIGNMENT     = 0x80000002
	EXCEPTION_BREAKPOINT                = 0x80000003
	EXCEPTION_SINGLE_STEP               = 0x80000004
	EXCEPTION_ACCESS_VIOLATION          = 0xC0000005
	EXCEPTION_IN_PAGE_ERROR             = 0xC0000006
	EXCEPTION_INVALID_HANDLE            = 0xC0000008
	EXCEPTION_ILLEGAL_INSTRUCTION       = 0xC000001D
	EXCEPTION_NONCONTINUABLE_EXCEPTION  = 0xC0000025
	EXCEPTION_INVALID_DISPOSITION       = 0xC0000026
	EXCEPTION_UNWIND                    = 0xC0000027
	EXCEPTION_ARRAY_BOUNDS_EXCEEDED     = 0xC000008C
	EXCEPTION_FLT_DENORMAL_OPERAND      = 0xC000008D
	EXCEPTION_FLT_DIVIDE_BY_ZERO        = 0xC000008E
	EXCEPTION_FLT_INEXACT_RESULT        = 0xC000008F
	EXCEPTION_FLT_INVALID_OPERATION     = 0xC0000090
	EXCEPTION_FLT_OVERFLOW              = 0xC0000091
	EXCEPTION_FLT_STACK_CHECK           = 0xC0000092
	EXCEPTION_FLT_UNDERFLOW             = 0xC0000093
	EXCEPTION_INT_DIVIDE_BY_ZERO        = 0xC0000094
	EXCEPTION_INT_OVERFLOW              = 0xC0000095
	EXCEPTION_PRIV_INSTRUCTION          = 0xC0000096
	EXCEPTION_STACK_OVERFLOW            = 0xC00000FD
	EXCEPTION_CONTROL_C_EXIT            = 0xC000013A
	EXCEPTION_UNHANDLED_                = 0xC0000144
	EXCEPTION_ILLEGAL_FLT_CONTEXT       = 0xC000014A
	EXCEPTION_POSSIBLE_DEADLOCK         = 0xC0000194
	EXCEPTION_FLT_MULTIPLE_FAULTS       = 0xC00002B4
	EXCEPTION_FLT_MULTIPLE_TRAPS        = 0xC00002B5
	EXCEPTION_DATATYPE_MISALIGNMENT     = 0xC00002C5
	EXCEPTION_REG_NAT_CONSUMPTION       = 0xC00002C9
	EXCEPTION_FATAL_USER_CALLBACK       = 0xC000041D
	EXCEPTION_CALLBACK_POP_STACK        = 0xC0000423
	EXCEPTION_FAIL_FAST                 = 0xC0000602
	EXCEPTION_THREADPOOL_HANDLE         = 0xC000070A
	EXCEPTION_MCA                       = 0xC0000713
	EXCEPTION_GRAPHICS_GPU_ON_DEVICE    = 0xC01E0200

	EXCEPTION_USER 						= 0xE0000000


	match =x64, __architecture\{
		include "Except\EXCEPT64.inc"
		struct EXCEPTION_RECORD EXCEPTION_RECORD64
		ends
	\}

	match =x86, __architecture\{
		include "Except\EXCEPT32.inc"
	\}


	struct Exception
		getExceptionInfo 	dm this:inline
		__genVEH			dm this
		__delVEH			dm this

		const EXCEPTION_RETURN = EXCEPTION_USER + 0xEC0
		rb 0
	ends

	include "Except\Except.fasm"
	
	local exceptionUsed
	macro try \{
		\local ..catch, ..no_except
		exceptionUsed = Exception.used
		last@catch equ ..catch
		last@no_except equ ..no_except
		break_try equ ..no_except

		Except.save_context
		
		@call Exception::__genVEH()
		mov pax, psp
		@call Except.SaveStack(..catch, pbp, pax)
	\}

	macro catch \{
		jmp last@no_except
		last@catch:
		Except.load_context
		@call Exception::__delVEH() 
		restore last@catch
	\}

	macro end_try \{
		\local ..skip_noexcept
		
		jmp ..skip_noexcept
		last@no_except:
		@call Exception::__delVEH()

		..skip_noexcept:
		restore last@no_except, break_try
	\}
@endm
