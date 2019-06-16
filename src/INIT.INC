;-------------------
;Loading texturemaps
;-------------------
LoadMaps      PROC
                mov    edx, 256*2*256              ;edx = Blocksize
                mov    ax, 0EE42h               ;DOS32: Allocate memory block
                int    31h
                jnc    Map_MemOK1
                mov    edx, LErrTable[ 3*4 ]
                jmp    Map_Failed
Map_MemOK1:
                mov    MapList[ 0*4 ], edx      ;Save offset

;----- Decoding map0
                mov    edx, OFFSET DataFile
                mov    esi, 645912
                mov    ecx, 30023
                mov    edi, TempOffs          ;Store picture here

                call   Gif
                jc     Map_Failed

                mov    esi, TempOffs
                mov    edi, MapList[ 0*4 ]
                mov    ecx, 65536
                call   Convert8_16

                mov    edx, 256*2*256              ;edx = Blocksize
                mov    ax, 0EE42h               ;DOS32: Allocate memory block
                int    31h
                jnc    Map_MemOK2
                mov    edx, LErrTable[ 3*4 ]
                jmp    Map_Failed
Map_MemOK2:
                mov    MapList[ 1*4 ], edx      ;Save offset

;----- Decoding map1
                mov    edx, OFFSET DataFile
                mov    esi, 675935
                mov    ecx, 51364
                mov    edi, TempOffs          ;Store picture here

                call   Gif
                jc     Map_Failed

                mov    esi, TempOffs
                mov    edi, MapList[ 1*4 ]
                mov    ecx, 65536
                call   Convert8_16

                mov    edx, 256*2*256              ;edx = Blocksize
                mov    ax, 0EE42h               ;DOS32: Allocate memory block
                int    31h
                jnc    Map_MemOK3
                mov    edx, LErrTable[ 3*4 ]
                jmp    Map_Failed
Map_MemOK3:
                mov    MapList[ 2*4 ], edx      ;Save offset

;----- Decoding map2
                mov    edx, OFFSET DataFile
                mov    esi, 727299
                mov    ecx, 52056
                mov    edi, TempOffs          ;Store picture here

                call   Gif
                jc     Map_Failed

                mov    esi, TempOffs
                mov    edi, MapList[ 2*4 ]
                mov    ecx, 65536
                call   Convert8_16

                clc
                ret                             ;Everything OK! :)
Map_Failed:
                mov    ax, 0900h
                int    21h

                mov    ax, 4C00h
                int    21h
LoadMaps      ENDP




;-------------------
;Loading vectorobjects
;-------------------
InitVector260 PROC
                mov    esi, ObjectOffs          ;Where to store header
                mov    edx, OFFSET DataFile     ;Point edx to filename
                mov    ecx, 12                  ;Dump FileHeader
                add    ecx, [ esi + 108 ]
                call   LoadV260
                jnc    NoErr260

                mov    ah, 09h                  ;Write errormessage
                int    21h

                mov    ax, 4C00h
                int    21h
NoErr260:

                clc
                ret
InitVector260 ENDP


InitVector258 PROC
                mov    esi, ObjectOffs          ;Where to store header
                mov    edx, OFFSET DataFile     ;Point edx to filename
                mov    ecx, 12                  ;Dump FileHeader
                add    ecx, [ esi + 108 ]
                call   LoadV258
                jnc    NoErr258

                mov    ah, 09h                  ;Write errormessage
                int    21h

                mov    ax, 4C00h
                int    21h
NoErr258:
                ret
InitVector258 ENDP


;-------------------
;Loading flaremaps
;-------------------
InitMap2D     PROC
                mov    edx, 64*64*2*10          ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h                      ;Returns: edx=offset
                jnc    Mem2D_OK1
                mov    edx, LErrTable[ 3*4 ]
                jmp    LoadErr
