@module csignal

define SIGINT   2
define SIGILL   4
define SIGFPE   8
define SIGSEGV  11
define SIGTERM  15
define SIGBREAK 21
define SIGABRT  22

define NSIG     (SIGABRT + 1)

define SIG_DFL 0
define SIG_IGN 1
define SIG_ERR (-1)

typedef sig_atomic_t, dword

importlib msvcrt,\
	signal,\
	raise

@endm
