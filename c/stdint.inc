@module cstdint

include "stddef.inc"

; 7.18.1.1  Exact-width integer types 
typedef int8_t, byte
typedef uint8_t, byte
typedef int16_t, word
typedef uint16_t, word
typedef int32_t, dword
typedef uint32_t, dword
typedef int64_t, qword
typedef uint64_t, qword

; 7.18.1.2  Minimum-width integer types 
typedef int_least8_t, byte
typedef uint_least8_t, byte
typedef int_least16_t, word
typedef uint_least16_t, word
typedef int_least32_t, dword
typedef uint_least32_t, dword
typedef int_least64_t, qword
typedef uint_least64_t, qword

;  7.18.1.3  Fastest minimum-width integer types
;  Not actually guaranteed to be fastest for all purposes
;  Here we use the exact-width types for 8 and 16-bit ints.

typedef int_fast8_t, byte
typedef uint_fast8_t, byte
typedef int_fast16_t, word
typedef uint_fast16_t, word
typedef int_fast32_t, dword
typedef uint_fast32_t, dword
typedef int_fast64_t, qword
typedef uint_fast64_t, qword

; 7.18.1.5  Greatest-width integer types 
typedef intmax_t, qword
typedef uintmax_t, qword

typedef intptr_t, pointer
typedef uintptr_t, pointer


; 7.18.2  Limits of specified-width integer types 

; 7.18.2.1  Limits of exact-width integer types 
define INT8_MIN (-0x80)
define INT16_MIN (-0x8000)
define INT32_MIN (-0x7fffffff - 1)
define INT64_MIN  (-0x7fffffffffffffff - 1)

define INT8_MAX 0x7f
define INT16_MAX 0x7fff
define INT32_MAX 0x7fffffff
define INT64_MAX 0x7fffffffffffffff

define UINT8_MAX 0xff
define UINT16_MAX 0xffff
define UINT32_MAX 0xffffffffu  ; 4294967295U 
define UINT64_MAX 0xffffffffffffffff ; 18446744073709551615ULL 

; 7.18.2.2  Limits of minimum-width integer types 
define INT_LEAST8_MIN INT8_MIN
define INT_LEAST16_MIN INT16_MIN
define INT_LEAST32_MIN INT32_MIN
define INT_LEAST64_MIN INT64_MIN

define INT_LEAST8_MAX INT8_MAX
define INT_LEAST16_MAX INT16_MAX
define INT_LEAST32_MAX INT32_MAX
define INT_LEAST64_MAX INT64_MAX

define UINT_LEAST8_MAX UINT8_MAX
define UINT_LEAST16_MAX UINT16_MAX
define UINT_LEAST32_MAX UINT32_MAX
define UINT_LEAST64_MAX UINT64_MAX

; 7.18.2.3  Limits of fastest minimum-width integer types 
define INT_FAST8_MIN INT8_MIN
define INT_FAST16_MIN INT16_MIN
define INT_FAST32_MIN INT32_MIN
define INT_FAST64_MIN INT64_MIN

define INT_FAST8_MAX INT8_MAX
define INT_FAST16_MAX INT16_MAX
define INT_FAST32_MAX INT32_MAX
define INT_FAST64_MAX INT64_MAX

define UINT_FAST8_MAX UINT8_MAX
define UINT_FAST16_MAX UINT16_MAX
define UINT_FAST32_MAX UINT32_MAX
define UINT_FAST64_MAX UINT64_MAX

; 7.18.2.4  Limits of integer types capable of holding
; object pointers 
match =x64, __architecture\{
	define INTPTR_MIN INT64_MIN
	define INTPTR_MAX INT64_MAX
	define UINTPTR_MAX UINT64_MAX
\}
match =x86, __architecture\{
	define INTPTR_MIN INT32_MIN
	define INTPTR_MAX INT32_MAX
	define UINTPTR_MAX UINT32_MAX
\}

; 7.18.2.5  Limits of greatest-width integer types 
define INTMAX_MIN INT64_MIN
define INTMAX_MAX INT64_MAX
define UINTMAX_MAX UINT64_MAX

; 7.18.3  Limits of other integer types 
match =x64, __architecture\{
	define PTRDIFF_MIN INT64_MIN
	define PTRDIFF_MAX INT64_MAX
\}
match =x86, __architecture\{
	define PTRDIFF_MIN INT32_MIN
	define PTRDIFF_MAX INT32_MAX
\}

define SIG_ATOMIC_MIN INT32_MIN
define SIG_ATOMIC_MAX INT32_MAX

match =x64, __architecture\{
	define SIZE_MAX UINT64_MAX
\}
match =x86, __architecture\{
	define SIZE_MAX UINT32_MAX
\}

; also in wchar.h 
define WCHAR_MIN 0U
define WCHAR_MAX 0xffffU

; wint_t is unsigned short for compatibility with MS runtime
define WINT_MIN 0
define WINT_MAX 0xffff


; CHANGED: no function-like macro
; 7.18.4  Macros for integer constants 

; 7.18.4.1  Macros for minimum-width integer constants

    ; According to Douglas Gwyn <gwyn@arl.mil>:
	; "This spec was changed in ISO/IEC 9899:1999 TC1; in ISO/IEC
	; 9899:1999 as initially published, the expansion was required
	; to be an integer constant of precisely matching type, which
	; is impossible to accomplish for the shorter types on most
	; platforms, because C99 provides no standard way to designate
	; an integer constant with width less than that of type int.
	; TC1 changed this to require just an integer constant
	; *expression* with *promoted* type."

	; The trick used here is from Clive D W Feather.

; define INT8_C(val) (INT_LEAST8_MAX-INT_LEAST8_MAX+(val))
; define INT16_C(val) (INT_LEAST16_MAX-INT_LEAST16_MAX+(val))
; define INT32_C(val) (INT_LEAST32_MAX-INT_LEAST32_MAX+(val))
; ;  The 'trick' doesn't work in C89 for long long because, without
;     suffix, (val) will be evaluated as int, not intmax_t 
; define INT64_C(val) val##LL

; define UINT8_C(val) (val)
; define UINT16_C(val) (val)
; define UINT32_C(val) (val##U)
; define UINT64_C(val) val##ULL

; ; 7.18.4.2  Macros for greatest-width integer constants 
; define INTMAX_C(val) val##LL
; define UINTMAX_C(val) val##ULL

@endm
