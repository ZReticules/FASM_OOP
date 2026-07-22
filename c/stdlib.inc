@module cstdlib

importlib msvcrt,\
	atof,\
	atoi,\
	atol,\
	atoll,\
	strtod,\
	strtof,\
	strtol,\
	strtold,\
	strtoll,\
	strtoul,\
	strtoull,\
	rand,\
	srand,\
	malloc,\
	calloc,\
	free,\
	abort,\
	atexit,\
	exit,\
	getenv,\
	system,\
	bsearch,\
	qsort,\
	abs, labs,\
	llabs, imaxlabs,\
	div_="div", ldiv,\	; CHANGED div
	mblen,\
	mbtowc, wctomb, mbstowcs, wcstombs

struct div_t
	quot 	rd 1
	rem 	rd 1
ends

struct ldiv_t
	quot 	rd 1
	rem 	rd 1
ends

; non-standard

struct lldiv_t
	quot 	rq 1
	rem 	rq 1
ends

; _set_error_mode() constants
define _OUT_TO_DEFAULT      0
define _OUT_TO_STDERR       1
define _OUT_TO_MSGBOX       2
define _REPORT_ERRMODE      3

; _set_abort_behavior codes
define _WRITE_ABORT_MSG     1
define _CALL_REPORTFAULT    2

; 
define EXIT_SUCCESS        0
define EXIT_FAILURE        -1
define RAND_MAX            0x7FFF

define _MAX_DRIVE          3
define _MAX_FNAME          256
define _MAX_DIR            _MAX_FNAME
define _MAX_EXT            _MAX_FNAME
define _MAX_PATH           260
define _MAX_ENV            32767

; Make the secure string functions (names end in "_s") truncate their output 
define _TRUNCATE  -1

importlib msvcrt,\
	mb_cur_max = '___mb_cur_max_func',\ ; CHANGED(const MB_CUR_MAX)
	_errno,\
	_get_errno,\
	_set_errno,\
	_set_fmode,\
	_get_fmode,\
	__wgetmainargs,\
	__getmainargs,\
	_atodbl,\
	_atodbl_l,\
	_atoflt,\
	_atoflt_l,\
	_atoi64,\
	_atoldbl,\
	_dupenv_s,\
	_ecvt, _ecvt_s,\
	fcvt, _fcvt_s,\
	_gcvt, _gcvt_s,\
	_itoa, _itoa_s,\
	_ltoa, _ltoa_s,\
	_i64toa, _i64toa_s,\
	_fullpath, _wfullpath,\
	_makepath, _wmakepath,\
	_makepath_s, _wmakepath_s,\
	_mbstrlen,\
	_onexit,\
	_putenv, _putenv_s,\
	_searchenv, _wsearchenv,\
	_searchenv_s, _wsearchenv_s,\
	_set_error_mode, _seterrormode,\
	_sleep,\
	_splitpath, _splitpath_s,\
	_swab,\
	_ultoa, _ultoa_s,\
	_ui64toa, _ui64toa_s,\
	_exit, exit,\
	abort,\
	at_quick_exit, atexit,\
	_atoi_l,\
	getenv, getenv_s,\
	wgetenv, wgetenv_s,\
	perror,\ 
	quick_exit,\
	rand_s,\
	_strtof_l,\
	_strtod_l,\
	_strtold_l,\
	_strtoll_l,\
	_strtoull_l,\
	_strtoi64, _strtoi64_l ,\
	_strtoui64, _strtoui64_l,\
	qsort_s,\
	_set_abort_behavior,\
	_set_purecall_handler,\
	_get_purecall_handler,\
	_set_invalid_parameter_handler,\
	_get_invalid_parameter_handler,\
	_get_thread_local_invalid_parameter_handler,\
	_set_thread_local_invalid_parameter_handler,\
	_invalid_parameter,\
	__p_osver="_osver",\
	__p_winver="_winver",\
	__p_winmajor="_winmajor",\
	__p_winminor="_winminor"

define ecvt 	_ecvt
define fcvt 	_fcvt
define gcvt 	_gcvt
define itoa 	_itoa
define ltoa 	_ltoa
define onexit 	_onexit
define putenv 	_putenv
define strtoll 	_strtoi64
define strtoull _strtoui64
define swab 	_swab
define ultoa 	_ultoa

if used _argc | used _argv | used _argc 
	bss _argc:DWORD, _argv:POINTER, _env:POINTER
end if

if used _wargc | used _wargv | used _wargc 
	bss _wargc:DWORD, _wargv:POINTER, _wenv:POINTER
end if

match =CARGS_WILDCARDS, CARGS_WILDCARDS\{
	define CARGS_WILDCARDS 0
\}

struct _startupinfo 
    newmode rd 1
ends

if used _osver
	bss _osver:DWORD
end if
if used _winver
	bss _winver:DWORD
end if
if used _winminor
	bss _winminor:DWORD
end if
if used _winmajor
	bss _winmajor:DWORD
end if

macro cstdlib.at_start\{
 	if used _argc | used _argv | used _argc
 		$call c [__getmainargs](_argc, _argv, _env, CARGS_WILDCARDS, const _startupinfo 0)
 	end if
	if used _wargc | used _wargv | used _wargc
 		$call c [__wgetmainargs](_wargc, _wargv, _wenv, CARGS_WILDCARDS, const _startupinfo 0)
 	end if
 	if used _osver
 		mov pax, [__p_osver]
 		mov eax, [pax]
 		mov [_osver], eax
 	end if
 	if used _winver
 		mov pax, [__p_winver]
 		mov eax, [pax]
 		mov [_winver], eax
 	end if
 	if used _winminor
 		mov pax, [__p_winminor]
 		mov eax, [pax]
 		mov [_winminor], eax
 	end if
 	if used _winmajor
 		mov pax, [__p_winmajor]
 		mov eax, [pax]
 		mov [_winmajor], eax
 	end if
\}

match =size_t@defined, size_t@defined\{
	typedef size_t, pointer
\}

@at_start cstdlib.at_start

@endm
