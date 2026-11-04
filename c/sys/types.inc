@module c_sys_types

match =_dev_t@defined, _dev_t@defined\{
	typedef _dev_t, dword
\}

match =_ino_t@defined, _ino_t@defined\{
	typedef _ino_t, word
\}

match =_mode_t@defined, _mode_t@defined\{
	typedef _mode_t, word
\}

match =_off_t@defined, _off_t@defined\{
	typedef _off_t, dword
\}

typedef dev_t, _dev_t
typedef ino_t, _ino_t
typedef mode_t, _mode_t
typedef off_t, _off_t

match =_pid_t@defined, _pid_t@defined\{
	typedef _pid_t, dword
\}

match =ssize_t@defined, ssize_t@defined\{
	typedef ssize_t, pointer
\}

@endm