Mem2D_OK1:
                mov    Map1[ 36 ], edx          ;Use same buffer
                mov    Map2[ 36 ], edx
                mov    Map3[ 36 ], edx
                mov    Map4[ 36 ], edx
                mov    Map5[ 36 ], edx
                mov    Map6[ 36 ], edx
                mov    Map13[ 36 ], edx
                mov    Map14[ 36 ], edx
                mov    Map15[ 36 ], edx

                add    edx, 64*64*2
                mov    Map7[ 36 ], edx
                add    edx, 64*64*2
                mov    Map8[ 36 ], edx
                add    edx, 64*64*2
                mov    Map9[ 36 ], edx
                add    edx, 64*64*2
                mov    Map10[ 36 ], edx
                add    edx, 64*64*2
                mov    Map11[ 36 ], edx
                add    edx, 64*64*2
                mov    Map12[ 36 ], edx
                add    edx, 64*64*2
                mov    Map13[ 36 ], edx
                add    edx ,64*64*2
                mov    Map14[ 36 ], edx
                add    edx, 64*64*2
                mov    Map15[ 36 ], edx

                mov    edi, TempOffs
                mov    edx, OFFSET DataFile
                mov    esi, 642278
                mov    ecx, 3634

                call   GIF                      ;Decode picture!
                jc     LoadErr                  ;Failed!

                mov    edi, Map1[ 36 ]
                mov    esi, TempOffs
                mov    ecx, 64*64               ;Flaresize
                call   Convert8_16

;--------------------------- Maps for part I --------------------------------

                mov    edi, TempOffs
                mov    edx, OFFSET DataFile
                mov    esi, 642278
                mov    ecx, 3634

                call   GIF                      ;Decode picture!

                mov    edi, Map7[ 36 ]
                mov    esi, TempOffs
                mov    ecx, 64*64               ;Flaresize
                call   Convert8_16

                mov    esi, Map7[ 36 ]
                mov    edi, Map8[ 36 ]
                mov    ecx, 64*64 /2
                rep    movsd

                mov    esi, Map7[ 36 ]
                mov    edi, Map9[ 36 ]
                mov    ecx, 64*64 /2
                rep    movsd

                mov    esi, Map7[ 36 ]
                mov    edi, Map10[ 36 ]
                mov    ecx, 64*64 /2
                rep    movsd

                mov    esi, Map7[ 36 ]
                mov    edi, Map11[ 36 ]
                mov    ecx, 64*64 /2
                rep    movsd

                mov    esi, Map7[ 36 ]
                mov    edi, Map12[ 36 ]
                mov    ecx, 64*64 /2
                rep    movsd

                mov    esi, Map7[ 36 ]
                mov    edi, Map13[ 36 ]
                mov    ecx, 64*64 /2
                rep    movsd

                mov    esi, Map7[ 36 ]
                mov    edi, Map14[ 36 ]
                mov    ecx, 64*64 /2
                rep    movsd

                mov    esi, Map7[ 36 ]
                mov    edi, Map15[ 36 ]
                mov    ecx, 64*64 /2
                rep    movsd


                mov    esi, Map7[ 36 ]
                mov    ecx, 64*64
FixMap1:
                and    WORD PTR [ esi ], 01111100000000000b
                add    esi, 2

                dec    ecx
                jnz    FixMap1

                mov    esi, Map8[ 36 ]
                mov    ecx, 64*64
FixMap2:
                and    WORD PTR [ esi ], 00000011111100000b
                add    esi, 2

                dec    ecx
                jnz    FixMap2


                mov    esi, Map9[ 36 ]
                mov    ecx, 64*64
FixMap3:
                and    WORD PTR [ esi ], 00000000000011111b
                add    esi, 2

                dec    ecx
                jnz    FixMap3

                mov    esi, Map10[ 36 ]
                mov    ecx, 64*64
FixMap4:
                and    WORD PTR [ esi ], 01111100000011111b
                add    esi, 2

                dec    ecx
                jnz    FixMap4

                mov    esi, Map11[ 36 ]
                mov    ecx, 64*64
FixMap5:
                and    WORD PTR [ esi ], 00000011111111111b
                add    esi, 2

                dec    ecx
                jnz    FixMap5


                mov    esi, Map12[ 36 ]
                mov    ecx, 64*64
FixMap6:
                and    WORD PTR [ esi ], 01111111111100000b
                add    esi, 2

                dec    ecx
                jnz    FixMap6

                mov    esi, Map13[ 36 ]
                mov    ecx, 64*64
FixMap7:
                and    WORD PTR [ esi ], 01111100000000000b
                add    esi, 2

                dec    ecx
                jnz    FixMap7

                mov    esi, Map14[ 36 ]
                mov    ecx, 64*64
