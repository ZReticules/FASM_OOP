@module cstddef

; CHANGED: _threadid as function 

importlib msvcrt,\
	_theradid="__threadid"

match =size_t@defined, size_t@defined\{
	typedef size_t, pointer
\}
match =ptrdiff_t@defined, ptrdiff_t@defined\{
	typedef ptrdiff_t, pointer
\}
match =wchar_t@defined, wchar_t@defined\{
	typedef wchar_t, word
\}
define NULL 0

@endm
