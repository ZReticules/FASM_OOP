; total@libArr equ "0"
; macro importlib @thislib*, [$procfull*]{
; 	common
; 	local newLib, @libname, @libstr, matched
; 	match @_lib==@_str, @thislib\{
; 		define matched
; 		@libname equ @_lib
; 		@libstr equ @_str
; 	\}
; 	match =matched, matched\{
; 		@libname equ @thislib
; 		@libstr equ `@thislib#".dll"
; 	\}
; 	match libname libstr, @libname @libstr\{
; 		irpv libs, total@libs\\{
; 			if ~(libs eq libname)
; 				define newLib
; 			end if
; 		\\}
; 		match ="TRUE", libname\#LibIsAdded\\{
; 			if ~(libstr eq libname\#@libStr)
; 				display "Library string conflict. Library ",\`libname, " tries to use ",\`libstr," instead of ", libname#@libStr
; 				err
; 			end if
; 		\\}
; 		match =libname\#LibIsAdded libarr@Buf, libname\#LibIsAdded total@libArr\\{
; 			restore total@libArr
; 			total@libArr equ libarr@Buf, libname, libstr
; 			match ="0", libarr@Buf\\\{
; 				restore total@libArr
; 				total@libArr equ libname, libstr
; 			\\\}
; 			libname\#@libStr equ libstr
; 			libname\#LibIsAdded equ "TRUE"
; 			libname\#@procArr equ libname
; 		\\}
; 		if ~($procfull eq)
; 			forward
; 			local fmathced, @$proc, @$procstr
; 			match @_proc==@_str, $procfull\\{
; 				define fmatched
; 				@$proc equ @_proc
; 				@$procstr equ @_str
; 			\\}
; 			match =fmatched, fmatched\{
; 				@$proc equ $procfull
; 				@$procstr equ `$procfull
; 			\\}
; 			restore fmatched
; 			match $proc $procstr, @$proc @$procstr\\{
; 				match ="TRUE", $proc\\#ProcIsAdded\\\{
; 					if ~($procstr eq $proc\\#@prcStr)
; 						display "Procedure string conflict. Procedure ",\\`$proc, " tries to use ",\\`$procstr," instead of ", $proc\\#@prcStr
; 						err
; 					end if
; 					if ~(libname eq $proc\\#@LibName)
; 						match _procname@fromlib, $proc\\#@LibName\\\\{
; 							display "Procedure library conflict. Trying to import ",\\`$proc, " from ",\\`libname," instead of ", \\\\`_procname@fromlib
; 							err
; 						\\\\}
; 					end if
; 				\\\}
; 				match =$proc\\#ProcIsAdded procarr@Buf, $proc\\#ProcIsAdded libname\#@procArr\\{
; 					restore libname\#@procArr
; 					libname\#@procArr equ procarr@Buf, $proc, $procstr
; 					$proc\\#@prcStr equ $procstr
; 					$proc\\#ProcIsAdded equ "TRUE"
; 					$proc\\#@LibName equ libname
; 				\\\}
; 			\\}
; 			common
; 		else
; 			display "Warning: There is no procedures from lib!"
; 		end if
; 	\}
; }

; macro @init_import{
; 	match =@init_import, @init_import\{
; 		local buf@libmatch, buf@procmatch
; 		match buf@libmatch, total@libArr\\{
; 			library buf@libmatch
; 			irp libname, buf@libmatch\\\{
; 				if ~(libname eqtype "")
; 					match buf@procmatch, libname\\\#@procArr\\\\{
; 						import buf@procmatch
; 					\\\\}
; 				end if
; 			\\\}
; 		\\}
; 		define @init_import
; 	\}
; }


; total_postpone equ @init_import_postpone

; macro @init_import_postpone{
; 	match =@init_import, @init_import\{
; 		data import
; 			@init_import
; 		end data
; 	\}
; }

; total@libArr equ

macro importlib library*, [procedure*]{
	common
	; local libname, libpath, libdef
	libdef equ library `library#".dll"
	match libname==libpath, library\{
		libdef equ libname libpath
	\}
	match libname libpath, libdef\{
		match , libname\#@lib_is_added\\{
			if ~(libpath eq libname\#@lib_path)
				display "Library path conflict. Library ",\`libname, " tries to use path ", libpath," instead of ", libname\#@lib_path
				err
			end if
		\\}
		match =libname\#@lib_is_added, libname\#@lib_is_added\\{
			define libname\#@lib_is_added
			libname\#@lib_path equ libpath
			libname\#@proc_arr equ libname

			total@lib_arr equ libname, libpath
		\\}
		forward
		\local procname, proclabel, procdef
		procdef equ procedure `procedure
		match procname == proclabel, procedure\\{
			procdef equ procname proclabel
		\\}
		match procname proclabel, procdef\\{
			match , procname\\#@proc_is_added\\\{
				if ~(proclabel eq procname\\#@proc_label)
					display "Procedure name conflict. Procedure ",\\`procname, " redifinition as ", \\`proclabel
					display ", already defined as ", procname\\#@proc_label
					err
				end if
				match prevname, procname\\#@lib_name\\\\{
					if ~(\`libname eq \\\\`prevname)
						display "Procedure library conflict. Attempt to import ",\\`procname, " from ", \`libname
						display ", already imported from ", \\\\`prevname
						err
					end if
				\\\\}
			rept 0\\\{\\\}rept 1\\\{
				define procname\\#@proc_is_added
				procname\\#@proc_label equ proclabel
				procname\\#@lib_name equ libname

				libname\#@proc_arr equ procname, proclabel
			\\\}
		\\}
		common
	\}
}

macro @init_import{
	match =@init_import, @init_import\{
		local libs, libname, libpath
		irpv libs, total@lib_arr\\{
			\\common
			library libs
			\\forward
			match libname=, libpath, libs\\\{
				irpv procedures, libname\\\#@proc_arr\\\\{
					\\\\common
					import procedures
				\\\\}
			\\\}
		\\}
		define @init_import
	\}
}

total_postpone equ @init_import_postpone

macro @init_import_postpone{
	match =@init_import, @init_import\{
		match =total@lib_arr, total@lib_arr\\{
		rept 0\\{\\}rept 1\\{
			data import
				@init_import
			end data
		\\}
	\}
}