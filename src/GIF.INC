;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : GIF.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 04.10.96
;Action          : Loads a GIF-file and decodes it.
;
;Notes : Look GIF.DOC.
;----------------------------------------------------------------------------
.386



;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : GIF
;Type         : Procedure
;Last update  : 04.10.96
;Action       : Loads a GIF-file and decodes it.
;Optimized    : -
;
;Input variables : edx = OFFSET GIFFile
;                  edi = Where to store picture and palette
;                  esi = Where in file to start reading
;                  ecx = Number of bytes to read. If zero then read to end
;                        of file.
;
;Registers changed : All
;----------------------------------------------------------------------------

                                                ;User-variables
GIF_SWidth    DD 0                              ;Screen horisontal res
GIF_SHeigth   DD 0                              ;Screen vertical res
GIF_IWidth    DD 0                              ;Image width
GIF_IHeigth   DD 0                              ;Image heigth
GIF_PosX      DD 0                              ;Image's horisontal pos
GIF_PosY      DD 0                              ;Image's vertical pos
GIF_Colors    DD 0                              ;Number of colors in GIF
GIF_Palette   DB 768 DUP (0)

                                                ;Loop variables
GIF_BitsPrPix DD 0                              ;Bits pr pixel
GIF_Last      DD 0                              ;Offset to last charstream
GIF_Current   DD 0                              ;Offset to current charstream
GIF_Mask      DD 0                              ;Bitmask to get code
GIF_MaxCode   DD 0                              ;Current number of charstreams
GIF_InitBits  DD 0                              ;Initial number of bitsprcode
GIF_Bits      DD 0                              ;Current number of bitsprcode
GIF_BitsPos   DD 0                              ;Position of code in DWORD

                                                ;Pointers...
GIF_Data      DD 0                              ;Loading file here
GIF_OutPut    DD 0                              ;Writing data here
GIF_CodeBuf   DD 0                              ;Storing codestream here
GIF_STables   DD 0                              ;Table of string pointers
GIF_EndBuf    DD 0                              ;Pointer to end of charstreams
GIF_EndList   DD 0                              ;Pointer to end of STable



GIFInit       MACRO
LOCAL GIFInitL
                mov    ecx, GIF_InitBits
                mov    GIF_Bits, ecx
                mov    eax, 1                   ;Set mask
                shl    eax, cl                  ;/
                dec    eax                      ;/
                mov    GIF_Mask, eax            ;/

                dec    ecx                      ;Set maxcode
                mov    eax, 1                   ;/
                shl    eax, cl                  ;/
                inc    eax                      ;/Add special codes
                mov    GIF_MaxCode, eax         ;/

                mov    ebp, GIF_STables
                xor    ecx, ecx
                mov    edx, GIF_Colors
                lea    eax, [ ebp + 16384 ]

GIFInitL:
                mov    [ ebp + ecx*4 ], eax

                mov    DWORD PTR [ eax ], 1     ;Length of string
                mov    BYTE PTR [ eax + 4 ], cl ;Character

                add    eax, 5
                inc    ecx
                cmp    ecx, edx
                jb     GIFInitL

                lea    ebx, [ ebp + ecx*4 ]
                mov    [ ebx ], eax
                mov    DWORD PTR [ eax ], 0     ;Save length=0 (clear code)
                add    eax, 5
                mov    [ ebx + 4 ], eax
                mov    DWORD PTR [ eax ], -1    ;Save length=-1 (EOI-code)
                add    eax, 5
                mov    GIF_EndBuf, eax              ;Save current end of buffer
                add    ebx, 8
                mov    GIF_EndList, ebx             ;Save current end of stable

              ENDM


GIFTestBits   MACRO
LOCAL GIFTest1, GIFTest2
                cmp    GIF_BitsPos, 32
                jb     GIFTest1
                mov    GIF_BitsPos, 8
                add    ebx, 3

GIFTest1:
                mov    esi, 32
                sub    esi, GIF_Bits
                cmp    GIF_BitsPos, esi
                jbe    GIFTest2
                sub    GIF_BitsPos, 16
                add    ebx, 2
