@module cwchar

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

match =wchar_t@defined, wchar_t@defined\{
	typedef wchar_t, word
\}
match =wint_t@defined, wint_t@defined\{
	typedef wint_t, word
\}

typedef mbstate_t, dword

importlib msvcrt,\
	_wsetlocale,\
	btowc,\
	mbrlen,\
	mbrtowc,\
	mbsrtowcs,\
	wcrtomb,\
	wcrtomb_s,\
	wcsrtombs,\
	wctob,\
	wmemcpy_s,\
	wmemmove_s

local wmemchr_, _wmemchr_
if used wmemchr
	_wmemchr_ @const dptr wmemchr_
	virtual at _wmemchr_
		wmemchr rptr 1
	end virtual
end if
.proc cdecl wmemchr_(.s:P_wchar_t, .c:wchar_t, .n:size_t) uses pdi
	@larg pdi, @arg1, pax, @arg2, pcx, @arg3
	repne scasw
	mov eax, 0
	cmove pax, psi
	ret
.endp
; static inline _CONST_RETURN wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n)
; {
;     const wchar_t *end;
;     for (end = s + n; s < end; s++)
;         if (*s == c) return (_CONST_RETURN wchar_t *)s;
;     return NULL;
; }

local wmemcmp_, _wmemcmp_
if used wmemcmp
	_wmemcmp_ @const dptr wmemcmp_
	virtual at _wmemcmp_
		wmemcmp rptr 1
	end virtual
end if
.proc cdecl wmemcmp_(.s1:P_wchar_t, .s2:P_wchar_t, .n:size_t) uses pdi psi
	@larg psi, @arg1, pdi, @arg2, pcx, @arg3
	xor pax, pax
	repne cmpsb
	mov edx, 1
	mov pcx, -1
	cmovg eax, edx
	cmovl pax, pcx
	.return: ret
.endp
; static inline int wmemcmp(const wchar_t *s1, const wchar_t *s2, size_t n)
; {
;     size_t i;
;     for (i = 0; i < n; i++)
;     {
;         if (s1[i] > s2[i]) return 1;
;         if (s1[i] < s2[i]) return -1;
;     }
;     return 0;
; }

local wmemcpy_, _wmemcpy_
if used wmemcpy
	_wmemcpy_ @const dptr wmemcpy_
	virtual at _wmemcpy_
		wmemcpy rptr 1
	end virtual
end if
.proc cdecl wmemcpy_(.dst:P_wchar_t, .src:P_wchar_t, .n:size_t) uses pdi psi
	shl 1, @arg3
	jmp [memcpy]
.endp
; static inline wchar_t* __cdecl wmemcpy(wchar_t *dst, const wchar_t *src, size_t n)
; {
;     return (wchar_t*)memcpy(dst, src, n * sizeof(wchar_t));
; }

local wmemmove_, _wmemmove_
if used wmemmove
	_wmemmove_ @const dptr wmemmove_
	virtual at _wmemmove_
		wmemmove rptr 1
	end virtual
end if
.proc cdecl wmemmove_(.dst:P_wchar_t, .src:P_wchar_t, .n:size_t) uses pdi psi
	shl 1, @arg3
	jmp [memmove]
.endp
; static inline wchar_t* __cdecl wmemmove(wchar_t *dst, const wchar_t *src, size_t n)
; {
;     return (wchar_t*)memmove(dst, src, n * sizeof(wchar_t));
; }

local wmemset_, _wmemset_
if used wmemset
	_wmemset_ @const dptr wmemset_
	virtual at _wmemset_
		wmemset rptr 1
	end virtual
end if
.proc cdecl wmemset_(.s1:P_wchar_t, .s2:P_wchar_t, .n:size_t) uses pdi
	@larg pdi, @arg1, pax, @arg2, pcx, @arg3
	rep stosw
	ret
.endp
; static inline wchar_t* __cdecl wmemset(wchar_t *s, wchar_t c, size_t n)
; {
;     size_t i;
;     for (i = 0; i < n; i++)
;         s[i] = c;
;     return s;
; }

local fwide_, _fwide_
if used fwide
	_fwide_ @const dptr fwide_
	virtual at _fwide_
		fwide rptr 1
	end virtual
end if
.proc cdecl fwide_(.fp:POINTER, .mode:DWORD)
	@larg pax, @arg2
	ret
.endp
; static inline int __cdecl fwide(FILE *file, int mode)
; {
;     return mode;
; }

local mbsinit_, _mbsinit_
if used mbsinit
	_mbsinit_ @const dptr mbsinit_
	virtual at _mbsinit_
		mbsinit rptr 1
	end virtual
end if
.proc cdecl mbsinit_(.state:P_mbstate_t)
	xor eax, eax
	@larg pcx, @arg1
		jecxz .return
	mov ecx, [pcx]
		jecxz .return
	xor eax, eax
	.return: ret
.endp
; static inline int __cdecl mbsinit(const mbstate_t *state)
; {
;     return !state || !*state;
; }

@endm
