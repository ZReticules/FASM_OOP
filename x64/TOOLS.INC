include "include_once.inc"
include_once "win64ax.inc"
include_once "TOOLS\x64\FASM_OOP\FASM_OOP.inc"
include_once "TOOLS\x64\TLS.inc"
include_once "TOOLS\x64\importlib.inc"

; macro stackAlloc Dest*, Size*, StackFrame{
; 	if ~(StackFrame eq)
; 		mov StackFrame, rsp
; 	end if
; 	sub rsp, Size
; 	mov Dest, rsp
; 	; and rsp, -16
; }

macro proc_noprologue{
	prologue@proc equ static_rsp_prologue
	epilogue@proc equ static_rsp_epilogue
	close@proc equ static_rsp_close
}

macro proc_resprologue{
	restore prologue@proc 
	restore epilogue@proc 
	restore close@proc 
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

; types enum,\ 
; 	array,\
; 	number,\
; 	string,\
; 	float,\
; 	mem,\
; 	freg,\
; 	qreg,\
; 	dreg,\
; 	wreg,\
; 	breg
	
; macro __GetType__ thistype, arg1&{
; 	match any=,any, arg1\{
; 		thistype = types.array
; 	\}
; 	match any =dup any, arg1\{
; 		thistype = types.array
; 	\}
; 	if arg1 eqtype 0
; 		thistype = types.number
; 	else if arg1 eqtype ""
; 		thistype = types.string
; 	else if arg1 eqtype 0.0
; 		thistype = types.float
; 	else if arg1 eqtype [0]
; 		thistype = types.mem
; 	else if arg1 in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15>
; 		thistype = types.freg
; 	else if arg1 in <rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, r8, r9, r10, r11, r12, r13, r14, r15>
; 		thistype = types.qreg
; 	else if arg1 in <eax, ebx, ecx, edx, esi, edi, esp, ebp, r8d, r9d, r10d, r11d, r12d, r13d, r14d, r15d>
; 		thistype = types.dreg
; 	else if arg1 in <ax, bx, cx, dx, si, di, sp, bp, r8w, r9w, r10w, r11w, r12w, r13w, r14w, r15w>
; 		thistype = types.wreg
; 	else if arg1 in <al, ah, bl, bh, cl, ch, dl, dh, sil, dil, spl, bpl, r8l, r9l, r10l, r11l, r12l, r13l, r14l, r15l>
; 		thistype = types.breg
; 	end if
; }

@endm fix }

macro @module name{
	match =name#_included, name#_included{
		define name#_included
}

include_once "TOOLS\x64\CNV.inc"