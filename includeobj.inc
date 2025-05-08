@module includeobj_m
	match , \{
		\local ..last_section
		..last_section::
		last@section equ ..last_section
	\}

	macro section args&\{
		section args
		\local ..last_section
		..last_section::
		last@section equ ..last_section
	\}

	macro @iobj_dbg_show_all\{
		define IOBJ_DBG
		define IOBJ_DBG_SHOW_STATIC
		define IOBJ_DBG_SHOW_PUBLIC
		define IOBJ_DBG_SHOW_DEBUG
		define IOBJ_DBG_SHOW_EXTRN
		define IOBJ_DBG_SHOW_ABS
		define IOBJ_DBG_SHOW_RELOCS
	\}

	macro @iobj_dbg_hide_all\{
		restore IOBJ_DBG, IOBJ_DBG_SHOW_STATIC, IOBJ_DBG_SHOW_PUBLIC, IOBJ_DBG_SHOW_DEBUG
		restore IOBJ_DBG_SHOW_EXTRN, IOBJ_DBG_SHOW_ABS, IOBJ_DBG_SHOW_RELOCS
	\}

	virtual at 0
		externals::
		externals.size = $
	end virtual

	; externs offsets
	EXTRN_LEN 	= 0  				; WORD
	EXTRN_USES 	= 2  				; WORD
	EXTRN_ADDR 	= 4  				; POINTER
	EXTRN_BYTES = 4 + pointer.size 	; rb [EXTRN_LEN]

	macro public lab_smb\{
		\local smb_len, smb_glob, smb_offset, smb_val, _char, _dest
		match lab =from virt_lab =size num =offset shift =into dest, lab_smb\\{
			\\local i
			smb_glob equ virt_lab
			smb_offset = shift
			smb_len = num
			smb_val equ (lab)
			_dest equ dest
		rept 0\\{\\} rept 1\\{
			\\local _symbols
			smb_val equ (lab_smb)
			_symbols equ \`lab_smb
			match lab =as symbols, lab_smb\\\{
				smb_val equ (lab)
				_symbols equ symbols
			\\\}
			\local ..start, ..end, ..symbol_start, ..str_table
			virtual at 0
				..str_table::
				..start:
				db _symbols
				..end:
			end virtual
			smb_glob equ ..str_table
			smb_offset = 0
			smb_len = ..end - ..start
		\\}
		\local i, succseded, offset, in_size, smb_table, smb_find, use_count, smb_find_val
		offset = 0
		i = 0
		succseded = 0
		while ~succseded & offset < externals.size & externals.size <> 0
			load in_size word from externals:offset + EXTRN_LEN
			; %display_ascii externals, offset + 2 + pointer.size, in_size
			if in_size = smb_len
				i = smb_len
				succseded = 1
				while i
					load smb_table byte from externals:offset + EXTRN_BYTES + i - 1
					load smb_find byte from smb_glob:smb_offset + i - 1
					if smb_find <> smb_table
						succseded = 0
						break
					end if
					i = i - 1
				end while
				if succseded
					load smb_find_val dword from externals:offset + EXTRN_ADDR
					_dest = smb_find_val
					if smb_find_val <> smb_val
						display "public symbol " 
						%display_ascii externals, externals.size - smb_len, smb_len
						display " already defined with different value", 0Ah
						assert 0
					else
						break
					end if
				end if
			end if
			offset = offset + EXTRN_BYTES + in_size
		end while
		if ~succseded
			i = 0
			virtual externals
				_dest = $
				dw smb_len
				dw 0
				dptr smb_val
				repeat smb_len
					load _char byte from smb_glob:smb_offset + % - 1
					db _char
					i = i + 1
				end repeat
				externals.size = $
			end virtual
		end if
	\}

	; obj@externs

	macro check@externs lab_name, is_used\{
		\local _count, _ext_offset, use_count, anon, len
		load _count dword from lab_name:0
		repeat _count
			load _ext_offset pointer from lab_name:4 + (% - 1) * pointer.size
			load use_count word from externals:_ext_offset + EXTRN_USES
			if use_count
				anon = is_used
				break
			end if
		end repeat
	\}

	macro check@externs\{
		irpv anon_lab, obj@externs\\{
			check@externs anon_lab
		\\}
	\}

	total_postpone equ check@externs

	macro extern@now lab_smb&\{
		\local str_size, str_table, str_offset, _label, matched
		match lab =from virt_lab =size num =offset shift, lab_smb\\{
			; define matched
			_label equ lab
			str_table equ virt_lab
		if used _label
			str_size = num
			str_offset = shift
		rept 0\\{\\}rept 1\\{ 
		; match =matched, matched\\{
			\\local _symbols, str_len, ..str_bytes, ..start, ..end
			_label equ lab_smb
			_symbols equ \`lab_smb
			match symbols =as lab, lab_smb\\\{
				_label equ lab
				_symbols equ symbols
			\\\}
		if used _label
			virtual at 0
				..str_bytes::
				..start:
				db _symbols
				..end:
				str_size = ..end - ..start
			end virtual
			str_table equ ..str_bytes
			str_offset = 0
		\\}
			\local i, succseded, offset, in_size, smb_table, smb_find, use_count
			offset = 0
			i = 0
			succseded = 0
			while ~succseded & offset < externals.size
				load in_size word from externals:offset + EXTRN_LEN
				; %display_ascii externals, offset + 2 + pointer.size, in_size
				if in_size = str_size
					i = str_size
					succseded = 1
					while i
						load smb_table byte from externals:offset + EXTRN_BYTES + i - 1
						load smb_find byte from str_table:str_offset + i - 1
						if smb_find <> smb_table
							succseded = 0
							break
						end if
						i = i - 1
					end while
					if succseded
						load use_count word from externals:offset + EXTRN_USES
						store word (use_count + 1) at externals:offset + EXTRN_USES
						load _label pointer from externals:offset + EXTRN_ADDR
						break
					end if
				end if
				offset = offset + EXTRN_BYTES + in_size
			end while
			if ~succseded
				display "external symbol " 
				%display_ascii str_table, str_offset, str_size
				display " not found", 0Ah
				assert 0
			end if
		end if
	\}

	macro calc@externs\{
		\local i
		i = 0
		irpv lab_smb, total@externs\\{
			extern@now lab_smb
		\\}
	\}

	macro extrn lab_smb\{
		total@externs equ lab_smb
	\}

	total_postpone equ calc@externs

	match =x86, __architecture\{
		obj_right_signature = 0x014c
	\}

	match =x64, __architecture\{
		obj_right_signature = 0x8664
	\}

	; obj file offsets
	OBJ_SIGNATURE 		= 0 	; word
	OBJ_COUNT_SECTIONS 	= 2 	; word
	OBJ_CREATION 		= 4  	; dword
	OBJ_SYMBOLS 		= 8  	; dword
	OBJ_SYMBOLS_COUNT 	= 12  	; dword
	OBJ_EXT_HEADER 		= 16  	; dword
	OBJ_FLAGS 			= 18 	; word
	OBJ_SECTIONS 		= 20  	; it`s offset

	; section offsets
	SEC_NAME 		= 0 	; rb 8
	SEC_VSIZE		= 8  	; dword
	SEC_RVA 		= 12  	; dword
	SEC_SIZE 		= 16  	; dword
	SEC_DATA 		= 20  	; dword
	SEC_RELOC 		= 24	; dword
	SEC_DEBUG		= 28	; dword
	SEC_RELOC_COUNT	= 32	; dword
	SEC_FLAGS 		= 36	; dword

	; symbol offsets
	SMB_NAME 		= 0 	; rb 8
	SMB_SEC_OFFSET 	= 8 	; dword
	SMB_SEC_NUM 	= 12 	; word
	SMB_TYPE 		= 14 	; word
	SMB_CLASS 		= 16 	; byte
	SMB_NAX 		= 17 	; byte

	; reloc table offsets
	RELOC_ADDR 		= 0 ; dword
	RELOC_SMB_INDEX = 4 ; dword
	RELOC_TYPE 		= 8 ; dword

	; size constants
	SMB_SIZE 		= 18
	SEC_IMG_SIZE 	= 40
	RELOC_SIZE 		= 10

	; symbols types
	IMAGE_SYM_CLASS_END_OF_FUNCTION 	= 0xFF
	IMAGE_SYM_CLASS_NULL 				= 0x00
	IMAGE_SYM_CLASS_AUTOMATIC 			= 0x01
	IMAGE_SYM_CLASS_EXTERNAL 			= 0x02
	IMAGE_SYM_CLASS_STATIC 				= 0x03
	IMAGE_SYM_CLASS_REGISTER 			= 0x04
	IMAGE_SYM_CLASS_EXTERNAL_DEF 		= 0x05
	IMAGE_SYM_CLASS_LABEL 				= 0x06
	IMAGE_SYM_CLASS_UNDEFINED_LABEL 	= 0x07
	IMAGE_SYM_CLASS_MEMBER_OF_STRUCT 	= 0x08
	IMAGE_SYM_CLASS_ARGUMENT 			= 0x09
	IMAGE_SYM_CLASS_STRUCT_TAG 			= 0x0A
	IMAGE_SYM_CLASS_MEMBER_OF_UNION 	= 0x0B
	IMAGE_SYM_CLASS_UNION_TAG 			= 0x0C
	IMAGE_SYM_CLASS_TYPE_DEFINITION 	= 0x0D
	IMAGE_SYM_CLASS_UNDEFINED_STATIC 	= 0x0E
	IMAGE_SYM_CLASS_ENUM_TAG 			= 0x0F
	IMAGE_SYM_CLASS_MEMBER_OF_ENUM 		= 0x10
	IMAGE_SYM_CLASS_REGISTER_PARAM 		= 0x11
	IMAGE_SYM_CLASS_BIT_FIELD 			= 0x12
	IMAGE_SYM_CLASS_RESERVED 			= 0x13
	IMAGE_SYM_CLASS_BLOCK 				= 0x14
	IMAGE_SYM_CLASS_FUNCTION 			= 0x20
	IMAGE_SYM_CLASS_END_OF_STRUCT 		= 0x21
	IMAGE_SYM_CLASS_FILE 				= 0x22
	IMAGE_SYM_CLASS_SECTION 			= 0x23
	IMAGE_SYM_CLASS_WEAK_EXTERNAL 		= 0x24
	IMAGE_SYM_CLASS_CLR_TOKEN 			= 0x25

	; x86 relocation types
	IMAGE_REL_I386_ABSOLUTE = 0x0000
	IMAGE_REL_I386_DIR16 	= 0x0001
	IMAGE_REL_I386_REL16 	= 0x0002
	IMAGE_REL_I386_DIR32 	= 0x0006
	IMAGE_REL_I386_DIR32NB 	= 0x0007
	IMAGE_REL_I386_SEG12 	= 0x0009
	IMAGE_REL_I386_SECTION 	= 0x000A
	IMAGE_REL_I386_SECREL 	= 0x000B
	IMAGE_REL_I386_TOKEN 	= 0x000C
	IMAGE_REL_I386_SECREL7 	= 0x000D
	IMAGE_REL_I386_REL32 	= 0x0014

	; x64 relocation types
	IMAGE_REL_AMD64_ADDR64		= 0x0001
	IMAGE_REL_AMD64_ADDR32		= 0x0002
	IMAGE_REL_AMD64_ADDR32NB	= 0x0003
	IMAGE_REL_AMD64_REL32		= 0x0004
	IMAGE_REL_AMD64_REL32_1		= 0x0005
	IMAGE_REL_AMD64_REL32_2		= 0x0006
	IMAGE_REL_AMD64_REL32_3		= 0x0007
	IMAGE_REL_AMD64_REL32_4		= 0x0008
	IMAGE_REL_AMD64_REL32_5		= 0x0009
	IMAGE_REL_AMD64_SECTION		= 0x000A
	IMAGE_REL_AMD64_SECREL		= 0x000B

	; obj section flags
	IMAGE_SCN_CNT_CODE					= 0x00000020
	IMAGE_SCN_CNT_INITIALIZED_DATA		= 0x00000040
	IMAGE_SCN_CNT_UNINITIALIZED_DATA	= 0x00000080
	IMAGE_SCN_LNK_INFO					= 0x00000200
	IMAGE_SCN_LNK_REMOVE				= 0x00000800
	IMAGE_SCN_LNK_COMDAT				= 0x00001000

	; PE section flags
	IMAGE_SCN_MEM_DISCARDABLE	= 0x02000000
	IMAGE_SCN_MEM_NOT_CACHED	= 0x04000000
	IMAGE_SCN_MEM_NOT_PAGED		= 0x08000000
	IMAGE_SCN_MEM_SHARED		= 0x10000000
	IMAGE_SCN_MEM_EXECUTE		= 0x20000000
	IMAGE_SCN_MEM_READ			= 0x40000000
	IMAGE_SCN_MEM_WRITE			= 0x80000000

	macro %get_name_smb strname, strlen, glob_lab, offset, strings_offset\{
		\local check_name, smb_string_offset, max_len
		load check_name dword from glob_lab:offset
		if check_name <> 0
			strname = offset
			%len_asciiz strlen, glob_lab, offset, 8
		else
			load smb_string_offset dword from glob_lab:offset + 4
			strname = strings_offset + smb_string_offset
			load max_len dword from glob_lab:strname - 4
			%len_asciiz strlen, glob_lab, strname, max_len
		end if
	\}

	macro %get_name_sec strname, strlen, glob_lab, offset, strings_offset\{
		\local check_name, smb_string_offset, max_len
		load check_name byte from glob_lab:offset
		if check_name <> "/"
			strname = offset
			%len_asciiz strlen, glob_lab, offset, 8
		else
			%ascii_to_dec smb_string_offset, glob_lab, (offset + 1), 7
			strname = strings_offset + smb_string_offset
			load max_len dword from glob_lab:strname - 4
			%len_asciiz strlen, glob_lab, strname, max_len
		end if
	\}

	macro calc@reloc global_file, sections_count, symbols_offset, symbols_count, strings_table, sec_bytes, is_used, obj_section\{
		; display last_byte
		if used is_used
			\local i
			i = 0
			; display "externals: ", 0Ah
			while i < symbols_count
				%get_name_smb offset_name_smb, name_len_smb, global_file, (symbols_offset + i * SMB_SIZE + SMB_NAME), strings_table
				load class_smb byte from global_file:symbols_offset + i * SMB_SIZE + SMB_CLASS
				load sec_smb word from global_file:symbols_offset + i * SMB_SIZE + SMB_SEC_NUM
				load offset_smb dword from global_file:symbols_offset + i * SMB_SIZE + SMB_SEC_OFFSET
				load nax_smb byte from global_file:symbols_offset + i * SMB_SIZE + SMB_NAX
				if sec_smb = 0
					; load sec_offset_smb dword from global_file:OBJ_SECTIONS + (sec_smb - 1) * SEC_IMG_SIZE + SEC_DATA
					; addr_smb = sec_offset_smb + offset_smb; - sec_bytes + bytes_start
					; display 9, "external: "
					; %display_ascii global_file, offset_name_smb, name_len_smb
					; display class_smb
					; display 0Ah
					extern@now addr_smb from global_file size name_len_smb offset offset_name_smb
					; __probus = addr_smb
					store pointer addr_smb at global_file:symbols_offset + i * SMB_SIZE + SMB_NAME
				end if 

				i = i + 1 + nax_smb
			end while
			match , IOBJ_DBG\\{
				display "sections", 0Ah
			\\}
			\local section_offset, offset_name_sec, name_len_sec, data_sec, size_sec, flags_sec, reloc_sec, reloc_count_sec
			\local offset_reloc, smb_index_reloc, type_reloc, real_addr_reloc, value_reloc
			repeat sections_count
				section_offset = OBJ_SECTIONS + (% - 1) * SEC_IMG_SIZE
				load data_sec dword from global_file:section_offset + SEC_DATA
				load size_sec dword from global_file:section_offset + SEC_SIZE
				load reloc_sec dword from global_file:section_offset + SEC_RELOC
				load reloc_count_sec dword from global_file:section_offset + SEC_RELOC_COUNT
				load flags_sec dword from global_file:section_offset + SEC_FLAGS

				if ~((flags_sec and IMAGE_SCN_LNK_INFO) | (flags_sec and IMAGE_SCN_LNK_REMOVE))
					match , IOBJ_DBG\\{
						%get_name_sec offset_name_sec, name_len_sec, global_file, section_offset, strings_table
						display 09
						%display_ascii global_file, offset_name_sec, name_len_sec
						display " size: "
						%display_num size_sec, 10
						display 0Ah
					\\}

					match , IOBJ_DBG IOBJ_DBG_SHOW_RELOCS\\{
						display 09h, 09h, "relocs: ", 0Ah
					\\}
					repeat reloc_count_sec
						; display "1"
						load offset_reloc dword from global_file:reloc_sec + (% - 1) * RELOC_SIZE + RELOC_ADDR
						load smb_index_reloc dword from global_file:reloc_sec + (% - 1) * RELOC_SIZE + RELOC_SMB_INDEX
						load type_reloc word from global_file:reloc_sec + (% - 1) * RELOC_SIZE + RELOC_TYPE
						
						load sec_smb word from global_file:symbols_offset + smb_index_reloc * SMB_SIZE + SMB_SEC_NUM
						if sec_smb = 0
							load addr_smb pointer from global_file:symbols_offset + smb_index_reloc * SMB_SIZE + SMB_NAME
						else if sec_smb < 0FFFEh
							load offset_smb dword from global_file:symbols_offset + smb_index_reloc * SMB_SIZE + SMB_SEC_OFFSET
							load sec_offset_smb dword from global_file:OBJ_SECTIONS + (sec_smb - 1) * SEC_IMG_SIZE + SEC_DATA
							addr_smb = sec_offset_smb + offset_smb; - sec_bytes + bytes_start
						else if sec_smb = 0xFFFF
							load addr_smb dword from global_file:symbols_offset + smb_index_reloc * SMB_SIZE + SMB_SEC_OFFSET
						end if
						
						real_addr_reloc = data_sec + offset_reloc; - sec_bytes + bytes_start
						load value_reloc dword from obj_section:real_addr_reloc
						match =x86, __architecture\\{
							if type_reloc = IMAGE_REL_I386_DIR32
								match , IOBJ_DBG IOBJ_DBG_SHOW_RELOCS\\\{
									display 09h, 09h, "a "
								\\\}
								store dword addr_smb + value_reloc at obj_section:real_addr_reloc
							else if type_reloc = IMAGE_REL_I386_REL32
								match , IOBJ_DBG IOBJ_DBG_SHOW_RELOCS\\\{
									display 09h, 09h, "r "
								\\\}
								store dword (addr_smb - (real_addr_reloc + 4) + value_reloc) at obj_section:real_addr_reloc
							else if type_reloc = IMAGE_REL_I386_DIR32NB
								match , IOBJ_DBG IOBJ_DBG_SHOW_RELOCS\\\{
									display 09h, 09h, "nb "
								\\\}
								store dword (RVA addr_smb + value_reloc) at obj_section:real_addr_reloc
							else
								display 09h, 09h, "u ", type_reloc
							end if
							match , IOBJ_DBG IOBJ_DBG_SHOW_RELOCS\\\{
								display "at 0x"
								%display_num addr_smb, 16
								display " ", 0Ah
							\\\}
						\\}
						match =x64, __architecture\\{
							if type_reloc = IMAGE_REL_AMD64_ADDR64
								match , IOBJ_DBG IOBJ_DBG_SHOW_RELOCS\\\{
									display 09h, 09h, "a64 "
								\\\}
								store qword addr_smb + value_reloc at obj_section:real_addr_reloc
							else if type_reloc = IMAGE_REL_AMD64_ADDR32
								match , IOBJ_DBG IOBJ_DBG_SHOW_RELOCS\\\{
									display 09h, 09h, "a32 "
								\\\}
								store dword (addr_smb and 0FFFFFFFFh) at obj_section:real_addr_reloc
							else if type_reloc = IMAGE_REL_AMD64_ADDR32NB
								match , IOBJ_DBG IOBJ_DBG_SHOW_RELOCS\\\{
									display 09h, 09h, "nb "
								\\\}
								store dword ((RVA addr_smb) and 0FFFFFFFFh) + value_reloc at obj_section:real_addr_reloc
							else if type_reloc = IMAGE_REL_AMD64_REL32
								match , IOBJ_DBG IOBJ_DBG_SHOW_RELOCS\\\{
									display 09h, 09h, "r "
								\\\}
								; display real_addr_reloc
								; %display_num (addr_smb - (real_addr_reloc + 4)), 16
								store dword (addr_smb - (real_addr_reloc + 4)) and 0FFFFFFFFh + value_reloc at obj_section:real_addr_reloc
							else if type_reloc >= IMAGE_REL_AMD64_REL32_1 & type_reloc <= IMAGE_REL_AMD64_REL32_5
								store (addr_smb and 0FFFFFFFFh) - (real_addr_reloc + 4 + (type_reloc - IMAGE_REL_AMD64_REL32_1)) at last_section:real_addr_reloc
								match , IOBJ_DBG IOBJ_DBG_SHOW_RELOCS\\\{
									display 09h, 09h, "a32_", "1" + (type_reloc - IMAGE_REL_AMD64_REL32_1), " "
								\\\}
							else
								display 09h, 09h, "u ", type_reloc
							end if
							match , IOBJ_DBG IOBJ_DBG_SHOW_RELOCS\\\{
								display "at 0x"
								%display_num addr_smb, 16
								display " ", 0Ah
							\\\}
						\\}
					end repeat
					match , IOBJ_DBG IOBJ_DBG_SHOW_RELOCS\{
						display 0Ah
					\\}
				end if
			end repeat
		end if
	\}

	macro calc@relocs\{
		irpv obj_info, total@relocs\\{
			calc@reloc obj_info
		\\}
	\}

	total_postpone equ calc@relocs

	macro includeobj fname\{
		\local ..bytes_start, ..is_used, is_used
		\local ..global_file
		match _fname =as _alias, fname\\{
			_alias = 0
			is_used equ _alias
		
			virtual at 0
				..global_file::
				file _fname
			end virtual
		rept 0\\{\\} rept 1\\{
			..is_used = 0
			is_used equ ..is_used

			virtual at 0
				..global_file::
				file fname
			end virtual
		\\}

		\local ..externs
		virtual at 0
			..externs::
			dd 0
		end virtual

		\local signature
		load signature word from ..global_file:OBJ_SIGNATURE
		if signature <> obj_right_signature
			display "Signature of object file '" # fname # "' is incorrect for this architecture"
			err
		end if

		\local sections_count, symbols_offset, symbols_count
		load sections_count word from ..global_file:OBJ_COUNT_SECTIONS
		load symbols_offset dword from ..global_file:OBJ_SYMBOLS
		load symbols_count dword from ..global_file:OBJ_SYMBOLS_COUNT

		\local strings_table
		strings_table = symbols_offset + symbols_count * SMB_SIZE

		\local sec_bytes
		sec_bytes = OBJ_SECTIONS + sections_count * SEC_IMG_SIZE

		\local btval
		..bytes_start:
		\local section_offset, data_sec, size_sec, flags_sec, addr_sec;, offset_name_sec, name_len_sec, reloc_sec, reloc_count_sec
		if used is_used
			repeat sections_count
				section_offset = OBJ_SECTIONS + (% - 1) * SEC_IMG_SIZE
				load data_sec dword from ..global_file:section_offset + SEC_DATA
				load size_sec dword from ..global_file:section_offset + SEC_SIZE
				load flags_sec dword from ..global_file:section_offset + SEC_FLAGS

				if ~(flags_sec and (IMAGE_SCN_LNK_INFO or IMAGE_SCN_LNK_REMOVE))
					if data_sec <> 0
						addr_sec = $
						repeat size_sec
							load btval from ..global_file:data_sec + % - 1
							db btval
						end repeat
					else
						@bss addr_sec rb size_sec
					end if
					store dword addr_sec at ..global_file:section_offset + SEC_DATA
				end if
			end repeat
		end if

		match , IOBJ_DBG\\{
			display "symbols", 0Ah
		\\}
		\local i, nax_smb, class_smb, sec_smb
		\local addr_smb, offset_name_smb, name_len_smb
		\local sec_name_smb, sec_name_len_smb, eq_names, sec_offset_smb
		\local public_offset
		i = 0
		while i < symbols_count
			%get_name_smb offset_name_smb, name_len_smb, ..global_file, (symbols_offset + i * SMB_SIZE + SMB_NAME), strings_table
			load class_smb byte from ..global_file:symbols_offset + i * SMB_SIZE + SMB_CLASS
			load sec_smb word from ..global_file:symbols_offset + i * SMB_SIZE + SMB_SEC_NUM
			load offset_smb dword from ..global_file:symbols_offset + i * SMB_SIZE + SMB_SEC_OFFSET
			load nax_smb byte from ..global_file:symbols_offset + i * SMB_SIZE + SMB_NAX
			if sec_smb = 0
				; load sec_offset_smb dword from global_file:OBJ_SECTIONS + (sec_smb - 1) * SEC_IMG_SIZE + SEC_DATA
				; addr_smb = sec_offset_smb + offset_smb; - sec_bytes + bytes_start

				match , IOBJ_DBG IOBJ_DBG_SHOW_EXTRN\\{

					display 9, "external: "
					%display_ascii ..global_file, offset_name_smb, name_len_smb
					display 0Ah
				\\}
			else if sec_smb < 0xFFFE
				repeat 1
					if class_smb = IMAGE_SYM_CLASS_STATIC & offset_smb = 0
						%get_name_sec sec_name_smb, sec_name_len_smb, ..global_file, (OBJ_SECTIONS + (sec_smb - 1) * SEC_IMG_SIZE), strings_table
						%eq_ascii eq_names, ..global_file, offset_name_smb, name_len_smb, sec_name_smb, sec_name_len_smb
						if eq_names
							
							match , IOBJ_DBG\\{
								display 09, "section: "
								%display_ascii ..global_file, offset_name_smb, name_len_smb
								display 0Ah
							\\}
							break
						end if
					end if
					if class_smb = IMAGE_SYM_CLASS_EXTERNAL
						load sec_offset_smb dword from ..global_file:OBJ_SECTIONS + (sec_smb - 1) * SEC_IMG_SIZE + SEC_DATA
						addr_smb = sec_offset_smb + offset_smb
						
						load public_offset dword from ..externs:0
						store dword (public_offset + 1) at ..externs:0
						public addr_smb from ..global_file size name_len_smb offset offset_name_smb into public_offset
						virtual ..externs
							dptr public_offset
						end virtual
						match , IOBJ_DBG IOBJ_DBG_SHOW_PUBLIC\\{
							display 09, "public: "
							%display_ascii ..global_file, offset_name_smb, name_len_smb
							display " at 0x"
							%display_num offset_smb, 16
							display 0Ah
						\\}
						
						break
					end if
					if class_smb = IMAGE_SYM_CLASS_STATIC
						; load sec_offset_smb dword from ..global_file:OBJ_SECTIONS + (sec_smb - 1) * SEC_IMG_SIZE + SEC_DATA
						; addr_smb = sec_offset_smb + offset_smb - sec_bytes + ..bytes_start
						
						; public addr_smb from ..global_file size name_len_smb offset offset_name_smb
						match , IOBJ_DBG IOBJ_DBG_SHOW_STATIC\\{
							display 09, "static: "
							%display_ascii ..global_file, offset_name_smb, name_len_smb
							display " at 0x"
							%display_num offset_smb, 16
							display 0Ah
						\\}
						
						break
					end if
					match , IOBJ_DBG\\{
						display 09, "unknown: ", class_smb
						%display_ascii ..global_file, offset_name_smb, name_len_smb
						display 0Ah
					\\}
				end repeat
			else if sec_smb = 0xFFFE
				match , IOBJ_DBG IOBJ_DBG_SHOW_DEBUG\\{
					display 09, "debug: "
					%display_ascii ..global_file, offset_name_smb, name_len_smb
					display 0Ah
				\\}
			else if sec_smb = 0xFFFF
				match , IOBJ_DBG IOBJ_DBG_SHOW_ABS\\{
					display 09, "absolute: "
					%display_ascii ..global_file, offset_name_smb, name_len_smb
					display 0Ah
				\\}
			end if
			i = i + 1 + nax_smb
		end while

		total@relocs equ ..global_file, sections_count, symbols_offset, symbols_count, strings_table, sec_bytes, is_used, last@section
		obj@externs equ ..externs, is_used
	\}

	macro dllpublic name, func\{
		match =name\#.defined, name\#.defined\\{
			define name\#.defined
			if used name
				name:
					jmp func
			end if
		\\}
	\}
@endm
