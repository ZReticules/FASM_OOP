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

macro define@proc name,statement{ 
	local params,flag,regs,parmbytes,localbytes,current
   	if used name
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
			\common deflocal@proc .,db,val
		\\}
		struc du [val] \\{ 
			\common deflocal@proc .,du,val
		\\}
		struc dw [val] \\{ 
			\common deflocal@proc .,dw,val 
		\\}
		struc dp [val] \\{ 
			\common deflocal@proc .,dp,val
		\\}
		struc dd [val] \\{ 
			\common deflocal@proc .,dd,val
		\\}
		struc dt [val] \\{ 
			\common deflocal@proc .,dt,val 
		\\}
		struc dq [val] \\{ 
			\common deflocal@proc .,dq,val
		\\}
		struc rb cnt \\{ 
			\common deflocal@proc .,rb,cnt 
		\\}
		struc rw cnt \\{ 
			\common deflocal@proc .,rw,cnt  
		\\}
		struc rp cnt \\{ 
			\common deflocal@proc .,rp,cnt  
		\\}
		struc rd cnt \\{ 
			\common deflocal@proc .,rd,cnt  
		\\}
		struc rt cnt \\{ 
			\common deflocal@proc .,rt,cnt  
		\\}
		struc rq cnt \\{ 
			\common deflocal@proc .,rq,cnt 
		\\} 
	\}
	macro endl \{ 
		purge label
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
	macro finish@proc\{ localbytes = current
	  	match close:reglist, close@proc:<regs> \\{ 
	  		close name,flag,parmbytes,localbytes,reglist 
	  	\\}
		end if 
	\} 
}

include_once "TOOLS\FASM_OOP\FASM_OOP.inc"
include_once "TOOLS\TLS.inc"
include_once "TOOLS\importlib.inc"

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

@endm fix }

macro @module name{
	match =name#_included, name#_included{
		define name#_included
}

include "cstr\cstr.inc"

macro @arch_include fname, ext=".asm"{
	match _arch, __architecture\{
		match _str, fname \# \`_arch \# ext\\{
			include _str
		\\}
	\}
}

include "TOOLS\CNV\CNV.inc"