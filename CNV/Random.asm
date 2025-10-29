proc_noprologue

macro Random.make this, seed{
	inlineObj _this, this, pcx
	local ..this, ._this
	virtObj ._this dptr ? at _this
	match any, seed\{
		if seed in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14>
			movss dword[._this + Random.__seed], seed
		else if seed eqtype [0] | seed eqtype byte [0]
			if ._this relativeto pax
				lea pcx, [_this]
				virtual at pcx
					..this:
				end virtual
			else
				virtual at _this
					..this:
				end virtual
			end if
			mov eax, seed
			mov dword[..this + Random.__seed], eax
		else if seed eqtype eax | seed eqtype 0 | seed eqtype 0f
			mov dword[._this + Random.__seed], seed
		else
			match =addr _seed, seed\\{
				if _seed eqtype 0 & _seed relativeto 0 
					mov dword[._this + Random.__seed], _seed
				else if _seed eqtype eax
					mov dword[._this + Random.__seed], _seed
				else
					if ._this relativeto pax
						lea pcx, [_this]
						virtual at pcx
							..this:
						end virtual
					else
						virtual at _this
							..this:
						end virtual
					end if
					lea eax, [_seed]
					mov dword[..this + Random.__seed], eax
				end if
			\\}
		end if
		rept 0\{
	\}
	rept 1\{
		if ._this relativeto pax
			lea pcx, [_this]
			virtual at pcx
				..this:
			end virtual
		else
			virtual at _this
				..this:
			end virtual
		end if
		rdtsc
		mov dword[..this + Random.__seed], eax
	\}
}

macro Random.next this, min, max{
	match =3, __argscount__\{
		@call c Random.__next(this, min, max)
	\}
	match =2, __argscount__\{
		@call c Random.__next(this, 0, min)
	\}
	match =1, __argscount__\{
		@call c Random.__next(this, INT_MIN, INT_MAX)
	\}
}

macro Random.unmake this{}

; uint32_t xorshift32 ( struct xorshift32_state * state ) {  
; 	uint32_t x = state -> a ; 
; 	x ^= x << 13 ; 
; 	x ^= x >> 17 ; 
; 	x ^= x << 5 ; 
; 	return state -> a = x ; 
; }  

proc Random.__next c, this:POINTER, min:DWORD, max:DWORD
	virtObj .this:arg Random from @arg1
	@sarg @arg2
	mov eax, [.this.__seed]
	mov edx, eax
	shl eax, 13
	xor eax, edx
	mov edx, eax
	shr eax, 17
	xor eax, edx
	mov edx, eax
	shl eax, 5
	xor eax, edx
	mov [.this.__seed], eax
	mov pcx, @arg3
	sub ecx, [min]
	xor edx, edx
	div ecx
	mov eax, [min]
	add eax, edx
	ret
endp

macro Random64.make this, seed{
	inlineObj _this, this, pcx
	local ..this, ._this, ..seed, lseed, hseed
	virtObj ._this dptr ? at _this
	match any, seed\{
		if seed in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14>
			movq [._this + Random.__seed], seed
		else if seed eqtype [0] | seed eqtype byte [0]
			if ._this relativeto pax
				lea pcx, [_this]
				virtual at pcx
					..this:
				end virtual
			else
				virtual at _this
					..this:
				end virtual
			end if
			movq xmm0, seed
			movq [..this + Random.__seed], xmm0
		else if seed eqtype eax | seed eqtype 0 | seed eqtype 0f
			virtual at $
				..seed dq seed
				load lseed dword from ..seed
				load hseed dword from ..seed+4
			end virtual
			mov dword[._this + Random.__seed], lseed
			mov dword[._this + Random.__seed], hseed
		else
			match =addr _seed, seed\\{
				if _seed eqtype 0 & _seed relativeto 0 
					virtual at $
						..seed dq seed
						load lseed dword from ..seed
						load hseed dword from ..seed+4
					end virtual
					mov dword[._this + Random.__seed], lseed
					mov dword[._this + Random.__seed], hseed
				else if _seed eqtype eax
					mov dword[._this + Random.__seed], _seed
				else
					if ._this relativeto pax
						lea pcx, [_this]
						virtual at pcx
							..this:
						end virtual
					else
						virtual at _this
							..this:
						end virtual
					end if
					lea pax, [_seed]
					match =x86, __architecture\\\{
						mov [..this + Random.__seed + 4], 0
					\\\}
					mov pointer[..this + Random.__seed], pax
				end if
			\\}
			match _high:_low, seed\\{
				mov [..this + Random.__seed], _low
				mov [..this + Random.__seed + 4], _high
			\\}
		end if
	rept 0\{\}rept 1\{
		if ._this relativeto pax
			lea pcx, [_this]
			virtual at pcx
				..this:
			end virtual
		else
			virtual at _this
				..this:
			end virtual
		end if
		rdtsc
		match =x86, __architecture\\{
			mov dword[..this + Random.__seed + 4], edx
		\\}
		mov pointer[..this + Random.__seed], pax
	\}
}

macro Random64.next this, min, max{
	match =3, __argscount__\{
		@call c Random64.__next(this, min, max)
	\}
	match =2, __argscount__\{
		@call c Random64.__next(this, 0, min)
	\}
	match =1, __argscount__\{
		@call c Random64.__next(this, qword INT64_MIN, qword INT64_MAX)
	\}
}

macro Random64.unmake this{}

match =x64, __architecture{
	proc Random64.__next, this, min, max
		virtObj .this:arg Random64
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
		xor edx, edx
		div r8
		add rdx, r9
		mov rax, rdx
		ret
	endp
}

match =x86, __architecture{
	proc Random64.__next c, this, min:QWORD, max:QWORD
		virtObj .this:arg Random64 from [this]

		\local result:Divq 
		movq xmm1, [.this.__seed]
		movq xmm2, xmm1
		psllq xmm1, 7
		pxor xmm1, xmm2
		movq xmm2, xmm1
		psrlq xmm1, 9
		pxor xmm1, xmm2
		movq [.this.__seed], xmm1
		movq xmm2, [max]
		movq xmm3, [min]
		psubq xmm2, xmm3
		@call c CNV.ui64div(addr result, qword xmm1, qword xmm2)
		movq xmm1, [result.reminder]
		movq xmm0, [min]
		paddq xmm1, xmm0
		movd eax, xmm1
		psrlq xmm1, 32
		movd edx, xmm1
		ret
	endp
}

proc_resprologue
