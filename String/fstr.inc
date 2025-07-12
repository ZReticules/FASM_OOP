FSTR_MINUS_FLAG equ 1
FSTR_PLUS_FLAG equ 10b
FSTR_ZERO_FLAG equ 100b
FSTR_SPACE_FLAG equ 1000b

FSTR_HAS_FLAGS equ 00010000b
FSTR_HAS_WIDTH equ 00100000b
FSTR_HAS_PRECISION equ 01000000b

FSTR_SIZE       equ 11110000b  
FSTR_SIZE_DWORD equ (0 shl 4)       ; default
FSTR_SIZE_BYTE  equ (1 shl 4)        ; t
FSTR_SIZE_WORD  equ (10b shl 4)      ; h
FSTR_SIZE_QWORD equ (11b shl 4)     ; l

macro fstr [args]{
    common
    local lendata, bytenow, smb, num
    forward
    local ..base, ..unformatData, ..start, ..end, ..end_format
    ; define no@local
    virtual at 0
        ..unformatData::
        ..start:
        db args
        ..end:
        lendata = ..end - ..start
    end virtual
    ; purge no@local
    ; dd $-..end_format
    bytenow = 0
    while bytenow < lendata
        load smb byte from ..unformatData:bytenow
        bytenow = bytenow + 1
        if smb = '\'
            load smb byte from ..unformatData:bytenow
            bytenow = bytenow + 1
            if smb = 'a'
                smb = 07h
            else if smb = 'b'
                smb = 08h
            else if smb = 'e'
                smb = 1Bh
            else if smb = 'f'
                smb = 0Ch
            else if smb = 'n'
                smb = 0Ah
            else if smb = 'r'
                smb = 0Dh
            else if smb = 't'
                smb = 09h
            else if smb = 'e'
                smb = 1Bh
            else if smb = 'v'
                smb = 0Bh
            else if smb = '\' | smb = '?' | smb = '"' | smb = "'"
                ;smb = smb
            else if smb = 'x'
                load smb byte from ..unformatData:bytenow
                bytenow = bytenow + 1
                if smb >= 'a' & smb <= 'f'
                    smb = (smb and 0DFh)
                end if
                smb = smb - 0x30
                if smb > 09h
                    smb = smb - 7
                end if
                num = smb
                if bytenow < lendata
                    load smb byte from ..unformatData:bytenow
                    if (smb >= '0' & smb <='9') | smb >= 'A' & smb <= 'F' | smb >= 'a' & smb <= 'f'
                        if smb >= 'a' & smb <= 'f'
                            smb = (smb and 0DFh)
                        end if
                        smb = smb - 30h
                        if smb > 09h
                            smb = smb - 7
                        end if
                        num = num shl 4 + smb
                        bytenow = bytenow + 1
                    end if
                end if
                smb = num
            else if smb < '8'
                num = smb - 30h
                if bytenow < lendata
                    load smb byte from ..unformatData:bytenow
                    if smb < '8'
                        bytenow = bytenow + 1
                        num = num shl 3 + (smb - 30h)
                        if bytenow < lendata
                            load smb byte from ..unformatData:bytenow
                            if smb < '8' & num < 255
                                bytenow = bytenow + 1
                                num = num shl 3 + (smb - 30h)
                            end if
                        end if
                    end if
                end if
                smb = num
            else
                display "Unknown sequence"
                err
            end if
            db smb
        else if smb = '%'
            load smb byte from ..unformatData:bytenow
            bytenow = bytenow + 1
            local _flags, flags, width, precision, type

            _flags = 0
            flags = 0
            while 1
                if smb = "-"
                    flags = flags or FSTR_MINUS_FLAG
                else if smb = "+"
                    flags = flags or FSTR_PLUS_FLAG
                else if smb = "0"
                    flags = flags or FSTR_ZERO_FLAG
                else if smb = " "
                    flags = flags or FSTR_SPACE_FLAG
                else
                    break
                end if
                _flags = _flags or 0001b
                load smb byte from ..unformatData:bytenow
                bytenow = bytenow + 1
            end while

            width = 0
            if smb = "*"
                width = -2
                load smb byte from ..unformatData:bytenow
                bytenow = bytenow + 1
            else
                while smb <= "9" & smb >= "0"
                    width = width * 10 + (smb - 0x30)
                    load smb byte from ..unformatData:bytenow
                    bytenow = bytenow + 1
                end while
            end if

            if width <> 0
                _flags = _flags or 0010b
            end if

            if smb = "."
                _flags = _flags or 0100b
                precision = 0
                load smb byte from ..unformatData:bytenow
                bytenow = bytenow + 1
                if smb = "*"
                    precision = -2
                    load smb byte from ..unformatData:bytenow
                    bytenow = bytenow + 1
                else
                    while smb <= "9" & smb >= "0"
                        precision = precision * 10 + (smb - 0x30)
                        load smb byte from ..unformatData:bytenow
                        bytenow = bytenow + 1
                    end while
                end if
            end if

            if smb = "t"
                _flags = _flags or 0001b 
                flags = flags or FSTR_SIZE_BYTE
                load smb byte from ..unformatData:bytenow
                bytenow = bytenow + 1
            else if smb = "h"
                _flags = _flags or 0001b 
                flags = flags or FSTR_SIZE_WORD
                load smb byte from ..unformatData:bytenow
                bytenow = bytenow + 1
            else if smb = "l"
                _flags = _flags or 0001b 
                flags = flags or FSTR_SIZE_QWORD
                load smb byte from ..unformatData:bytenow
                bytenow = bytenow + 1
            end if

            if smb = "c"
                type = 0
            else if smb = "d" | smb = "i"
                type = 1
            else if smb = "u"
                type = 2
            else if smb = "x"
                type = 3
            else if smb = "o"
                type = 4
            else if smb = "s"
                type = 5
            else if smb = "S"
                type = 6
            else if smb = "g"
                type = 7
            else if smb = "e"
                type = 8
            else if smb = "%"
                type = 15
            else
                display "unknown operation type: ", smb, 0Ah
                err
            end if

            db "%"
            db type or _flags shl 4
            if _flags and 0001b
                db flags
            end if
            if _flags and 0010b
                dd width
            end if
            if _flags and 0100b
                dd precision
            end if
        else
            db smb
        end if
    end while
    common
    db 0
    ..end_format:
}

