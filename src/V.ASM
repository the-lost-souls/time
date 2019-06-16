;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : v.asm
;Author          : Cyberfish of The Lost Souls
;Last update     : Autumn 96
;Action          : "time"
;
;Include files : see below
;
;----------------------------------------------------------------------------

.386
.Model  FLAT
.Stack  100h

BigSeg        SEGMENT

include       general.inc
include       main.inc
include       var.inc
include       math1.inc
include       gourad64.inc
include       text64.inc
include       lights.inc
include       ilights.inc
include       ltest.inc
include       vtest.inc
include       v260.inc
include       bump2d.inc
include       map2d.inc
include       gif.inc
include       play.inc
include       vesa.inc
include       objects.inc
include       lists.inc
include       init.inc


EXTRN         Debug : PROC

Objects       DD ISortO                      ;Init sorting arryas
              DD ClearO                      ;Blank canvas
              DD Map1
              DD 0FFFFh
              DD 0FFFFh
              DD 0FFFFh
              DD 0FFFFh
              DD 0FFFFh
              DD 0FFFFh
              DD 0FFFFh
              DD 0FFFFh
              DD 0FFFFh
              DD 0FFFFh
              DD SortO                       ;Sort polys
              DD RPartsO                     ;Draw polys
              DD DispO                       ;Move canvas to screen
              DD TimerO
              DD TimingO
              DD 0000                        ;End of objectlist

DataFile      DB 'time.tls',0

InitStr       DB 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿', 0Dh, 0Ah
              DB '³             The Lost Souls              ³', 0Dh, 0Ah
              DB '³                 "time"                  ³', 0Dh, 0Ah
              DB '³                                         ³', 0Dh, 0Ah
              DB '³ Featuring...                            ³', 0Dh, 0Ah
              DB '³   DOS32              - Adam Seychell    ³', 0Dh, 0Ah
              DB '³   Noise Music System - Alexander Boczar ³', 0Dh, 0Ah
              DB 'ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ', 0Dh, 0Ah, 0Dh, 0Ah, '$'
LoadInit      DB '.Loading and initializing$'
Dot           DB '.$'
VESAMsg       DB 'Switching to VESA mode. Computer may hang here...', 0Dh, 0Ah, '$'

Main          PROC
                PrintString(InitStr)

;----------------------------- INITIALIZING... ------------------------------
                call   SetupDOS32
                jc     Failed

                mov    XRes, 320*2
                mov    YRes, 200
                call   SetupVGA
                jc     Failed

                call   InitVESA

                PrintString(LoadInit)

                mov    ObjectOffs, OFFSET Vector1
                call   InitVector260
                PrintString(Dot)                ;"Working"-dot
                mov    ObjectOffs, OFFSET Vector4
                call   InitVector260
                PrintString(Dot)                ;"Working"-dot
                mov    ObjectOffs, OFFSET Vector2
                call   InitVector258
                PrintString(Dot)                ;"Working"-dot
                mov    ObjectOffs, OFFSET Vector3
                call   InitVector258
                PrintString(Dot)                ;"Working"-dot

                mov    ObjectOffs, OFFSET Bump
                call   InitBump
                PrintString(Dot)                ;"Working"-dot
                call   Setup258                 ;Set light-tables
                PrintString(Dot)                ;"Working"-dot
                call   LoadMaps                 ;Load maps for vectorobjects
                PrintString(Dot)                ;"Working"-dot
                call   InitMap2d                ;Load map for flares
                PrintString(Dot)                ;"Working"-dot
                call   InitSort                 ;Allocate sorting arrays
                PrintString(Dot)                ;"Working"-dot

                call   InitPlayer
                PrintString(Dot)                ;"Working"-dot

                mov    edx, OFFSET DataFile
                mov    ecx, 493588              ;filesize
                mov    esi, 148690              ;fileoffset
                call   InitModule
                PrintString(Dot)                ;"Working"-dot

