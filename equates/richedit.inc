@module RichEditWinAPI

.alignmode_cdecl
.alignmode_set_pack 4

; To mimic older RichEdit behavior, set _RICHEDIT_VER to appropriate value
;		Version 1.0 	0x0100	
;		Version 2.0 	0x0200	
;		Version 2.1 	0x0210	
match =_RICHEDIT_VER, _RICHEDIT_VER\{
	_RICHEDIT_VER equ 0x0810
\}

define cchTextLimitDefault 32767

define MSFTEDIT_CLASS "RICHEDIT50W"

; On Windows CE, avoid possible conflicts on Win95
define CERICHEDIT_CLASSA	"RichEditCEA"
define CERICHEDIT_CLASSW	"RichEditCEW"

define RICHEDIT_CLASSA 		"RichEdit20A"
define RICHEDIT_CLASS10A	"RICHEDIT"			; Richedit 1.0

define RICHEDIT_CLASSW 		"RichEdit20W"

local res, _any1, _any2
%preproc_cmp res, _RICHEDIT_VER, 0x0200
match _any1 =res any2, [::] EQ GT [::]\{

\}

; #if (_RICHEDIT_VER >= 0x0200 )
; #ifdef UNICODE
; #define RICHEDIT_CLASS		RICHEDIT_CLASSW
; #else
; #define RICHEDIT_CLASS		RICHEDIT_CLASSA
; #endif ; UNICODE 
; #else
; #define RICHEDIT_CLASS		RICHEDIT_CLASS10A
; #endif ; _RICHEDIT_VER >= 0x0200 

; RichEdit messages 

if ~definite WM_CONTEXTMENU
WM_CONTEXTMENU			= 0x007B
end if

if ~definite WM_UNICHAR
WM_UNICHAR				= 0x0109
end if

if ~definite WM_PRINTCLIENT
WM_PRINTCLIENT			= 0x0318
end if

if ~definite EM_GETLIMITTEXT
EM_GETLIMITTEXT 		= (WM_USER + 37)
end if

if ~definite EM_POSFROMCHAR	
EM_POSFROMCHAR			= (WM_USER + 38)
EM_CHARFROMPOS			= (WM_USER + 39)
end if

if ~definite EM_SCROLLCARET
EM_SCROLLCARET			= (WM_USER + 49)
end if

EM_CANPASTE 			= (WM_USER + 50)
EM_DISPLAYBAND			= (WM_USER + 51)
EM_EXGETSEL 			= (WM_USER + 52)
EM_EXLIMITTEXT			= (WM_USER + 53)
EM_EXLINEFROMCHAR		= (WM_USER + 54)
EM_EXSETSEL 			= (WM_USER + 55)
EM_FINDTEXT 			= (WM_USER + 56)
EM_FORMATRANGE			= (WM_USER + 57)
EM_GETCHARFORMAT		= (WM_USER + 58)
EM_GETEVENTMASK 		= (WM_USER + 59)
EM_GETOLEINTERFACE		= (WM_USER + 60)
EM_GETPARAFORMAT		= (WM_USER + 61)
EM_GETSELTEXT			= (WM_USER + 62)
EM_HIDESELECTION		= (WM_USER + 63)
EM_PASTESPECIAL 		= (WM_USER + 64)
EM_REQUESTRESIZE		= (WM_USER + 65)
EM_SELECTIONTYPE		= (WM_USER + 66)
EM_SETBKGNDCOLOR		= (WM_USER + 67)
EM_SETCHARFORMAT		= (WM_USER + 68)
EM_SETEVENTMASK 		= (WM_USER + 69)
EM_SETOLECALLBACK		= (WM_USER + 70)
EM_SETPARAFORMAT		= (WM_USER + 71)
EM_SETTARGETDEVICE		= (WM_USER + 72)
EM_STREAMIN 			= (WM_USER + 73)
EM_STREAMOUT			= (WM_USER + 74)
EM_GETTEXTRANGE 		= (WM_USER + 75)
EM_FINDWORDBREAK		= (WM_USER + 76)
EM_SETOPTIONS			= (WM_USER + 77)
EM_GETOPTIONS			= (WM_USER + 78)
EM_FINDTEXTEX			= (WM_USER + 79)
; #ifdef _WIN32
EM_GETWORDBREAKPROCEX	= (WM_USER + 80)
EM_SETWORDBREAKPROCEX	= (WM_USER + 81)
; #endif

; RichEdit 2.0 messages 
EM_SETUNDOLIMIT 		= (WM_USER + 82)
EM_REDO 				= (WM_USER + 84)
EM_CANREDO				= (WM_USER + 85)
EM_GETUNDONAME			= (WM_USER + 86)
EM_GETREDONAME			= (WM_USER + 87)
EM_STOPGROUPTYPING		= (WM_USER + 88)

EM_SETTEXTMODE			= (WM_USER + 89)
EM_GETTEXTMODE			= (WM_USER + 90)

; ; enum for use with EM_GET/SETTEXTMODE 
; typedef enum tagTextMode
; {
TM_PLAINTEXT			= 1	
TM_RICHTEXT 			= 2		; Default behavior 
TM_SINGLELEVELUNDO		= 4	
TM_MULTILEVELUNDO		= 8		; Default behavior 
TM_SINGLECODEPAGE		= 16	
TM_MULTICODEPAGE		= 32	; Default behavior 
; } TEXTMODE;

EM_AUTOURLDETECT		= (WM_USER + 91)

AURL_ENABLEURL			= 1
AURL_ENABLEEMAILADDR	= 2
AURL_ENABLETELNO		= 4
AURL_ENABLEEAURLS		= 8
AURL_ENABLEDRIVELETTERS	= 16
AURL_DISABLEMIXEDLGC	= 32	; Disable mixed Latin Greek Cyrillic IDNs

EM_GETAUTOURLDETECT		= (WM_USER + 92)
EM_SETPALETTE			= (WM_USER + 93)
EM_GETTEXTEX			= (WM_USER + 94)
EM_GETTEXTLENGTHEX		= (WM_USER + 95)
EM_SHOWSCROLLBAR		= (WM_USER + 96)
EM_SETTEXTEX			= (WM_USER + 97)

; East Asia specific messages 
EM_SETPUNCTUATION		= (WM_USER + 100)
EM_GETPUNCTUATION		= (WM_USER + 101)
EM_SETWORDWRAPMODE		= (WM_USER + 102)
EM_GETWORDWRAPMODE		= (WM_USER + 103)
EM_SETIMECOLOR			= (WM_USER + 104)
EM_GETIMECOLOR			= (WM_USER + 105)
EM_SETIMEOPTIONS		= (WM_USER + 106)
EM_GETIMEOPTIONS		= (WM_USER + 107)
EM_CONVPOSITION		= (WM_USER + 108)

EM_SETLANGOPTIONS		= (WM_USER + 120)
EM_GETLANGOPTIONS		= (WM_USER + 121)
EM_GETIMECOMPMODE		= (WM_USER + 122)

EM_FINDTEXTW			= (WM_USER + 123)
EM_FINDTEXTEXW			= (WM_USER + 124)

; RE3.0 FE messages 
EM_RECONVERSION		= (WM_USER + 125)
EM_SETIMEMODEBIAS		= (WM_USER + 126) 
EM_GETIMEMODEBIAS		= (WM_USER + 127)

; BiDi specific messages 
EM_SETBIDIOPTIONS		= (WM_USER + 200)
EM_GETBIDIOPTIONS		= (WM_USER + 201)

EM_SETTYPOGRAPHYOPTIONS= (WM_USER + 202)
EM_GETTYPOGRAPHYOPTIONS= (WM_USER + 203)

; Extended edit style specific messages 
EM_SETEDITSTYLE		= (WM_USER + 204)
EM_GETEDITSTYLE		= (WM_USER + 205)

; Extended edit style masks 
SES_EMULATESYSEDIT		= 1
SES_BEEPONMAXTEXT		= 2
SES_EXTENDBACKCOLOR		= 4
SES_MAPCPS				= 8		; Obsolete (never used)
; #if (_RICHEDIT_VER >= 0x0500)
SES_HYPERLINKTOOLTIPS	= 8
; #endif
SES_EMULATE10			= 16		; Obsolete (never used)
; #if (_RICHEDIT_VER >= 0x0700)
SES_DEFAULTLATINLIGA	= 16
; #endif
SES_USECRLF				= 32		; Obsolete (never used)
; #if (_RICHEDIT_VER >= 0x0700)
SES_NOFOCUSLINKNOTIFY	= 32
; #endif
SES_USEAIMM				= 64
SES_NOIME				= 128

SES_ALLOWBEEPS			= 256
SES_UPPERCASE			= 512
SES_LOWERCASE			= 1024
SES_NOINPUTSEQUENCECHK	= 2048
SES_BIDI				= 4096
SES_SCROLLONKILLFOCUS	= 8192
SES_XLTCRCRLFTOCR		= 16384
SES_DRAFTMODE			= 32768

SES_USECTF					= 0x00010000
SES_HIDEGRIDLINES			= 0x00020000
SES_USEATFONT				= 0x00040000
SES_CUSTOMLOOK				= 0x00080000
SES_LBSCROLLNOTIFY			= 0x00100000
SES_CTFALLOWEMBED			= 0x00200000
SES_CTFALLOWSMARTTAG		= 0x00400000
SES_CTFALLOWPROOFING		= 0x00800000
; #if (_RICHEDIT_VER >= 0x0500)
SES_LOGICALCARET			= 0x01000000
SES_WORDDRAGDROP			= 0x02000000
SES_SMARTDRAGDROP			= 0x04000000
SES_MULTISELECT				= 0x08000000
SES_CTFNOLOCK				= 0x10000000
SES_NOEALINEHEIGHTADJUST	= 0x20000000
SES_MAX						= 0x20000000
; #endif

; Options for EM_SETLANGOPTIONS and EM_GETLANGOPTIONS 
IMF_AUTOKEYBOARD		= 0x0001
IMF_AUTOFONT			= 0x0002
IMF_IMECANCELCOMPLETE	= 0x0004	; High completes comp string when aborting, low cancels
IMF_IMEALWAYSSENDNOTIFY	= 0x0008
IMF_AUTOFONTSIZEADJUST	= 0x0010
IMF_UIFONTS				= 0x0020
; #if (_RICHEDIT_VER >= 0x0800)
IMF_NOIMPLICITLANG		= 0x0040
; #endif
IMF_DUALFONT			= 0x0080
; #if (_RICHEDIT_VER >= 0x0800)
IMF_NOKBDLIDFIXUP		= 0x0200
; #endif
IMF_NORTFFONTSUBSTITUTE	= 0x0400
; #if (_RICHEDIT_VER >= 0x0800)
IMF_SPELLCHECKING		= 0x0800
IMF_TKBPREDICTION		= 0x1000
; #endif
; #if (_RICHEDIT_VER >= 0x0810)
IMF_IMEUIINTEGRATION	= 0x2000
; #endif

; Values for EM_GETIMECOMPMODE 
ICM_NOTOPEN				= 0x0000
ICM_LEVEL3				= 0x0001
ICM_LEVEL2				= 0x0002
ICM_LEVEL2_5			= 0x0003
ICM_LEVEL2_SUI			= 0x0004
ICM_CTF					= 0x0005

; Options for EM_SETTYPOGRAPHYOPTIONS 
TO_ADVANCEDTYPOGRAPHY	= 0x0001
TO_SIMPLELINEBREAK		= 0x0002
TO_DISABLECUSTOMTEXTOUT	= 0x0004
TO_ADVANCEDLAYOUT		= 0x0008

; Pegasus outline mode messages (RE 3.0) 

; Outline mode message
EM_OUTLINE				= (WM_USER + 220)
; Message for getting and restoring scroll pos
EM_GETSCROLLPOS			= (WM_USER + 221)
EM_SETSCROLLPOS			= (WM_USER + 222)
; Change fontsize in current selection by wParam
EM_SETFONTSIZE			= (WM_USER + 223)
EM_GETZOOM				= (WM_USER + 224)
EM_SETZOOM				= (WM_USER + 225)
EM_GETVIEWKIND			= (WM_USER + 226)
EM_SETVIEWKIND			= (WM_USER + 227)

; RichEdit 4.0 messages
EM_GETPAGE				= (WM_USER + 228)
EM_SETPAGE				= (WM_USER + 229)
EM_GETHYPHENATEINFO		= (WM_USER + 230)
EM_SETHYPHENATEINFO		= (WM_USER + 231)

EM_GETPAGEROTATE		= (WM_USER + 235)
EM_SETPAGEROTATE		= (WM_USER + 236)
EM_GETCTFMODEBIAS		= (WM_USER + 237)
EM_SETCTFMODEBIAS		= (WM_USER + 238)
EM_GETCTFOPENSTATUS		= (WM_USER + 240)
EM_SETCTFOPENSTATUS		= (WM_USER + 241)
EM_GETIMECOMPTEXT		= (WM_USER + 242)
EM_ISIME				= (WM_USER + 243)
EM_GETIMEPROPERTY		= (WM_USER + 244)

; These messages control what rich edit does when it comes accross
; OLE objects during RTF stream in.  Normally rich edit queries the client
; application only after OleLoad has been called.  With these messages it is possible to
; set the rich edit control to a mode where it will query the client application before
; OleLoad is called
EM_GETQUERYRTFOBJ		= (WM_USER + 269)
EM_SETQUERYRTFOBJ		= (WM_USER + 270)

; EM_SETPAGEROTATE wparam values
EPR_0					= 0		; Text flows left to right and top to bottom
EPR_270					= 1		; Text flows top to bottom and right to left
EPR_180					= 2		; Text flows right to left and bottom to top
EPR_90					= 3		; Text flows bottom to top and left to right
; #if (_RICHEDIT_VER >= 0x0800)
EPR_SE					= 5		; Text flows top to bottom and left to right (Mongolian text layout)
; #endif


; EM_SETCTFMODEBIAS wparam values
CTFMODEBIAS_DEFAULT					= 0x0000
CTFMODEBIAS_FILENAME				= 0x0001
CTFMODEBIAS_NAME					= 0x0002
CTFMODEBIAS_READING					= 0x0003
CTFMODEBIAS_DATETIME				= 0x0004
CTFMODEBIAS_CONVERSATION			= 0x0005
CTFMODEBIAS_NUMERIC					= 0x0006
CTFMODEBIAS_HIRAGANA				= 0x0007
CTFMODEBIAS_KATAKANA				= 0x0008
CTFMODEBIAS_HANGUL					= 0x0009
CTFMODEBIAS_HALFWIDTHKATAKANA		= 0x000A
CTFMODEBIAS_FULLWIDTHALPHANUMERIC	= 0x000B
CTFMODEBIAS_HALFWIDTHALPHANUMERIC	= 0x000C

; EM_SETIMEMODEBIAS lparam values
IMF_SMODE_PLAURALCLAUSE	= 0x0001
IMF_SMODE_NONE			= 0x0002

; EM_GETIMECOMPTEXT wparam structure
struct IMECOMPTEXT
	cb 		rd 1
	flags 	rd 1
ends
ICT_RESULTREADSTR		= 1

; Outline mode wparam values
EMO_EXIT				= 0		; Enter normal mode,  lparam ignored
EMO_ENTER				= 1		; Enter outline mode, lparam ignored
EMO_PROMOTE				= 2		; LOWORD(lparam) == 0 ==>
										;	promote  to body-text
										; LOWORD(lparam) != 0 ==>
										;	promote/demote current selection
										;	by indicated number of levels
EMO_EXPAND				= 3		; HIWORD(lparam) = EMO_EXPANDSELECTION
										;	-> expands selection to level
										;	indicated in LOWORD(lparam)
										;	LOWORD(lparam) = -1/+1 corresponds
										;	to collapse/expand button presses
										;	in winword (other values are
										;	equivalent to having pressed these
										;	buttons more than once)
										;	HIWORD(lparam) = EMO_EXPANDDOCUMENT
										;	-> expands whole document to
										;	indicated level
