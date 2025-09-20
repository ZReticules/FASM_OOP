arg@mems equ
;current@buf
macro defargs@proc [arg] { 
	common
	if ~ arg eq
		local _counter, _numarg
		_counter equ 0
	   	forward
		_counter equ _counter + 1
	 	local ..arg,current@arg
		rept 1 _numarg:_counter\{
			current@numarg equ @arg\#_numarg, @float_arg\#_numarg
			@arg\#_numarg\#mem equ [..arg]
			@farg\#_numarg\#mem equ [..arg]
		\}
		match argname:type, arg \{ 
			current@arg equ argname
		 	label ..arg type
		 	argname equ ..arg
			if qqword eq type
				dq ?,?,?,?
			else if dqword eq type
				dq ?,?
			else if tbyte eq type
				dq ?,?
			else
				dq ?
			end if 
		\}
		match =current@arg,current@arg \{ 
			current@arg equ arg
			arg equ ..arg
			..arg dq ? 
		\}
	   	common
		arg@mems equ current@numarg
		args@proc equ current@arg
	   	forward
		restore current@numarg
		restore current@arg
   		common
	end if 
}


macro endp { 
	purge ret,locals,endl
	finish@proc
	purge finish@proc
	restore regs@proc
	match all,args@proc \{ 
		restore all 
	\}
	restore args@proc
	match all,all@vars \{ 
		restore all 
	\} 
	match all, arg@mems\{
		restore all
	\}
	restore arg@mems
}

define @arg1 rcx
define @arg2 rdx
define @arg3 r8
define @arg4 r9

define @farg1 xmm0
define @farg2 xmm1
define @farg3 xmm2
define @farg4 xmm3

macro @sarg [argreg]{
	match argmem,argreg#mem\{
		if argreg in <rcx, rdx, r8, r9>
			mov qword argmem, argreg
		else
			movsd qword argmem, argreg
		end if
	\}
}

macro @larg [dest, argreg]{
	local _argreg
	_argreg equ argreg
	match any arg, argreg\{
		restore _argreg
		_argreg equ arg
	\}
	if argreg in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14> | dest in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14>
		if ~dest eq argreg
			movq dest, argreg
		end if
	else
		if ~dest eq argreg
			mov dest, argreg
		end if
	end if
} 

;USE_LOCAL
;USE_ESP
macro @fcvtretv dest_type*, size_type*{
	match =FPU, dest_type\{
		\local _temp
		match =@$fret_local|=USE_LOCAL, @$fret_local|flag\\{
			locals
				@$fret_local dq ?
			endl
		\\}
		match =USE_LOCAL, flag\\{
			_temp equ @$fret_local
		\\}
		match =USE_ESP, flag\\{
			_temp equ esp-8
		\\}
		match =double, size_type\\{
			movsd [_temp], xmm0
			fld qword[_temp]
		\\}
		match =float, size_type\\{
			movss [_temp], xmm0
			fld dword[_temp]
		\\}
		\restore _temp
	\}
}

;USE_LOCAL
;USE_ESP
macro @fstdretv val*, size_type*, flag=USE_LOCAL{
	if val eq st0
		local _temp
		match =@$fret_local|=USE_LOCAL, @$fret_local|flag\{
			locals
				@$fret_local dq ?
			endl
		\}
		match =USE_LOCAL, flag\{
			_temp equ @$fret_local
		\}
		match =USE_ESP, flag\{
			_temp equ esp-8
		\}
		match =double, size_type\{
			fstp qword[_temp]
			movsd xmm0, [_temp]
		\}
		match =float, size_type\{
			fstp dword[_temp]
			movss xmm0, [_temp]
		\}
		restore _temp
	else if val eqtype [0] | val eqtype byte [0] | val in <pax, pcx, pdx, pbx, psp, pbp, psi, pdi> | val in <xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7> | val eqtype 0f
		match =double, size_type\{
			if val eqtype 0f
				mov rax, val
				movsd xmm0, rax
			else
				movsd xmm0, val
			end if
		\}
		match =float, size_type\{
			if val eqtype 0f
				mov eax, val
				movss xmm0, eax
			else
				movss xmm0, val
			end if
		\}
		match =nat32, size_type\{
			if val eqtype 0f
				mov eax, val
				cvtsi2sd xmm0, eax
			else
				cvtsi2sd xmm0, val
			end if
		\}
		match =nat64, size_type\{
			if val eqtype 0f
				mov rax, val
				cvtsi2sd xmm0, rax
			else
				cvtsi2sd xmm0, val
			end if
		\}
	end if
}

macro @qcvtretv {
	mov rdx, rax
	ror rdx, 32
}