;------------------

                mov    edx, 131072              ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h                      ;Returns: edx=offset
                jc     Failed
                mov    BackO[ 04 ], edx         ;Save offset

                mov    edi, TempOffs
                mov    edx, OFFSET DataFile
                mov    ecx, 32212
                mov    esi, 7040

                call   GIF                      ;Decode picture!

                mov    edi, BackO[ 04 ]
                mov    esi, TempOffs
                mov    ecx, 65536
                call   Convert8_16
                PrintString(Dot)                ;"Working"-dot

                mov    edx, 131072              ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h                      ;Returns: edx=offset
                jc     Failed
                mov    BackO2[ 04 ], edx         ;Save offset

                mov    edi, TempOffs
                mov    edx, OFFSET DataFile
                mov    ecx, 18969
                mov    esi, 825737

                call   GIF                      ;Decode picture!
                jc     Failed

                mov    edi, BackO2[ 04 ]
                mov    esi, TempOffs
                mov    ecx, 65536
                call   Convert8_16
                PrintString(Dot)                ;"Working"-dot

                mov    edx, 131072              ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h                      ;Returns: edx=offset
                jc     Failed
                mov    BackO3[ 04 ], edx         ;Save offset

                mov    edi, TempOffs
                mov    edx, OFFSET DataFile
                mov    ecx, 19286
                mov    esi, 791411

                call   GIF                      ;Decode picture!
                jc     Failed

                mov    edi, BackO3[ 04 ]
                mov    esi, TempOffs
                mov    ecx, 65536
                call   Convert8_16
                PrintString(Dot)                ;"Working"-dot


                mov    edx, 131072              ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h                      ;Returns: edx=offset
                jc     Failed
                mov    CrossO[ 08 ], edx         ;Save offset

                mov    edi, TempOffs
                mov    edx, OFFSET DataFile
                mov    ecx, 34187
                mov    esi, 39252

                call   GIF                      ;Decode picture!
                jc     Failed

                mov    edi, CrossO[ 08 ]
                mov    esi, TempOffs
                mov    ecx, 65536
                call   Convert8_16
                PrintString(Dot)                ;"Working"-dot

;---- Initializing VESA...
                PrintString(EOL)
                PrintString(VESAMsg)
                cmp    LFB, 0
                je     OrdinaryVESA

                mov    ah, 4Fh
                mov    al, 02
                mov    ebx, VESA_Mode
                or     ebx, 4000h               ;Enable linear framebuffer
                int    10h
                jmp    DoneSetVESA
OrdinaryVESA:
                mov    ah, 4Fh
                mov    al, 02
                mov    ebx, VESA_Mode
                int    10h
DoneSetVESA:

                call   InitTimer

                call   nms_modulestart          ;Play

                MainL

                call   nms_modulestop          ;Stop
                call   nms_moduleexit          ;Uninit module
                call   nms_driverexit          ;Uninit driver
                call   nms_exit                ;Uninit nms

                VGAMode(03h)
                jmp    NoFails
Failed:
                mov    ax, 0900h
                int    21h
NoFails:

                mov    ax, 4C00h
                int    21h
Main          ENDP


TimerPtr      DD OFFSET TimerList
TimerList     DD 000000, A01
              DD 005000, A02
              DD 020000, A03
              DD 027000, A04
              DD 033000, A05
              DD 035000, A06
              DD 055000, A07
              DD 062500, A08                    ;Display maps
              DD 113000, A09
              DD 116000, A10
              DD 122500, A11                    ;Display TLS-logo
              DD 124500, A12
              DD 136000, A13
              DD 138000, A14
              DD 150000, A15
              DD 196000, A16                    ;Remove lights
              DD 207000, A17                    ;Display "time"
              DD 236000, A18
              DD 265000, A19                    ;Display env-mapping1
              DD 340000, A20                    ;env-mapping2
              DD 360000, A21
Part          DD 410000, A22                    ;Env-map2, move in
              DD 450000, A23                    ;stop
              DD 520000, A24                    ;move out
              DD 550000, A25                    ;Fade out and stop
              DD 570000, A26                    ;Bump
              DD 620000, A27                    ;fade out
              DD 670000, A28                    ;quit
              DD 840000, A29                    ;Quit