GIFTest2:


              ENDM







GIF           PROC
                mov    GIF_OutPut, edi
                mov    GIF_BitsPos, 0

                mov    ah, 3Dh
                mov    al, 0                    ;Read access
                int    21h
                jnc    GIF_OpenOK
                mov    edx, LErrTable[ 0*4 ]   ;Error opening file
                jmp    GIF_Error
GIF_OpenOK:
                mov    FHandle, ax              ;Save filehandle

                or     ecx, ecx
                jnz    GIF_Load

                xor    edx, edx                 ;Don't move!
                mov    ah, 42h                  ;Move filepointer
                mov    al, 2                    ;..from start of file
                mov    bx, FHandle              ;Load filehandle
                int    21h
                mov    ecx, eax                 ;FileSize
                sub    ecx, esi

GIF_Load:
                mov    edx, ecx                 ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h                      ;Returns: edx=offset

                jnc    GIF_MemOK1
                mov    edx, LErrTable[ 3*4 ]   ;Error allocating memory
                jmp    GIF_Error
GIF_MemOK1:
                mov    GIF_Data, edx

                mov    edx, esi                 ;edx = FileOffset
                mov    ah, 42h                  ;Move filepointer
                mov    al, 0                    ;..from start of file
                mov    bx, FHandle              ;Load filehandle
                int    21h

                                                ;ecx = Bytes to read
                mov    ah, 3Fh                  ;Read from file
                mov    bx, FHandle              ;Load filehandle
                mov    edx, GIF_Data            ;Use canvas as temp-buffer
                int    21h
                jnc    GIF_ReadOK
                mov    edx, LErrTable[ 2*4 ]   ;Error reading file
                jmp    GIF_Error
GIF_ReadOK:

                mov    ah, 3Eh                  ;Close filehandle
                mov    bx, FHandle              ;Load filehandle
                int    21h
                jnc    GIF_CloseOK
                mov    eax, 3
                jmp    GIF_Error
GIF_CloseOK:

                mov    edx, 65536               ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h                      ;return edx = offset
                jnc    GIF_MemOK2
                mov    edx, LErrTable[ 3*4 ]   ;Error allocating memory
                jmp    GIF_Error
GIF_MemOK2:
                mov    GIF_CodeBuf, edx
                                                ;Allocating memory
                mov    edx, 262144              ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h                      ;return edx = offset
                jnc    GIF_MemOK3
                mov    edx, LErrTable[ 3*4 ]   ;Error allocating memory
                jmp    GIF_Error
GIF_MemOK3:
                mov    GIF_STables, edx


;------------------------- READING GIF-HEADER ------------------------------

                mov    esi, GIF_Data

                movzx  eax, WORD PTR [ esi + 6 ];Horisontal screenres
                mov    GIF_SWidth, eax          ;/Save!

                movzx  eax, WORD PTR [ esi + 8 ];Vertical screenres
                mov    GIF_SHeigth, eax         ;/Save!

                movzx  ebx, BYTE PTR [ esi + 10 ];Bits pr. color
                and    bl, 00000111b            ;Mask out lower 3 bits

                lea    ecx, [ ebx + 2 ]         ;Calculate initial codesize
                mov    GIF_InitBits, ecx        ;/Save!

                lea    ecx, [ ebx + 1 ]         ;Calculate nr. of colors
                mov    eax, 1                   ;/
                shl    eax, cl                  ;/
                mov    GIF_Colors, eax          ;/Save!

                movzx  ebx, BYTE PTR [ esi + 10 ];Looking for global cmap
                add    esi, 13                  ;Dump GIF-header
                test   bl, 10000000b
                jz     NoGMap

                cld
                mov    edi, OFFSET GIF_Palette  ;Store palette

                mov    ecx, GIF_Colors          ;Size of palette
                lea    ecx, [ ecx + ecx*2 ]     ;Mul by 3
