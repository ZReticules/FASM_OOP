
macro @push [args]{
	push args
	common
	_lastargs_ equ args
}

macro @pop [args]{
	reverse
	pop args
	common
	restore _lastargs_
}

macro @popl{
	@pop _lastargs_
	common
	restore _lastargs_
}