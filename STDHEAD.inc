match =x64, __architecture{
	format PE64 Console

	section ".code" readable writeable executable
}

match =x86, __architecture{
	format PE Console

	section ".code" readable writeable executable
}