FixMap8:
                and    WORD PTR [ esi ], 00000011111100000b
                add    esi, 2

                dec    ecx
                jnz    FixMap8


                mov    esi, Map15[ 36 ]
                mov    ecx, 64*64
FixMap9:
                and    WORD PTR [ esi ], 00000000000011111b
                add    esi, 2

                dec    ecx
                jnz    FixMap9

                clc                             ;Everything OK!!
                ret

;------------------------------ LOADING FAILED! ----------------------------

LoadErr:
                mov    ax, 0900h                ;Display errormessage
                int    21h

                mov    ax, 4C00h
                int    21h

                ret
InitMap2d     ENDP




InitBump      PROC
                mov    esi, ObjectOffs

                mov    edx, 65536               ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h                      ;Returns: edx=offset
                jnc    MemOK1
                mov    edx, LErrTable[ 3*4 ]    ;Error allocating memory
                jmp    BumpLoadErr
MemOK1:
                mov    [ esi + 24 ], edx

                mov    edi, edx
                mov    edx, OFFSET DataFile
                mov    ecx, 45589
                mov    esi, 73439

                call   GIF                      ;Decode picture!
                jc     BumpLoadErr                  ;Failed!


;----- Colormap...
                mov    edx, 131072              ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h                      ;Returns: edx=offset
                mov    ColorMap, edx            ;Save offset

                mov    edi, TempOffs
                mov    edx, OFFSET DataFile
                mov    ecx, 29662
                mov    esi, 119028

                call   GIF                      ;Decode picture!
                jc     BumpLoadErr

                mov    edi, ColorMap
                mov    esi, TempOffs
                mov    ecx, 65536
                call   Convert8_16



                mov    edx, 65536               ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h                      ;Returns: edx=offset

;-----------------
                mov    BumpTables, edx
                mov    esi, BumpTables

                mov    ebp, 0
OuterL:
                mov    eax, ebp
                shl    eax, 8
                xor    edx, edx

                mov    ebx, 32
                div    ebx
                                                ;eax = slope
                                                ;edx = counter
                mov    ecx, 32
                mov    edx, 0
BTableL:
                add    edx, eax
                mov    ebx, edx
                shr    ebx, 8
                mov    [ esi ], bx

                add    esi, 2
                dec    ecx
                jnz    BTableL

                inc    ebp
                cmp    ebp, 32
                jb     OuterL


                mov    BumpTables2, esi
                mov    ebp, 0
OuterL2:
                mov    eax, ebp
                shl    eax, 8
                xor    edx, edx

                mov    ebx, 64
                div    ebx
                                                ;eax = slope
                                                ;edx = counter
                mov    ecx, 64
                mov    edx, 0
GTableL:
                add    edx, eax
                mov    ebx, edx
                shr    ebx, 8
                mov    [ esi ], bx

                add    esi, 2
                dec    ecx
                jnz    GTableL

                inc    ebp
                cmp    ebp, 64
                jb     OuterL2

                ret

BumpLoadErr:
                mov    ax, 0900h                ;Display errormessage
                int    21h

                mov    ax, 4C00h
                int    21h

                ret
InitBump      ENDP







Setup258      PROC
;----------- Fixing light-thingys...
                mov    esi, OFFSET Light1
                mov    eax, [ esi + 52 ]        ;R
                shl    eax, 3

                lea    edi, [ esi + 64 ]
                mov    ecx, 32
                mov    ebx, 256

FL1LoopR:
                mov    edx, ebx
                shr    edx, 8
                mov    [ edi ], edx
                add    ebx, eax
                add    edi, 4
                dec    ecx
                jnz    FL1LoopR

                mov    eax, [ esi + 56 ]        ;G
                shl    eax, 2

                lea    edi, [ esi + 64 + (32*4) ]
                mov    ecx, 64
                xor    ebx, ebx

FL1LoopG:
                mov    edx, ebx
                shr    edx, 8
                mov    [ edi ], edx
                add    ebx, eax
                add    edi, 4
                dec    ecx
                jnz    FL1LoopG


                mov    eax, [ esi + 60 ]        ;B
                shl    eax, 3

                lea    edi, [ esi + 64 + (96*4) ]
                mov    ecx, 32
                xor    ebx, ebx