struc fstr args&{
    local ..bytes, ..start, ..end, lendata
    virtual at 0
        ..bytes::
        ..start:
        fstr args
        ..end:
        lendata = ..end - ..start
    end virtual
    . db lendata dup(?)
    init@cstr ., lendata, ..bytes
    .len = lendata
}

macro fwstr [args]{
    common
    local lendata, wordnow, smb, num
    forward
    local ..base, ..unformatData, ..start, ..end, ..end_format
    ; define no@local
    virtual at 0
        ..unformatData::
        ..start:
        du args
        ..end:
        lendata = ..end - ..start
    end virtual
    ; purge no@local
    ; dd $-..end_format
    wordnow = 0
    while wordnow < (lendata / 2)
        load smb word from ..unformatData:wordnow * 2
        wordnow = wordnow + 1
        if smb = '\'
            load smb word from ..unformatData:wordnow * 2
            wordnow = wordnow + 1
            if smb = 'a'
                smb = 07h
            else if smb = 'b'
                smb = 08h
            else if smb = 'e'
                smb = 1Bh
            else if smb = 'f'
                smb = 0Ch
            else if smb = 'n'
                smb = 0Ah
            else if smb = 'r'
                smb = 0Dh
            else if smb = 't'
                smb = 09h
            else if smb = 'e'
                smb = 1Bh
            else if smb = 'v'
                smb = 0Bh
            else if smb = '\' | smb = '?' | smb = '"' | smb = "'"
                ;smb = smb
            else if smb = 'x'
                load smb word from ..unformatData:wordnow * 2
                wordnow = wordnow + 1
                if smb >= 'a' & smb <= 'f'
                    smb = (smb and 0DFh)
                end if
                smb = smb - 0x30
                if smb > 09h
                    smb = smb - 7
                end if
                num = smb
                if wordnow < lendata
                    load smb word from ..unformatData:wordnow * 2
                    if (smb >= '0' & smb <='9') | smb >= 'A' & smb <= 'F' | smb >= 'a' & smb <= 'f'
                        if smb >= 'a' & smb <= 'f'
                            smb = (smb and 0DFh)
                        end if
                        smb = smb - 30h
                        if smb > 09h
                            smb = smb - 7
                        end if
                        num = num shl 4 + smb
                        wordnow = wordnow + 1
                    end if
                end if
                smb = num
            else if smb < '8'
                num = smb - 30h
                if wordnow < lendata
                    load smb word from ..unformatData:wordnow * 2
                    if smb < '8'
                        wordnow = wordnow + 1
                        num = num shl 3 + (smb - 30h)
                        if wordnow < lendata
                            load smb word from ..unformatData:wordnow * 2
                            if smb < '8' & num < 255
                                wordnow = wordnow + 1
                                num = num shl 3 + (smb - 30h)
                            end if
                        end if
                    end if
                end if
                smb = num
            else
                display "Unknown sequence"
                err
            end if
            dw smb
        else if smb = '%'
            load smb word from ..unformatData:wordnow * 2
            wordnow = wordnow + 1
            local _flags, flags, width, precision, type

            _flags = 0
            flags = 0
            while 1
                if smb = "-"
                    flags = flags or FSTR_MINUS_FLAG
                else if smb = "+"
                    flags = flags or FSTR_PLUS_FLAG
                else if smb = "0"
                    flags = flags or FSTR_ZERO_FLAG
                else if smb = " "
                    flags = flags or FSTR_SPACE_FLAG
                else
                    break
                end if
                _flags = _flags or 0001b
                load smb word from ..unformatData:wordnow * 2
                wordnow = wordnow + 1
            end while

            width = 0
            if smb = "*"
                width = -2
                load smb word from ..unformatData:wordnow * 2
                wordnow = wordnow + 1
            else
                while smb <= "9" & smb >= "0"
                    width = width * 10 + (smb - 0x30)
                    load smb word from ..unformatData:wordnow * 2
                    wordnow = wordnow + 1
                end while
            end if

            if width <> 0
                _flags = _flags or 0010b
            end if

            if smb = "."
                _flags = _flags or 0100b
                precision = 0
                load smb word from ..unformatData:wordnow * 2
                wordnow = wordnow + 1
                if smb = "*"
                    precision = -2
                    load smb word from ..unformatData:wordnow * 2
                    wordnow = wordnow + 1
                else
                    while smb <= "9" & smb >= "0"
                        precision = precision * 10 + (smb - 0x30)
                        load smb word from ..unformatData:wordnow * 2
                        wordnow = wordnow + 1
                    end while
                end if
            end if

            if smb = "t"
                _flags = _flags or 0001b 
                flags = flags or FSTR_SIZE_BYTE
                load smb word from ..unformatData:wordnow * 2
                wordnow = wordnow + 1
            else if smb = "h"
                _flags = _flags or 0001b 
                flags = flags or FSTR_SIZE_WORD
                load smb word from ..unformatData:wordnow * 2
                wordnow = wordnow + 1
            else if smb = "l"
                _flags = _flags or 0001b 
                flags = flags or FSTR_SIZE_QWORD
                load smb word from ..unformatData:wordnow * 2
                wordnow = wordnow + 1
            end if

            if smb = "c"
                type = 0
            else if smb = "d" | smb = "i"
                type = 1
            else if smb = "u"
                type = 2
            else if smb = "x"
                type = 3
            else if smb = "o"
                type = 4
            else if smb = "s"
                type = 5
            else if smb = "S"
                type = 6
            else if smb = "g"
                type = 7
            else if smb = "e"
                type = 8
            else if smb = "%"
                type = 15
            else
                display "unknown operation type: ", smb, 0Ah
                err
            end if

            dw "%"
            db type or _flags shl 4
            if _flags and 0001b
                db flags
            end if
            if _flags and 0010b
                dd width
            end if
            if _flags and 0100b
                dd precision
            end if
        else
            dw smb
        end if
    end while
    common
    dw 0
    ..end_format:
}

struc fwstr args&{
    local ..bytes, ..start, ..end, lendata
    virtual at 0
        ..bytes::
        ..start:
        fwstr args
        ..end:
        lendata = ..end - ..start
    end virtual
    . db lendata dup(?)
    init@cstr ., lendata, ..bytes
    .len = lendata
}