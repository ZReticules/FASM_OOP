@module cstdio

	local dllname
	dllname equ ucrtbase
	match =ucrt, ucrt\{
		dllname equ msvcrt
	\}

	match =msvcrt, dllname\{
		importlib msvcrt,\
			freopen,\
			fclose,\
			feof,\
			clearerr,\
			ferror,\
			fflush,\
			fgetpos,\
			fseek,\
			fsetpos,\
			ftell,\
			rewind,\
			setbuf,\
			setvbuf,\
			tmpfile,\
			fread,\
			fopen, wfopen="_wfopen",\
			fgetc, fgetwc,\
			fgets, fgetws,\
			fprintf, fwprintf,\
			fputc, fputwc,\
			fputs, fputws,\
			fscanf, fwrite,\
			getc, getchar,\
			gets, getws="_getws",\
			perror, wperror="_wperror",\
			printf, wprintf,\
			putc, putwc,\
			putchar, putwchar,\
			puts, putws="_putws",\
			remove, wremove="_wremove",\
			rename, wrename="_wrename",\
			scanf, wscanf,\
			snprintf="_snprintf", snwprintf="_snwprintf"\
			sprintf, swprintf,\
			sscanf, swscanf,\
			tmpnam, wtmpnam="_wtmpnam",\
			ungetc, ungetwc,\
			vfprintf, vfwprintf,\
			vfscanf, vfwscanf,\
			vprintf, vwprintf,\
			vscanf, vwscanf,\
			vsnprintf, vsnwprintf="_vsnwprintf",\
			vsprintf, vswprintf,\
			vsscanf, vswscanf,\
			setlocale,\
			setmode="_setmode",\
			fileno="_fileno",\
			_iob
	\}
	match =ucrtbase, dllname\{
		importlib ucrtbase,\
			freopen,\
			fclose,\
			feof,\
			clearerr,\
			ferror,\
			fflush,\
			fgetpos,\
			fseek,\
			fsetpos,\
			ftell,\
			rewind,\
			setbuf,\
			setvbuf,\
			tmpfile,\
			fread,\
			fopen, wfopen="_wfopen",\
			fgetc, fgetwc,\
			fgets, fgetws,\
			fprintf, fwprintf,\
			fputc, fputwc,\
			fputs, fputws,\
			fscanf, fwrite,\
			getc, getchar,\
			gets, getws="_getws",\
			perror, wperror="_wperror",\
			printf, wprintf,\
			putc, putwc,\
			putchar, putwchar,\
			puts, putws="_putws",\
			remove, wremove="_wremove",\
			rename, wrename="_wrename",\
			scanf, wscanf,\
			snprintf="_snprintf", snwprintf="_snwprintf"\
			sprintf, swprintf,\
			sscanf, swscanf,\
			tmpnam, wtmpnam="_wtmpnam",\
			ungetc, ungetwc,\
			vfprintf, vfwprintf,\
			vfscanf, vfwscanf,\
			vprintf, vwprintf,\
			vscanf, vwscanf,\
			vsnprintf, vsnwprintf="_vsnwprintf",\
			vsprintf, vswprintf,\
			vsscanf, vswscanf,\
			setlocale,\
			__acrt_iob_func
	\}

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
			match =msvcrt, dllname\\{
				mov pax, [_iob]
				mov [stdin], pax
				add pax, 10h + 4 * pointer.size
				mov [stdout], pax
				add pax, 10h + 4 * pointer.size
				mov [stderr], pax
			\\}
			match =ucrtbase, dllname\\{
				$call [stdin] = c [__acrt_iob_func](0)
				$call [stdout] = c [__acrt_iob_func](1)
				$call [stderr] = c [__acrt_iob_func](2)
			\\}
		end if
	\}

	TLS_AddMacro cstdio_init

@endm