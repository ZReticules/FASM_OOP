@module cmath

; CHANGED: no size autocheck

.proc_frame_mode_static

@const_align equ pointer.size

define _DOMAIN   	1 ; domain error in argument
define _SING     	2 ; singularity
define _OVERFLOW 	3 ; range overflow
define _UNDERFLOW	4 ; range underflow
define _TLOSS    	5 ; total loss of precision
define _PLOSS    	6 ; partial loss of precision

define FP_INFINITE   1
define FP_NAN        2
define FP_NORMAL    -1
define FP_SUBNORMAL -2
define FP_ZERO       0

define _C2 1
define FP_ILOGB0 (-0x7fffffff - _C2)
define FP_ILOGBNAN 0x7fffffff

define _FP_LT  1
define _FP_EQ  2
define _FP_GT  4

define M_E         2.71828182845904523536
define M_LOG2E     1.44269504088896340736
define M_LOG10E    0.434294481903251827651
define M_LN2       0.693147180559945309417
define M_LN10      2.30258509299404568402
define M_PI        3.14159265358979323846
define M_PI_2      1.57079632679489661923
define M_PI_4      0.785398163397448309616
define M_1_PI      0.318309886183790671538
define M_2_PI      0.636619772367581343076
define M_2_SQRTPI  1.12837916709551257390
define M_SQRT2     1.41421356237309504880
define M_SQRT1_2   0.707106781186547524401

match =_exception@defined, _exception@defined\{
	.alignmode_cdecl
	struct _exception
		type 	rd 1
		name 	rptr 1
		arg1	rq 1
		arg2	rq 1
		retval	rq 1
	ends
	.alignmode_previous
\}
match =_complex@defined, _complex@defined\{
	.alignmode_cdecl
	struct _complex
		x rq 1
		y rq 1
	ends
	.alignmode_previous
\}
importlib msvcrt,\
	sin,\
	cos,\
	tan,\
	sinh,\
	cosh,\
	tanh,\
	asin,\
	acos,\
	atan,\
	atan2,\
	asinh,\
	acosh,\
	atanh,\
	exp,\
	log,\
	log10,\
	pow,\
	sqrt,\
	ceil,\
	floor,\
	fabs_='fabs',\ ; CHANGED
	ldexp,\
	frexp,\
	modf,\
	fdim,\
	fmod,\
	fmin,\
	fmax,\
	erf,\
	remainder,\
	remquo,\
	remquof,\
	lgamma,\
	tgamma,\
	_hypot,\
	_j0,\
	_j1,\
	_jn,\
	_y0,\
	_y1,\
	_yn,\
	cbrt,\
	erfc,\
	exp2,\
	expm1,\
	log1p,\
	log2,\
	logb,\
	rint,\
	round,\
	trunc,\
	cbrtf,\
	erfcf,\
	exp2f,\
	expm1f,\
	log1pf,\
	log2f,\
	logbf,\
	rintf,\
	roundf,\
	truncf,\
	ilogb,\
	ilogbf,\
	fmaf,\
	fma,\
	llrint,\
	llrintf,\
	llround,\
	llroundf,\
	lrint,\
	lrintf,\
	lround,\
	lroundf,\
	scalbn,\
	scalbnf,\
	scalbln,\
	scalblnf,\
	_copysign,\
	_chgsign,\
	_scalb,\
	_logb,\
	_nextafter,\
	_finite,\
	_isnan,\
	_fpclass,\
	nextafter,\
	nearbyint,\
	nearbyintf,\
	_hypotf,\
	_matherr,\
	_cabs,\
	_dtest,\
	_ldtest,\
	_fdtest,\
	_dsign,\
	_ldsign,\
	_fdsign,\
	_dpcomp,\
	_ldpcomp,\
	_fdpcomp,\
	nan,\
	nanf

match =x64, __architecture\{

importlib msvcrt,\
	sinf,\
	cosf,\
	tanf,\
	sinhf,\
	coshf,\
	tanhf,\
	asinf,\
	acosf,\
	atanf,\
	atan2f,\
	atanhf,\
	expf,\
	logf,\
	log10f,\
	powf,\
	sqrtf,\
	ceilf,\
	floorf,\
	modff,\
	fmodf,\
	_finitef,\
	_isnanf,\
	_fpclassf,\
	frexpf,\
	_chgsignf,\
	_copysignf,\
	_logbf,\
	acoshf,\
	asinhf,\
	atanhf,\
	erff,\
	fdimf,\
	fmaxf,\
	fminf,\
	lgammaf,\
	nextafterf,\
	remainderf,\
	tgammaf
\}

