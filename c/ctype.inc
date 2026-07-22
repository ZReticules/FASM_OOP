@module cctype

; ASCII char classification table - binary compatible */
define _UPPER   	0x0001  ; C1_UPPER */
define _LOWER   	0x0002  ; C1_LOWER */
define _DIGIT   	0x0004  ; C1_DIGIT */
define _SPACE   	0x0008  ; C1_SPACE */
define _PUNCT   	0x0010  ; C1_PUNCT */
define _CONTROL 	0x0020  ; C1_CNTRL */
define _BLANK   	0x0040  ; C1_BLANK */
define _HEX     	0x0080  ; C1_XDIGIT */
define _LEADBYTE	0x8000
define _ALPHA       (0x0100 or _UPPER or _LOWER)  ; (C1_ALPHA|_UPPER|_LOWER) */

define WEOF 0xFFFF

match =locale_t@defined, locale_t@defined\{
	typedef locale_t, pointer
\}

importlib msvcrt,\
	mb_cur_max = '___mb_cur_max_func',\ ; CHANGED(const MB_CUR_MAX)
	isascii="__isascii",\
	iscsym="__iscsym",\
	iscsymf="__iscsymf",\
	toascii="__toascii",\
	_isblank_l,\
	_isctype,\
	_isctype_l,\
	_islower_l,\
	_isupper_l,\
	_isdigit_l,\
	_isxdigit_l,\
	_tolower,\
	_tolower_l,\
	_toupper,\
	_toupper_l,\
	isalnum,\
	isalpha,\
	isblank,\
	iscntrl,\
	isdigit,\
	isgraph,\
	islower,\
	isprint,\
	_isprint_l,\
	ispunct,\
	isspace,\
	_isspace_l,\
	isupper,\
	isxdigit,\
	tolower,\
	toupper

importlib msvcrt,\
	_isleadbyte_l,\
	_iswalnum_l,\
	_iswalpha_l,\
	_iswblank_l,\
	_iswcntrl_l,\
	_iswctype_l,\
	_iswdigit_l,\
	_iswgraph_l,\
	_iswlower_l,\
	_iswprint_l,\
	_iswpunct_l,\
	_iswspace_l,\
	_iswupper_l,\
	_iswxdigit_l,\
	_towlower_l,\
	_towupper_l,\
	is_wctype,\
	isleadbyte,\
	iswalnum,\
	iswalpha,\
	iswascii,\
	iswblank,\
	iswcntrl,\
	iswctype,\
	iswdigit,\
	iswgraph,\
	iswlower,\
	iswprint,\
	iswpunct,\
	iswspace,\
	iswupper,\
	iswxdigit,\
	towlower,\
	towupper

@endm
