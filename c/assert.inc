@module cassert

importlib msvcrt,\
	_assert, _wassert

; CHANGED: assert->cassert, no $call, just `assert exp`

match =NDEBUG, NDEBUG\{
	\local result_, smb, conc_line
	macro to_text result, line&\\{
		result_ equ
		irps smb, line\\\{
			match , result_\\\\{
				result_ equ \\\`smb 
			rept 0\\\\{\\\\}rept 1\\\\{
				result_ equ result_, ' ', \\\`smb 
			\\\\}
		\\\}
		result equ result_
	\\}
	macro cassert _expr&\\{
		\\local text_expr
		to_text text_expr, _expr
		.if ~(_expr)
			$call c [_assert](<db text_expr, 0>, <db __FILE__, 0>, __LINE__)
		.endif
	\\}
	macro cwassert _expr&\\{
		\\local text_expr
		to_text text_expr, _expr
		.if ~(_expr)
			$call c [_wassert](<du text_expr, 0>, <du __FILE__, 0>, __LINE__)
		.endif
	\\}
rept 0\{\}rept 1\{
	macro cassert _expr&\\{\\}
	macro cwassert _expr&\\{\\}
\}

@endm