match =x86, __architecture\{

local sinf_, _sinf_
if used sinf
	_sinf_ @const dd sinf_
	virtual at _sinf_
		sinf rd 1
	end virtual
end if
.proc cdecl sinf_(.x:DWORD)
	fld [.x]
	$call c [sin](double st0)
	ret
.endp

local cosf_, _cosf_
if used cosf
	_cosf_ @const dd cosf_
	virtual at _cosf_
		cosf rd 1
	end virtual
end if
.proc cdecl cosf_(.x:DWORD)
	fld [.x]
	$call c [cos](double st0)
	ret
.endp

local tanf_, _tanf_
if used tanf
	_tanf_ @const dd tanf_
	virtual at _tanf_
		tanf rd 1
	end virtual
end if
.proc cdecl tanf_(.x:DWORD)
	fld [.x]
	$call c [tan](double st0)
	ret
.endp

local sinhf_, _sinhf_
if used sinhf
	_sinhf_ @const dd sinhf_
	virtual at _sinhf_
		sinhf rd 1
	end virtual
end if
.proc cdecl sinhf_(.x:DWORD)
	fld [.x]
	$call c [sinh](double st0)
	ret
.endp

local coshf_, _coshf_
if used coshf
	_coshf_ @const dd coshf_
	virtual at _coshf_
		coshf rd 1
	end virtual
end if
.proc cdecl coshf_(.x:DWORD)
	fld [.x]
	$call c [cosh](double st0)
	ret
.endp

local tanhf_, _tanhf_
if used tanhf
	_tanhf_ @const dd tanhf_
	virtual at _tanhf_
		tanhf rd 1
	end virtual
end if
.proc cdecl tanhf_(.x:DWORD)
	fld [.x]
	$call c [tanh](double st0)
	ret
.endp

local asinf_, _asinf_
if used asinf
	_asinf_ @const dd asinf_
	virtual at _asinf_
		asinf rd 1
	end virtual
end if
.proc cdecl asinf_(.x:DWORD)
	fld [.x]
	$call c [asin](double st0)
	ret
.endp

local acosf_, _acosf_
if used acosf
	_acosf_ @const dd acosf_
	virtual at _acosf_
		acosf rd 1
	end virtual
end if
.proc cdecl acosf_(.x:DWORD)
	fld [.x]
	$call c [acos](double st0)
	ret
.endp

local atanf_, _atanf_
if used atanf
	_atanf_ @const dd atanf_
	virtual at _atanf_
		atanf rd 1
	end virtual
end if
.proc cdecl atanf_(.x:DWORD)
	fld [.x]
	$call c [atan](double st0)
	ret
.endp

local atan2f_, _atan2f_
if used atan2f
	_atan2f_ @const dd atan2f_
	virtual at _atan2f_
		atan2f rd 1
	end virtual
end if
.proc cdecl atan2f_(.x:DWORD)
	fld [.x]
	$call c [atan2](double st0)
	ret
.endp

local expf_, _expf_
if used expf
	_expf_ @const dd expf_
	virtual at _expf_
		expf rd 1
	end virtual
end if
.proc cdecl expf_(.x:DWORD)
	fld [.x]
	$call c [exp](double st0)
	ret
.endp

local logf_, _logf_
if used logf
	_logf_ @const dd logf_
	virtual at _logf_
		logf rd 1
	end virtual
end if
.proc cdecl logf_(.x:DWORD)
	fld [.x]
	$call c [log](double st0)
	ret
.endp

local log10f_, _log10f_
if used log10f
	_log10f_ @const dd log10f_
	virtual at _log10f_
		log10f rd 1
	end virtual
end if
.proc cdecl log10f_(.x:DWORD)
	fld [.x]
	$call c [log10](double st0)
	ret
.endp

local powf_, _powf_
if used powf
	_powf_ @const dd powf_
	virtual at _powf_
		powf rd 1
	end virtual
end if
.proc cdecl powf_(.x:DWORD)
	fld [.x]
	$call c [pow](double st0)
	ret
.endp

local sqrtf_, _sqrtf_
if used sqrtf
	_sqrtf_ @const dd sqrtf_
	virtual at _sqrtf_
		sqrtf rd 1
	end virtual
end if
.proc cdecl sqrtf_(.x:DWORD)
	fld [.x]
	$call c [sqrt](double st0)
	ret
.endp

local ceilf_, _ceilf_
if used ceilf
	_ceilf_ @const dd ceilf_
	virtual at _ceilf_
		ceilf rd 1
	end virtual
end if
.proc cdecl ceilf_(.x:DWORD)
	fld [.x]
	$call c [ceil](double st0)
	ret
.endp

local floorf_, _floorf_
if used floorf
	_floorf_ @const dd floorf_
	virtual at _floorf_
		floorf rd 1
	end virtual
end if
.proc cdecl floorf_(.x:DWORD)
	fld [.x]
	$call c [floor](double st0)
	ret
.endp

local modff_, _modff_
if used modff
	_modff_ @const dd modff_
	virtual at _modff_
		modff rd 1
	end virtual
end if
.proc cdecl modff_(.x:DWORD)
	fld [.x]
	$call c [modf](double st0)
	ret
.endp

local fmodf_, _fmodf_
if used fmodf
	_fmodf_ @const dd fmodf_
	virtual at _fmodf_
		fmodf rd 1
	end virtual
end if
.proc cdecl fmodf_(.x:DWORD)
	fld [.x]
	$call c [fmod](double st0)
	ret
.endp
local expf_, _expf_
if used expf
	_expf_ @const dd expf_
	virtual at _expf_
		expf rd 1
	end virtual
end if
.proc cdecl expf_(.x:DWORD)
	fld [.x]
	$call c [exp](double st0)
	ret
.endp

local logf_, _logf_
if used logf
	_logf_ @const dd logf_
	virtual at _logf_
		logf rd 1
	end virtual
end if
.proc cdecl logf_(.x:DWORD)
	fld [.x]
	$call c [log](double st0)
	ret
.endp

local sqrtf_, _sqrtf_
if used sqrtf
	_sqrtf_ @const dd sqrtf_
	virtual at _sqrtf_
		sqrtf rd 1
	end virtual
end if
.proc cdecl sqrtf_(.x:DWORD)
	fld [.x]
	$call c [sqrt](double st0)
	ret
.endp

local ceilf_, _ceilf_
if used ceilf
	_ceilf_ @const dd ceilf_
	virtual at _ceilf_
		ceilf rd 1
	end virtual
end if
.proc cdecl ceilf_(.x:DWORD)
	fld [.x]
	$call c [ceil](double st0)
	ret
.endp

local log10f_, _log10f_
if used log10f
	_log10f_ @const dd log10f_
	virtual at _log10f_
		log10f rd 1
	end virtual
end if
.proc cdecl log10f_(.x:DWORD)
	fld [.x]
	$call c [log10](double st0)
	ret
.endp

local floorf_, _floorf_
if used floorf
	_floorf_ @const dd floorf_
	virtual at _floorf_
		floorf rd 1
	end virtual
end if
.proc cdecl floorf_(.x:DWORD)
	fld [.x]
	$call c [floor](double st0)
	ret
.endp

local _finitef_, __finitef_
if used _finitef
	__finitef_ @const rd _finitef_
	virtual at __finitef_
		_finitef rd ?
	end virtual
end if
.proc cdecl _finitef_(.x:DWORD)
	fld [.x]
	$call c [_finite](double st0)
	ret
.endp

local _isnanf_, __isnanf_
if used _isnanf
	__isnanf_ @const rd _isnanf_
	virtual at __isnanf_
		_isnanf rd ?
	end virtual
end if
.proc cdecl _isnanf_(.x:DWORD)
	fld [.x]
	$call c [_isnan](double st0)
	ret
.endp

local atan2f_, _atan2f_
if used atan2f
	_atan2f_ @const dd atan2f_
	virtual at _atan2f_
		atan2f rd 1
	end virtual
end if
.proc cdecl atan2f_(.x:DWORD, .y:DWORD)
	fld [.x]
	fld [.y]
	$call c [atan2](double st0, double st0)
	ret
.endp

local powf_, _powf_
if used powf
	_powf_ @const dd powf_
	virtual at _powf_
		powf rd 1
	end virtual
end if
.proc cdecl powf_(.x:DWORD, .y:DWORD)
	fld [.x]
	fld [.y]
	$call c [pow](double st0, double st0)
	ret
.endp

local fmodf_, _fmodf_
if used fmodf
	_fmodf_ @const dd fmodf_
	virtual at _fmodf_
		fmodf rd 1
	end virtual
end if
.proc cdecl fmodf_(.x:DWORD, .y:DWORD)
	fld [.x]
	fld [.y]
	$call c [fmod](double st0, double st0)
	ret
.endp

local modff_, _modff_
if used modff
	_modff_ @const dd modff_
	virtual at _modff_
		modff rd 1
	end virtual
end if
.proc cdecl modff_(.x:DWORD, .y:POINTER)
	.local .yd:QWORD
	fld [.x]
	$call c [modff](double st0, &.yd)
	mov pax, [.y]
	fld [.yd]
	fstp dword[pax]
	ret
.endp

local _fpclassf_, __fpclassf_
if used _fpclassf
	__fpclassf_ @const dd _fpclassf_
	virtual at __fpclassf_
		_fpclassf rd 1
	end virtual
end if
.proc cdecl _fpclassf_(.x:DWORD)
	mov eax, [.x]
	fld [.x]
	mov edx, eax
	mov ecx, eax
	; movd xmm0, eax
	@block
		shr eax, 23
		test eax, 0xFF
			jnz @fb
		shl edx, 1
		test edx, edx
			jz @fb

		.local .id:QWORD
		mov edx, ecx
		and edx, 0x80000000
		mov eax, 1
		mov dword[.id], eax
		mov dword[.id + 4], edx
		fstp st0
		fld [.id]
	@endb
	$call c [_fpclass](double st0)
	ret
.endp
; static inline int _fpclassf(float x)
; {
;     unsigned int ix = *(int*)&x;
;     double d = x;

;     /* construct denormal double */
;     if (!(ix >> 23 & 0xff) && (ix << 1))
;     {
;         unsigned __int64 id = (((unsigned __int64)ix >> 31) << 63) | 1;
;         d = *(double*)&id;
;     }
;     return _fpclass(d);
; }

local frexpf_, _frexpf_
if used frexpf
	_frexpf_ @const dd frexpf_
	virtual at _frexpf_
		frexpf rd 1
	end virtual
end if
.proc cdecl frexpf_(.x:DWORD)
	fld [.x]
	$call c [frexp](double st0)
	ret
.endp

local fabsf_, _fabsf_
if used fabsf
	_fabsf_ @const dd fabsf_
	virtual at _fabsf_
		fabsf rd 1
	end virtual
end if
.proc cdecl fabsf_(.x:DWORD)
	fld [.x]
	$call c [fabs](double st0)
	ret
.endp

local _chgsignf_, __chgsignf_
if used _chgsignf
	__chgsignf_ @const dd _chgsignf_
	virtual at __chgsignf_
		_chgsignf rd 1
	end virtual
end if
.proc cdecl _chgsignf_(.x:DWORD)
	fld [.x]
	$call c [_chgsign](double st0)
	ret
.endp

local _copysignf_, __copysignf_
if used _copysignf
	__copysignf_ @const dd _copysignf_
	virtual at __copysignf_
		_copysignf rd 1
	end virtual
end if
.proc cdecl _copysignf_(.x:DWORD)
	fld [.x]
	$call c [_copysign](double st0)
	ret
.endp

local _logbf_, __logbf_
if used _logbf
	__logbf_ @const dd _logbf_
	virtual at __logbf_
		_logbf rd 1
	end virtual
end if
.proc cdecl _logbf_(.x:DWORD)
	fld [.x]
	$call c [_logb](double st0)
	ret
.endp

\}

