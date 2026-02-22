arg@nums equ 
;current@buf
macro defargs@proc [arg] { 
	common
	if ~ arg eq
		local _counter, _numarg
		_counter equ 0
		forward
		_counter equ _counter + 1
		local ..arg,current@arg,current@numarg
		rept 1 _numarg:_counter\{
			; display \\`_counter
			; match any, arg@nums\\{
			; 	arg@nums equ arg@nums,
			; \\}
			current@numarg equ @arg\#_numarg, @float_arg\#_numarg
			@arg\#_numarg equ [..arg]
			@farg\#_numarg equ [..arg]
		\}
		match argname:type, arg \{ 
			current@arg equ argname
			argname equ ..arg
			if [type] in <[qqword], [dqword], [tbyte], [qword], [pword], [dword], [word], [byte]>
				label ..arg type
				if qqword eq type
					dd ?,?,?,?,?,?,?,?
				else if dqword eq type
					dd ?,?,?,?
				else if tbyte eq type
					dd ?,?,?
				else if qword eq type | pword eq type
					dd ?,?
				else
					dd ?
				end if 
			else
				argname type
			end if
		\}
		match =current@arg,current@arg\{ 
		 	current@arg equ arg
			arg equ ..arg
			..arg dd ? 
		\}
		common
		arg@nums equ current@numarg
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
	match all, arg@nums\{
		restore all
	\}
	restore arg@nums
}

rept 1{
	local _mp1
	macro @sarg [arg]\{
		match _mp1, arg\#mem\\{
			if arg in < eax ebx ecx edx esi edi esp ebp >
				mov dword _mp1, arg
			else if arg in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7>
				movq qword _mp1, arg
			end if
		\\}
	\}
}

macro @larg [dest, arg]{
	if arg in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7> | dest in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7>
		if arg eq arg\#mem
			movq dest, qword arg
		else if ~dest eq arg
			movq dest, arg
		end if
	else if arg in < eax ebx ecx edx esi edi esp ebp > | dest in < eax ebx ecx edx esi edi esp ebp >
		if arg eq arg\#mem
			mov dest, dword arg
		else if ~dest eq arg
			mov dest, arg
		end if
	end if
} 

; macro @larg [dest, argmem]{
; 	local origin, opcode
; 	if dest in <xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7>
; 		virtual
; 			use64
; 			origin = $	
			
; 			inc argmem
			
; 			load opcode byte from origin
			
; 			if opcode = 67h | opcode = 41h
; 				load opcode byte from origin+1
; 			end if
; 			use32
; 		end virtual
; 		if opcode and 0F8h = 48h
; 			movq dest, argmem
; 		else if opcode = 66h
; 			movd dest, argmem
; 		else
; 			display "invalid operand size"
; 			err
; 		end if
; 	else
; 		if argmem eqtype qword [0]
; 			movq xmm0, argmem
; 			movq dest, xmm0
; 		else
; 			if dest eqtype eax
; 				mov dest, argmem
; 			else
; 				mov eax, argmem
; 				mov dest, eax
; 			end if
; 		end if
; 	end if
; }

;USE_LOCAL
;USE_ESP
macro @fcvtretv dest_type*, size_type*, flag=USE_LOCAL{
	match =SSE, dest_type\{
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
			fstp qword[_temp]
			movq xmm0, [_temp]
		\\}
		match =float, size_type\\{
			fstp dword[_temp]
			movd xmm0, dword[_temp]
		\\}
		\restore _temp
	\}
}

;USE_LOCAL
;USE_ESP
macro @fstdretv val*, size_type*, flag=USE_LOCAL{
	if val in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7> | val eqtype 0f | val in <eax, ecx, edx, ebx, esp, ebp, esi, edi>
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
			if ~val in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
				local ..val, ldword, hdword
				virtual at $
					..val dq val
					load ldword dword from ..val
					load hdword dword from ..val+4
				end virtual
				mov dword[_temp], ldword
				mov dword[_temp + 4], hdword
			else
				movq [_temp], val
			end if
			fld qword[_temp]
		\}
		match =float, size_type\{
			if ~val in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
				mov dword[_temp], val
			else
				movd dword[_temp], val
			end if
			fld dword[_temp]
		\}
		match =nat32, size_type\{
			if ~val in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
				mov dword[_temp], val
			else
				movd dword[_temp], val
			end if
			fild [_temp]
		\}
		match =nat64, size_type\{
			if val in <xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7>
				movq [_temp], val
			else if val eqtype 0 | val eqtype 0f
				local ..val, ldword, hdword
				virtual at $
					..val dq val
					load ldword dword from ..val
					load hdword dword from ..val+4
				end virtual
				mov dword[_temp], ldword
				mov dword[_temp + 4], hdword
			else 
				local ..pos
				mov dword[_temp+4], 0
				cmp val, 0
				jg ..pos
					mov dword[_temp+4], -1
				..pos:
				mov dword[_temp], val
			end if
			fild [_temp]
		\}
		restore _temp
	else if val eqtype [0] | val eqtype byte [0]
		fld val
	end if
}

macro @qcvtretv {}