dptr fix dd

rptr fix rd

pointer fix dword

POINTER fix DWORD

pointer.size fix 4

pointer.shift fix 2

pax fix eax
pbx fix ebx
pcx fix ecx
pdx fix edx
pbp fix ebp
psp fix esp
psi fix esi
pdi fix edi

define PTR_MAX 0x7FFFFFFF
define PTR_MIN 0x80000000
define UPTR_MAX 0xFFFFFFFF

macro movptr dest, src{
	movd dest, src
} 

macro movsptr{
	movsd
}

struc movsptr{
	. movsd
}

XMM_REGS equ xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7

qword_result equ edx:eax

PEB equ fs:0x30

macro jpcxz arg {
	jecxz arg
}