FL1LoopB:
                mov    edx, ebx
                shr    edx, 8
                mov    [ edi ], edx
                add    ebx, eax
                add    edi, 4
                dec    ecx
                jnz    FL1LoopB


;----------- Fixing light-thingys...
                mov    esi, OFFSET Light2
                mov    eax, [ esi + 52 ]        ;R
                shl    eax, 3

                lea    edi, [ esi + 64 ]
                mov    ecx, 32
                mov    ebx, 256

FL1LoopR2:
                mov    edx, ebx
                shr    edx, 8
                mov    [ edi ], edx
                add    ebx, eax
                add    edi, 4
                dec    ecx
                jnz    FL1LoopR2

                mov    eax, [ esi + 56 ]        ;G
                shl    eax, 2

                lea    edi, [ esi + 64 + (32*4) ]
                mov    ecx, 64
                xor    ebx, ebx

FL1LoopG2:
                mov    edx, ebx
                shr    edx, 8
                mov    [ edi ], edx
                add    ebx, eax
                add    edi, 4
                dec    ecx
                jnz    FL1LoopG2


                mov    eax, [ esi + 60 ]        ;B
                shl    eax, 3

                lea    edi, [ esi + 64 + (96*4) ]
                mov    ecx, 32
                xor    ebx, ebx

FL1LoopB2:
                mov    edx, ebx
                shr    edx, 8
                mov    [ edi ], edx
                add    ebx, eax
                add    edi, 4
                dec    ecx
                jnz    FL1LoopB2

;----------- Fixing light-thingys...
                mov    esi, OFFSET Light3
                mov    eax, [ esi + 52 ]        ;R
                shl    eax, 3

                lea    edi, [ esi + 64 ]
                mov    ecx, 32
                mov    ebx, 256

FL1LoopR3:
                mov    edx, ebx
                shr    edx, 8
                mov    [ edi ], edx
                add    ebx, eax
                add    edi, 4
                dec    ecx
                jnz    FL1LoopR3

                mov    eax, [ esi + 56 ]        ;G
                shl    eax, 2

                lea    edi, [ esi + 64 + (32*4) ]
                mov    ecx, 64
                xor    ebx, ebx

FL1LoopG3:
                mov    edx, ebx
                shr    edx, 8
                mov    [ edi ], edx
                add    ebx, eax
                add    edi, 4
                dec    ecx
                jnz    FL1LoopG3


                mov    eax, [ esi + 60 ]        ;B
                shl    eax, 3

                lea    edi, [ esi + 64 + (96*4) ]
                mov    ecx, 32
                xor    ebx, ebx

FL1LoopB3:
                mov    edx, ebx
                shr    edx, 8
                mov    [ edi ], edx
                add    ebx, eax
                add    edi, 4
                dec    ecx
                jnz    FL1LoopB3


;------------------
                ret
Setup258      ENDP


Convert8_16   PROC

CLoop:
                movzx  eax, BYTE PTR [ esi ]
                lea    ebx, [ OFFSET GIF_Palette + eax + eax*2 ]

                movzx  eax, BYTE PTR [ ebx + 2 ]             ;Load R
                shr    eax, 1
                movzx  edx, BYTE PTR [ ebx + 1 ]         ;Load G
                shl    edx, 5
                or     eax, edx
                movzx  edx, BYTE PTR [ ebx + 0 ]         ;Load B
                shr    edx, 1
                shl    edx, 11
                or     eax, edx
                mov    [ edi ], ax

                inc    esi
                add    edi, 2

                dec    ecx
                jnz    CLoop


                ret
Convert8_16   ENDP



Msg1          DB 'Error: No VESA 2.0 support.', 0Ah, 0Dh, '$'
Msg2          DB 'Error:320x200x64k not supported.', 0Ah, 0Dh, '$'
Msg3          DB '.Using VESA mode (hex) $'
Msg4          DB '.Using linear framebuffer at $'
Msg5          DB 'Error: Error setting up linear framebuffer.', 0Ah, 0Dh, '$'
Msg6          DB '.Using bankswitching$'
Msg7          DB 'h', 0Ah, 0Dh, '$'

VESA_Mode     DD 0
LFBOffs       DD 0
LFB           DD 0
VESA_VideoMem DD 0

