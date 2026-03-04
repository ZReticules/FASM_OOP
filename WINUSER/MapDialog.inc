importlib user32,\
    GetDC,\
    ReleaseDC
importlib gdi32,\
    SelectObject,\
    GetTextMetricsA,\
    GetTextExtentPoint32A,\
    GetCharWidth32A

@const_align equ 16
; MapDialog.shiftMap @const dd 2, 3, 2, 3
MapDialog.addMap @const dd 2, 4, 2, 4
; MapDialog.andMask @const dd 11b, 111b, 11b, 111b
; MapDialog.shiftMap2 @const dd 1, 2, 1, 2
restore @const_align

struct MapDialog
    _size        SIZE

	make 		dm this
	unmake 		dm this:inline
	mapRect		dm this:inline
    mapPoint    dm this:inline
ends

.proc_frame_mode_static

.proc cdecl MapDialog.make(.pthis, .hWnd) uses pbx
	virtObj .this:arg MapDialog at pbx from @arg1

	@sarg @arg2
    .local .coeffRect:RECT
    $call CNV|fill(&.coeffRect, <const RECT 4, 8, 0, 0>, sizeof.RECT)
    $call [MapDialogRect]([.hWnd], &.coeffRect)
    $call CNV|fill(&.this._size, &.coeffRect, sizeof.SIZE)
	ret
.endp

macro MapDialog.unmake this{}

macro MapDialog.mapRect this, lpRect{
    local _this, _rect
    inlineObj _this, this, pdx
    inlineObj _rect, lpRect, pcx
    movups xmm0, xword[_rect]
    movq xmm1, qword[_this + MapDialog._size]
    punpcklqdq xmm1, xmm1                ; xmmN[127:64]=xmmN[63:0]
    pmulld xmm0, xmm1
    paddd xmm0, [MapDialog.addMap]
    ; pshufd xmm0, xmm0, 
    ; vpsrlvd xmm0, xmm0, [MapDialog.shiftMap]
    movups xword[_rect], xmm0
    shr [_rect + RECT.left], 1
    shr [_rect + RECT.top], 2
    shr [_rect + RECT.right], 1
    shr [_rect + RECT.bottom], 2
}

macro MapDialog.mapPoint this, lpPoint{
    local _this, _rect
    inlineObj _this, this, pdx
    inlineObj _point, lpPoint, pcx
    movq xmm0, qword[_point]
    movq xmm1, qword[_this + MapDialog._size]
    pmulld xmm0, xmm1
    paddd xmm0, [MapDialog.addMap]
    movq qword[_point], xmm0
    shr [_point + POINT.x], 1
    shr [_point + POINT.y], 2
}

.proc_frame_mode_previous
