match =PROGRAM_TYPE, PROGRAM_TYPE{
	define PROGRAM_TYPE Console
}

match =x64, __architecture{
	format PE64 PROGRAM_TYPE

	section ".code" readable writeable executable
}

match =x86, __architecture{
	format PE PROGRAM_TYPE

	section ".code" readable writeable executable
}
