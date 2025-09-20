include "include_once.inc"

match =x64, __architecture{
	include "win64ax.inc"
	macro ccall args&\{
		@oopcall args
	\}
	macro fastcall args&\{
		@oopcall args
	\}
	macro thiscall args&\{
		@oopcall args
	\}
}

match =x86, __architecture{
	include "win32ax.inc"
	purge stdcall, stdcall
	purge ccall, ccall
}
purge defargs@proc
purge endp
purge static_rsp_close

define INT_MIN 	80000000h
define INT_MAX 	7FFFFFFFh
define UINT_MAX 0FFFFFFFFh

define INT64_MIN 	8000000000000000h
define INT64_MAX 	7FFFFFFFFFFFFFFFh
define UINT64_MAX 	0FFFFFFFFFFFFFFFFh

; va@list equ 
; va_list@counter equ
; va_list@count equ

purge deflocal@proc
macro deflocal@proc name, def, val& { 
	; common
	; match vars, all@vars \{ 
	; 	all@vars equ all@vars, 
	; \}
	local all_vars
	match all_vars, all@vars\{
		all_vars equ name
	\}
	; match any, all@vars\{
	; 	display \`any
	; \}
	; display `name, 0Ah
   	; forward
	local ..var,..tmp, first, rest, tmp, value, any
	; display `..var, 0Ah
	
	..var def val
	; match all,all@vars \{
	; 	; \reverse 
	; 	display \`all, 0Ah
	; 	; restore all
	; 	; restore all@vars 
	; \}
	match =?, val \{ 
		..tmp equ 
	\}
	match , val \{ 
		..tmp equ 
	\}
	match any =?, val \{ 
		..tmp equ 
	\}
	match any (=?), val \{ 
		..tmp equ 
	\}
	match =label, def \{ 
		..tmp equ 
	\}
	match tmp : value, ..tmp : val \{ 
		end virtual
		initlocal@proc ..var,def value
		virtual proc@locals
	\}
   	; common
	match first rest, ..var, \{ 
		; display `..var, 0Ah
		name equ first 
	\} 
}

proc@name equ 
macro make@proc_label name{
	local father_name
	match father_name, proc@name \{
		\local full_name

		restore regs@proc
		match all,args@proc \\{ restore all \\}
		restore args@proc
		local all_vars
		match all_vars, all@vars\\{
			irpv all, all_vars \\\{
				\\\common 
				\\\restore all
				\\\reverse 
				\\\restore all@vars 
			\\\}
		\\}
		restore all@vars

		match full_name, father_name\#name \\{
   			if used full_name
			
			full_name:
			proc@name equ full_name
		\\}
	rept 0\{\} rept 1 \{

   		if used name

		proc@name equ name
		name:
	\}
}

close@proc equ closedef

macro closedef procname,flag,parmbytes,localbytes,reglist {
	restore localbase@proc, parmbase@proc
}