local fabsf_, _fabsf_
if used fabsf
	_fabsf_ @const dptr fabsf_
	virtual at _fabsf_
		fabsf rptr 1
	end virtual
end if
.proc cdecl fabsf_(.x:DWORD)
	match =x64, __architecture\{
		@larg xmm0, @farg1
		cvtss2sd xmm0, xmm0
		$call c [fabs_](double xmm0)
		cvtsd2ss xmm0, xmm0
	\}
	match=x86, __architecture\{
		fld [.x]
		$call c [fabs_](double st0)
	\}
	ret
.endp

local ldexpf_, _ldexpf_
if used ldexpf
	_ldexpf_ @const dptr ldexpf_
	virtual at _ldexpf_
		ldexpf rptr 1
	end virtual
end if
.proc cdecl ldexpf_(.x:DWORD, .y:DWORD)
	match =x64, __architecture\{
		cvtss2sd xmm0, xmm0
		$call c [ldexp](double xmm0, rdx)
		cvtsd2ss xmm0, xmm0
	\}
	match =x86, __architecture\{
		fld [.x]
		$call c [ldexp](double st0, [.y])
	\}
	ret
.endp

define copysign _copysign
define copysignf _copysignf

local isinff_, _isinff_
if used isinff
	_isinff_ @const dptr isinff_
	virtual at _isinff_
		isinff rptr 1
	end virtual
