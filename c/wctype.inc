@module cwctype

; ASCII char classification table - binary compatible */
define _UPPER        0x0001  ; C1_UPPER */
define _LOWER        0x0002  ; C1_LOWER */
define _DIGIT        0x0004  ; C1_DIGIT */
define _SPACE        0x0008  ; C1_SPACE */
define _PUNCT        0x0010  ; C1_PUNCT */
define _CONTROL      0x0020  ; C1_CNTRL */
define _BLANK        0x0040  ; C1_BLANK */
define _HEX          0x0080  ; C1_XDIGIT */
define _LEADBYTE     0x8000
define _ALPHA       (0x0100 or _UPPER or _LOWER)  ; (C1_ALPHA|_UPPER|_LOWER) */

define WCHAR_MIN 0
define WCHAR_MAX 0xffff

define WEOF 0xFFFF

match =wchar_t@defined, wchar_t@defined\{
	typedef wchar_t, word
\}
match =wint_t@defined, wint_t@defined\{
	typedef wint_t, word
\}
typedef wctype_t, word
typedef wctrans_t, wchar_t

importlib msvcrt,\
	is_wctype,\
	isleadbyte,\
	iswalnum,\
	iswalpha,\
	iswascii,\
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
	towupper,\
	towctrans,\
	wctrans,\
	wctype

@endm
