match =PROGRAM_TYPE, PROGRAM_TYPE{
	define PROGRAM_TYPE Console
}

match =PROGRAM_FORMAT, PROGRAM_FORMAT{
	define PROGRAM_FORMAT PE
}

match =x64|_pf, __architecture|PROGRAM_FORMAT{
	format _pf#64 PROGRAM_TYPE

	section ".code" readable writeable executable
}

match =x86, __architecture{
	format PROGRAM_FORMAT PROGRAM_TYPE

	section ".code" readable writeable executable
}