Space         DB '                    '


InitVESA      PROC
                mov    edi, TempOffs
                call   VESAInfo                 ;Get SVGA info
                jc     VESA_Failed              ;[ edx ] = error message

                mov    ebp, TempOffs

                movzx  eax, WORD PTR [ ebp + 04 ]     ;VESA version
                cmp    eax, 0200h                     ;VESA 2.0 support?
                je     VersionOK
                mov    edx, OFFSET Msg1
                jmp    VESA_Failed
VersionOK:

                movzx  eax, WORD PTR [ ebp + 18 ]
                mov    VESA_VideoMem, eax


;---- Getting VESA-modes...
                movzx  esi, WORD PTR [ ebp + 16 ]
                shl    esi, 4
                movzx  eax, WORD PTR [ ebp + 14 ]
                add    esi, eax
                sub    esi, Program_Base

                mov    edi, TempOffs
                add    edi, 4096
VESA_ModeL:
                movzx  eax, WORD PTR [ esi ]
                cmp    eax, 0FFFFh
                je     VESA_DoneModes

                mov    [ edi ], eax

                push   esi
                push   edi

                mov    edi, TempOffs
                call   VESAModeInfo             ;eax = VESA mode

                pop    edi
                pop    esi

                jc     VESA_Failed              ;Mode error?

                mov    ebx, TempOffs
                movzx  eax, WORD PTR [ ebx + 18 ]
                mov    [ edi + 04 ], eax
                movzx  eax, WORD PTR [ ebx + 20 ]
                mov    [ edi + 08 ], eax
                movzx  eax, BYTE PTR [ ebx + 25 ]
                mov    [ edi + 12 ], eax

                add    esi, 2                   ;Next mode
                add    edi, 32                  ;/
                jmp    VESA_ModeL
VESA_DoneModes:
                mov    [ edi ], 0FFFFFFFFh      ;Mark end of list

                mov    edi, TempOffs
                add    edi, 4096
VESA_TestM:
                mov    eax, [ edi ]
                cmp    eax, 0FFFFFFFFh
                je     VESA_DoneTest

                mov    eax, [ edi + 04 ]        ;Horisontal resolution
                cmp    eax, 320
                jne    VESA_WrongMode

                mov    eax, [ edi + 08 ]        ;Vertical resolution
                cmp    eax, 200
                jne    VESA_WrongMode

                mov    eax, [ edi + 12 ]        ;Bits pr. color
                cmp    eax, 16
                jne    VESA_WrongMode

                jmp    VESA_FoundMode           ;Found 320x200x64k
VESA_WrongMode:

                add    edi, 32
                jmp    VESA_TestM

VESA_DoneTest:
                mov    edx, OFFSET Msg2
                jmp    VESA_Failed

VESA_FoundMode:
                PrintString(Msg3)
                mov    eax, [ edi ]             ;eax = VESAMode
                call   PrintHex
                PrintString(EOL)

                mov    eax, [ edi ]
                mov    VESA_Mode, eax

                mov    edi, TempOffs
                call   VESAModeInfo

                mov    LFB, 0

                mov    edi, TempOffs
                mov    ax, [ edi ]
                test   ax, 10000000b            ;LFB available?
                jz     NoLFB

                mov    LFB, 1

                PrintString(Msg4)

                mov    eax, [ edi + 40 ]
                mov    LFBOffs, eax
                call   PrintHex
                PrintString(Msg7)

                mov    ax, 0800h
                mov    ebx, LFBOffs
                shr    ebx, 16
                mov    ecx, LFBOffs
                and    ecx, 0FFFFh
                mov    esi, VESA_VideoMem
                xor    edi, edi
                int    31h
                jnc    DOS32_LFB
                mov    edx, OFFSET Msg5
                jmp    VESA_Failed
DOS32_LFB:

                shl    ebx, 16
                mov    bx, cx
                sub    ebx, Program_Base
                mov    VGAAddress, ebx

                mov    ProcList[ 5*4 ], OFFSET DisplayLFB
                ret

NoLFB:
                ret

;---------------------- VESA INITIALIZATION FAILED... -----------------------

VESA_Failed:
                mov    ax, 0900h
                int    21h

                mov    ax, 4C00h
                int    21h

InitVESA      ENDP