Timing        PROC
                mov    esi, TimerPtr
                mov    eax, Time

                cmp    eax, [ esi ]
                jb     NoAction
                jmp    [ esi + 04 ]

A01:
                mov    Map1[ 72 ], -35
                mov    Map1[ 80 ], 4
                mov    Map1[ 144 ], 20
                jmp    DoneAction
A02:
                mov    Map1[ 72 ], 0
                mov    Map1[ 152 ], 100
                mov    Map1[ 148 ], 100
                jmp    DoneAction
A03:
                mov    Map1[ 72 ], -35
                mov    Map1[ 152 ], 0
                jmp    DoneAction
A04:
                mov    Map1[ 72 ], 0
                mov    Map1[ 80 ], 0
                mov    Map1[ 152 ], 0
                jmp    DoneAction
A05:
                mov    Objects[ 12 ], OFFSET Map2

                mov    Map1[ 84 ], -600*256
                mov    Map1[ 88 ], 0
                mov    Map1[ 72 ], 64
                mov    Map1[ 80 ], -5
                mov    Map1[ 144 ], 30
                mov    Map1[ 148 ], 50
                mov    Map1[ 152 ], 10

                jmp    DoneAction
A06:
                mov    Map1[ 72 ], 0
                mov    Map1[ 80 ], 0

                mov    Map2[ 72 ], 0
                mov    Map2[ 80 ], 0
                mov    Map2[ 84 ], 0

                jmp    DoneAction
A07:
                mov    Map1[ 72 ], 64
                mov    Map1[ 144 ], 30
                mov    Map1[ 148 ], 50
                mov    Map1[ 152 ], 10
                jmp    DoneAction
A08:
                mov    Objects[ 08 ], OFFSET Map6
                mov    Objects[ 12 ], OFFSET Map7
                mov    Objects[ 16 ], OFFSET Map8
                mov    Objects[ 20 ], OFFSET Map9
                mov    Objects[ 24 ], OFFSET Map10
                mov    Objects[ 28 ], OFFSET Map11
                mov    Objects[ 32 ], OFFSET Map12
                mov    Objects[ 36 ], 0FFFFh

                jmp    DoneAction
A09:
                mov    Map6[ 72 ], 25
                mov    Map7[ 72 ], -25
                mov    Map8[ 76 ], 25
                mov    Map9[ 76 ], -25
                mov    Map10[ 80 ], 25
                mov    Map11[ 80 ], -25
                mov    Map12[ 80 ], -25
                jmp    DoneAction
A10:
                mov    Objects[ 08 ], 0FFFFh
                mov    Objects[ 12 ], 0FFFFh
                mov    Objects[ 16 ], 0FFFFh
                mov    Objects[ 20 ], 0FFFFh
                mov    Objects[ 24 ], 0FFFFh
                mov    Objects[ 28 ], 0FFFFh
                mov    Objects[ 32 ], 0FFFFh
                mov    Objects[ 36 ], 0FFFFh
                jmp    DoneAction
A11:
                mov    Objects[ 08 ], OFFSET InitLO
                mov    Objects[ 12 ], OFFSET Vector2
                mov    Objects[ 16 ], OFFSET Vector3
                jmp    DoneAction
A12:
                mov    Lights[ 04 ], OFFSET Light2
                jmp    DoneAction
A13:
                mov    Light2[ 516 + 84 ], -200
                jmp    DoneAction
A14:
                mov    Light1[ 516 + 84 ], 200
                jmp    DoneAction
