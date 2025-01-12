include "GraphicsObjUtils.inc"

importlib gdi32,\
	CreateDCA,\
	CreateCompatibleBitmap,\
	SelectObject,\
	DeleteObject,\
	CreatePen,\
	Rectangle,\
	CreateCompatibleDC,\
	CreateSolidBrush,\
	DeleteDC,\
	CreatePatternBrush,\
	CreateHatchBrush,\
	Ellipse,\
	RoundRect,\
	SetTextColor,\
	SetBkColor,\
	SetBkMode,\
	GetTextColor,\
	GetBkColor,\
	GetBkMode,\
	GetStockObject

importlib user32,\
	DrawTextA,\
	DrawTextW,\
	FillRect,\
	FrameRect

macro Graphics.createObjects this, [args]{
	common
	local _this, field
	_BMP#field equ Graphics.BMP
	_pen#field equ Graphics.Pen
	_sBrush#field equ Graphics.Brush
	_pBrush#field equ Graphics.Brush
	_hBrush#field equ Graphics.Brush
	inlineObj _this, this
	forward
	match prefix:value, args\{
		local ..noHollow, ..hollow, .noErr
		@call GraphicsObjUtils:create\#prefix(addr _this, value)
		match nameField, prefix\#field\\{
			@call Graphics:selectObject(addr _this, rax, nameField)
		\\} 
	\}
}

