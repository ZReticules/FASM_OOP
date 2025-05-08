importlib user32,\
    GetDC,\
    ReleaseDC
importlib gdi32,\
    SelectObject,\
    GetTextMetricsA,\
    GetTextExtentPoint32A,\
    GetCharWidth32A

@const_align equ 16
MapDialog.shiftMap @const dd 2, 3, 2, 3
MapDialog.addMap @const dd 2, 4, 2, 4
; MapDialog.andMask @const dd 11b, 111b, 11b, 111b
; MapDialog.shiftMap2 @const dd 1, 2, 1, 2
restore @const_align

struct MapDialog
    _size        SIZE

	make 		dm this
	unmake 		dm this:inline
	mapRect		dm this:inline
ends

proc MapDialog.make c uses pbx, this, hWnd
	virtObj .this:arg MapDialog at pbx from @arg1
    .alphabet @const db "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

	@sarg @arg2
    local coeffRect:RECT
    @call CNV::fill(addr coeffRect, <const RECT 4, 8, 0, 0>, sizeof.RECT)
    @call [MapDialogRect]([hWnd], addr coeffRect)
    @call CNV::fill(addr .this._size, addr coeffRect, sizeof.SIZE)
	ret
endp

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
    vpsrlvd xmm0, xmm0, [MapDialog.shiftMap]
    movups xword[_rect], xmm0
}

; proc MapDialog.mapRect, hwndDlg, lpRect
    ; // Получаем HDC
    ; HDC hdc = GetDC(hwndDlg);

    ; // Получаем шрифт, используемый в окне (обычно диалоговый)
    ; HFONT hFont = (HFONT)SendMessage(hwndDlg, WM_GETFONT, 0, 0);
    ; HFONT hOldFont = NULL;

    ; if (hFont)
    ;     hOldFont = (HFONT)SelectObject(hdc, hFont);

    ; // Высчитываем точную среднюю ширину символа через строку
    ; const wchar_t* sample = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    ; SIZE sz;
    ; GetTextExtentPoint32W(hdc, sample, 52, &sz);
    ; int avgCharWidth = (sz.cx + 13) / 26; // округление как в оригинале

    ; // Получаем метрики шрифта
    ; TEXTMETRIC tm;
    ; GetTextMetricsA(hdc, &tm);

    ; // Средняя ширина символа и высота строки
    ; int avgCharHeight = tm.tmHeight;

    ; if (hOldFont)
    ;     SelectObject(hdc, hOldFont);
    ; ReleaseDC(hwndDlg, hdc);

    ; // Переводим диалоговые единицы в пиксели
    ; rectDLU->left   = MulDiv(rectDLU->left,   avgCharWidth, 4);
    ; rectDLU->top    = MulDiv(rectDLU->top,    avgCharHeight, 8);
    ; rectDLU->right  = MulDiv(rectDLU->right,  avgCharWidth, 4);
    ; rectDLU->bottom = MulDiv(rectDLU->bottom, avgCharHeight, 8);
; endp

; proc MapDialog.make c uses pbx psi pdi, this, hWnd
;     virtObj .this:arg MapDialog at pbx from @arg1

;     .alphabet @const db "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

;     @sarg @arg2

;     local hDC:POINTER, hOldFont:POINTER, tm:TEXTMETRIC, sz:POINTER
;     @call [hDC] = [GetDC](@arg2)
;     @call [hOldFont] = [SendMessageA]([hWnd], WM_GETFONT, NULL, NULL)
;     test pax, pax
;     jz @f
;         @call [hOldFont] = [SelectObject]([hWnd], [hOldFont])
;     @@:
;     @call [GetTextMetricsA]([hDC], addr tm)
;     mov eax, [tm.tmHeight]
;     mov [.this.height], eax
;     @call [GetTextExtentPoint32A]([hDC], .alphabet, .alphabet.len, addr sz)
;     xor psi, psi
;     mov pdi, "Z" - "A"
;     @@:
;         @call [GetCharWidth32A]([hDC], addr pdi + "A", addr pdi + "A", addr sz)
;         add psi, [sz]
;     dec pdi
;     jns @b
;     mov pdi, "z" - "a"
;     @@:
;         @call [GetCharWidth32A]([hDC], addr pdi + "z", addr pdi + "z", addr sz)
;         add psi, [sz]
;     dec pdi
;     jns @b
;     ; int3
;     mov eax, esi
;     ; add eax, 26
;     mov ecx, 52
;     xor edx, edx
;     div ecx
;     mov [.this.width], eax
;     cmp [hOldFont], 0
;     je @f
;         @call [SelectObject]([hWnd], [hOldFont])
;     @@:
;     ret
; endp

    ; local hDC:POINTER, hOldFont:POINTER, tm:TEXTMETRIC, sz:SIZE
    ; @call [hDC] = [GetDC](@arg2)
    ; @call [hOldFont] = [SendMessageA]([hWnd], WM_GETFONT, NULL, NULL)
    ; ; int3
    ; test pax, pax
    ; jz @f
    ;     @call [hOldFont] = [SelectObject]([hWnd], [hOldFont])
    ; @@:
    ; @call [GetTextMetricsA]([hDC], addr tm)
    ; mov eax, [tm.tmHeight]
    ; mov [.this.height], eax
    ; @call [GetTextExtentPoint32A]([hDC], .alphabet, .alphabet.len, addr sz)
    ; ; int3
    ; mov eax, [sz.cx]
    ; add eax, 26
    ; mov ecx, 52
    ; xor edx, edx
    ; div ecx
    ; mov [.this.width], eax
    ; cmp [hOldFont], 0
    ; je @f
    ;     @call [SelectObject]([hWnd], [hOldFont])
    ; @@: