importlib msvcrt,\
	_iob,\
	puts,\
	fputs,\
	fgets,\
	putch = "putchar",\
	getch="_getwch",\
	strlen,\
	strcpy,\
	malloc

proc_noprologue

; proc Console.read
; 	@call [getch]()
; 	cmp eax, 0E0h
; 	jne @f
; 		jmp .special
; 	@@:
; 	cmp eax, 0
; 	jne @f
; 		.special:
; 		@call [getch]()
; 		or eax, 10000h
; 	@@:
; 	ret
; endp

proc Console.setCurPos
	mov r8, rcx
	mov rcx, .outfmt
	jmp [printf]
	.outfmt db 1Bh, "[%d;%df", 0
endp

Console.hideCur:
	mov rcx, .outfmt
	mov rdx, [stdout]
	jmp [fputs]
	.outfmt db 1Bh, "[?25l", 0

Console.showCur:
	mov rcx, .outfmt
	mov rdx, [stdout]
	jmp [fputs]
	.outfmt db 1Bh, "[?25h", 0

Console.changeCur:
	lea rdx, [rcx+30h]
	mov rcx, .outfmt
	jmp [printf]
	.outfmt db 1Bh, "[%c q", 0

Console.saveCurPos:
	mov rcx, .outfmt
	mov rdx, [stdout]
	jmp [fputs]
	.outfmt db 1Bh, "7", 0

Console.restCurPos:
	mov rcx, .outfmt
	mov rdx, [stdout]
	jmp [fputs]
	.outfmt db 1Bh, "8", 0
	
Console.curMove:
	mov r8, rdx
	mov rdx, rcx
	mov rcx, .outfmt
	jmp [printf]
	.outfmt db 1Bh, "[%d%c", 0

proc_resprologue