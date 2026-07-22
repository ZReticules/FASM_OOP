@module climits

define CHAR_BIT 8
define MB_LEN_MAX 5

define SCHAR_MIN (-0x80)
define SCHAR_MAX   0x7f
define UCHAR_MAX   0xff

match =__CHAR_UNSIGNED__, __CHAR_UNSIGNED__\{
	define CHAR_MIN SCHAR_MIN
	define CHAR_MAX SCHAR_MAX
rept 0\{\}rept 1\{\}
	define CHAR_MIN 0
	define CHAR_MAX UCHAR_MAX
\}

define SHRT_MIN  	(-0x8000)
define SHRT_MAX    	0x7fff
define USHRT_MAX   	0xffff

define INT_MIN   	(-0x7fffffff - 1)
define INT_MAX     	0x7fffffff
define UINT_MAX    	0xffffffff

define LONG_MIN  	(-0x7fffffff - 1)
define LONG_MAX    	0x7fffffff
define ULONG_MAX   	0xffffffff

define LLONG_MAX   0x7fffffffffffffff
define LLONG_MIN   (-LLONG_MAX-1)
define ULLONG_MAX  0xffffffffffffffff

define _I64_MAX    0x7fffffffffffffff
define _I64_MIN    (-_I64_MAX-1)
define _UI64_MAX   0xffffffffffffffff

define I64_MIN  _I64_MIN
define I64_MAX  _I64_MAX
define UI64_MAX _UI64_MAX

match =x64, __architecture\{
	define SIZE_MAX UI64_MAX
\}
match =x86, __architecture\{
	define SIZE_MAX UINT_MAX
\}

@endm