EMO_MOVESELECTION		= 4		; LOWORD(lparam) != 0 -> move current
										;	selection up/down by indicated amount
EMO_GETVIEWMODE			= 5		; Returns VM_NORMAL or VM_OUTLINE

; EMO_EXPAND options
EMO_EXPANDSELECTION		= 0
EMO_EXPANDDOCUMENT		= 1

VM_NORMAL				= 4		; Agrees with RTF \viewkindN
VM_OUTLINE				= 2
VM_PAGE					= 9		; Screen page view (not print layout)

;/ New messages as of Win8
; #if (_RICHEDIT_VER >= 0x0800)

EM_INSERTTABLE			= (WM_USER + 232)

; #pragma warning(push)
; #pragma warning(disable:4214) ; Microsoft extension -- Bitfield not int

; Data type defining table rows for EM_INSERTTABLE
struct TABLEROWPARMS 		; EM_INSERTTABLE wparam is a (TABLEROWPARMS *)
	cbRow 			rb 1 	; Count of bytes in this structure
	cbCell 			rb 1 	; Count of bytes in TABLECELLPARMS
	cCell 			rb 1 	; Count of cells
	cRow 			rb 1 	; Count of rows
	dxCellMargin 	rd 1 	; Cell left/right margin (\trgaph)
	dxIndent 		rd 1 	; Row left (right if fRTL indent (similar to \trleft)
	dyHeoght 		rd 1 	; Row height (\trrh)
	flags 			rd 1
	; DWORD	nAlignment:3;	; Row alignment (like PARAFORMAT::bAlignment, \trql, trqr, \trqc)
	; DWORD	fRTL:1; 		; Display cells in RTL order (\rtlrow)
	; DWORD	fKeep:1;		; Keep row together (\trkeep}
	; DWORD	fKeepFollow:1;	; Keep row on same page as following row (\trkeepfollow)
	; DWORD	fWrap:1;		; Wrap text to right/left (depending on bAlignment)
	; ; (see \tdfrmtxtLeftN, \tdfrmtxtRightN)
	; DWORD	fIdentCells:1;	; lparam points at single struct valid for all cells
	cpStartRow 		rd 1 	; cp where to insert table (-1 for selection cp)
	;	(can be used for either TRD by EM_GETTABLEPARMS)
	bTableLevel 	rb 1 	; Table nesting level (EM_GETTABLEPARMS only)
	iCell 			rb 1 	; Index of cell to insert/delete (EM_SETTABLEPARMS only)
ends

; Data type defining table cells for EM_INSERTTABLE
struct TABLECELLPARMS		; EM_INSERTTABLE lparam is a (TABLECELLPARMS *)
	dxWidth 		rd 1		; Cell width (\cellx)
	flags 			rw 1
	; WORD	nVertAlign:2;	; Vertical alignment (0/1/2 = top/center/bottom
	; ;	\clvertalt (def), \clvertalc, \clvertalb)
	; WORD	fMergeTop:1;	; Top cell for vertical merge (\clvmgf)
	; WORD	fMergePrev:1;	; Merge with cell above (\clvmrg)
	; WORD	fVertical:1;	; Display text top to bottom, right to left (\cltxtbrlv)
	; WORD	fMergeStart:1;	; Start set of horizontally merged cells (\clmgf)
	; WORD	fMergeCont:1;	; Merge with previous cell (\clmrg)
	wShading 		rw 1		; Shading in .01%		(\clshdng) e.g., 10000 flips fore/back

	dxBrdrLeft		rw 1 	; Left border width	(\clbrdrl\brdrwN) (in twips)
	dyBrdrTop		rw 1		; Top border width 	(\clbrdrt\brdrwN)
	dxBrdrRight		rw 1	; Right border width	(\clbrdrr\brdrwN)
	dyBrdrBottom	rw 1	; Bottom border width	(\clbrdrb\brdrwN)
	crBrdrLeft		rd 1;	; Left border color	(\clbrdrl\brdrcf)
	crBrdrTop		rd 1; 	; Top border color 	(\clbrdrt\brdrcf)
	crBrdrRight		rd 1;	; Right border color	(\clbrdrr\brdrcf)
	crBrdrBottom	rd 1;	; Bottom border color	(\clbrdrb\brdrcf)
	crBackPat		rd 1; 	; Background color 	(\clcbpat)
	crForePat		rd 1; 	; Foreground color 	(\clcfpat)
ends

EM_GETAUTOCORRECTPROC	= (WM_USER + 233)
EM_SETAUTOCORRECTPROC	= (WM_USER + 234)
EM_CALLAUTOCORRECTPROC	= (WM_USER + 255)

; AutoCorrect callback
; typedef int (WINAPI *AutoCorrectProc)(LANGID langid, const WCHAR *pszBefore, WCHAR *pszAfter, LONG cchAfter, LONG *pcchReplaced);

ATP_NOCHANGE			= 0
ATP_CHANGE				= 1
ATP_NODELIMITER			= 2
ATP_REPLACEALLTEXT		= 4

EM_GETTABLEPARMS			= (WM_USER + 265)

EM_SETEDITSTYLEEX			= (WM_USER + 275)
EM_GETEDITSTYLEEX			= (WM_USER + 276)
; wparam values for EM_SETEDITSTYLEEX/EM_GETEDITSTYLEEX
; All unused bits are reserved.
SES_EX_NOTABLE				= 0x00000004
SES_EX_NOMATH				= 0x00000040
SES_EX_HANDLEFRIENDLYURL	= 0x00000100
SES_EX_NOTHEMING			= 0x00080000
SES_EX_NOACETATESELECTION	= 0x00100000
SES_EX_USESINGLELINE		= 0x00200000
SES_EX_MULTITOUCH			= 0x08000000	; Only works under Win8+
SES_EX_HIDETEMPFORMAT		= 0x10000000
SES_EX_USEMOUSEWPARAM		= 0x20000000	; Use wParam when handling WM_MOUSEMOVE message and do not call GetAsyncKeyState

EM_GETSTORYTYPE				= (WM_USER + 290)
EM_SETSTORYTYPE				= (WM_USER + 291)

EM_GETELLIPSISMODE			= (WM_USER + 305)
EM_SETELLIPSISMODE			= (WM_USER + 306)

; DWORD: *lparam for EM_GETELLIPSISMODE, lparam for EM_SETELLIPSISMODE
ELLIPSIS_MASK				= 0x00000003	; all meaningful bits
ELLIPSIS_NONE				= 0x00000000	; ellipsis disabled
ELLIPSIS_END				= 0x00000001	; ellipsis at the end (forced break)
ELLIPSIS_WORD				= 0x00000003	; ellipsis at the end (word break)

EM_SETTABLEPARMS			= (WM_USER + 307)

EM_GETTOUCHOPTIONS			= (WM_USER + 310)
EM_SETTOUCHOPTIONS			= (WM_USER + 311)
EM_INSERTIMAGE				= (WM_USER + 314)
EM_SETUIANAME				= (WM_USER + 320)
EM_GETELLIPSISSTATE			= (WM_USER + 322)


; Values for EM_SETTOUCHOPTIONS/EM_GETTOUCHOPTIONS
RTO_SHOWHANDLES				= 1
RTO_DISABLEHANDLES			= 2
RTO_READINGMODE				= 3

; lparam for EM_INSERTIMAGE
struct RICHEDIT_IMAGE_PARAMETERS
	xWidth				rd 1;					; Units are HIMETRIC
	yHeight				rd 1;				; Units are HIMETRIC
	Ascent				rd 1;					; Units are HIMETRIC
	Type				rd 1;					; Valid values are TA_TOP, TA_BOTTOM and TA_BASELINE
	pwszAlternateText	rptr 1;
	pIStream 			rptr 1;
ends

; #endif

; New notifications 
EN_MSGFILTER			= 0x0700
EN_REQUESTRESIZE		= 0x0701
EN_SELCHANGE			= 0x0702
EN_DROPFILES			= 0x0703
EN_PROTECTED			= 0x0704
EN_CORRECTTEXT			= 0x0705			; PenWin specific 
EN_STOPNOUNDO			= 0x0706
EN_IMECHANGE			= 0x0707			; East Asia specific 
EN_SAVECLIPBOARD		= 0x0708
EN_OLEOPFAILED			= 0x0709
EN_OBJECTPOSITIONS		= 0x070a
EN_LINK					= 0x070b
EN_DRAGDROPDONE			= 0x070c
EN_PARAGRAPHEXPANDED	= 0x070d
EN_PAGECHANGE			= 0x070e
EN_LOWFIRTF				= 0x070f
EN_ALIGNLTR				= 0x0710			; BiDi specific notification
EN_ALIGNRTL				= 0x0711			; BiDi specific notification
; #if (_RICHEDIT_VER >= 0x0800)
EN_CLIPFORMAT			= 0x0712
EN_STARTCOMPOSITION		= 0x0713
EN_ENDCOMPOSITION		= 0x0714

; Notification structure for EN_ENDCOMPOSITION
struct ENDCOMPOSITIONNOTIFY
	nmhdr 	NMHDR
	dwCode 	rd 1;
ends

; Constants for ENDCOMPOSITIONNOTIFY dwCode
ECN_ENDCOMPOSITION	= 0x0001
ECN_NEWTEXT			= 0x0002
; #endif

; Event notification masks 
ENM_NONE				= 0x00000000
ENM_CHANGE				= 0x00000001
ENM_UPDATE				= 0x00000002
ENM_SCROLL				= 0x00000004
ENM_SCROLLEVENTS		= 0x00000008
ENM_DRAGDROPDONE		= 0x00000010
ENM_PARAGRAPHEXPANDED	= 0x00000020
ENM_PAGECHANGE			= 0x00000040
; #if (_RICHEDIT_VER >= 0x0800)
ENM_CLIPFORMAT			= 0x00000080
; #endif
ENM_KEYEVENTS			= 0x00010000
ENM_MOUSEEVENTS			= 0x00020000
ENM_REQUESTRESIZE		= 0x00040000
ENM_SELCHANGE			= 0x00080000
ENM_DROPFILES			= 0x00100000
ENM_PROTECTED			= 0x00200000
ENM_CORRECTTEXT			= 0x00400000		; PenWin specific 
ENM_IMECHANGE			= 0x00800000		; Used by RE1.0 compatibility
ENM_LANGCHANGE			= 0x01000000
ENM_OBJECTPOSITIONS		= 0x02000000
ENM_LINK				= 0x04000000
ENM_LOWFIRTF			= 0x08000000
; #if (_RICHEDIT_VER >= 0x0800)
ENM_STARTCOMPOSITION	= 0x10000000
ENM_ENDCOMPOSITION		= 0x20000000
ENM_GROUPTYPINGCHANGE	= 0x40000000
ENM_HIDELINKTOOLTIP		= 0x80000000
; #endif

; New edit control styles 
ES_SAVESEL				= 0x00008000
ES_SUNKEN				= 0x00004000
ES_DISABLENOSCROLL		= 0x00002000
; Same as WS_MAXIMIZE, but that doesn't make sense so we re-use the value 
ES_SELECTIONBAR			= 0x01000000
; Same as ES_UPPERCASE, but re-used to completely disable OLE drag'n'drop 
ES_NOOLEDRAGDROP		= 0x00000008

; Obsolete Edit Style
ES_EX_NOCALLOLEINIT		= 0x00000000		; Not supported in RE 2.0/3.0 

; These flags are used in FE Windows 
ES_VERTICAL				= 0x00400000		; Not supported in RE 2.0/3.0 
ES_NOIME				= 0x00080000
ES_SELFIME				= 0x00040000

; Edit control options 
ECO_AUTOWORDSELECTION	= 0x00000001
ECO_AUTOVSCROLL			= 0x00000040
ECO_AUTOHSCROLL			= 0x00000080
ECO_NOHIDESEL			= 0x00000100
ECO_READONLY			= 0x00000800
ECO_WANTRETURN			= 0x00001000
ECO_SAVESEL				= 0x00008000
ECO_SELECTIONBAR		= 0x01000000
ECO_VERTICAL			= 0x00400000		; FE specific 


; ECO operations 
ECOOP_SET				= 0x0001
ECOOP_OR				= 0x0002
ECOOP_AND				= 0x0003
ECOOP_XOR				= 0x0004

; New word break function actions 
WB_CLASSIFY			= 3
WB_MOVEWORDLEFT		= 4
WB_MOVEWORDRIGHT	= 5
WB_LEFTBREAK		= 6
WB_RIGHTBREAK		= 7

; East Asia specific flags 
WB_MOVEWORDPREV		= 4
WB_MOVEWORDNEXT		= 5
WB_PREVBREAK		= 6
WB_NEXTBREAK		= 7

PC_FOLLOWING		= 1
PC_LEADING			= 2
PC_OVERFLOW			= 3
PC_DELIMITER		= 4
WBF_WORDWRAP		= 0x010
WBF_WORDBREAK		= 0x020
WBF_OVERFLOW		= 0x040	
WBF_LEVEL1			= 0x080
WBF_LEVEL2			= 0x100
WBF_CUSTOM			= 0x200

; East Asia specific flags 
IMF_FORCENONE			= 0x0001
IMF_FORCEENABLE			= 0x0002
IMF_FORCEDISABLE		= 0x0004
IMF_CLOSESTATUSWINDOW	= 0x0008
IMF_VERTICAL			= 0x0020
IMF_FORCEACTIVE			= 0x0040
IMF_FORCEINACTIVE		= 0x0080
IMF_FORCEREMEMBER		= 0x0100
IMF_MULTIPLEEDIT		= 0x0400

; Word break flags (used with WB_CLASSIFY) 
WBF_CLASS			= 0x0F
WBF_ISWHITE			= 0x10
WBF_BREAKLINE		= 0x20
WBF_BREAKAFTER		= 0x40

; Data types 

; #ifdef _WIN32
; ; Extended edit word break proc (character set aware) 
; typedef LONG (*EDITWORDBREAKPROCEX)(char *pchText, LONG cchText, BYTE bCharSet, INT action);
; #endif

; All character format measurements are in twips 
if ~definite LF_FACESIZE
	LF_FACESIZE = 32
end if
struct CHARFORMATA
	cbSize			rd 1;
	dwMask			rd 1;
	dwEffects		rd 1;
	yHeight			rd 1;
	yOffset			rd 1;
	crTextColor		rd 1;
	bCharSet		rb 1;
	bPitchAndFamily	rb 1;
	szFaceName		rb LF_FACESIZE;
ends

struct CHARFORMATW
	cbSize			rd 1;
	dwMask			rd 1;
	dwEffects		rd 1;
	yHeight			rd 1;
	yOffset			rd 1;
	crTextColor		rd 1;
	bCharSet		rb 1;
	bPitchAndFamily	rb 1;
	szFaceName		rw LF_FACESIZE;
ends

; #if (_RICHEDIT_VER >= 0x0200)
; #ifdef UNICODE
; #define CHARFORMAT CHARFORMATW
; #else
; #define CHARFORMAT CHARFORMATA
; #endif ; UNICODE 
; #else
; #define CHARFORMAT CHARFORMATA
; #endif ; _RICHEDIT_VER >= 0x0200 

; CHARFORMAT2 structure 

; #if defined(__cplusplus)

struct CHARFORMAT2A CHARFORMATA
	wWeight		rw 1			; Font weight (LOGFONT value)
	sSpacing	rw 1			; Amount to space between letters
	crBackColor	rd 1			; Background color
	lcid  		rd 1			; Locale ID
	union
		dwReserved	rd 1		; Name up to 5.0
		dwCookie	rd 1	 	; Client cookie opaque to RichEdit
	ends
	sStyle			rw 1		; Style handle
	wKerning		rw 1		; Twip size above which to kern char pair
	bUnderlineType	rb 1		; Underline type
	bAnimation		rb 1		; Animated text like marching ants
	bRevAuthor		rb 1		; Revision author index
	bUnderlineColor	rb 1		; Underline color
ends

struct CHARFORMAT2W CHARFORMATW
	wWeight		rw 1			; Font weight (LOGFONT value)
	sSpacing	rw 1			; Amount to space between letters
	crBackColor	rd 1			; Background color
	lcid  		rd 1			; Locale ID
	union
		dwReserved	rd 1		; Name up to 5.0
		dwCookie	rd 1	 	; Client cookie opaque to RichEdit
	ends
	sStyle			rw 1		; Style handle
	wKerning		rw 1		; Twip size above which to kern char pair
	bUnderlineType	rb 1		; Underline type
	bAnimation		rb 1		; Animated text like marching ants
	bRevAuthor		rb 1		; Revision author index
	bUnderlineColor	rb 1		; Underline color
ends

; CFM_COLOR mirrors CFE_AUTOCOLOR, a little hack to easily deal with autocolor

; CHARFORMAT masks 
CFM_BOLD		= 0x00000001
CFM_ITALIC		= 0x00000002
CFM_UNDERLINE	= 0x00000004
CFM_STRIKEOUT	= 0x00000008
CFM_PROTECTED	= 0x00000010
CFM_LINK		= 0x00000020			; Exchange hyperlink extension 
CFM_SIZE		= 0x80000000
CFM_COLOR		= 0x40000000
CFM_FACE		= 0x20000000
CFM_OFFSET		= 0x10000000
CFM_CHARSET		= 0x08000000

; CHARFORMAT effects 
CFE_BOLD			= 0x00000001
CFE_ITALIC			= 0x00000002
CFE_UNDERLINE		= 0x00000004
CFE_STRIKEOUT		= 0x00000008
CFE_PROTECTED		= 0x00000010
CFE_LINK			= 0x00000020
CFE_AUTOCOLOR		= 0x40000000		; NOTE: this corresponds to 
											; CFM_COLOR, which controls it 
; Masks and effects defined for CHARFORMAT2 -- an (*) indicates
; that the data is stored by RichEdit 2.0/3.0, but not displayed
CFM_SMALLCAPS		= 0x00000040		; (*)	
CFM_ALLCAPS			= 0x00000080		; Displayed by 3.0 
CFM_HIDDEN			= 0x00000100		; Hidden by 3.0 
CFM_OUTLINE			= 0x00000200		; (*)	
CFM_SHADOW			= 0x00000400		; (*)	
CFM_EMBOSS			= 0x00000800		; (*)	
CFM_IMPRINT			= 0x00001000		; (*)	
CFM_DISABLED		= 0x00002000
CFM_REVISED			= 0x00004000

CFM_REVAUTHOR		= 0x00008000
CFE_SUBSCRIPT		= 0x00010000		; Superscript and subscript are 
CFE_SUPERSCRIPT		= 0x00020000		;	mutually exclusive			 
CFM_ANIMATION		= 0x00040000		; (*)	
CFM_STYLE			= 0x00080000		; (*)	
CFM_KERNING			= 0x00100000
CFM_SPACING			= 0x00200000		; Displayed by 3.0 
CFM_WEIGHT			= 0x00400000
CFM_UNDERLINETYPE	= 0x00800000		; Many displayed by 3.0 
; #if (_RICHEDIT_VER >= 0x0600)
CFM_COOKIE			= 0x01000000		; RE 6.0
; #endif
CFM_LCID			= 0x02000000
CFM_BACKCOLOR		= 0x04000000		; Higher mask bits defined above

CFM_SUBSCRIPT		= (CFE_SUBSCRIPT or CFE_SUPERSCRIPT)
CFM_SUPERSCRIPT		= CFM_SUBSCRIPT

; CHARFORMAT "ALL" masks
CFM_EFFECTS = (CFM_BOLD or CFM_ITALIC or CFM_UNDERLINE or CFM_COLOR or \
					 CFM_STRIKEOUT or CFE_PROTECTED or CFM_LINK)
CFM_ALL = (CFM_EFFECTS or CFM_SIZE or CFM_FACE or CFM_OFFSET or CFM_CHARSET)

CFM_EFFECTS2 = (CFM_EFFECTS or CFM_DISABLED or CFM_SMALLCAPS or CFM_ALLCAPS \
					or CFM_HIDDEN  or CFM_OUTLINE or CFM_SHADOW or CFM_EMBOSS \
					or CFM_IMPRINT or CFM_REVISED \
					or CFM_SUBSCRIPT or CFM_SUPERSCRIPT or CFM_BACKCOLOR)

; #if (_RICHEDIT_VER >= 0x0600)
CFM_ALL2	=  (CFM_ALL or CFM_EFFECTS2 or CFM_BACKCOLOR or CFM_LCID \
					or CFM_UNDERLINETYPE or CFM_WEIGHT or CFM_REVAUTHOR \
					or CFM_SPACING or CFM_KERNING or CFM_STYLE or CFM_ANIMATION \
					or CFM_COOKIE)
; #else
; CFM_ALL2	=  (CFM_ALL or CFM_EFFECTS2 or CFM_BACKCOLOR or CFM_LCID \
; 					or CFM_UNDERLINETYPE or CFM_WEIGHT or CFM_REVAUTHOR \
; 					or CFM_SPACING or CFM_KERNING or CFM_STYLE or CFM_ANIMATION)
; #endif

CFE_SMALLCAPS		= CFM_SMALLCAPS
CFE_ALLCAPS			= CFM_ALLCAPS
CFE_HIDDEN			= CFM_HIDDEN
CFE_OUTLINE			= CFM_OUTLINE
CFE_SHADOW			= CFM_SHADOW
CFE_EMBOSS			= CFM_EMBOSS
CFE_IMPRINT			= CFM_IMPRINT
CFE_DISABLED		= CFM_DISABLED
CFE_REVISED			= CFM_REVISED

; CFE_AUTOCOLOR and CFE_AUTOBACKCOLOR correspond to CFM_COLOR and
; CFM_BACKCOLOR, respectively, which control them
CFE_AUTOBACKCOLOR	= CFM_BACKCOLOR

CFM_FONTBOUND				= 0x00100000
CFM_LINKPROTECTED			= 0x00800000	; Word hyperlink field
CFM_EXTENDED				= 0x02000000
CFM_MATHNOBUILDUP			= 0x08000000
CFM_MATH					= 0x10000000
CFM_MATHORDINARY			= 0x20000000

CFM_ALLEFFECTS				= (CFM_EFFECTS2 or CFM_FONTBOUND or CFM_EXTENDED or CFM_MATHNOBUILDUP or CFM_MATH or CFM_MATHORDINARY)

CFE_FONTBOUND				= 0x00100000	; Font chosen by binder, not user
CFE_LINKPROTECTED			= 0x00800000
CFE_EXTENDED				= 0x02000000
CFE_MATHNOBUILDUP			= 0x08000000
CFE_MATH					= 0x10000000
CFE_MATHORDINARY			= 0x20000000

; Underline types. RE 1.0 displays only CFU_UNDERLINE
CFU_CF1UNDERLINE				= 0xFF	; Map charformat's bit underline to CF2
CFU_INVERT						= 0xFE	; For IME composition fake a selection
CFU_UNDERLINETHICKLONGDASH		= 18	; (*) display as dash
CFU_UNDERLINETHICKDOTTED		= 17	; (*) display as dot
CFU_UNDERLINETHICKDASHDOTDOT	= 16	; (*) display as dash dot dot
CFU_UNDERLINETHICKDASHDOT		= 15	; (*) display as dash dot
CFU_UNDERLINETHICKDASH			= 14	; (*) display as dash
CFU_UNDERLINELONGDASH			= 13	; (*) display as dash
CFU_UNDERLINEHEAVYWAVE			= 12	; (*) display as wave
CFU_UNDERLINEDOUBLEWAVE			= 11	; (*) display as wave
CFU_UNDERLINEHAIRLINE			= 10	; (*) display as single	
CFU_UNDERLINETHICK				= 9
CFU_UNDERLINEWAVE				= 8
CFU_UNDERLINEDASHDOTDOT			= 7
CFU_UNDERLINEDASHDOT			= 6
CFU_UNDERLINEDASH				= 5
CFU_UNDERLINEDOTTED				= 4
CFU_UNDERLINEDOUBLE				= 3	; (*) display as single
CFU_UNDERLINEWORD				= 2	; (*) display as single	
CFU_UNDERLINE					= 1
CFU_UNDERLINENONE				= 0

yHeightCharPtsMost = 1638

; EM_SETCHARFORMAT wParam masks 
SCF_SELECTION		= 0x0001
SCF_WORD			= 0x0002
SCF_DEFAULT			= 0x0000	; Set default charformat or paraformat
SCF_ALL				= 0x0004	; Not valid with SCF_SELECTION or SCF_WORD
SCF_USEUIRULES		= 0x0008	; Modifier for SCF_SELECTION; says that
									;	format came from a toolbar, etc., and
									;	hence UI formatting rules should be
									;	used instead of literal formatting
SCF_ASSOCIATEFONT	= 0x0010	; Associate fontname with bCharSet (one
									;	possible for each of Western, ME, FE,
									;	Thai)
SCF_NOKBUPDATE		= 0x0020	; Do not update KB layout for this change
									;	even if autokeyboard is on
SCF_ASSOCIATEFONT2	= 0x0040	; Associate plane-2 (surrogate) font
; #if (_RICHEDIT_VER >= 0x0500)
SCF_SMARTFONT		= 0x0080	; Apply font only if it can handle script (5.0)
SCF_CHARREPFROMLCID	= 0x0100	; Get character repertoire from lcid (5.0)

SPF_DONTSETDEFAULT	= 0x0002	; Suppress setting default on empty control
SPF_SETDEFAULT		= 0x0004	; Set the default paraformat
; #endif

struct CHARRANGE
	cpMin rd 1
	cpMax rd 1
ends

struct TEXTRANGE
	chrg 		CHARRANGE
	lpstrText 	rptr 1	; Allocated by caller, zero terminated by RichEdit 
ends

struct TEXTRANGEA TEXTRANGE
ends

struct TEXTRANGEW TEXTRANGE
ends

; typedef DWORD (CALLBACK *EDITSTREAMCALLBACK)(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

struct EDITSTREAM
	dwCookie 	rptr 1; 	; User value passed to callback as first parameter 
	dwError 	rd 1;		; Last error 
	pfnCallback rptr 1;
ends

; Stream formats. Flags are all in low word, since high word
; gives possible codepage choice. 
SF_TEXT			= 0x0001
SF_RTF			= 0x0002
SF_RTFNOOBJS	= 0x0003		; Write only 
SF_TEXTIZED		= 0x0004		; Write only

SF_UNICODE			= 0x0010		; Unicode file (UCS2 little endian) 
SF_USECODEPAGE		= 0x0020		; CodePage given by high word 
SF_NCRFORNONASCII	= 0x40		; Output \uN for nonASCII 
SFF_WRITEXTRAPAR	= 0x80		; Output \par at end

; Flag telling stream operations to operate on selection only 
; EM_STREAMIN	replaces current selection 
; EM_STREAMOUT streams out current selection 
SFF_SELECTION	= 0x8000

; Flag telling stream operations to ignore some FE control words 
; having to do with FE word breaking and horiz vs vertical text. 
; Not used in RichEdit 2.0 and later	
SFF_PLAINRTF	= 0x4000

; Flag telling file stream output (SFF_SELECTION flag not set) to persist 
; \viewscaleN control word. 
SFF_PERSISTVIEWSCALE = 0x2000

; Flag telling file stream input with SFF_SELECTION flag not set not to 
; close the document 
SFF_KEEPDOCINFO = 0x1000

; Flag telling stream operations to output in Pocket Word format 
SFF_PWD	= 0x0800

; 3-bit field specifying the value of N - 1 to use for \rtfN or \pwdN 
SF_RTFVAL = 0x0700

struct FINDTEXT
	chrg 		CHARRANGE;
	lpstrText 	rptr 1;
ends

struct FINDTEXTA FINDTEXT
ends

struct FINDTEXTW FINDTEXT
ends

struct FINDTEXTEX
	chrg 		CHARRANGE
	lpstrText 	rptr 1
	chrgText 	CHARRANGE
ends

struct FINDTEXTEXA FINDTEXTEX
ends

struct FINDTEXTEXW FINDTEXTEX
ends


struct FORMATRANGE
	hdc 		rptr 1
	hdcTarget 	rptr 1
	rc			RECT
	rcPage		RECT
	chrg 		CHARRANGE
ends

; All paragraph measurements are in twips 

MAX_TAB_STOPS	= 32
lDefaultTab		= 720
MAX_TABLE_CELLS	= 63

struct PARAFORMAT
	cbSize		rd 1;
	dwMask		rd 1;
	wNumbering 	rw 1;
	union
		wReserved	rw 1;
		wEffects	rw 1;
	ends
	dxStartIndent	rd 1;
	dxRightIndent	rd 1;
	dxOffset		rd 1;
	wAlignment		rw 1;
	cTabCount		rw 1;
	rgxTabs 		rd MAX_TAB_STOPS;
ends

struct PARAFORMAT2 PARAFORMAT
	dySpaceBefore		rd 1		; Vertical spacing before para
	dySpaceAfter		rd 1		; Vertical spacing after para
	dyLineSpacing		rd 1		; Line spacing depending on Rule
	sStyle 				rw 1		; Style handle
	bLineSpacingRule	rb 1		; Rule for line spacing (see tom.doc)
	bOutlineLevel		rb 1		; Outline level
	wShadingWeight		rw 1		; Shading in hundredths of a per cent
	wShadingStyle		rw 1		; Nibble 0: style, 1: cfpat, 2: cbpat
	wNumberingStart		rw 1		; Starting value for numbering
	wNumberingStyle		rw 1		; Alignment, roman/arabic, (), ), ., etc.
	wNumberingTab		rw 1		; Space bet FirstIndent & 1st-line text
	wBorderSpace		rw 1		; Border-text spaces (nbl/bdr in pts)
	wBorderWidth		rw 1		; Pen widths (nbl/bdr in half pts)
	wBorders			rw 1		; Border styles (nibble/border)
ends

; PARAFORMAT mask values 
PFM_STARTINDENT 		= 0x00000001
PFM_RIGHTINDENT 		= 0x00000002
PFM_OFFSET				= 0x00000004
PFM_ALIGNMENT			= 0x00000008
PFM_TABSTOPS			= 0x00000010
PFM_NUMBERING			= 0x00000020
PFM_OFFSETINDENT		= 0x80000000

; PARAFORMAT 2.0 masks and effects 
PFM_SPACEBEFORE 		= 0x00000040
PFM_SPACEAFTER			= 0x00000080
PFM_LINESPACING 		= 0x00000100
PFM_STYLE				= 0x00000400
PFM_BORDER				= 0x00000800	; (*)	
PFM_SHADING 			= 0x00001000	; (*)	
PFM_NUMBERINGSTYLE		= 0x00002000	; RE 3.0	
PFM_NUMBERINGTAB		= 0x00004000	; RE 3.0	
PFM_NUMBERINGSTART		= 0x00008000	; RE 3.0	

PFM_RTLPARA 			= 0x00010000
PFM_KEEP				= 0x00020000	; (*)	
PFM_KEEPNEXT			= 0x00040000	; (*)	
PFM_PAGEBREAKBEFORE 	= 0x00080000	; (*)	
PFM_NOLINENUMBER		= 0x00100000	; (*)	
PFM_NOWIDOWCONTROL		= 0x00200000	; (*)	
PFM_DONOTHYPHEN 		= 0x00400000	; (*)	
PFM_SIDEBYSIDE			= 0x00800000	; (*)	

; The following two paragraph-format properties are read only
PFM_COLLAPSED			= 0x01000000	; RE 3.0 
PFM_OUTLINELEVEL		= 0x02000000	; RE 3.0 
PFM_BOX 				= 0x04000000	; RE 3.0 
PFM_RESERVED2			= 0x08000000	; RE 4.0 
PFM_TABLEROWDELIMITER	= 0x10000000	; RE 4.0 
PFM_TEXTWRAPPINGBREAK	= 0x20000000	; RE 3.0 
PFM_TABLE				= 0x40000000	; RE 3.0 

; PARAFORMAT "ALL" masks
PFM_ALL = (PFM_STARTINDENT or PFM_RIGHTINDENT or PFM_OFFSET or \
				 PFM_ALIGNMENT	 or PFM_TABSTOPS    or PFM_NUMBERING or \
				 PFM_OFFSETINDENT or PFM_RTLPARA)

; Note: PARAFORMAT has no effects (BiDi RichEdit 1.0 does have PFE_RTLPARA)
PFM_EFFECTS = (PFM_RTLPARA or PFM_KEEP or PFM_KEEPNEXT or PFM_TABLE \
					or PFM_PAGEBREAKBEFORE or PFM_NOLINENUMBER  \
					or PFM_NOWIDOWCONTROL or PFM_DONOTHYPHEN or PFM_SIDEBYSIDE \
					or PFM_TABLE or PFM_TABLEROWDELIMITER)

PFM_ALL2	= (PFM_ALL or PFM_EFFECTS or PFM_SPACEBEFORE or PFM_SPACEAFTER \
					or PFM_LINESPACING or PFM_STYLE or PFM_SHADING or PFM_BORDER \
					or PFM_NUMBERINGTAB or PFM_NUMBERINGSTART or PFM_NUMBERINGSTYLE)

PFE_RTLPARA 			= (PFM_RTLPARA		 shr 16)
PFE_KEEP				= (PFM_KEEP			 shr 16) ; (*)	
PFE_KEEPNEXT			= (PFM_KEEPNEXT		 shr 16) ; (*)	
PFE_PAGEBREAKBEFORE 	= (PFM_PAGEBREAKBEFORE shr 16) ; (*)	
PFE_NOLINENUMBER		= (PFM_NOLINENUMBER	 shr 16) ; (*)	
PFE_NOWIDOWCONTROL		= (PFM_NOWIDOWCONTROL  shr 16) ; (*)	
PFE_DONOTHYPHEN 		= (PFM_DONOTHYPHEN	 shr 16) ; (*)	
PFE_SIDEBYSIDE			= (PFM_SIDEBYSIDE 	 shr 16) ; (*)	
PFE_TEXTWRAPPINGBREAK	= (PFM_TEXTWRAPPINGBREAK shr 16) ; (*)	

; The following four effects are read only
PFE_COLLAPSED			= (PFM_COLLAPSED		 shr 16) ; (+)	
PFE_BOX 				= (PFM_BOX			 shr 16) ; (+)	
PFE_TABLE				= (PFM_TABLE			 shr 16) ; Inside table row. RE 3.0 
PFE_TABLEROWDELIMITER	= (PFM_TABLEROWDELIMITER shr 16) ; Table row start. RE 4.0 

; PARAFORMAT numbering options 
PFN_BULLET		= 1		; tomListBullet

; PARAFORMAT2 wNumbering options 
PFN_ARABIC		= 2		; tomListNumberAsArabic:	0, 1, 2,	...
PFN_LCLETTER	= 3		; tomListNumberAsLCLetter: a, b, c,	...
PFN_UCLETTER	= 4		; tomListNumberAsUCLetter: A, B, C,	...
PFN_LCROMAN 	= 5		; tomListNumberAsLCRoman:	i, ii, iii, ...
PFN_UCROMAN 	= 6		; tomListNumberAsUCRoman:	I, II, III, ...

; PARAFORMAT2 wNumberingStyle options 
PFNS_PAREN		= 0x000	; default, e.g.,				  1)	
PFNS_PARENS 	= 0x100	; tomListParentheses/256, e.g., (1)	
PFNS_PERIOD 	= 0x200	; tomListPeriod/256, e.g., 	  1.	
PFNS_PLAIN		= 0x300	; tomListPlain/256, e.g.,		  1 	
PFNS_NONUMBER	= 0x400	; Used for continuation w/o number