A15:
                                                ;Here we go... :)
                mov    Vector2[ 64 ], 10
                mov    Lights[ 08 ], OFFSET Light3

                mov    Light1[ 516 + 84 ], 0
                mov    Light1[ 516 + 88 ], 0
                mov    Light1[ 516 + 92 ], 500*256
                mov    Light1[ 516 + 120 ], 0
                mov    Light1[ 516 + 124 ], 0
                mov    Light1[ 516 + 128 ], 150*256
                mov    Light1[ 516 + 144 ], 20
                mov    Light1[ 516 + 148 ], 10
                mov    Light1[ 516 + 152 ], 10
                mov    Light1[ 516 + 156 ], 0
                mov    Light1[ 516 + 160 ], 0
                mov    Light1[ 516 + 164 ], 0
                mov    Light1[ 516 + 72 ], 0

                mov    Light2[ 516 + 84 ], 0
                mov    Light2[ 516 + 88 ], 0
                mov    Light2[ 516 + 92 ], 500*256
                mov    Light2[ 516 + 120 ], 150*256
                mov    Light2[ 516 + 124 ], 0
                mov    Light2[ 516 + 128 ], -150*256
                mov    Light2[ 516 + 144 ], 30
                mov    Light2[ 516 + 148 ], 30
                mov    Light2[ 516 + 152 ], 10
                mov    Light2[ 516 + 156 ], 0
                mov    Light2[ 516 + 160 ], 0
                mov    Light2[ 516 + 164 ], 0
                mov    Light2[ 516 + 72 ], 0

                mov    Light3[ 516 + 84 ], 0
                mov    Light3[ 516 + 88 ], 0
                mov    Light3[ 516 + 92 ], 500*256
                mov    Light3[ 516 + 120 ], 150*256
                mov    Light3[ 516 + 124 ], -150*256
                mov    Light3[ 516 + 128 ], 150*256
                mov    Light3[ 516 + 144 ], 20
                mov    Light3[ 516 + 148 ], 40
                mov    Light3[ 516 + 152 ], 10
                mov    Light3[ 516 + 156 ], 0
                mov    Light3[ 516 + 160 ], 0
                mov    Light3[ 516 + 164 ], 0
                mov    Light3[ 516 + 72 ], 0

                mov    Objects[ 20 ], OFFSET Map13
                mov    Objects[ 24 ], OFFSET Map14
                mov    Objects[ 28 ], OFFSET Map15

                jmp    DoneAction
A16:
                mov    Light1[ 516 + 72 ], 20
                mov    Light1[ 516 + 76 ], 20
                mov    Light1[ 516 + 80 ], 0

                mov    Light2[ 516 + 72 ], -20
                mov    Light2[ 516 + 76 ], 20
                mov    Light2[ 516 + 80 ], 0

                mov    Light3[ 516 + 72 ], -20
                mov    Light3[ 516 + 76 ], -20
                mov    Light3[ 516 + 80 ], 0

                mov    Map13[ 72 ], 20
                mov    Map13[ 76 ], 20
                mov    Map13[ 80 ], 0

                mov    Map14[ 72 ], -20
                mov    Map14[ 76 ], 20
                mov    Map14[ 80 ], 0

                mov    Map15[ 72 ], -20
                mov    Map15[ 76 ], -20
                mov    Map15[ 80 ], 0


                jmp    DoneAction
A17:
                mov    Light1[ 516 + 72 ], 0
                mov    Light1[ 516 + 76 ], 0
                mov    Light1[ 516 + 80 ], 0

                mov    Light2[ 516 + 72 ], 0
                mov    Light2[ 516 + 76 ], 0
                mov    Light2[ 516 + 80 ], 0

                mov    Light3[ 516 + 72 ], 0
                mov    Light3[ 516 + 76 ], 0
                mov    Light3[ 516 + 80 ], 0

                mov    Objects[ 08 ], OFFSET BackO2
                mov    Objects[ 12 ], OFFSET FadeO
                mov    Objects[ 16 ], 0FFFFh
                mov    Objects[ 20 ], 0FFFFh
                mov    Objects[ 24 ], 0FFFFh
                mov    Objects[ 28 ], 0FFFFh
                mov    Objects[ 32 ], 0FFFFh
                mov    Objects[ 36 ], 0FFFFh
                mov    eax, BackO2[ 04 ]
                mov    FadeO[ 04 ], eax

                jmp    DoneAction
