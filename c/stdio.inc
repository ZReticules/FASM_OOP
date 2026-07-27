@module cstdio

define EOF 			-1
define FILENAME_MAX 260
define FOPEN_MAX 	20
define L_tmpnam 	260
define TMP_MAX 		0x7fff
define TMP_MAX_S 	0x7fffffff

define NULL 0

define SEEK_SET 0
define SEEK_CUR 1
define SEEK_END 2

define _IOFBF 	0
define _IONBF 	0x0004
define _IOLBF 	0x0040

define BUFSIZ 512

define STDIN_FILENO  0
define STDOUT_FILENO 1
define STDERR_FILENO 2

; return value for _get_output_format
define _TWO_DIGIT_EXPONENT 0x1

; local dllname
; dllname equ ucrtbase
; match =UCRT, UCRT\{
; 	dllname equ msvcrt
; \}

; match =msvcrt, dllname\{
importlib msvcrt,\
	_fcloseall,\
	_fdopen,\
	_fgetchar,\
	_filbuf,\
	_fileno,\
	_flsbuf,\
	_flushall,\
	_fputchar,\
	_fsopen,\
	_get_printf_count_output,\
	_getmaxstdio,\
	_getw,\
	_pclose,\
	_popen,\
	_putw,\
	_rmtmp,\
	_set_printf_count_output,\
	_setmaxstdio,\
	_tempnam,\
	_unlink,\
	_lock_file,\
	_unlock_file,\
	_fread_nolock,\
	_fread_nolock_s,\
	_fwrite_nolock,\
	_fclose_nolock,\
	_fflush_nolock,\
	_fgetc_nolock,\
	_fputc_nolock,\
	_fseek_nolock,\
	_fseeki64_nolock,\
	_ftell_nolock,\
	_ftelli64_nolock,\
	_getc_nolock,\
	_putc_nolock,\
	_ungetc_nolock,\
	clearerr,\
	clearerr_s,\
	fclose,\
	feof,\
	ferror,\
	fflush,\
	fgetc,\
	fgetpos,\
	fgets,\
	fopen,\
	fopen_s,\
	fputc,\
	fputs,\
	fread,\
	fread_s,\
	freopen,\
	freopen_s,\
	fseek,\
	_fseeki64,\
	fsetpos,\
	ftell,\
	_ftelli64,\
	fwrite,\
	getc,\
	getchar,\
	gets,\
	perror,\
	putc,\
	putchar,\
	puts,\
	remove,\
	rename,\
	rewind,\
	setbuf,\
	setvbuf,\
	tmpfile,\
	tmpfile_s,\
	tmpnam,\
	ungetc,\
	_get_output_format,\
	_set_output_format,\
	_scprintf,\
	_scprintf_l,\
	_snprintf_s,\
	_snprintf_s_l,\
	_vscprintf,\
	_vscprintf_l,\
	_vsnprintf_s,\
	_vsnprintf_s_l,\
	_vsprintf_p_l,\
	fprintf,\
	_fprintf_l,\
	fprintf_s,\
	_fprintf_s_l,\
	printf,\
	_printf_l,\
	printf_s,\
	_printf_s_l,\
	sprintf_s,\
	_sprintf_s_l,\
	vfprintf,\
	_vfprintf_l,\
	vfprintf_s,\
	_vfprintf_s_l,\
	vprintf,\
	_vprintf_l,\
	vprintf_s,\
	_vprintf_s_l,\
	vsprintf,\
	_vsprintf_l,\
	vsprintf_s,\
	_vsprintf_s_l,\
	_vsnprintf,\
	_vsnprintf_l,\
	_snscanf_l,\
	fscanf,\
	_fscanf_l,\
	fscanf_s,\
	_fscanf_s_l,\
	scanf,\
	_scanf_l,\
	scanf_s,\
	_scanf_s_l,\
	sscanf,\
	_sscanf_l,\
	sscanf_s,\
	_sscanf_s_l,\
	vscanf,\
	vfscanf,\
	vsscanf,\
	snprintf,\
	_snprintf,\
	_snprintf_l,\
	sprintf,\
	_sprintf_l,\
	_iob

match =FILE_t#@defined, FILE_t#@defined\{
	.alignmode_cdecl
	struct FILE_t
		_ptr		rptr 1;
		_cnt		rd 1;
		_base		rptr 1;
		_flag		rd 1;
		_file		rd 1;
		_charbuf	rd 1;
		_bufsiz		rd 1;
		_tmpfname	rptr 1;
	ends
	.alignmode_previous
\}

define WEOF        0xFFFF

