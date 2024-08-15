include_once "TOOLS\x86\TLS.inc"

importlib msvcrt,\
	clearerr,\
	fclose,\
	feof,\
	ferror,\
	fflush,\
	fgetc,\
	fgetpos,\
	fgets,\
	fopen,\
	fprintf,\
	fputc,\
	fputs,\
	fread,\
	freopen,\
	fscanf,\
	fseek,\
	fsetpos,\
	ftell,\
	fwrite,\
	getc,\
	getchar,\
	gets,\
	perror,\
	printf,\
	putc,\
	putchar,\
	puts,\
	remove,\
	rename,\
	rewind,\
	scanf,\
	setbuf,\
	setvbuf,\
	snprintf="_snprintf",\
	sprintf,\
	sscanf,\
	tmpfile,\
	tmpnam,\
	ungetc,\
	vfprintf,\
	vfscanf,\
	vprintf,\
	vscanf,\
	vsnprintf,\
	vsprintf,\
	vsscanf,\
	_iob

define EOF -1
define FOPEN_MAX 20
define NULL 0
define SEEK_SET 0
define SEEK_CUR 1
define SEEK_END 2

stdin 	equ cstdio_iob
stdout 	equ cstdio_iob+4
stderr 	equ cstdio_iob+8
if used cstdio_iob
	label cstdio_iob dword
		dd ?
		dd ?
		dd ?

	proc cstdio_init
		_static_args .handle, .reason, .reserved
		mov eax, [_iob]
		mov [stdin], eax
		add eax, 20h
		mov [stdout], eax
		add eax, 20h
		mov [stderr], eax
		ret args_space
	endp

	TLS_AddCallback cstdio_init
end if