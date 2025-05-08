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
	purge defargs@proc
	purge endp
}

match =x86, __architecture{
	include "win32ax.inc"
	purge stdcall, stdcall
	purge ccall, ccall
	purge defargs@proc
	purge endp
}

define INT_MIN 	80000000h
define INT_MAX 	7FFFFFFFh
define UINT_MAX 0FFFFFFFFh

define INT64_MIN 	8000000000000000h
define INT64_MAX 	7FFFFFFFFFFFFFFFh
define UINT64_MAX 	0FFFFFFFFFFFFFFFFh

; va@list equ 
; va_list@counter equ
; va_list@count equ


macro deflocal@proc name,def,[val] { 
	common
	match vars, all@vars \{ 
		all@vars equ all@vars, 
	\}
	all@vars equ all@vars name
   	forward
	local ..var,..tmp
	..var def val
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
		tmp: end virtual
		initlocal@proc ..var,def value
		virtual at tmp
	\}
   	common
	match first rest, ..var, \{ 
		name equ first 
	\} 
}


macro define@proc name,statement{ 
	local params,flag,regs,parmbytes,localbytes,current, va_list, va_list_counter, va_list_count
   	
   	if used name
   	va_list_counter = 0
   	va_list@counter equ va_list_counter
   	va_list@count equ va_list_count
	
	name:
	match =stdcall args, statement \{ params equ args
	             flag = 11b \}
	match =stdcall, statement \{ params equ
	        flag = 11b \}
	match =c args, statement \{ params equ args
	            flag = 10001b \}
	match =c, statement \{ params equ
	       flag = 10001b \}
	match =params, params \{ params equ statement
	         flag = 0 \}
	match =uses reglist=,args, params \{ regs equ reglist
	           params equ args \}
	match =regs =uses reglist, regs params \{ regs equ reglist
	                params equ \}
	match =regs, regs \{ regs equ \}
	match prologue:reglist, prologue@proc:<regs> \{ prologue name,flag,parmbytes,localbytes,reglist \}
	virtual at parmbase@proc
	match =,args, params \{ defargs@proc args \}
	match =args@proc args, args@proc params \{ defargs@proc args \}
	parmbytes = $-(parmbase@proc)
	end virtual
	name # % = parmbytes/pointer.size
	all@vars equ
	current = 0
	macro locals \{ 
		virtual at localbase@proc+current
	  	macro label def \\{ 
	  		match . type,def> \\\{
	  			deflocal@proc .,label,<type 
	  		\\\} 
	  	\\}
		struc db [val] \\{
			\\common 
			deflocal@proc .,db,val
		\\}
		struc du [val] \\{ 
			\\common 
			deflocal@proc .,du,val
		\\}
		struc dw [val] \\{ 
			\\common 
			deflocal@proc .,dw,val 
		\\}
		struc dp [val] \\{ 
			\\common 
			deflocal@proc .,dp,val
		\\}
		struc dd [val] \\{ 
			\\common 
			deflocal@proc .,dd,val
		\\}
		struc dt [val] \\{ 
			\\common 
			deflocal@proc .,dt,val 
		\\}
		struc dq [val] \\{ 
			\\common 
			deflocal@proc .,dq,val
		\\}
		struc rb cnt \\{ 
			\\common 
			deflocal@proc .,db,(cnt) dup(?) 
		\\}
		struc rw cnt \\{ 
			\\common 
			deflocal@proc .,dw,(cnt) dup(?) 
		\\}
		struc rp cnt \\{ 
			\\common 
			deflocal@proc .,dp,(cnt) dup(?)  
		\\}
		struc rd cnt \\{ 
			\\common 
			deflocal@proc .,dd,(cnt) dup(?)  
		\\}
		struc rt cnt \\{ 
			\\common 
			deflocal@proc .,dt,(cnt) dup(?)  
		\\}
		struc rq cnt \\{ 
			\\common 
			deflocal@proc .,dq,(cnt) dup(?) 
		\\} 
		macro align num\\{
			\\local ..tmp
			if num > 16
				display "cannot align more than 16 in locals"
				err
			end if
			..tmp:
			if ..tmp relativeto pbp & (num - (..tmp - pbp) mod num) <> 0
				db (num - (..tmp - pbp) mod num) dup(?)
			else if ..tmp relativeto psp & (num - (..tmp - psp) mod num) <> 0
				db (num - (..tmp - psp) mod num) dup(?)
			else 
				display "locals base must be sp or bp"
				err
			end if
		\\}
	\}
	macro endl \{ 
		purge label, align
	  	restruc db,du,dw,dp,dd,dt,dq
	  	restruc rb,rw,rp,rd,rt,rq
	  	current = $-(localbase@proc)
	  	end virtual 
	\}
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
	macro finish@proc\{ 
		localbytes = current
	  	match close:reglist, close@proc:<regs> \\{ 
	  		close name,flag,parmbytes,localbytes,reglist 
	  	\\}
		end if 
	\} 
	locals 
		va_list rb va_list_count
	endl
	va@list equ va_list
}

macro endp {
	va_list@count = va_list@counter
	restore va@list, va_list@counter, va_list@count
	endp
}

include_once "TOOLS\FASM_OOP\FASM_OOP.inc"
include_once "TOOLS\importlib.inc"
include_once "TOOLS\TLS.inc"

include "cstr\cstr.inc"

macro @arch_include fname, ext=".asm"{
	match _arch, __architecture\{
		match _str, fname \# \`_arch \# ext\\{
			include _str
		\\}
	\}
}
include "TOOLS\CNV\CNV.inc"