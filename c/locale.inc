@module clocale

define LC_ALL      	0
define LC_COLLATE  	1
define LC_CTYPE    	2
define LC_MONETARY 	3
define LC_NUMERIC  	4
define LC_TIME     	5
define LC_MIN      	LC_ALL
define LC_MAX      	LC_TIME

match =locale_t@defined, locale_t@defined\{
	typedef locale_t, pointer
\}

match =lconv@defined, lconv@defined\{
	.alignmode_cdecl
	struct lconv
	    decimal_point			rptr 1;
	    thousands_sep			rptr 1;
	    grouping				rptr 1;
	    int_curr_symbol			rptr 1;
	    currency_symbol			rptr 1;
	    mon_decimal_point		rptr 1;
	    mon_thousands_sep		rptr 1;
	    mon_grouping			rptr 1;
	    positive_sign			rptr 1;
	    negative_sign			rptr 1;
	    int_frac_digits			rb 1;
	    frac_digits				rb 1;
	    p_cs_precedes			rb 1;
	    p_sep_by_space			rb 1;
	    n_cs_precedes			rb 1;
	    n_sep_by_space			rb 1;
	    p_sign_posn				rb 1;
	    n_sign_posn				rb 1;
	    _W_decimal_point		rptr 1;
	    _W_thousands_sep		rptr 1;
	    _W_int_curr_symbol		rptr 1;
	    _W_currency_symbol		rptr 1;
	    _W_mon_decimal_point	rptr 1;
	    _W_mon_thousands_sep	rptr 1;
	    _W_positive_sign		rptr 1;
	    _W_negative_sign		rptr 1;
    ends
	.alignmode_previous
\}

define _ENABLE_PER_THREAD_LOCALE		 	0x1
define _DISABLE_PER_THREAD_LOCALE		 	0x2
define _ENABLE_PER_THREAD_LOCALE_GLOBAL		0x10
define _DISABLE_PER_THREAD_LOCALE_GLOBAL	0x20
define _ENABLE_PER_THREAD_LOCALE_NEW		0x100
define _DISABLE_PER_THREAD_LOCALE_NEW		0x200

importlib msvcrt,\
	setlocale,\
	localeconv,\
	_Strftime,\
	_configthreadlocale,\
	_get_current_locale,\
	_create_locale,\
	_wcreate_locale,\
	_free_locale,\
	_Gettnames,\
	_W_Gettnames,\
	_wsetlocale

@endm
