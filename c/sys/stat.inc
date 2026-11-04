@module c_sys_stat

include "../crtdefs.inc"
include "types.inc"

.alignmode_cdecl

match =_dev_t@defined, _dev_t@defined\{
	typedef _dev_t, DWORD
\}

match =_ino_t@defined, _ino_t@defined\{
	typedef _ino_t, DWORD
\}

match =_off_t@defined, _off_t@defined\{
	typedef _off_t, DWORD
\}

_S_IEXEC  = 0x0040
_S_IWRITE = 0x0080
_S_IREAD  = 0x0100
_S_IFIFO  = 0x1000
_S_IFCHR  = 0x2000
_S_IFDIR  = 0x4000
_S_IFREG  = 0x8000
_S_IFMT   = 0xF000

match =struct_stat@defined, struct_stat@defined\{
	struct struct_stat
		st_dev 		_dev_t ? 
		st_ino 		_ino_t ?
	  	st_mode 	rw 1
		st_nlink 	rw 1
		st_uid 		rw 1
		st_gid 		rw 1
		st_rdev		_dev_t ?
		st_size 	_off_t ?
	  	st_atime 	time_t ?
	 	st_mtime	time_t ?
		st_ctime	time_t ?
	ends

	struct struct_stat32
		st_dev 		_dev_t ? 
		st_ino 		_ino_t ?
	  	st_mode 	rw 1
		st_nlink 	rw 1
		st_uid 		rw 1
		st_gid 		rw 1
		st_rdev		_dev_t ?
		st_size 	_off_t ?
	  	st_atime 	__time32_t ?
	 	st_mtime	__time32_t ?
		st_ctime	__time32_t ?
	ends

	struct struct_stat32i64
		st_dev 		_dev_t ? 
		st_ino 		_ino_t ?
	  	st_mode 	rw 1
		st_nlink 	rw 1
		st_uid 		rw 1
		st_gid 		rw 1
		st_rdev		_dev_t ?
		st_size  	rq 1
	  	st_atime 	time_t ?
	 	st_mtime	time_t ?
		st_ctime	time_t ?
	ends

	struct struct_stat64i32
		st_dev 		_dev_t ? 
		st_ino 		_ino_t ?
	  	st_mode 	rw 1
		st_nlink 	rw 1
		st_uid 		rw 1
		st_gid 		rw 1
		st_rdev		_dev_t ?
		st_size  	rq 1
	  	st_atime 	__time64_t ?
	 	st_mtime	__time64_t ?
		st_ctime	__time64_t ?
	ends

	struct struct_stati64
		st_dev 		_dev_t ? 
		st_ino 		_ino_t ?
	  	st_mode 	rw 1
		st_nlink 	rw 1
		st_uid 		rw 1
		st_gid 		rw 1
		st_rdev		_dev_t ?
		st_size  	rq 1
	  	st_atime 	time_t ?
	 	st_mtime	time_t ?
		st_ctime	time_t ?
	ends

	struct struct_stat64
		st_dev 		_dev_t ? 
		st_ino 		_ino_t ?
	  	st_mode 	rw 1
		st_nlink 	rw 1
		st_uid 		rw 1
		st_gid 		rw 1
		st_rdev		_dev_t ?
		st_size  	rq 1
	  	st_atime 	__time64_t ?
	 	st_mtime	__time64_t ?
		st_ctime	__time64_t ?
	ends
\}

importlib msvcrt,\
	_fstat,\
	_stat,\
	_fstat32,\
	_stat32,\
	_fstati64,\
	_stati64,\
	_fstat64,\
	_stat64,\
	_umask,\
	_wstat,\
	_wstat32,\
	_wstati64,\
	_wstat64

S_IFMT   = _S_IFMT
S_IFDIR  = _S_IFDIR
S_IFCHR  = _S_IFCHR
S_IFREG  = _S_IFREG
S_IREAD  = _S_IREAD
S_IWRITE = _S_IWRITE
S_IEXEC  = _S_IEXEC

if used fstat
	virtual at _fstat
		fstat rptr 1
	end virtual
end if

if used stat
	virtual at _stat
		stat rptr 1
	end virtual
end if

if used fstat
	virtual at _umask
		umask rptr 1
	end virtual
end if

.alignmode_previous

@endm
