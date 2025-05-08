dptr fix dq

rptr fix rq

pointer fix qword

POINTER fix QWORD

pointer.size fix 8

pax fix rax
pbx fix rbx
pcx fix rcx
pdx fix rdx
pbp fix rbp
psp fix rsp
psi fix rsi
pdi fix rdi

define PTR_MAX 0x7FFFFFFFFFFFFFFF
define PTR_MIN 0x8000000000000000
define UPTR_MAX 0xFFFFFFFFFFFFFFFF

macro movptr dest, src{
	movq dest, src
} 

struc movsptr{
	movsq
}

macro movsptr{
	movsq
}

XMM_REGS equ xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm11, xmm12, xmm13, xmm14, xmm15
