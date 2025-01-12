total@includs equ
macro makeinclude{
	macro include_once argum&\{
		\makeinclude
		local flag
		irpv i, total@includs\\{
			match =argum, i\\\{
				define flag
			\\\}
		\\}
		match =flag, flag\\{
			include argum
			total@includs equ argum
		\\}
	\}
}

makeinclude