GIF_GMap:                                       ;Copy local palette
                mov    al, [ esi ]
                shr    al, 2
                mov    [ edi ], al
                inc    esi
                inc    edi
                dec    ecx
                jnz    GIF_GMap
NoGMap:

;------------------------ READING IMAGE DESCRIPTOR --------------------------

                movzx  eax, WORD PTR [ esi + 1 ];Image startx
                mov    GIF_PosX, eax            ;/Save!

                movzx  eax, WORD PTR [ esi + 3 ];Image starty
                mov    GIF_PosY, eax            ;/Save!

                movzx  eax, WORD PTR [ esi + 5 ];Image width
                mov    GIF_IWidth, eax          ;/Save!

                movzx  eax, WORD PTR [ esi + 7 ];Image heigth
                mov    GIF_IHeigth, eax         ;/Save!

                movzx  ebx, BYTE PTR [ esi + 9 ];Looking for local cmap
                add    esi, 10                  ;Dump image header
                test   bl, 10000000b
                jz     NoLMap

                cld
                mov    edi, OFFSET GIF_Palette

                mov    ecx, GIF_Colors          ;Size of palette
                lea    ecx, [ ecx + ecx*2 ]     ;Mul by 3
GIF_LMap:                                       ;Copy local palette
                mov    al, [ esi ]
                shr    al, 2
                mov    [ edi ], al
                inc    esi
                inc    edi
                dec    ecx
                jnz    GIF_LMap

NoLMap:

;------------------------- SETTING UP... ------------------------------------
;esi = start of image data


                GIFInit

                mov    ebp, GIF_STables         ;String-table-pointesr
                mov    edi, GIF_CodeBuf
                inc    esi                      ;Dump bits-pr-color-byte

                mov    ecx, GIF_Bits            ;Mask out bits to be used
                mov    eax, 1                   ;/
                shl    eax, cl                  ;/
                dec    eax                      ;/
                mov    GIF_Mask, eax

GIF_LCodes:
                movzx  ecx, BYTE PTR [ esi ]    ;Load BlockSize
                or     ecx, ecx                 ;End Of Information? (EOI)
                jz     GIF_DoneLC
                inc    esi                      ;Dump

                rep    movsb                    ;Copy block to codebuffer
                jmp    GIF_LCodes
GIF_DoneLC:

                mov    edi, GIF_Output          ;Output buffer
                mov    ebx, GIF_CodeBuf         ;Code-buffer

;------------------------ DECODING GIFDATA... -------------------------------
DLoop:
                mov    ecx, GIF_BitsPos         ;Code's position in bits
                mov    eax, DWORD PTR [ ebx ]   ;Load code
                shr    eax, cl                  ;Make current code lower bits

                and    eax, GIF_Mask            ;Mask out bits to be used

                mov    ecx, GIF_Bits
                add    GIF_BitsPos, ecx         ;Move bit-position

                cmp    eax, GIF_MaxCode
                jg     CodeNotFound




;----------- Charstream for current code exists :

                lea    esi, [ ebp + eax*4 ]     ;Load pointer to STable
                mov    esi, [ esi ]             ;Load pointer to charstream
                mov    ecx, [ esi ]             ;Load length of charstream

                cmp    ecx, -1                  ;Test of <EOI>
                je     GIFDone                  ;End of data
                cmp    ecx, 0                   ;Test of <CC>
                je     ResetTables              ;Clear String-tables

                mov    GIF_Current, esi         ;Saving current charstreamoffs
                add    esi, 4                   ;Dump length of charstream

                rep    movsb                    ;Output charstream

                                                ;Adding new string to STable
                push   edi                      ;Save output-offset

                mov    esi, GIF_EndList         ;Updating list-end-pointer
                mov    edi, GIF_EndBuf          ;/
                mov    [ esi ], edi             ;/
                add    GIF_EndList, 4           ;/

                mov    esi, GIF_Last            ;Load offs of last charstream
                mov    ecx, [ esi ]             ;Load length of charstream
                inc    ecx                      ;One byte longer
                mov    [ edi ], ecx             ;Save length of new charstream
                add    ecx, 4                   ;Update buffer-end-pointer
                add    GIF_EndBuf, ecx

                mov    ecx, [ esi ]             ;Load length of charstream

                add    edi, 4                   ;Dump length
                add    esi, 4                   ;/

                rep    movsb                    ;Copy charstream

                mov    edx, GIF_Current         ;Restore current offset
                mov    GIF_Last, edx            ;Make current -> last
                mov    cl, [ edx + 4 ]          ;Load first byte of current
                mov    [ edi ], cl              ;Add byte to charstream

                pop    edi                      ;Restore Destination pointer

                inc    GIF_MaxCode
                mov    eax, GIF_Mask
                cmp    GIF_MaxCode, eax
                jb     BitsOK1

                cmp    GIF_Bits, 12             ;Maximum bits?
                je     BitsOK1
                inc    GIF_Bits
                mov    ecx, GIF_Bits            ;Mask out bits to be used
                mov    esi, 1                   ;/
                shl    esi, cl                  ;/
                dec    esi                      ;/
                mov    GIF_Mask, esi