A18:
                mov    Objects[ 08 ], OFFSET BackO3
                mov    Objects[ 12 ], OFFSET FadeO
                mov    Objects[ 16 ], 0FFFFh
                mov    Objects[ 20 ], 0FFFFh
                mov    Objects[ 24 ], 0FFFFh
                mov    Objects[ 28 ], 0FFFFh
                mov    Objects[ 32 ], 0FFFFh
                mov    Objects[ 36 ], 0FFFFh
                mov    eax, BackO3[ 04 ]
                mov    FadeO[ 04 ], eax

                jmp    DoneAction
A19:
                mov    Objects[ 08 ], OFFSET BackO
                mov    Objects[ 12 ], OFFSET Vector1
                mov    Objects[ 16 ], OFFSET Map1
                mov    Objects[ 20 ], OFFSET Map2
                mov    Objects[ 24 ], OFFSET Map3
                mov    Objects[ 28 ], OFFSET Map4
                mov    Objects[ 32 ], OFFSET Map5
                mov    Objects[ 36 ], OFFSET Map6


                mov    Map1[ 84 ], 0
                mov    Map1[ 88 ], 0
                mov    Map1[ 92 ], 500*256
                mov    Map1[ 120 ], -150*256
                mov    Map1[ 124 ], 150*256
                mov    Map1[ 128 ], 150*256
                mov    Map1[ 144 ], 40
                mov    Map1[ 148 ], 10
                mov    Map1[ 152 ], 60

                mov    Map2[ 84 ], 0
                mov    Map2[ 88 ], 0
                mov    Map2[ 92 ], 500*256
                mov    Map2[ 120 ], 150*256
                mov    Map2[ 124 ], 50*256
                mov    Map2[ 128 ], -150*256
                mov    Map2[ 144 ], 60
                mov    Map2[ 148 ], 10
                mov    Map2[ 152 ], 30

                jmp    DoneAction
A20:
                mov    Vector1[ 100 ], -5
                mov    Map1[ 76 ], -15
                mov    Map2[ 76 ], -15
                mov    Map3[ 76 ], -15
                mov    Map4[ 76 ], -15
                mov    Map5[ 76 ], -15
                mov    Map6[ 76 ], -15

                jmp    DoneAction
A21:
                mov    Vector1[ 100 ], 0
                mov    Map1[ 76 ], 0
                mov    Map2[ 76 ], 0
                mov    Map3[ 76 ], 0
                mov    Map4[ 76 ], 0
                mov    Map5[ 76 ], 0
                mov    Map6[ 76 ], 0

                mov    Objects[ 12 ], OFFSET CrossO
                mov    eax, BackO[ 04 ]
                mov    CrossO[ 04 ], eax

                jmp    DoneAction
A22:
                                                ;04 = BackO
                mov    Objects[ 12 ], OFFSET Vector4
                mov    Objects[ 16 ], 0FFFFh
                mov    Objects[ 20 ], 0FFFFh
                mov    Objects[ 24 ], 0FFFFh
                mov    Objects[ 28 ], 0FFFFh
                mov    Objects[ 32 ], 0FFFFh
                mov    Objects[ 36 ], 0FFFFh

                mov    Vector4[ 88 ], 700*256
                mov    Vector4[ 100 ], -5
                jmp    DoneAction
A23:
                mov    Vector4[ 100 ], 0
                jmp    DoneAction
A24:
                mov    Vector4[ 96 ], -8
                jmp    DoneAction
A25:
                mov    Vector4[ 96 ], 0
                mov    Objects[ 12 ], 0FFFFh
                mov    Objects[ 12 ], OFFSET FadeO

                mov    eax, BackO[ 04 ]
                mov    FadeO[ 04 ], eax
                jmp    DoneAction
A26:
                mov    Objects[ 08 ], OFFSET Bump
                jmp    DoneAction
