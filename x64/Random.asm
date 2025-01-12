macro Random.new this, seed{
	inlineObj _this, this, rcx
	match any, seed\{
		inlineObj _seed, seed, rdx
		mov [_this+Random.__seed], _seed
		rept 0\{
	\}
	rept 1\{
		; if (this eqtype 0 & this relativeto 0) | param eq rax
		local ..this
		virtObj ..this dq ? at _this
		if ..this relativeto rax | ..this relativeto rdx
			lea r8, [..this]
		end if
		rdtsc
		shl rdx, 32
		add rax, rdx
		if ..this relativeto rax | ..this relativeto rdx
			mov [r8 + Random.__seed], rax
		else
			mov [_this + Random.__seed], rax
		end if
	\}
}

; macro GetType name*, val*, type{
; 	type equ
; 	match [any], val\{
; 		type equ mem
; 		name equ val
; 	\}
; 	match =ptr any, val\{
; 		type equ mem
; 		name equ [any]
; 	\}
; 	match _any[any], val\{
; 		type equ mem
; 		name equ [any]
; 	\}
; 	match _any =ptr any, val\{
; 		type equ mem
; 		name equ [any]
; 	\}
; 	match =addr any, val\{
; 		type equ addr
; 		name equ any
; 	\}
; 	match , type\{
; 		restore type
; 		type equ label
; 		name equ val
; 	\}
; }

macro Random.next this, min, max{
	match =3, __argscount__\{
		@call Random.__next(this, min, max)
	\}
	match =2, __argscount__\{
		@call Random.__next(this, 0, min)
	\}
	match =1, __argscount__\{
		@call Random.__next(this, 0x8000000000000000, 0x7FFFFFFFFFFFFFFF)
	\}
}

proc_noprologue

proc Random.__next, this, min, max
	virtObj .this:arg Random
	mov r9, rdx
	mov rax, [.this.__seed]
	mov rdx, rax
	shl rax, 7
	xor rax, rdx
	mov rdx, rax
	shr rax, 9
	xor rax, rdx
	mov [.this.__seed], rax
	sub r8, r9
	; cqo
	xor rdx, rdx
	div r8
	add rdx, r9
	mov rax, rdx
	ret
endp

proc_resprologue