BitsOK1:
                GIFTestBits

                jmp    DLoop

CodeNotFound:
                mov    esi, GIF_Last            ;Load last charstream
                mov    ecx, [ esi ]

                add    esi, 4
                mov    al, [ esi ]
                rep    movsb
                stosb
                                                ;Adding new string to stable
                push   edi

                mov    esi, GIF_EndList         ;Updating STable
                mov    edi, GIF_EndBuf          ;/
                mov    [ esi ], edi             ;/
                add    GIF_EndList, 4

                mov    esi, GIF_Last            ;Load offs of last charstream
                mov    ecx, [ esi ]             ;Load length of charstream
                inc    ecx                      ;One byte longer...
                mov    [ edi ], ecx             ;Save length of new charstream
                add    ecx, 4
                add    GIF_EndBuf, ecx          ;Update buffer-end-pointer
                mov    ecx, [ esi ]             ;Load length of charstream
                mov    GIF_Last, edi            ;Set old charstream
                add    edi, 4
                add    esi, 4
                mov    al, [ esi ]
                rep    movsb
                stosb

                pop    edi                      ;Restore Destination pointer

                inc    GIF_MaxCode
                mov    ecx, GIF_Bits
                mov    eax, 1
                shl    eax, cl
                sub    eax, 1
                cmp    GIF_MaxCode, eax
                jb     BitsOK2

                cmp    GIF_Bits, 12
                je     BitsOK2
                inc    GIF_Bits
                mov    ecx, GIF_Bits            ;Mask out bits to be used
                mov    esi, 1                   ;/
                shl    esi, cl                  ;/
                dec    esi                      ;/
                mov    GIF_Mask, esi

BitsOK2:
                GIFTestBits
                jmp    DLoop

;------------------------- RESETTING TABLES --------------------------------

ResetTables:

                push   ebx

                GIFInit                         ;Reset string-tables

                pop    ebx

                GIFTestBits

                mov    ecx, GIF_BitsPos
                mov    eax, DWORD PTR [ ebx ]   ;Load code
                shr    eax, cl
                and    eax, GIF_Mask
                mov    esi, GIF_Bits
                add    GIF_BitsPos, esi

                GIFTestBits

                lea    esi, [ ebp + eax*4 ]     ;Load pointer to stable
                mov    esi, [ esi ]             ;Load pointer to charstream
                mov    ecx, [ esi ]             ;Load length of charstream
                cmp    ecx, -1
                je     GIFDone

                mov    GIF_Last, esi
                add    esi, 4
                rep    movsb
                jmp    DLoop


GIFDone:
                                                ;Deallocating buffers...

                mov    eax, 0EE40h              ;Deallocating STables
                int    31h

                mov    eax, 0EE40h              ;Deallocating CodeBuffer
                int    31h

                mov    eax, 0EE40h              ;Deallocating GIF_Data
                int    31h

                clc
                ret

GIF_Error:
                stc                             ;[ edx ] = errormessage
                ret

GIF           ENDP





