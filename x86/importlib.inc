total@libArr equ "0"
macro importlib @thislib*, [$procfull]{
	common
	local newLib, @libname, @libstr, matched
	@libname equ @thislib
	@libstr equ `@thislib#".dll"
	match @_lib==@_str, @thislib\{
		restore @libname, @libstr
		@libname equ @_lib
		@libstr equ @_str
	\}
	match libname libstr, @libname @libstr\{
		irpv libs, total@libs\\{
			if ~(libs eq libname)
				define newLib
			end if
		\\}
		match ="TRUE", libname\#LibIsAdded\\{
			if ~(libstr eq libname\#@libStr)
				display "Library string conflict. Library ",\`libname, " tries to use ",\`libstr," instead of ", libname#@libStr
				err
			end if
		\\}
		match =libname\#LibIsAdded libarr@Buf, libname\#LibIsAdded total@libArr\\{
			restore total@libArr
			total@libArr equ libarr@Buf, libname, libstr
			match ="0", libarr@Buf\\\{
				restore total@libArr
				total@libArr equ libname, libstr
			\\\}
			libname\#@libStr equ libstr
			libname\#LibIsAdded equ "TRUE"
			libname\#@procArr equ libname
		\\}
		forward
		local @$proc, @$procstr
		@$proc equ $procfull
		@$procstr equ `$procfull
		match @_proc==@_str, $procfull\\{
			restore @$proc, @$procstr
			@$proc equ @_proc
			@$procstr equ @_str
		\\}
		match $proc $procstr, @$proc @$procstr\\{
			match ="TRUE", $proc\\#ProcIsAdded\\\{
				if ~($procstr eq $proc\\#@prcStr)
					display "Procedure string conflict. Procedure ",\\`$proc, " tries to use ",\\`$procstr," instead of ", $proc\\#@prcStr
					err
				end if
				if ~(libname eq $proc\\#@LibName)
					match _procname@fromlib, $proc\\#@LibName\\\\{
						display "Procedure library conflict. Trying to import ",\\`$proc, " from ",\\`libname," instead of ", \\\\`_procname@fromlib
						err
					\\\\}
				end if
			\\\}
			match =$proc\\#ProcIsAdded procarr@Buf, $proc\\#ProcIsAdded libname\#@procArr\\{
				restore libname\#@procArr
				libname\#@procArr equ procarr@Buf, $proc, $procstr
				$proc\\#@prcStr equ $procstr
				$proc\\#ProcIsAdded equ "TRUE"
				$proc\\#@LibName equ libname
			\\\}
		\\}
		common
	\}
}

macro makeimport{
	common
	local buf@libmatch, buf@procmatch
	match buf@libmatch, total@libArr\{
		library buf@libmatch
		irp libname, buf@libmatch\\{
			if ~(libname eqtype "")
				match buf@procmatch, libname\\#@procArr\\\{
					import buf@procmatch
				\\\}
			end if
		\\}
	\}
}

postpone{
	data import
		makeimport
	end data
}