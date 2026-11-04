@module crtdefs

typedef __int64, qword

_ARGMAX = 100

match =__msvcrt_long@defined, __msvcrt_long@defined\{
	typedef __msvcrt_long, dword
	typedef __msvcrt_ulong, dword
\}

match =__time32_t@defined, __time32_t@defined\{
	typedef __time32_t, __msvcrt_long
\}

match =__time64_t@defined, __time64_t@defined\{
	typedef __time64_t, qword
\}

match =time_t@defined, time_t@defined\{
	match =_USE_32BIT_TIME_T, _USE_32BIT_TIME_T\\{
		typedef time_t, __time64_t
	rept 0\\{\\}rept 1\\{
		typedef time_t, __time32_t
	\\}
\}

match =wchar_t@defined, wchar_t@defined\{
	typedef wchar_t, word
\}

match =wint_t@defined, wint_t@defined\{
	typedef wint_t, word
	typedef wctype_t, word
\}

match =errno_t@defined, errno_t@defined\{
	typedef errno_t, dword
\}

typedef pthreadlocinfo, pointer
typedef pthreadmbcinfo, pointer

.alignmode_cdecl

struct localeinfo_struct
	locinfo pthreadlocinfo ?
	mbcinfo pthreadmbcinfo ?
ends

match =LC_ID@defined, LC_ID@defined\{
	struct LC_ID
	    wLanguage	rw 1
	    wCountry	rw 1
	    wCodePage	rw 1
	ends
	typedef LPLC_ID, P_LC_ID
\}

local lc_category_t
struct lc_category_t
    locale 		rptr 1
    wlocale 	rptr 1
    refcount	rptr 1
    wrefcount	rptr 1
ends

match =threadlocaleinfostruct@defined, threadlocaleinfostruct@defined\{

	struct threadlocaleinfostruct
		refcount 			rd 1
	    lc_codepage			rd 1;
	    lc_collate_cp		rd 1;
	    lc_handle 			__msvcrt_ulong 6 dup(?);
	    lc_id				LC_ID
						    rept 6 - 1\\{
						    	LC_ID
					    	\\}
	    lc_category 		lc_category_t
						    rept 6 - 1\\{
						    	lc_category_t
					    	\\}
	    lc_clike			rd 1
	    mb_cur_max			rd 1
	    lconv_intl_refcount	rptr 1
	    lconv_num_refcount	rptr 1
	    lconv_mon_refcount	rptr 1
	    lconv_ 				rptr 1
	    ctype1_refcount		rptr 1
	    ctype1 				rptr 1
	    pctype				rptr 1
	    pclmap				rptr 1
	    pcumap				rptr 1
	    lc_time_curr		rptr 1
	ends
\}

match =threadlocaleinfostruct@defined, threadlocaleinfostruct\{
	struct threadlocaleinfostruct
	    refcount 			rd 1
	    lc_codepage			rd 1
	    lc_collate_cp		rd 1
	    lc_handle			__msvcrt_ulong 6 dup(?)
	    lc_id 				LC_ID
						    rept 6 - 1\\{
						    	LC_ID
					    	\\}
	    lc_category 		lc_category_t
						    rept 6 - 1\\{
						    	lc_category_t
					    	\\}
	    lc_clike			rd 1
	    mb_cur_max			rd 1
	    lconv_intl_refcount	rptr 1
	    lconv_num_refcount	rptr 1
	    lconv_mon_refcount	rptr 1
	    lconv_ 				rptr 1
	    ctype1_refcount	 	rptr 1
	    ctype1 				rptr 1
	    pctype				rptr 1
	    pclmap				rptr 1
	    pcumap				rptr 1
	    lc_time_curr 		rptr 1
	ends
\}

.alignmode_previous

@endm