A27:
                mov    Objects[ 12 ], OFFSET FadeO
                mov    eax, ColorMap
                mov    FadeO[ 04 ], eax
                jmp    DoneAction
A28:
                call   nms_modulestop          ;Stop
                call   nms_moduleexit          ;Uninit module
                call   nms_driverexit          ;Uninit driver
                call   nms_exit                ;Uninit nms

                VGAMode(03h)
                mov    ax, 4C00h
                int    21h

                jmp    DoneAction
A29:

                jmp    DoneAction
A30:
                jmp    DoneAction


DoneAction:
                add   TimerPtr, 8

NoAction:
                ret
Timing        ENDP

BackGroundVESA PROC
                 mov   esi, ObjectOffs
                 mov   esi, [ esi + 04 ]
                 mov   edi, CanvasAddr
                 mov   ecx, 320*2*200/4
                 rep   movsd

                 ret
BackGroundVESA ENDP

FadeF          DD 0

FadePicture    PROC
                 mov   esi, ObjectOffs
                 mov   esi, [ esi + 04 ]

                 cmp   FadeF, 1
                 je    NoFade

                 mov   FadeF, 1

                 mov   ecx, 65536
FadeLoop:
                 xor   edx, edx
                 mov   bx, [ esi ]
                 mov   ax, 01111100000000000b
                 and   ax, bx
                 jz    NoFadeR

                 dec   ax
                 and   ax, 01111100000000000b
                 or    dx, ax
NoFadeR:

                 mov   ax, 00000011111100000b
                 and   ax, bx
                 jz    NoFadeG
                 dec   ax
                 and   ax, 00000011111100000b
                 or    dx, ax
NoFadeG:

                 mov   ax, 00000000000011111b
                 and   ax, bx
                 jz    NoFadeB
                 dec   ax
                 and   ax, 00000000000011111b
                 or    dx, ax
NoFadeB:
                 mov   [ esi ], dx
                 add   esi, 2

                 dec   ecx
                 jnz   FadeLoop

                 jmp   Dilldall

NoFade:
                 mov   FadeF, 0

DillDall:
                 ret
FadePicture    ENDP

CrossFade      PROC
                 mov   esi, ObjectOffs
                 mov   edi, [ esi + 04 ]
                 mov   esi, [ esi + 08 ]

                 mov   ecx, 64000
CrossLoop:
                 xor   ebp, ebp
                 mov   ax, 01111100000000000b
                 mov   bx, 01111100000000000b
                 and   ax, [ esi ]              ;Load Org
                 and   bx, [ edi ]              ;Load work

                 cmp   bx, ax
                 je    NoCrossR
                 jb    IncR
                 sub   bx, 00000100000000000b
                 jmp   NoCrossR

IncR:
                 add   bx, 00000100000000000b

NoCrossR:
                 and   bx, 01111100000000000b
                 or    bp, bx

                 mov   ax, 00000011111100000b
                 mov   bx, 00000011111100000b
                 and   ax, [ esi ]              ;Load Org
                 and   bx, [ edi ]              ;Load work

                 cmp   bx, ax
                 je    NoCrossG
                 jb    IncG
                 sub   bx, 00000000000100000b
                 jmp   NoCrossG

IncG:
                 add   bx, 00000000000100000b
NoCrossG:
                 and   bx, 00000011111100000b
                 or    bp, bx

                 mov   ax, 00000000000011111b
                 mov   bx, 00000000000011111b
                 and   ax, [ esi ]              ;Load Org
                 and   bx, [ edi ]              ;Load work

                 cmp   bx, ax
                 je    NoCrossB
                 jb    IncB
                 dec   bx
                 jmp   NoCrossB

IncB:
                 inc   bx

NoCrossB:
                 and   bx, 00000000000011111b
                 or    bp, bx

                 mov   [ edi ], bp
                 add   esi, 2
                 add   edi, 2

                 dec   ecx
                 jnz   CrossLoop

                 ret
CrossFade      ENDP


BigSeg        ENDS

END           Main