; macro Graphics.destroyObjects this, [args]{
; 	common
; 	local _this, field
; 	_BMP#field equ __hBMP_prev
; 	_pen#field equ __hPen_prev
; 	_brush#field equ __hBrush_prev
; 	inlineObj _this, this
; 	forward
; 	local ..noHollow, ..hollow, .noErr
; 	match nameField, args#field\{
; 		xor edx, edx
; 		xchg pdx, [_this+Graphics.\#nameField]
; 		@call [SelectObject]([_this+Graphics.hDC], [_this+Graphics.\#nameField])
; 		@call [DeleteObject](rax)
; 	\} 
; }

; macro Graphics.selectObjects this, [args]{
; 	common
; 	local _this, field
; 	_BMP#field equ __hBMP_prev
; 	_pen#field equ __hPen_prev
; 	_brush#field equ __hBrush_prev
; 	inlineObj _this, this
; 	forward
; 	match prefix:value, args\{
; 		local ..noHollow, ..hollow, .noErr
; 		@call [SelectObject]([_this+Graphics.hDC], value)
; 		match nameField, prefix\#field\\{
; 			cmp [_this+Graphics.\\#nameField], 0
; 			jne ..noHollow
; 				mov [_this+Graphics.\\#nameField], rax
; 			..noHollow:
; 		\\} 
; 	\}
; }

macro Graphics.destroyObject this, elemIndex{
	local _this
	inlineObj _this, this, pcx
	xor edx, edx
	xchg pdx, [_this+Graphics.__hArr+elemIndex*8]
	@call [SelectObject]([_this+Graphics.__hArr+Graphics.hDC], pdx)
	@call [DeleteObject](rax)
}

macro Graphics.freeObject this, elemIndex{
	local _this
	inlineObj _this, this, pcx
	xor edx, edx
	xchg pdx, [_this+Graphics.__hArr+elemIndex*8]
	@call [SelectObject]([_this+Graphics.hDC], pdx)
}

macro Graphics.rectangle this, x, y, _x, _y{
	local _this
	inlineObj _this, this, pcx
	@call [Rectangle]([_this+Graphics.hDC], x, y, _x, _y)
}

macro Graphics.ellipse this, x, y, _x, _y{
	local _this
	inlineObj _this, this, pcx
	@call [Ellipse]([_this+Graphics.hDC], x, y, _x, _y)
}

macro Graphics.roundRect this, x, y, _x, _y, width, height{
	local _this
	inlineObj _this, this, pcx
	@call [RoundRect]([_this+Graphics.hDC], x, y, _x, _y, width, height)
}

macro Graphics.drawText this, lpStr, lpRect, [args]{
	common
	local _this, sufx
	_format#sufx equ DT_NOCLIP or DT_SINGLELINE
	_len#sufx equ -1

	forward
	match prefix:value, args\{
		restore prefix\#sufx
		prefix\#sufx equ value
	\}

	common
	inlineObj _this, this, pcx
	@call [DrawTextA]([_this+Graphics.hDC], lpStr, _len#sufx, lpRect, _format#sufx)
	restore _len#sufx, _format#sufx
}

macro Graphics.drawTextW this, lpStr, lpRect, [args]{
	common
	local _this, sufx
	_format#sufx equ DT_NOCLIP or DT_SINGLELINE
	_len#sufx equ -1

	forward
	match prefix:value, args\{
		restore prefix\#sufx
		prefix\#sufx equ value
	\}

	common
	inlineObj _this, this, pcx
	@call [DrawTextW]([_this+Graphics.hDC], lpStr, _len#sufx, lpRect, _format#sufx)
	restore _len#sufx, _format#sufx
}

macro Graphics.setBkColor this, colorref{
	local _this
	inlineObj _this, this, pcx
	@call [SetBkColor]([_this+Graphics.hDC], colorref)
}

macro Graphics.getBkColor this{
	local _this
	inlineObj _this, this, pcx
	@call [GetBkColor]([_this+Graphics.hDC])
}

macro Graphics.setTextColor this, colorref{
	local _this
	inlineObj _this, this, pcx
	@call [SetTextColor]([_this+Graphics.hDC], colorref)
}

macro Graphics.getTextColor this{
	local _this
	inlineObj _this, this, pcx
	@call [GetTextColor]([_this+Graphics.hDC])
}

macro Graphics.setBkMode this, mode{
	local _this
	inlineObj _this, this, pcx
	@call [SetBkMode]([_this+Graphics.hDC], mode)
}

macro Graphics.getBkMode this{
	local _this
	inlineObj _this, this, pcx
	@call [GetBkMode]([_this+Graphics.hDC])
}

macro Graphics.setDCBrushColor this, colorref{
	local _this
	inlineObj _this, this, pcx
	@call [SetDCBrushColor]([_this+Graphics.hDC], colorref)
}

macro Graphics.getDCBrushColor this{
	local _this
	inlineObj _this, this, pcx
	@call [GetDCBrushColor]([_this+Graphics.hDC])
}

macro Graphics.setDCPenColor this, colorref{
	local _this
	inlineObj _this, this, pcx
	@call [SetDCPenColor]([_this+Graphics.hDC], colorref)
}

macro Graphics.getDCPenColor this{
	local _this
	inlineObj _this, this, pcx
	@call [GetDCPenColor]([_this+Graphics.hDC])
}

macro Graphics.fillRect this, lpRect, hBrush{
	local _this
	inlineObj _this, this, pcx
	@call [FillRect]([_this+Graphics.hDC], lpRect, hBrush)
}

macro Graphics.frameRect this, lpRect, hBrush{
	local _this
	inlineObj _this, this, pcx
	@call [FrameRect]([_this+Graphics.hDC], lpRect, hBrush)
}

proc_noprologue
	
proc Graphics.create uses rbx, this, isBW
	virtObj .this:arg Graphics at rbx from rcx
	mov [isBW], rdx
	@call [CreateDCA]("DISPLAY", NULL, NULL)
	mov [.this.hDC], rax
	@call [CreateCompatibleBitmap]([.this.hDC], 1, 1)
	mov [.this.__hBMP_prev], rax
	@call [CreateCompatibleDC]([.this.hDC])
	xchg rax, [.this.hDC]
	@call [DeleteDC](rax)
	@call [SelectObject]([.this.hDC], [.this.__hBMP_prev])
	mov [.this.__hBMP_prev], rax
	ret
endp

proc Graphics.createCompatible, this, hDC
	mov [this], rcx
	@call [CreateCompatibleDC](rdx)
	mov rcx, [this]
	mov [rcx+Graphics.hDC], rax
	ret
endp

proc Graphics.destroy uses rbx rsi, this
	virtObj .this:arg Graphics at rbx from rcx
	mov rsi, 5
	.loop1:
		cmp [.this.__hArr+(rsi-1)*8], 0
		je .inActive
			@call [SelectObject]([.this.hDC], [.this.__hArr+rsi])
			@call [DeleteObject](rax)
			mov [.this.__hArr+rsi], 0
		.inActive:
	dec rsi
	jnz .loop1
	@jret [DeleteDC]([.this.hDC])
endp

proc Graphics.selectObject, this, handle, elemIndex
	virtObj .this:arg Graphics
	mov [this], rcx
	mov [elemIndex], r8
	@call [SelectObject]([.this.hDC], pdx)
	mov rcx, [this]
	mov r8, [elemIndex]
	cmp [.this.__hArr+r8*8], 0
	jne ..noHollow
		mov [.this.__hArr+r8*8], rax
		ret
	..noHollow:
	@jret [DeleteObject](rax)
endp

proc_resprologue

; proc Graphics.sreateObject

; __safeStack = 32
; ;__restoreRegs
; macro @safeObjs [name, value, reg]{
; 	common
; 	local regs_restored, corrector
; 	corrector = 0
; 	__restoreRegs equ corrector|regs_restored
; 	forward
; 	local _valBuf, need_restore
; 	_valBuf equ value
; 	regs_restored equ reg|need_restore
; 	need_restore = 0
; 	param@parser _valBuf#type, _valBuf
; 	if _valBuf eqtype 0
; 		if _valBuf relativeto 0
; 			virtual at _valBuf
; 				name dq ?
; 			end virtual
; 		else if _valBuf relativeto rbx
; 			virtual at _valBuf
; 				name dq ?
; 			end virtual
; 		else if _valBuf relativeto rsi
; 			virtual at _valBuf
; 				name dq ?
; 			end virtual
; 		else if _valBuf relativeto rdi
; 			virtual at _valBuf
; 				name dq ?
; 			end virtual
; 		else if _valBuf relativeto r12
; 			virtual at _valBuf
; 				name dq ?
; 			end virtual
; 		else if _valBuf relativeto r13
; 			virtual at _valBuf
; 				name dq ?
; 			end virtual
; 		else if _valBuf relativeto r14
; 			virtual at _valBuf
; 				name dq ?
; 			end virtual
; 		else if _valBuf relativeto r15
; 			virtual at _valBuf
; 				name dq ?
; 			end virtual
; 		else
; 			if defined current@frame
; 				if current@frame < __safeStack+8
; 					current@frame = __safeStack+16
; 				end if
; 				mov [rsp+__safeStack], reg
; 				__safeStack = __safeStack + 8
; 			else
; 				push reg
; 				corrector = corrector + 1
; 			end if
; 			fillParam reg, value
; 			virtual at reg
; 				name dq ?
; 			end virtual
; 			need_restore = 1
; 		end if
; 	else
; 		if defined current@frame
; 			if current@frame < __safeStack+8
; 				current@frame = __safeStack+16
; 			end if
; 			mov [rsp+__safeStack], reg
; 			__safeStack = __safeStack + 8
; 		else
; 			push reg
; 			corrector = corrector + 1
; 		end if
; 		fillParam reg, value
; 		virtual at reg
; 			name dq ?
; 		end virtual
; 		need_restore = 1
; 	end if
; 	if corrector mod 2 > 0
; 		sub rsp, 8
; 	end if
; } 

; macro @safeRestore{
; 	local locname
; 	match corrector|locname, __restoreRegs\{
; 		if corrector mod 2 > 0
; 			add rsp, 8
; 		end if
; 		irpv regneed, locname\\{
; 			match reg|need_restore, regneed\\\{
; 				if need_restore
; 					if defined current@frame
; 						__safeStack = __safeStack - 8
; 						mov reg, [rsp+__safeStack]
; 					else
; 						pop reg
; 					end if
; 				end if
; 			\\\}
; 		\\}
; 	\}
; 	restore __restoreRegs
; }