PFNS_NEWNUMBER	= 0x8000	; Start new number with wNumberingStart		
								; (can be combined with other PFNS_xxx)
; PARAFORMAT alignment options 
PFA_LEFT			 = 1
PFA_RIGHT			 = 2
PFA_CENTER			 = 3

; PARAFORMAT2 alignment options 
PFA_JUSTIFY 		 = 4	; New paragraph-alignment option 2.0 (*) 
PFA_FULL_INTERWORD	 = 4	; These are supported in 3.0 with advanced

; Notification structures 
struct MSGFILTER
	nmhdr 	NMHDR
	msg 	rd 1
	wParam 	rptr 1
	lParam 	rptr 1
ends

struct REQRESIZE
	nmhdr 	NMHDR
	rc 		RECT
ends

struct SELCHANGE
	nmhdr 	NMHDR
	chrg 	CHARRANGE
	seltyp 	rw 1
ends

; #if (_RICHEDIT_VER >= 0x0800)
struct GROUPTYPINGCHANGE
	nmhdr 			NMHDR
	fGroupTyping 	rb 1
ends
; #endif

struct CLIPBOARDFORMAT
	nmhdr 	NMHDR
	cf 		rd 1
ends

SEL_EMPTY		= 0x0000
SEL_TEXT		= 0x0001
SEL_OBJECT		= 0x0002
SEL_MULTICHAR	= 0x0004
SEL_MULTIOBJECT = 0x0008

; Used with IRichEditOleCallback::GetContextMenu, this flag will be
; passed as a "selection type".  It indicates that a context menu for
; a right-mouse drag drop should be generated.  The IOleObject parameter
; will really be the IDataObject for the drop
GCM_RIGHTMOUSEDROP	= 0x8000

; #if (_RICHEDIT_VER >= 0x0800)
struct GETCONTEXTMENUEX
	chrg 		CHARRANGE 
	dwFlags 	rd 1
	pt 			POINT
	pvReserved 	rptr 1
ends

; bits for GETCONTEXTMENUEX::dwFlags
GCMF_GRIPPER			= 0x00000001
GCMF_SPELLING			= 0x00000002	; pSpellingSuggestions is valid
											; and points to the list of spelling suggestions
GCMF_TOUCHMENU			= 0x00004000
GCMF_MOUSEMENU			= 0x00002000
; #endif

struct ENDROPFILES
	nmhdr 		NMHDR
	hDrop 		rptr 1
	cp 			rd 1
	fProtected 	rb 1
ends

struct ENPROTECTED
	nmhdr 	NMHDR
	msg 	rd 1
	wParam 	rptr 1
	lParam 	rptr 1
	chrg CHARRANGE
ends

struct ENSAVECLIPBOARD
	nmhdr 			NMHDR
	cObjectCount 	rd 1
	cch 			rd 1
