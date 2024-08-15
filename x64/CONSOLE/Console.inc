include_once "TOOLS\cstdio.inc"

struct Console
	setCurPos 	dm this
	hideCur		dm this
	showCur		dm this
	changeCur	dm this
	saveCurPos	dm this
	restCurPos	dm this
	curMove		dm this
	const UP = 'A'
	const DOWN = 'B'
	const RIGHT = 'C'
	const LEFT = 'D'
	db ?
ends

include "Console.asm"
