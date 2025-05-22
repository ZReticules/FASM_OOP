

;// Код операции раскрутки является одним из следующих значений:
;// https://docs.microsoft.com/en-us/cpp/build/exception-handling-x64?view=msvc-170

UWOP_PUSH_NONVOL     =  0  ; 1 узел (поместите энергонезависимый целочисленный регистр, уменьшив RSP на 8)
UWOP_ALLOC_LARGE     =  1  ; 2 узла (выделите в стеке область большого размера)
UWOP_ALLOC_SMALL     =  2  ; 1 узел (выделите небольшую область в стеке, от 8 до 128 байт)
UWOP_SET_FPREG       =  3  ; 1 узел (установите регистр указателя кадра, установив в регистре некоторое смещение текущего RSP)
UWOP_SAVE_NONVOL     =  4  ; 2 узла (сохраните энергонезависимый регистр в стеке, используя MOV вместо PUSH)
UWOP_SAVE_NONVOL_FAR =  5  ; 3 узла (сохраните энергонезависимый целочисленный регистр в стеке с длинным смещением, используя MOV вместо PUSH)
UWOP_SAVE_XMM128     =  8  ; 2 узла (сохраните все 128 бит энергонезависимого регистра XMM в стеке)
UWOP_SAVE_XMM128_FAR =  9  ; 3 узла (сохраните все 128 бит энергонезависимого регистра XMM в стеке с длинным смещением)
UWOP_PUSH_MACHFRAME  =  10 ; 1 узел

; macro PushNonVolReg {push rsi rdi rbp rbx r12 r13 r14 r15}
; macro PopNonVolReg  {pop  r15 r14 r13 r12 rbx rbp rdi rsi}

struct RUNTIME_FUNCTION      ;//----> IMAGE_RUNTIME_FUNCTION_ENTRY
  BeginAddress     	dd  0  ;// RVA
  EndAddress       	dd  0  ;// RVA
  UnwindInfoAddress	dd  0  ;// RVA линк на "UNWIND_INFO"
ends
;//---------------------------------
;      db  19h,0,0,0
;      dd  RVA Seh (линк на обработчик)
;      dd  0
;//---------------------------------
struct UNWIND_INFO
	VersionFlags    	db  19h
	SizeOfProlog    	db  0
	CountOfCodes    	db  0
	FrameRegOffset  	db  0
	ExceptionHandler	dd  0  ;// RVA обработчика
	FunctionEntry   	dd  0
ends
;
;Формат обработчика исключений:
; EXCEPTION_DISPOSITION (pEXCEPTION_ROUTINE)
;    In.     RCX = ExceptionRecord   --> указатель на структуру
;    In.     RDX = EstablishedFrame  --> адрес выделенного стека для этой функции
;    In.Out. R8  = ContextRecord     --> указатель на структуру
;    In.Out. R9  = DispatcherContext --> указатель на контекст диспетчеризации для этой функции
;//--------------------------------

struct EXCEPTION_RECORD64
	ExceptionCode   			dd  0
	ExceptionFlags  			dd  0   ;// см.константы ниже
	ExceptionRecord 			dq  0   ;// линк на связанную "EXCEPTION_RECORD" (если в цепочке)
	ExceptionAddress			dq  0
	NumberParameters			dd  0   ;// записей в "ExceptionInfo"
	unusedAlignment 			dd  0   ;// (дворд выравнивания структуры на 8)
	ExceptionInformation  rq  EXCEPTION_MAXIMUM_PARAMETERS
ends
;// ExceptionFlags constantes 
;// --------------------------
EXCEPTION_CONTINUABLE     = 0
EXCEPTION_NONCONTINUABLE  = 1

struct DISPATCHER_CONTEXT
	ControlPc       	dq  0  ; RIP на момент исключения
	ImageBase       	dq  0  ; база программы
	FunctionEntry   	dq  0  ; указатель на IMAGE_RUNTIME_FUNCTION_ENTRY
	EstablisherFrame	dq  0  ; 
	TargetIp        	dq  0  ; 
	ContextRecord   	dq  0  ; указатель на CONTEXT
	LanguageHandler 	dq  0  ; (для конкретного языка) EXCEPTION_DISPOSITION
	HandlerData     	dq  0  ; (для конкретного языка)
	HistoryTable    	dq  0  ; указатель на структуру кэша-раскрутки UNWIND_HISTORY_TABLE
	ScopeIndex      	dd  0  ; индекс области
	Fill0           	dd  0  ; выравнивание..
