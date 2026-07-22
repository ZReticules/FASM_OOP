@module cstring

define _NLSCMPERROR 0x7fffffff

define NULL 0
match =size_t@defined, size_t@defined\{
	typedef size_t, pointer
\}

importlib msvcrt,\
	memchr,\
	memcmp,\
	memcpy,\
	memcpy_s,\
	memset,\
	_memccpy,\
	_memicmp,\
	_memicmp_l,\
	_wcsdup,\
	_wcserror,\
	_wcsicmp,\
	_wcsicmp_l,\
	_wcsicoll,\
	_wcsicoll_l,\
	_wcslwr,\
	_wcslwr_s,\
	_wcscoll_l,\
	_wcsncoll,\
	_wcsncoll_l,\
	_wcsnicmp,\
	_wcsnicoll,\
	_wcsnicoll_l,\
	_wcsnlen,\
	_wcsnset,\
	_wcsrev,\
	_wcsset,\
	_wcsupr,\
	_wcsupr_s,\
	_wcsxfrm_l,\
	wcscat,\
	wcscat_s,\
	wcschr,\
	wcscmp,\
	wcscoll,\
	wcscpy,\
	wcscpy_s,\
	wcscspn,\
	wcslen,\
	wcsncat,\
	wcsncat_s,\
	wcsncmp,\
	wcsncpy,\
	wcsncpy_s,\
	wcsnlen,\
	wcspbrk,\
	wcsrchr,\
	wcsspn,\
	wcsstr,\
	wcstok_s,\
	wcsxfrm,\
	memmove,\
	wcstok

define memicmp _memicmp
define memccpy _memccpy

define _wcstok wcstok

importlib msvcrt,\
	_strcmpi,\
	_strcoll_l,\
	_strdup,\
	_strerror,\
	strerror_s,\
	_stricmp,\
	_stricoll,\
	_stricoll_l,\
	_strlwr,\
	_strlwr_s,\
	_strncoll,\
	_strncoll_l,\
	_strnicmp,\
	_strnicmp_l,\
	_strnicoll,\
	_strnicoll_l,\
	_strnset,\
	_strrev,\
	_strset,\
	_strupr,\
	_strupr_s,\
	_strxfrm_l,\
	memmove_s,\
	strcat,\
	strcat_s,\
	strchr,\
	strcmp,\
	strcoll,\
	strcpy,\
	strcpy_s,\
	strcspn,\
	strerror,\
	strlen,\
	strncat,\
	strncat_s,\
	strncmp,\
	strncpy,\
	strncpy_s,\
	strnlen,\
	strpbrk,\
	strrchr,\
	strspn,\
	strstr,\
	strtok,\
	strtok_s,\
	strxfrm
	
define strcasecmp	_stricmp
define strcmpi		_strcmpi
define strdup		_strdup
define stricmp		_stricmp
define stricoll		_stricoll
define strlwr		_strlwr
define strncasecmp	_strnicmp
define strnicmp		_strnicmp
define strnset		_strnset
define strrev		_strrev
define strset		_strset
define strupr		_strupr
define wcsdup		_wcsdup
define wcsicoll		_wcsicoll
define wcslwr		_wcslwr
define wcsicmp		_wcsicmp
define wcsnicmp		_wcsnicmp
define wcsnset		_wcsnset
define wcsrev		_wcsrev
define wcsset		_wcsset
define wcsupr		_wcsupr

@endm