end if
.proc cdecl isinff_(.x:DWORD)
	@larg pax, @arg1
	and eax, 0x7fffffff
	cmp eax, 0x7f800000
	sete al
	and eax, 0xFF
	ret
.endp

local isinf_, _isinf_
if used isinf
	_isinf_ @const dptr isinf_
	virtual at _isinf_
		isinf rptr 1
	end virtual
end if
.proc cdecl isinf_(.x:QWORD)
	@larg xmm0, @farg1
	
	@const_align equ 16
	.unsignedMask 	@const dq not (1 shl 63)
	.infVal 		@const dq 0x7ff shl 52
	restore @const_align

	pand xmm0, xword[.unsignedMask]
	pcmpeqq xmm0, xword[.infVal]
	movd eax, xmm0
	ret
.endp

local isnormalf_, _isnormalf_
if used isnormalf
	_isnormalf_ @const dptr isnormalf_
	virtual at _isnormalf_
		isnormalf rptr 1
	end virtual
end if
.proc cdecl isnormalf_(.x:DWORD)
	@larg pax, @arg1
	add eax, 0x00800000
	and eax, 0x7fffffff
	cmp eax, 0x01000000
	setae al
	and eax, 0xFF
	ret
.endp

local isnormal_, _isnormal_
if used isnormal
	_isnormal_ @const dptr isnormal_
	virtual at _isnormal_
		isnormal rptr 1
	end virtual
