@module ctime

.alignmode_cdecl

define CLOCKS_PER_SEC 1000

struct tm
    tm_sec		rd 1
    tm_min		rd 1
    tm_hour		rd 1
    tm_mday		rd 1
    tm_mon		rd 1
    tm_year		rd 1
    tm_wday		rd 1
    tm_yday		rd 1
    tm_isdst	rd 1
ends

importlib msvcrt,\
	_wasctime,\
	wcsftime,\
	_wcsftime_l,\
	_wctime32,\
	_wctime64,\
	_wstrdate,\
	_wstrdate_s,\
	_wstrtime,\
	_wstrtime_s

if used _wctime
	virtual at _wctime64
		_wctime rptr 1
	end virtual
end if

typedef time_t, qword
typedef clock_t, dword

struct _timespec32
    tv_sec 	rd 1;
    tv_nsec rd 1;
ends

struct _timespec64
    tv_sec 	rq 1;
    tv_nsec rd 1;
ends

struct timespec
    tv_sec 	time_t
    tv_nsec rd 1
ends

importlib msvcrt,\
	_daylight="__daylight",\
	_dstbias="__dstbias",\
	_timezone="__timezone",\
	_tzname="__tzname"
	_localtime32,\
	_localtime32_s,\
	_localtime64,\
	_localtime64_s,\
	_mktime32,\
	_mktime64,\
	strftime,\
	_strftime_l,\
	_time32,\
	_time64

define ctime		_ctime64
define ctime_s		_ctime64_s
define difftime		_difftime64
define gmtime		_gmtime64
define gmtime_s		_gmtime64_s
define localtime	_localtime64
define localtime_s	_localtime64_s
define mktime		_mktime64
define time			_time64

.alignmode_previous

@endm