importlib msvcrt,\
	_fgetwc_nolock,\
	_fgetwchar,\
	_fputwc_nolock,\
	_fputwchar,\
	_getwc_nolock,\
	_getws,\
	_putwc_nolock,\
	_putws,\
	_ungetwc_nolock,\
	_wfdopen,\
	_wfopen,\
	_wfopen_s,\
	_wfreopen,\
	_wfsopen,\
	_wperror,\
	_wpopen,\
	_wremove,\
	_wtempnam,\
	_wtmpnam,\
	fgetwc,\
	fgetws,\
	fputwc,\
	fputws,\
	getwc,\
	getwchar,\
	putwc,\
	putwchar,\
	ungetwc,\
	_scwprintf,\
	_scwprintf_l,\
	_snwprintf,\
	_snwprintf_l,\
	_snwprintf_s,\
	_snwprintf_s_l,\
	_vscwprintf,\
	_vscwprintf_l,\
	_vscwprintf_p_l,\
	_vsnwprintf,\
	_vsnwprintf_l,\
	_vsnwprintf_s,\
	_vsnwprintf_s_l,\
	_vswprintf_p_l,\
	fwprintf,\
	_fwprintf_l,\
	fwprintf_s,\
	_fwprintf_s_l,\
	swprintf_s,\
	_swprintf_s_l,\
	vfwprintf,\
	_vfwprintf_l,\
	vfwprintf_s,\
	_vfwprintf_s_l,\
	vswprintf_s,\
	_vswprintf_s_l,\
	vwprintf,\
	_vwprintf_l,\
	vwprintf_s,\
	_vwprintf_s_l,\
	wprintf,\
	_wprintf_l,\
	wprintf_s,\
	_wprintf_s_l,\
	vswprintf,\
	swprintf,\
	fwscanf,\
	_fwscanf_l,\
	fwscanf_s,\
	_fwscanf_s_l,\
	swscanf,\
	_swscanf_l,\
	swscanf_s,\
	_swscanf_s_l,\
	vfwscanf,\
	vswscanf,\
	vwscanf,\
	wscanf,\
	_wscanf_l,\
	wscanf_s,\
	_wscanf_s_l

; match =ucrtbase, dllname\{
; 	importlib ucrtbase,\
; 		freopen,\
; 		fclose,\
; 		feof,\
; 		clearerr,\
; 		ferror,\
; 		fflush,\
; 		fgetpos,\
; 		fseek,\
; 		fsetpos,\
; 		ftell,\
; 		rewind,\
; 		setbuf,\
; 		setvbuf,\
; 		tmpfile,\
; 		fread,\
; 		fopen, wfopen="_wfopen",\
; 		fgetc, fgetwc,\
; 		fgets, fgetws,\
; 		fprintf, fwprintf,\
; 		fputc, fputwc,\
; 		fputs, fputws,\
; 		fscanf, fwrite,\
; 		getc, getchar,\
; 		gets, getws="_getws",\
; 		perror, wperror="_wperror",\
; 		printf, wprintf,\
; 		putc, putwc,\
; 		putchar, putwchar,\
; 		puts, putws="_putws",\
; 		remove, wremove="_wremove",\
; 		rename, wrename="_wrename",\
; 		scanf, wscanf,\
; 		snprintf="_snprintf", snwprintf="_snwprintf"\
; 		sprintf, swprintf,\
; 		sscanf, swscanf,\
; 		tmpnam, wtmpnam="_wtmpnam",\
; 		ungetc, ungetwc,\
; 		vfprintf, vfwprintf,\
; 		vfscanf, vfwscanf,\
; 		vprintf, vwprintf,\
; 		vscanf, vwscanf,\
; 		vsnprintf, vsnwprintf="_vsnwprintf",\
; 		vsprintf, vswprintf,\
; 		vsscanf, vswscanf,\
; 		setlocale,\
; 		__acrt_iob_func
; \}

define vsnprintf 	_vsnprintf
define fdopen 		_fdopen
define fgetchar 	_fgetchar
define fileno 		_fileno
define fputchar 	_fputchar
define pclose 		_pclose
define popen 		_popen
define tempnam 		_tempnam
define fgetwchar 	_fgetwchar
define fputwchar 	_fputwchar
define getw 		_getw
define putw 		_putw
define wpopen 		_wpopen
define unlink 		_unlink
define getws		_getws
define putws		_putws

if used cstdio_iob
	cstdio_iob dptr 3 dup(?)
end if

stdin 	equ cstdio_iob
stdout 	equ (cstdio_iob+pointer.size)
stderr 	equ (cstdio_iob+pointer.size*2)

macro cstdio_init \{
	if used cstdio_iob
		; match =msvcrt, dllname\\{
			mov rax, [_iob]
			mov [stdin], rax
			add rax, 10h + 4 * 8
			mov [stdout], rax
			add rax, 10h + 4 * 8
			mov [stderr], rax
		; \\}
		; match =ucrtbase, dllname\\{
		; 	$call [stdin] = c [__acrt_iob_func](0)
		; 	$call [stdout] = c [__acrt_iob_func](1)
		; 	$call [stderr] = c [__acrt_iob_func](2)
		; \\}
	end if
\}

TLS_AddMacro cstdio_init

@endm
