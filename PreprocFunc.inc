virtual at 0
	..numeric_buff::
	rb 64
end virtual

macro %display_num num, radix{
	local i, _num, digit
	_num = num
	i = 0
	while _num
		digit = _num mod radix
		if digit > 9
			digit = digit - 10 + "A"
		else
			digit = digit + "0"
		end if
		store byte digit at ..numeric_buff:i
		_num = _num / radix
		i = i + 1
	end while
	if ~num
		store byte "0" at ..numeric_buff:0
		i = 1
	end if
	repeat i
		load digit byte from ..numeric_buff:i-%
		display digit
	end repeat
}

macro %len_asciiz return, glob_lab, offset, max_len{
	local smb, i
	smb = 1
	i = 0
	while smb <> 0 & i <> max_len + 1
		load smb byte from glob_lab:offset + i
		i = i + 1
	end while
	return = i - 1
}

macro %display_ascii glob_lab, offset, len{
	local smb
	repeat len
		load smb byte from glob_lab:offset + % - 1
		display smb
	end repeat
}

macro %ascii_to_dec return, glob_lab, offset, max_len{
	local smb, i, num
	smb = 1
	i = 0
	num = 0
	repeat max_len
		load smb byte from glob_lab:offset + i
		if smb = 0
			break
		end if
		num = num * 10 + (smb - 0x30)
		i = i + 1
	end repeat
	return = num
}

macro %eq_ascii return, glob_lab, offset1, len1, offset2, len2{
	local smb1, smb2
	repeat 1
		if len1 <> len2
			return = 0
			break
		end if
		repeat len1
			load smb1 byte from glob_lab:offset1 + % - 1 
			load smb2 byte from glob_lab:offset2 + % - 1 
			if smb1 <> smb2
				return = 0
				break
			end if
		end repeat
		if smb1 = smb2
			return = 1
		end if
	end repeat
}