ends

; #ifndef MACPORT
; typedef struct _enoleopfailed
; {
; 	NMHDR nmhdr;
; 	LONG iob;
; 	LONG lOper;
; 	HRESULT hr;
; } ENOLEOPFAILED;
; #endif

OLEOP_DOVERB = 1

struct OBJECTPOSITIONS
	nmhdr NMHDR
	cObjectCount rd 1
	pcpPositions rptr 1
ends

struct ENLINK
	nmhdr 	NMHDR
	msg 	rd 1
	wParam 	rptr 1
	lParam 	rptr 1
	chrg 	CHARRANGE
ends

struct ENLOWFIRTF
	nmhdr 		NMHDR
	szControl 	rptr 1
ends

; PenWin specific 
struct ENCORRECTTEXT
	nmhdr 	NMHDR
	chrg 	CHARRANGE
	seltyp 	rw 1
ends

; East Asia specific 
struct PUNCTUATION
	iSize 			rd 1
	szPunctuation 	rptr 1
ends

; East Asia specific 
struct COMPCOLOR
	crText 			rd 1
	crBackground 	rd 1
	dwEffects 		rd 1
ends


; Clipboard formats - use as parameter to RegisterClipboardFormat() 
define CF_RTF		"Rich Text Format"
define CF_RTFNOOBJS	"Rich Text Format Without Objects"
define CF_RETEXTOBJ	"RichEdit Text and Objects"

