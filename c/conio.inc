@module cconio


importlib msvcrt,\
	_cgets,\
	_cprintf,\
	_cputs,\
	_cscanf,\
	_getch,\
	_getche,\
	_kbhit,\
	_putch,\
	_ungetch

define cgets	_cgets
define cputs	_cputs
define getch	_getch
define getche	_getche
define kbhit	_kbhit
define putch	_putch
define ungetch	_ungetch

match =x86, __architecture\{

importlib msvcrt,\
	_inp,\
	_inpd,\
	_inpw,\
	_outp,\
	_outpd,\
	_outpw

define inp		_inp
define inpw		_inpw
define outp		_outp
define outpw	_outpw

\}

define cprintf 	_cprintf
define cscanf 	_cscanf

@endm
