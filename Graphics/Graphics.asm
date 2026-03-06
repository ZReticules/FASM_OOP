; include "GraphicsObjUtils.inc"

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
	GetStockObject,\
	SetDCBrushColor,\
	GetDCBrushColor,\
	SetDCPenColor,\
	GetDCPenColor

importlib user32,\
	DrawTextA,\
	DrawTextW,\
	FillRect,\
	FrameRect

define DC_BRUSH 18 
define DC_PEN 19

; macro Graphics.createObjects this, [args]{
; 	common
; 	local _this, field
; 	_BMP#field equ Graphics.BMP
; 	_pen#field equ Graphics.PEN
; 	_sBrush#field equ Graphics.BRUSH
; 	_pBrush#field equ Graphics.BRUSH
; 	_hBrush#field equ Graphics.BRUSH
; 	inlineObj _this, this
; 	reverse
; 	match prefix:value, args\{
; 		local ..noHollow, ..hollow, .noErr
; 		$call GraphicsObjUtils::create\#prefix(addr _this, value)
; 		match nameField, prefix\#field\\{
; 			$call Graphics::selectObject(addr _this, pax, nameField)
; 		\\} 
; 	\}
; }

macro Graphics.make this, hDC{
	match =2, __argscount__\{
		$call createCompatible<Graphics>(this, hDC)
	\}
	match =1, __argscount__\{
		$call create<Graphics>(this)
	\}
}

