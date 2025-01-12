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

macro cstr [args]{
	common
	local lendata, bytenow, smb, num
	forward
	local ..base, ..unformatData, ..start, ..end, lendata, bytenow, smb, num
	; define no@local
	virtual at 0
		..unformatData::
		..start:
		db args
		..end:
		lendata = ..end - ..start
	end virtual
	; purge no@local
	bytenow = 0
	while bytenow < lendata
		load smb byte from ..unformatData:bytenow
		bytenow = bytenow + 1
		if smb = '\'
			load smb byte from ..unformatData:bytenow
			bytenow = bytenow + 1
			if smb = 'a'
				smb = 07h
			else if smb = 'b'
				smb = 08h
			else if smb = 'e'
				smb = 1Bh
			else if smb = 'f'
				smb = 0Ch
			else if smb = 'n'
				smb = 0Ah
			else if smb = 'r'
				smb = 0Dh
			else if smb = 't'
				smb = 09h
			else if smb = 'e'
				smb = 1Bh
			else if smb = 'v'
				smb = 0Bh
			else if smb = '\' | smb = '?' | smb = '"' | smb = "'"
				;smb = smb
			else if smb = 'x'
				load smb byte from ..unformatData:bytenow
				bytenow = bytenow + 1
				if smb >= 'a' & smb <= 'f'
					smb = (smb and 0DFh)
				end if
				smb = smb - 0x30
				if smb > 09h
					smb = smb - 7
				end if
				num = smb
				if bytenow < lendata
					load smb byte from ..unformatData:bytenow
					if (smb >= '0' & smb <='9') | smb >= 'A' & smb <= 'F' | smb >= 'a' & smb <= 'f'
						if smb >= 'a' & smb <= 'f'
							smb = (smb and 0DFh)
						end if
						smb = smb - 30h
						if smb > 09h
							smb = smb - 7
						end if
						num = num shl 4 + smb
						bytenow = bytenow + 1
					end if
				end if
				smb = num
			else if smb < '8'
				num = smb - 30h
				if bytenow < lendata
					load smb byte from ..unformatData:bytenow
					if smb < '8'
						bytenow = bytenow + 1
						num = num shl 3 + (smb - 30h)
						if bytenow < lendata
							load smb byte from ..unformatData:bytenow
							if smb < '8' & num < 255
								bytenow = bytenow + 1
								num = num shl 3 + (smb - 30h)
							end if
						end if
					end if
				end if
				smb = num
			else
				display "Unknown sequence"
				err
			end if
		end if
		db smb
	end while
	common
	db 0
}

struc cstr args&{
	local ..bytes, ..start, ..end, lendata
	virtual at 0
		..bytes::
		..start:
		cstr args
		..end:
		lendata = ..end - ..start
	end virtual
	. db lendata dup(?)
	init@cstr ., lendata, ..bytes
	.len = lendata
}

macro init@cstr name, lendata, bytes{
	local numbyte, nowval
	numbyte = 0
	if name relativeto 0
		while numbyte < lendata
			load nowval from bytes:numbyte
			store byte nowval at name + numbyte
			numbyte = numbyte + 1
		end while
	else
		local ..pausevirt
		..pausevirt: end virtual
		while numbyte < (lendata - lendata mod 4)
			load nowval dword from bytes:numbyte
			mov dword[name + numbyte], nowval
			numbyte = numbyte + 4
		end while
		if lendata mod 4 <> 0
			load nowval word from bytes:numbyte
			mov word[name + numbyte], nowval
			numbyte = numbyte + 2
		end if
		if lendata mod 2 <> 0
			load nowval byte from bytes:numbyte
			mov byte[name + numbyte], nowval
			numbyte = numbyte + 1
		end if
		virtual at ..pausevirt
	end if
}

macro cwstr [args]{
	common
	local lendata, bytenow, smb, num, hex_count
	forward
	local ..unformatData, ..start, ..end
	virtual at 0
		..unformatData::
		..start:
		du args
		..end:
		lendata = ..end - ..start
	end virtual
	wordnow = 0
	while wordnow < lendata
		load smb word from ..unformatData:wordnow
		wordnow = wordnow + 2
		if smb = '\'
			load smb word from ..unformatData:wordnow
			wordnow = wordnow + 2
			if smb = 'a'
				dw 07h
			else if smb = 'b'
				dw 08h
			else if smb = 'e'
				dw 1Bh
			else if smb = 'f'
				dw 0Ch
			else if smb = 'n'
				dw 0Ah
			else if smb = 'r'
				dw 0Dh
			else if smb = 't'
				dw 09h
			else if smb = 'e'
				dw 1Bh
			else if smb = 'v'
				dw 0Bh
			else if smb = '\' | smb = '?' | smb = '"' | smb = "'"
				dw smb
			else if smb = 'x'
				load smb word from ..unformatData:wordnow
				num = 0
				hex_count = 0
				while (wordnow < lendata) & (hex_count < 4)
					load smb word from ..unformatData:wordnow
					if (smb >= '0' & smb <='9') | smb >= 'A' & smb <= 'F' | smb >= 'a' & smb <= 'f'
						if smb >= 'a' & smb <= 'f'
							smb = (smb and 0DFh)
						end if
						smb = smb - 30h
						if smb > 09h
							smb = smb - 7
						end if
						num = num shl 4 + smb
						wordnow = wordnow + 2
					else
						break
					end if
					hex_count = hex_count + 1
				end while
				dw num
			else if smb < '8'
				num = smb - 30h
				if wordnow < lendata
					load smb word from ..unformatData:wordnow
					if smb < '8'
						wordnow = wordnow + 2
						num = num shl 3 + (smb - 30h)
						if wordnow < lendata
							load smb word from ..unformatData:wordnow
							if smb < '8' & num < 255
								wordnow = wordnow + 2
								num = num shl 3 + (smb - 30h)
							end if
						end if
					end if
				end if
				dw num
			else
				display "Unknown sequence"
				err
			end if
		else
			dw smb
		end if
	end while
	common
	dw 0
}

struc cwstr [args]{
	local ..bytes, ..start, ..end, lendata
	virtual at 0
		..bytes::
		..start:
		cwstr args
		..end:
		lendata = ..end - ..start
	end virtual
	. db lendata dup(?)
	init@cstr ., lendata, ..bytes
	.len = lendata
}

macro @arch_include fname, ext=".asm"{
	match _arch, __architecture\{
		match _str, fname \# \`_arch \# ext\\{
			include _str
		\\}
	\}
}

include "TOOLS\CNV\CNV.inc"