; Paste Special 
struct REPASTESPECIAL
	dwAspect 	rd 1;
	dwParam 	rptr 1;
ends

;	UndoName info 
; typedef enum _undonameid
; {
	UID_UNKNOWN 	= 0
	UID_TYPING		= 1
	UID_DELETE		= 2
	UID_DRAGDROP	= 3
	UID_CUT 		= 4
	UID_PASTE		= 5
	UID_AUTOTABLE	= 6
; } UNDONAMEID;

; Flags for the SETEXTEX data structure 
ST_DEFAULT		= 0
ST_KEEPUNDO 	= 1
ST_SELECTION	= 2
ST_NEWCHARS 	= 4
; #if (_RICHEDIT_VER >= 0x0500)
ST_UNICODE	 	= 8
; #endif

; EM_SETTEXTEX info; this struct is passed in the wparam of the message 
struct SETTEXTEX
	flags		rd 1;			; Flags (see the ST_XXX defines)
	codepage	rd 1;		; Code page for translation (CP_ACP for sys default,
							;	1200 for Unicode, -1 for control default)
ends

; Flags for the GETEXTEX data structure 
GT_DEFAULT		= 0
GT_USECRLF		= 1
GT_SELECTION	= 2
GT_RAWTEXT		= 4
GT_NOHIDDENTEXT = 8

