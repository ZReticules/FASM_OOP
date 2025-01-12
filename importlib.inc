total@libArr equ "0"
macro importlib @thislib*, [$procfull*]{
	common
	local newLib, @libname, @libstr, matched
	match @_lib==@_str, @thislib\{
		define matched
		@libname equ @_lib
		@libstr equ @_str
	\}
	match =matched, matched\{
		@libname equ @thislib
		@libstr equ `@thislib#".dll"
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
		if ~($procfull eq)
			forward
			local fmathced, @$proc, @$procstr
			match @_proc==@_str, $procfull\\{
				define fmatched
				@$proc equ @_proc
				@$procstr equ @_str
			\\}
			match =fmatched, fmatched\{
				@$proc equ $procfull
				@$procstr equ `$procfull
			\\}
			restore fmatched
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
		else
			display "Warning: There is no procedures from lib!"
		end if
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