macro finish@proc{ 
	; match ..locals, proc@locals\\{
		; display \`..locals
	match localbytes, proc@localbytes\{
		virtual proc@locals
			localbytes = $-(localbase@proc)
		end virtual
	  	match close:reglist, close@proc:<proc@regs> \\{ 
	  		close name,flag,parmbytes,localbytes,reglist 
	  	\\}
	\}
	; \\}
	end if 
} 

macro locals { 
	virtual proc@locals
  	macro label def \{ 
  		match . type,def> \\{
  			deflocal@proc .,label,<type 
  		\\} 
  	\}
	struc db [val] \{
		\common 
		deflocal@proc .,db,val
	\}
	struc du [val] \{ 
		\common 
		deflocal@proc .,du,val
	\}
	struc dw [val] \{ 
		\common 
		deflocal@proc .,dw,val 
	\}
	struc dp [val] \{ 
		\common 
		deflocal@proc .,dp,val
	\}
	struc dd [val] \{ 
		\common 
		; end virtual
		; virtual proc@locals
		deflocal@proc .,dd,val
	\}
	struc dt [val] \{ 
		\common 
		deflocal@proc .,dt,val 
	\}
	struc dq [val] \{ 
		\common 
		deflocal@proc .,dq,val
	\}
	struc rb cnt \{ 
		\common 
		deflocal@proc .,db,(cnt) dup(?) 
	\}
	struc rw cnt \{ 
		\common 
		deflocal@proc .,dw,(cnt) dup(?) 
	\}
	struc rp cnt \{ 
		\common 
		deflocal@proc .,dp,(cnt) dup(?)  
	\}
	struc rd cnt \{ 
		\common 
		deflocal@proc .,dd,(cnt) dup(?)  
	\}
	struc rt cnt \{ 
		\common 
		deflocal@proc .,dt,(cnt) dup(?)  
	\}
	struc rq cnt \{ 
		\common 
		deflocal@proc .,dq,(cnt) dup(?) 
	\} 
	; macro align num\{
	; 	\local ..tmp
	; 	if num > 16
	; 		display "cannot align more than 16 in locals"
	; 		err
	; 	end if
	; 	..tmp:
	; 	if ..tmp relativeto pbp & (num - (..tmp - pbp) mod num) <> 0
	; 		db (num - (..tmp - pbp) mod num) dup(?)
	; 	else if ..tmp relativeto psp & (num - (..tmp - psp) mod num) <> 0
	; 		db (num - (..tmp - psp) mod num) dup(?)
	; 	else 
	; 		display "locals base must be sp or bp"
	; 		err
	; 	end if
	; \}
}
macro endl { 
	purge label, align
  	restruc db,du,dw,dp,dd,dt,dq
  	restruc rb,rw,rp,rd,rt,rq
  	; current = $-(localbase@proc)
  	end virtual 
}
macro define@proc name,statement{ 
	local params,flag,regs,parmbytes,localbytes,current, va_list, va_list_counter, va_list_count
   	local args, reglist, prologue

	proc@regs equ regs
	proc@localbytes equ localbytes
	make@proc_label name

   	va_list_counter = 0
   	va_list@counter equ va_list_counter
   	va_list@count equ va_list_count

   	local all_vars
   	all@vars equ all_vars
	match =stdcall args, statement \{ 
		params equ args
		flag = 11b 
	\}
	match =stdcall, statement \{ 
		params equ
	    flag = 11b 
    \}
	match =c args, statement \{ 
		params equ args
	    flag = 10001b 
    \}
	match =c, statement \{ 
		params equ
	    flag = 10001b 
    \}
	match =params, params \{ 
		params equ statement
	    flag = 0 
    \}
	match =uses reglist=,args, params \{ 
		regs equ reglist
	    params equ args 
    \}
	match =regs =uses reglist, regs params \{ 
		regs equ reglist
	    params equ 
    \}
	match =regs, regs \{ regs equ \}
	match prologue:reglist, prologue@proc:<regs> \{ prologue name,flag,parmbytes,localbytes,reglist \}
	virtual at parmbase@proc
	match =,args, params \{ defargs@proc args \}
	match =args@proc args, args@proc params \{ defargs@proc args \}
	parmbytes = $-(parmbase@proc)
	end virtual
	name # % = parmbytes/pointer.size
	; all@vars equ
	current = 0
	local ..locals
	proc@locals equ ..locals
	virtual at localbase@proc
		..locals::
	end virtual

	macro ret operand\{ 
		match any, operand \\{ 
			retn operand 
		\\}
	  	match , operand \\{ 
	  		match epilogue:reglist, epilogue@proc:<regs> \\\{ 
	  			epilogue name,flag,parmbytes,localbytes,reglist 
	  		\\\}
  		\\} 
  	\}
	locals 
		va_list rb va_list_count
	endl
	; match all,all@vars \{
	; 	; \reverse 
	; 	display \`all, 0Ah
	; 	; restore all
	; 	; restore all@vars 
	; \}
	va@list equ va_list
	; macro proc args& \{
	; 	\local name, any
	; 	match name any, args [::]\\{
	; 		\\local ..name
	; 		name equ ..name
	; 		proc args
	; 		macro endp \\\{
	; 			purge endp
	; 			restore name
	; 		\\\}
	; 	\\}
	; \}
}

macro endp {
	va_list@count = va_list@counter
	restore va@list, va_list@counter, va_list@count
 	purge ret
 	; purge locals,endl
	finish@proc
	restore proc@locals, proc@regs, proc@localbytes
	; match name, proc@name\{
	; 	display \`name, 0Ah
	; \}
	restore proc@name
	; match name, proc@name\{
	; 	display \`name, 0Ah
	; \}
	; purge finish@proc
	restore regs@proc
	match all,args@proc \{ restore all \}
	restore args@proc
	local all_vars
	match all_vars, all@vars\{
		irpv all, all_vars \\{
			\\common 
			\\restore all
			\\reverse 
			\\restore all@vars 
		\\}
	\}
	restore all@vars
}

include "FASM_OOP\FASM_OOP.inc"
include "importlib.inc"
include "TLS.inc"

include "cstr\cstr.inc"

macro @arch_include fname, ext=".asm"{
	match _arch, __architecture\{
		match _str, fname \# \`_arch \# ext\\{
			include _str
		\\}
	\}
}
include "CNV\CNV.inc"