ends

struct UNWIND_HISTORY_TABLE
	Count      	dd  0
	LocalHint  	db  0
	GlobalHint 	db  0
	Search     	db  0
	Once       	db  0
	LowAddress 	dq  0
	HighAddress	dq  0
	Entry      	rq  12  ; _UNWIND_HISTORY_TABLE_ENTRY
ends

struct UNWIND_HISTORY_TABLE_ENTRY
	ImageBase    	dq  0
	FunctionEntry	dq  0  ; _IMAGE_RUNTIME_FUNCTION_ENTRY
ends

UNWIND_HISTORY_TABLE_SIZE    =  12
UNWIND_HISTORY_TABLE_NONE    =  0
UNWIND_HISTORY_TABLE_GLOBAL  =  1
UNWIND_HISTORY_TABLE_LOCAL   =  2

struct EXCEPTION_POINTERS
	ExceptionRecord	dq  0   ;// линк на "EXCEPTION_RECORD64"
	ContextRecord  	dq  0   ;// линк на "CONTEXT"
ends

struct XMM_SAVE_FORMAT
	ControlWord  	dw  0
	StatusWord   	dw  0
	TagWord      	db  0
	Reserved1    	db  0
	ErrorOpcode  	dw  0
	ErrorOffset  	dd  0
	ErrorSelector	dw  0
	Reserved2    	dw  0
	DataOffset   	dd  0
	DataSelector 	dw  0
	Reserved3    	dw  0
	MxCsr        	dd  0
	MxCsr_Mask   	dd  0
	FloatRegisters 	rb  8 * 16    ; every 128-bit holds st in low 80-bits or mm in low 64-bits
	XmmRegisters   	rb  16 * 16   ; yeah, AMD64 in long mode (64-bit) has 16 xmm registers
	Reserved4      	rb  96
ends

struct CONTEXT64
	P1Home            dq  0
	P2Home            dq  0
	P3Home            dq  0
	P4Home            dq  0
	P5Home            dq  0
	P6Home            dq  0
	ContextFlags      dd  0
	MxCsr             dd  0
	SegCs             dw  0
	SegDs             dw  0
	SegEs             dw  0
	SegFs             dw  0
	SegGs             dw  0
	SegSs             dw  0
	EFlags            dd  0
	Dr0               dq  0
	Dr1               dq  0
	Dr2               dq  0
	Dr3               dq  0
	Dr6               dq  0
	Dr7               dq  0
	Rax               dq  0
	Rcx               dq  0
	Rdx               dq  0
	Rbx               dq  0
	Rsp               dq  0
	Rbp               dq  0
	Rsi               dq  0
	Rdi               dq  0
	R8                dq  0
	R9                dq  0
	R10               dq  0
	R11               dq  0
	R12               dq  0
	R13               dq  0
	R14               dq  0
	R15               dq  0
	Rip               dq  0
	FltSave           XMM_SAVE_FORMAT
	Header            rq  2*16
	Legacy            rq  8*16
	Xmm0              dq  0,0
	Xmm1              dq  0,0
	Xmm2              dq  0,0
	Xmm3              dq  0,0
	Xmm4              dq  0,0
	Xmm5              dq  0,0
	Xmm6              dq  0,0
	Xmm7              dq  0,0
	Xmm8              dq  0,0
	Xmm9              dq  0,0
	Xmm10             dq  0,0
	Xmm11             dq  0,0
	Xmm12             dq  0,0
	Xmm13             dq  0,0
	Xmm14             dq  0,0
	Xmm15             dq  0,0
	VectorRegister    rq  26*16
	VectorControl     dq  0
	DebugControl      dq  0
	LastBranchToRip   dq  0
	LastBranchFromRip dq  0
	LastExceptToRip   dq  0
	LastExceptFromRip dq  0
ends
