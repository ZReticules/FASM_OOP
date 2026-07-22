sizeof.BYTE 	= 1
sizeof.WORD 	= 2
sizeof.DWORD 	= 4
sizeof.PWORD 	= 6
sizeof.FWORD 	= 6
sizeof.QWORD 	= 8
sizeof.TBYTE 	= 10
sizeof.TWORD 	= 10
sizeof.DQWORD 	= 16
sizeof.XWORD 	= 16
sizeof.QQWORD 	= 32
sizeof.YWORD 	= 32
sizeof.DQQWORD 	= 64
sizeof.ZWORD 	= 64

sizeof.byte 	= 1
sizeof.word 	= 2
sizeof.dword 	= 4
sizeof.pword 	= 6
sizeof.fword 	= 6
sizeof.qword 	= 8
sizeof.tbyte 	= 10
sizeof.tword 	= 10
sizeof.dqword 	= 16
sizeof.xword 	= 16
sizeof.qqword 	= 32
sizeof.yword 	= 32
sizeof.dqqword 	= 64
sizeof.zword 	= 64

alignof.BYTE 	= 1
alignof.WORD 	= 2
alignof.DWORD 	= 4
alignof.PWORD 	= 8
alignof.FWORD 	= 8
alignof.QWORD 	= 8
alignof.TBYTE 	= 16
alignof.TWORD 	= 16
alignof.DQWORD 	= 16
alignof.XWORD 	= 16
alignof.QQWORD 	= 32
alignof.YWORD 	= 32
alignof.DQQWORD = 64
alignof.ZWORD 	= 64

alignof.byte 	= 1
alignof.word 	= 2
alignof.dword 	= 4
alignof.pword 	= 8
alignof.fword 	= 8
alignof.qword 	= 8
alignof.tbyte 	= 16
alignof.tword 	= 16
alignof.dqword 	= 16
alignof.xword 	= 16
alignof.qqword 	= 32
alignof.yword 	= 32
alignof.dqqword = 64
alignof.zword 	= 64

; BYTE@builtin 	equ 1
; BYTE@datadef 	equ db
; WORD@builtin 	equ 1
; WORD@datadef 	equ dw
; DWORD@builtin 	equ 1
; DWORD@datadef 	equ dd
; PWORD@builtin 	equ 1
; PWORD@datadef 	equ dp
; FWORD@builtin 	equ 1
; FWORD@datadef 	equ df
; QWORD@builtin 	equ 1
; QWORD@datadef 	equ dq
; TBYTE@builtin 	equ 1
; TBYTE@datadef 	equ dt
; TWORD@builtin 	equ 1
; TWORD@datadef 	equ dt
; DQWORD@builtin 	equ 1
; DQWORD@datadef 	equ ddq
; XWORD@builtin 	equ 1
; XWORD@datadef 	equ dxw
; QQWORD@builtin 	equ 1
; YWORD@builtin 	equ 1
; DQQWORD@builtin equ 1
; ZWORD@builtin 	equ 1

byte@builtin 	equ 1
byte@datadef 	equ db
word@builtin 	equ 1
word@datadef 	equ dw
dword@builtin 	equ 1
dword@datadef 	equ dd
pword@builtin 	equ 1
pword@datadef 	equ dp
fword@builtin 	equ 1
fword@datadef 	equ df
qword@builtin 	equ 1
qword@datadef 	equ dq
tbyte@builtin 	equ 1
tbyte@datadef 	equ dt
tword@builtin 	equ 1
tword@datadef 	equ dt
dqword@builtin 	equ 1
dqword@datadef 	equ ddq
xword@builtin 	equ 1
xword@datadef 	equ dxw
qqword@builtin 	equ 1
yword@builtin 	equ 1
dqqword@builtin equ 1
zword@builtin 	equ 1

real@builtin 	equ 1
real@datadef 	equ dw
float@builtin 	equ 1
float@datadef 	equ dd
double@builtin 	equ 1
double@datadef 	equ dq

struc dxw [val] {
    match , @struct \{ 
    	common
    	field@struct equ struct@aligner, ., dxw, <val> 
	\}
    match no, @struct \{ 
		common
		\local cntr, val_def
		label . XWORD
        cntr = 1
        
        forward
        val_def equ val
        match _mp1 =dup _mp2, val_def\\{
        	cntr = _mp1
        	match (_mp3), _mp2\\\{
        		val_def equ _mp3
        	rept 0\\\{\\\} rept 1\\\{
        		val_def equ _mp2
    		\\\}
    	\\}
		match high:low, val_def\\{
			times cntr dq low, high
		rept 0\\{\\}rept 1\\{
			times cntr dq val_def, 0
		\\}
		common
    \}
}

macro dxw [val] {
    match , @struct \{ 
        \local anonymous
        define field@struct anonymous, dxw, <values> 
    \}
    match no, @struct \\\{
		common
		\local cntr, val_def
		label . XWORD
        cntr = 1
        
        forward
        val_def equ val
        match _mp1 =dup _mp2, val_def\\{
        	cntr = _mp1
        	match (_mp3), _mp2\\\{
        		val_def equ _mp3
        	rept 0\\\{\\\} rept 1\\\{
        		val_def equ _mp2
    		\\\}
    	\\}
		match high:low, val_def\\{
			times cntr dq low, high
		rept 0\\{\\}rept 1\\{
			match =?, val_def\\\{
				times cntr dq ?, ?
			rept 0\\\{\\\}rept 1\\\{
				times cntr dq val_def, 0
			\\\}
		\\}
    \\\} 
}

struc rxw cntr {
    match , @struct \{ 
    	common
        define field@struct ., dxw, cntr dup(?)
    \}
    match no, @struct \{ 
		label . XWORD
        rq 2 * cntr
    \}
}

macro rxw cntr {
    rq 2 * cntr
}

struc ddq any&{
	. dxw any
}

macro ddq any&{
	dxw any
}

struc rdq cntr{
	. rxw cntr
}

macro ddq cntr{
	rxw cntr
}

alignof.dxw = 16
sizeof.dxw = 16
alignof.rxw = 16
sizeof.rxw = 16

macro gen_sizeof [suff, size]{
	sizeof.#d#suff = size
	sizeof.#r#suff = size
	alignof.#d#suff = 1 shl (bsr size + (1 - (1 shr (size and (1 shl bsr size - 1)))))
	alignof.#r#suff = alignof.#d#suff
}

gen_sizeof b, 1, w, 2, u, 2, d, 4, p, 6, f, 6, q, 8, t, 10