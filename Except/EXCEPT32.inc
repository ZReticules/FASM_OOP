struct EXCEPTION_RECORD
  ExceptionCode      dd ?
  ExceptionFlags     dd ?
  ExceptionRecord    dd ?  ; pointer to next in chain
  ExceptionAddress   dd ?
  NumberParameters   dd ?
  ExceptionInformation rd EXCEPTION_MAXIMUM_PARAMETERS
ends

struct CONTEXT32
  ContextFlags     dd ?
  Dr0              dd ?
  Dr1              dd ?
  Dr2              dd ?
  Dr3              dd ?
  Dr6              dd ?
  Dr7              dd ?
  FloatSave        rb 112
  SegGs            dd ?
  SegFs            dd ?
  SegEs            dd ?
  SegDs            dd ?
  Edi              dd ?
  Esi              dd ?
  Ebx              dd ?
  Edx              dd ?
  Ecx              dd ?
  Eax              dd ?
  Ebp              dd ?
  Eip              dd ?
  SegCs            dd ?
  EFlags           dd ?
  Esp              dd ?
  SegSs            dd ?
  ExtendedRegisters rb 512
ends

struct EXCEPTION_POINTERS
  ExceptionRecord   dd ?  ; → EXCEPTION_RECORD
  ContextRecord     dd ?  ; → CONTEXT32
ends
