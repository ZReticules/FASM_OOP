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