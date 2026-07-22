@module cfenv

.alignmode_cdecl

include "float.inc"

define FE_TONEAREST   _RC_NEAR
define FE_UPWARD      _RC_UP
define FE_DOWNWARD    _RC_DOWN
define FE_TOWARDZERO  _RC_CHOP

define FE_INEXACT     _SW_INEXACT
define FE_UNDERFLOW   _SW_UNDERFLOW
define FE_OVERFLOW    _SW_OVERFLOW
define FE_DIVBYZERO   _SW_ZERODIVIDE
define FE_INVALID     _SW_INVALID
define FE_ALL_EXCEPT  (FE_DIVBYZERO or FE_INEXACT or FE_INVALID or FE_OVERFLOW or FE_UNDERFLOW)

struct fenv_t
    _Fe_ctl 	rd 1
    _Fe_stat 	rd 1
ends

importlib msvcrt,\
	fegetenv,\
	fesetenv,\
	fegetexceptflag,\
	fegetround,\
	feholdexcept,\
	fesetround,\
	fesetexceptflag,\
	feclearexcept,\
	fetestexcept,\
	feupdateenv

.alignmode_previous

@endm