; EM_GETTEXTEX info; this struct is passed in the wparam of the message 
struct GETTEXTEX
	cb				rd 1 			; Count of bytes in the string
	flags			rd 1			; Flags (see the GT_XXX defines
	codepage		rd 1		; Code page for translation (CP_ACP for sys default,
							;	1200 for Unicode, -1 for control default)
	lpDefaultChar	rptr 1	; Replacement for unmappable chars
	lpUsedDefChar	rptr 1	; Pointer to flag set when def char used
ends

; Flags for the GETTEXTLENGTHEX data structure
GTL_DEFAULT 	= 0		; Do default (return # of chars)
GTL_USECRLF 	= 1		; Compute answer using CRLFs for paragraphs
GTL_PRECISE 	= 2		; Compute a precise answer
GTL_CLOSE		= 4		; Fast computation of a "close" answer
GTL_NUMCHARS	= 8		; Return number of characters
GTL_NUMBYTES	= 16	; Return number of _bytes_

; EM_GETTEXTLENGTHEX info; this struct is passed in the wparam of the msg 
struct GETTEXTLENGTHEX
	flags		rd 1		; Flags (see GTL_XXX defines)
	codepage	rd 1		; Code page for translation (CP_ACP for default,
							;	1200 for Unicode)
ends
	
; BiDi specific features 
struct BIDIOPTIONS
	cbSize 		rd 1
	wMask		rw 1
	wEffects	rw 1 
ends

; BIDIOPTIONS masks 
; #if (_RICHEDIT_VER == 0x0100)
BOM_DEFPARADIR			= 0x0001	; Default paragraph direction (implies alignment) (obsolete) 
BOM_PLAINTEXT			= 0x0002	; Use plain text layout (obsolete) 
; #endif ; _RICHEDIT_VER == 0x0100 
BOM_NEUTRALOVERRIDE 	= 0x0004	; Override neutral layout (obsolete) 
BOM_CONTEXTREADING		= 0x0008	; Context reading order 
BOM_CONTEXTALIGNMENT	= 0x0010	; Context alignment 
BOM_LEGACYBIDICLASS		= 0x0040	; Legacy Bidi classification (obsolete)
; #if (_RICHEDIT_VER >= 0x0600)
BOM_UNICODEBIDI			= 0x0080	; Use Unicode BiDi algorithm
; #endif

; BIDIOPTIONS effects 
; #if (_RICHEDIT_VER == 0x0100)
BOE_RTLDIR				= 0x0001	; Default paragraph direction (implies alignment) (obsolete) 
BOE_PLAINTEXT			= 0x0002	; Use plain text layout (obsolete) 
; #endif ; _RICHEDIT_VER == 0x0100 
BOE_NEUTRALOVERRIDE 	= 0x0004	; Override neutral layout (obsolete) 
BOE_CONTEXTREADING		= 0x0008	; Context reading order 
BOE_CONTEXTALIGNMENT	= 0x0010	; Context alignment 
; #if (_RICHEDIT_VER >= 0x0800)
BOE_FORCERECALC			= 0x0020	; Force recalc and redraw 
; #endif
BOE_LEGACYBIDICLASS		= 0x0040	; Legacy Bidi classification (obsolete) 
; #if (_RICHEDIT_VER >= 0x0600)
BOE_UNICODEBIDI			= 0x0080	; Use Unicode BiDi algorithm
; #endif

; Additional EM_FINDTEXT[EX] flags 
FR_MATCHDIAC			= 0x20000000
FR_MATCHKASHIDA 		= 0x40000000
FR_MATCHALEFHAMZA		= 0x80000000
	
; UNICODE embedding character 
; #ifndef WCH_EMBEDDING
WCH_EMBEDDING = 0xFFFC
; #endif ; WCH_EMBEDDING 

; khyph - Kind of hyphenation
; typedef enum tagKHYPH
; {
khyphNil = 0				; No Hyphenation
khyphNormal = 1				; Normal Hyphenation
khyphAddBefore = 2 			; Add letter before hyphen
khyphChangeBefore = 3		; Change letter before hyphen
khyphDeleteBefore = 4		; Delete letter before hyphen
khyphChangeAfter = 4		; Change letter after hyphen
khyphDelAndChange = 5		; Delete letter before hyphen and change
							;	letter preceding hyphen
; } KHYPH;

struct HYPHRESULT
	khyph 	rd 1;			; Kind of hyphenation
	ichHyph rd 1;			; Character which was hyphenated
	chHyph 	rw 1;			; Depending on hyphenation type, character added, changed, etc.
ends

; void WINAPI HyphenateProc(_In_ WCHAR *pszWord, LANGID langid, long ichExceed, HYPHRESULT *phyphresult);
struct HYPHENATEINFO
	cbSize			rw 1;			; Size of HYPHENATEINFO structure
	dxHyphenateZone	rw 1;	; If a space character is closer to the margin
							;	than this value, don't hyphenate (in TWIPs)
	pfnHyphenate 	rptr 1;
ends

; #ifdef _WIN32
; #include <poppack.h>
; #elif !defined(RC_INVOKED)
; #pragma pack()
; #endif

; Additional class for Richedit 6.0
; #if (_RICHEDIT_VER >= 0x0600 )
; #ifndef RICHEDIT60_CLASS
RICHEDIT60_CLASS equ "RICHEDIT60W"
; #endif
; #endif

; The following identifiers are deprecated
; They were in previous versions of richedit.h
PFA_FULL_NEWSPAPER	 = 5	; Not implemented
PFA_FULL_INTERLETTER = 6	; Not implemented
PFA_FULL_SCALED 	 = 7	; Not implemented
PFA_FULL_GLYPHS 	 = 8	; Not implemented

; This is deprecated identifier.
; This constant is used for a discontinued lparam value in EM_AUTOURLDETECT handling
AURL_ENABLEEA			= 1	; Deprecated: use AURL_ENABLEEAURLS

GCM_TOUCHMENU		= 0x4000	; Deprecated: Use GCMF_TOUCHMENU
GCM_MOUSEMENU		= 0x2000	; Deprecated: Use GCMF_MOUSEMENU

.alignmode_restore_pack
.alignmode_previous

@endm
