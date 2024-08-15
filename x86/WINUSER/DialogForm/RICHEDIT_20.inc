importlib riched20,\
	smth_lol=2
__a$@!=smth_lol

define EM_SETBKGNDCOLOR 1091
define EM_SETCHARFORMAT 0x444
define EM_GETCHARFORMAT 0x43a

define ES_SELECTIONBAR 		16777216
define ES_SUNKEN			16384
define ES_VERTICAL			4194304
define ES_EX_ZOOMABLE		0x0010
define ES_DISABLENOSCROLL	8192

define CFM_BOLD 			0x1
define CFM_CHARSET 			0x8000000
define CFM_COLOR 			0x40000000
define CFM_EFFECTS 			0x4000003f
define CFM_FACE 			0x20000000
define CFM_ITALIC 			0x2
define CFM_OFFSET 			0x10000000
define CFM_PROTECTED 		0x10
define CFM_SIZE 			0x80000000
define CFM_STRIKEOUT 		0x8
define CFM_UNDERLINE 		0x4
define CFM_ALL 				CFM_EFFECTS or CFM_SIZE or CFM_FACE or CFM_OFFSET or CFM_CHARSET
define CFM_WEIGHT 			0x400000
define CFM_SPACING 			0x200000
define CFM_BACKCOLOR 		0x4000000
define CFM_LCID 			0x2000000
define CFM_STYLE 			0x80000
define CFM_KERNING 			0x100000
define CFM_UNDERLINETYPE 	0x800000
define CFM_ANIMATION  		0x40000
define CFM_REVAUTHOR  		0x8000

define LF_FACESIZE 32

struct CHARFORMATA
    cbSize 				dd sizeof.CHARFORMATA
    dwMask 				dd ?
    dweffects 			dd ?
    yHeight 			dd ?
    yOffset 			dd ?
    crTextColor 		dd ?
    bCharSet 			db ?
    bPitchAndFamily 	db ?
    szFaceName 			db LF_FACESIZE dup(?)
    dw ?
ends

struct CHARFORMAT2A CHARFORMATA
	wWeight 		dw ?;
	sSpacing 		dw ?;
	crBackColor 	dd ?;
	LCID     		dd ?;
	union
		dwReserved 	dd ?
		dwCookie 	dd ?
	ends
	sStyle 			dw ?;
	wKerning 		dw ?;
	bUnderlineType 	db ?;
	bAnimation 		db ?;
	bRevAuthor 		db ?;
	bUnderlineColor db ?;
ends

struc CHATFORMAT2A size, [args]{
	common
	local argums
	argums equ size
	match , argums\{
		argums equ sizeof.CHARFORMAT2A
	\}
	match any, args\{
		argums equ argums, args
	\}
	match any, argums\{
		. CHARFORMAT2A argums
	\}
}

macro CHATFORMAT2A size, [args]{
	common
	local argums
	argums equ size
	match , argums\{
		argums equ sizeof.CHARFORMAT2A
	\}
	match any, args\{
		argums equ argums, args
	\}
	match any, argums\{
		CHARFORMAT2A argums
	\}
}


struct RICHEDIT_20 CONTROL
	const CType equ "RICHEDIT20A"
	setBgColor 	override this:inline
ends

macro RICHEDIT_20.setBgColor this, colorref{
	local _this
	inlineObj _this, this, ecx
	@call [SendMessageA]([_this+CONTROL.hWnd], EM_SETBKGNDCOLOR , 0, colorref)
}

define SCF_ALL 				0x4
define SCF_ASSOCIATEFONT 	0x10
define SCF_ASSOCIATEFONT2 	0x40
define SCF_CHARREPFROMLCID 	0x100
define SCF_DEFAULT 			0x0
define SCF_NOKBUPDATE 		0x20
define SCF_SELECTION 		0x1
define SCF_SMARTFONT 		0x80
define SCF_USEUIRULES 		0x8
define SCF_WORD 			0x2

macro RICHEDIT_20.setFormat this, charformat, flags=SCF_ALL{
	local _this
	inlineObj _this, this, ecx
	@call [SendMessageA]([_this+CONTROL.hWnd], EM_SETCHARFORMAT, flags, charformat)
}