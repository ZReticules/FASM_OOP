@module cstdio

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
	stdout 	equ (cstdio_iob+pointer.size)
	stderr 	equ (cstdio_iob+pointer.size*2)

	if used cstdio_iob
		cstdio_iob dptr 3 dup(?)
	end if

	macro cstdio_init \{
		if used cstdio_iob
			mov pax, [_iob]
			mov [stdin], pax
			add pax, 10h + 4 * pointer.size
			mov [stdout], pax
			add pax, 10h + 4 * pointer.size
			mov [stderr], pax
		end if
	\}

	TLS_AddMacro cstdio_init

@endm