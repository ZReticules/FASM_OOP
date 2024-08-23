include "include_once.inc"
include_once "win32ax.inc"
include_once "TOOLS\x86\FASM_OOP\FASM_OOP.inc"
include_once "TOOLS\x86\TLS.inc"
include_once "TOOLS\x86\importlib.inc"

; macro stackAlloc Dest*, Size*, StackFrame{
; 	if ~(StackFrame eq)
; 		mov StackFrame, rsp
; 	end if
; 	sub rsp, Size
; 	mov Dest, rsp
; 	; and rsp, -16
; }

; macro proc_noprologue{
; 	prologue@proc equ static_rsp_prologue
; 	epilogue@proc equ static_rsp_epilogue
; 	close@proc equ static_rsp_close
; }

; macro proc_resprologue{
; 	restore prologue@proc 
; 	restore epilogue@proc 
; 	restore close@proc 
; }

macro _static_args [args]{
	common
	local args_space
	args_space equ 0
	forward
	local argname, argsize, argtype, argnow
	argsize equ 4
	argtype equ dd ?
	argnow equ args
	match _arg:type, args\{
		restore argtype, argsize, argnow
		match =BYTE, type\\{
			argsize equ 1
			argtype equ db ?
		\\}
		match =WORD, type\\{
			argsize equ 2
			argtype equ dw ?
		\\}
		match =DWORD, type\\{
			argsize equ 4
			argtype equ dd ?
		\\}
		match =QWORD, type\\{
			argsize equ 8
			argtype equ dq ?
		\\}
		match =argtype, argtype\\{
			argtype equ type
			argsize equ sizeof.\#type
		\\}
		argnow equ _arg
	\}
	virtual at esp+args_space+argsize
		argnow argtype
	end virtual
	args_space equ args_space+argsize
	common
	\args_space equ args_space
	macro do_endp\{
		restore \args_space
	\}
}

do_endp equ

macro endp{
	endp
	do_endp
	purge do_endp
}

macro setenum parent, name, value{
	parent#.#name = value
}

struc enum [args]{
	common
	local counter
	counter = 0
	forward
	local matched
	match argum==num, args\{
		counter = num
		setenum ., argum, counter
		define matched
	\}
	match =matched, matched\{
		setenum ., args, counter
	\}
	counter = counter + 1
}

@endm fix }

macro @module name{
	match =name#_included, name#_included{
		define name#_included
}

include_once "TOOLS\x86\CNV.inc"