end if
.proc cdecl isnormal_(.x:QWORD)
	@larg xmm0, @farg1
	
	@const_align equ 16
	.incConst 		@const dq 1 shl 52
	.unsignedMask 	@const dq not (1 shl 63)
	.normalBound	@const dq 1 shl 53
	restore @const_align

	paddq xmm0, xword[.incConst]
	pand xmm0, xword[.unsignedMask]
	movq xmm1, xmm0
	pcmpgtq xmm0, xword[.normalBound]
	movq xmm0, xmm0
	ptest xmm0, xmm0
	@block jnz @fb
		pcmpeqq xmm1, xword[.normalBound]
		movq xmm0, xmm1
	@endb
	movd eax, xmm0
	ret
.endp

local signbitf_, _signbitf_
if used signbitf
	_signbitf_ @const dptr signbitf_
	virtual at _signbitf_
		signbitf rptr 1
	end virtual
end if
.proc cdecl signbitf_(.x:DWORD)
	@larg pax, @arg1
	shr eax, 31
	ret
.endp

local signbit_, _signbit_
if used signbit
	_signbit_ @const dptr signbit_
	virtual at _signbit_
		signbit rptr 1
	end virtual
end if
.proc cdecl signbit_(.x:QWORD)
	@larg xmm0, @farg1
	psrlq xmm0, 63
	movd eax, xmm0
	ret
.endp


define isnanf  _isnanf
define isnan  _isnan

local isfinitef_, _isfinitef_
if used isfinitef
	_isfinitef_ @const dptr isfinitef_
	virtual at _isfinitef_
		isfinitef rptr 1
	end virtual
end if
.proc cdecl isfinitef_(.x:DWORD)
	@sarg @farg1
	$call c isinff_(float @farg1)
	test eax, eax
	@block jz @fb
		$call c isnanf_(float [.x])
		test eax, eax
	@endb
	setz eax
	and eax, 0xFF
	ret
.endp

local isfinite_, _isfinite_
if used isfinite
	_isfinite_ @const dptr isfinite_
	virtual at _isfinite_
		isfinite rptr 1
	end virtual
end if
.proc cdecl isfinite_(.x:QWORD)
	@sarg @farg1
	$call c isinf_(double @farg1)
	test eax, eax
	@block jz @fb
		$call c isnan_(double [.x])
		test eax, eax
	@endb
	setz eax
	and eax, 0xFF
	ret
.endp

define hypot 	_hypot
define j0 		_j0
define j1 		_j1
define jn 		_jn
define y0 		_y0
define y1 		_y1
define yn 		_yn
define hypotf 	_hypotf

restore @const_align

.proc_frame_mode_previous

@endm
