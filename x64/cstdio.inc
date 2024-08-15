include_once "TOOLS\x64\TLS.inc"

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
stdout 	equ cstdio_iob+8
stderr 	equ cstdio_iob+16
if used cstdio_iob
	label cstdio_iob qword
		dq ?
		dq ?
		dq ?

	cstdio_init:
		mov rax, [_iob]
		mov [stdin], rax
		add rax, 30h
		mov [stdout], rax
		add rax, 30h
		mov [stderr], rax
		ret

	TLS_AddCallback cstdio_init
end if