macro Graphics.createBMP this, args&{
	match =4, __argscount__\{
		$call createCompatibleBMP<Graphics>(args)
	\}
	match =3, __argscount__\{
		local _this
		inlineObj _this, this, rcx
		$call createCompatibleBMP<Graphics>(&_this, [_this+Graphics.hDC], args)
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
; 		xchg pdx, [_this + Graphics.\#nameField]
; 		$call [SelectObject]([_this + Graphics.hDC], [_this + Graphics.\#nameField])
; 		$call [DeleteObject](rax)
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
; 		$call [SelectObject]([_this + Graphics.hDC], value)
; 		match nameField, prefix\#field\\{
; 			cmp [_this + Graphics.\\#nameField], 0
; 			jne ..noHollow
; 				mov [_this + Graphics.\\#nameField], rax
; 			..noHollow:
; 		\\} 
; 	\}
; }

macro Graphics.destroyObject this, elemIndex{
	local _this
	inlineObj _this, this, pcx
	xor edx, edx
	xchg pdx, [_this + Graphics.__hArr + elemIndex * pointer.size]
	$call [SelectObject]([_this + Graphics.__hArr+Graphics.hDC], pdx)
	$call [DeleteObject](pax)
}

macro Graphics.freeObject this, elemIndex{
	local _this
	inlineObj _this, this, pcx
	xor edx, edx
	xchg pdx, [_this + Graphics.__hArr + elemIndex * pointer.size]
	$call [SelectObject]([_this + Graphics.hDC], pdx)
}

macro Graphics.rectangle this, x, y, _x, _y{
	local _this
	inlineObj _this, this, pcx
	$call [Rectangle]([_this + Graphics.hDC], x, y, _x, _y)
}

macro Graphics.ellipse this, x, y, _x, _y{
	local _this
	inlineObj _this, this, pcx
	$call [Ellipse]([_this + Graphics.hDC], x, y, _x, _y)
}

macro Graphics.roundRect this, x, y, _x, _y, width, height{
	local _this
	inlineObj _this, this, pcx
	$call [RoundRect]([_this + Graphics.hDC], x, y, _x, _y, width, height)
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
	$call [DrawTextA]([_this + Graphics.hDC], lpStr, _len#sufx, lpRect, _format#sufx)
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
	$call [DrawTextW]([_this + Graphics.hDC], lpStr, _len#sufx, lpRect, _format#sufx)
	restore _len#sufx, _format#sufx
}

macro Graphics.setBkColor this, colorref{
	local _this
	inlineObj _this, this, pcx
	$call [SetBkColor]([_this + Graphics.hDC], colorref)
}

macro Graphics.getBkColor this{
	local _this
	inlineObj _this, this, pcx
	$call [GetBkColor]([_this + Graphics.hDC])
}

macro Graphics.setTextColor this, colorref{
	local _this
	inlineObj _this, this, pcx
	$call [SetTextColor]([_this + Graphics.hDC], colorref)
}

macro Graphics.getTextColor this{
	local _this
	inlineObj _this, this, pcx
	$call [GetTextColor]([_this + Graphics.hDC])
}

macro Graphics.setBkMode this, mode{
	local _this
	inlineObj _this, this, pcx
	$call [SetBkMode]([_this + Graphics.hDC], mode)
}

macro Graphics.getBkMode this{
	local _this
	inlineObj _this, this, pcx
	$call [GetBkMode]([_this + Graphics.hDC])
}

macro Graphics.setDCBrushColor this, colorref{
	local _this
	inlineObj _this, this, pcx
	$call [SetDCBrushColor]([_this + Graphics.hDC], colorref)
}

macro Graphics.getDCBrushColor this{
	local _this
	inlineObj _this, this, pcx
	$call [GetDCBrushColor]([_this + Graphics.hDC])
}

macro Graphics.setDCPenColor this, colorref{
	local _this
	inlineObj _this, this, pcx
	$call [SetDCPenColor]([_this + Graphics.hDC], colorref)
}

macro Graphics.getDCPenColor this{
	local _this
	inlineObj _this, this, pcx
	$call [GetDCPenColor]([_this + Graphics.hDC])
}

macro Graphics.fillRect this, lpRect, hBrush{
	local _this
	inlineObj _this, this, pcx
	$call [FillRect]([_this + Graphics.hDC], lpRect, hBrush)
}

macro Graphics.frameRect this, lpRect, hBrush{
	local _this
	inlineObj _this, this, pcx
	$call [FrameRect]([_this + Graphics.hDC], lpRect, hBrush)
}

.proc_frame_mode_static

.proc cdecl Graphics.createCompatibleBMP(.pthis:P_Graphics, .hDC, .cx, .cy)
	@sarg @arg1
	$call [CreateCompatibleBitmap](@arg2, @arg3, @arg4)
	$call [.pthis]::selectObject(pax, Graphics.BMP)
	ret
.endp

.proc cdecl Graphics.createPen(.pthis:P_Graphics, .style, .width, .colorref)
	@sarg @arg1
	$call [CreatePen](@arg2, @arg3, @arg4)
	$call [.pthis]::selectObject(pax, Graphics.PEN)
	ret
.endp

.proc cdecl Graphics.createSolidBrush(.pthis:P_Graphics, .colorref)
	@sarg @arg1
	$call [CreateSolidBrush](@arg2)
	$call [.pthis]::selectObject(pax, Graphics.BRUSH)
	ret
.endp

.proc cdecl Graphics.createPatternBrush(.pthis:P_Graphics, .hBMP)
	@sarg @arg1
	$call [CreatePatternBrush](@arg2)
	$call [.pthis]::selectObject(pax, Graphics.BRUSH)
	ret
.endp

.proc cdecl Graphics.createHatchBrush(.pthis:P_Graphics, .iHatch, .colorref)
	@sarg @arg1
	$call [CreateHatchBrush](@arg2, @arg3)
	$call [.pthis]::selectObject(pax, Graphics.BRUSH)
	ret
.endp
	
.proc cdecl Graphics.create(.pthis) uses pbx
	virtObj .this Graphics at pbx from @arg1
	$call [CreateDCA]("DISPLAY", NULL, NULL, NULL)
	mov [.this.hDC], pax
	$call [CreateCompatibleBitmap]([.this.hDC], 1, 1)
	mov [.this.__hBMP_prev], pax
	$call [CreateCompatibleDC]([.this.hDC])
	xchg pax, [.this.hDC]
	$call [DeleteDC](pax)
	$call [SelectObject]([.this.hDC], [.this.__hBMP_prev])
	mov [.this.__hBMP_prev], pax
	ret
.endp

.proc cdecl Graphics.createCompatible(.pthis, .hDC)
	@sarg @arg1
	$call [CreateCompatibleDC](@arg2)
	mov pcx, [.pthis]
	mov [pcx + Graphics.hDC], pax
	ret
.endp

.proc cdecl Graphics.unmake(.pthis) uses pbx psi
	virtObj .this Graphics at pbx from @arg1
	mov psi, 5
	.loop1:
		cmp [.this.__hArr + (psi - 1) * pointer.size], 0
		je .inActive
			$call [SelectObject]([.this.hDC], [.this.__hArr + psi])
			$call [DeleteObject](pax)
			mov [.this.__hArr + psi], 0
		.inActive:
	dec psi
	jnz .loop1
	$call [DeleteDC]([.this.hDC])
	ret
.endp

.proc cdecl Graphics.selectObject(.pthis, .handle, .elemIndex)
	@sarg @arg1, @arg3
	virtObj .this Graphics at pcx from @arg1
	$call [SelectObject]([.this.hDC], @arg2)
	mov pcx, [.pthis]
	mov pdx, [.elemIndex]
	cmp [.this.__hArr + pdx * pointer.size], 0
	jne ..noHollow
		mov [.this.__hArr + pdx * pointer.size], pax
		ret
	..noHollow:
	$call [DeleteObject](pax)
	ret
.endp

.proc_frame_mode_previous

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