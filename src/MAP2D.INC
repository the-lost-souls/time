;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : BUMP2D.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 09.09.1996
;Action          : 2D-bumpmapping
;
;Notes : Simple, slow effect.
;----------------------------------------------------------------------------

.386

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : Bump2D
;Type         : Procedure
;Last update  : 09.09.1996
;Action       : 2D-bumpmapping, object type 128
;Optimized    : No. Don't bother.
;
;Notes : This procedure saves the bumpmapping-part in the sorting arrays. It
;also moves the lightsource.
;----------------------------------------------------------------------------

XAngle          DD 0
YAngle          DD 0
ZAngle          DD 0
XMove           EQU 20
YMove           EQU 10
ZMove           EQU 5

Map2D        PROC
                mov    esi, ObjectOffs
;------ Increasing angles...
                call   DWORD PTR [ esi + 56 ]   ;Additional routine

                mov    eax, [ esi + 04 ]        ;Load X
                mov    ebx, [ esi + 08 ]        ;Load Y
                mov    ecx, [ esi + 12 ]        ;Load Z

                mov    esi, ObjectOffs

                cmp    ecx, 0                   ;Test if map behind user...
                jle    Dumpmap                  ;/

                F3DTo2D

                add    ebx, 160
                add    eax, 100

                mov    [ esi + 20 ], ecx        ;Save Z-value to sort
                mov    [ esi + 28 ], ebx        ;Save 2D-X
                mov    [ esi + 32 ], eax        ;Save 2D-Y

                mov    eax, [ esi + 40 ]
                mov    ebx, [ esi + 44 ]

                F3DTo2D

                inc    ebx
                inc    eax
                mov    DWORD PTR [ esi + 48 ], ebx
                mov    DWORD PTR [ esi + 52 ], eax

                shr    ebx, 1
                shr    eax, 1


;Test maps location, evt. dumping it...

                mov    edx, [ esi + 28 ]
                mov    ebp, [ esi + 32 ]

                sub    edx, ebx
                sub    ebp, eax

                cmp    edx, 320
                jge    DumpMap
                cmp    ebp, 200
                jge    DumpMap

                lea    edx, [ edx + ebx*2 ]
                lea    ebp, [ ebp + eax*2 ]

                cmp    edx, 0
                jle    DumpMap
                cmp    ebp, 0
                jle    Dumpmap

;Map OK. Setting up sorting...
                lea    edi, [ esi + 16 ]        ;Offset to part-data

                and    ecx, 255
                mov    ebx, 255
                sub    ebx, ecx

                mov    ecx, ListEnd1
                mov    eax, [ebx*4 + ecx ]      ;Load offset to end
                mov    [ eax ], edi             ;Update link
                mov    [ ebx*4 + ecx ], edi     ;Update end-pointer

DumpMap:

                ret
Map2D         ENDP


;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : DBump2D
;Type         : Procedure
;Last update  : 09.09.1996
;Action       : Draws the 2D-bump. Called from RParts
;Optimized    : Yes
;
;Notes : Look BUMP2D.DOC for explanation.
;----------------------------------------------------------------------------

XSlope_129      DD 0
YSlope_129      DD 0
XStart          DD 0
YStart          DD 0
MapLeft         DD 0

RBLookUp        DB 0

                I = 1
              REPT 31
                DB I
                I = I + 1
              ENDM

                DB 32 DUP (31)

GLookUp         DB 0
                I = 1
              REPT 63
                DB I
                I = I + 1
              ENDM

                DB 64 DUP (63)


DMap2D       PROC
;call debug
                mov    eax, [ esi + 20 ]        ;Load scaled X-size
                sar    eax, 1
                mov    ebp, [ esi + 00 ]        ;Load 2D X-position
                sub    ebp, eax                 ;X-starting point
                sal    ebp, 1
                mov    XStart, ebp

                mov    eax, [ esi + 24 ]        ;Load scaled Y-size
                sar    eax, 1
                mov    edi, [ esi + 04 ]        ;Load 2D Y-position
                sub    edi, eax                 ;Y-starting point
                mov    YStart, edi

;---- Slopes...
                mov    eax, [ esi + 12 ]        ;Load X-size
                mov    edx, [ esi + 12 ]        ;/
                sar    edx, 31                  ;Sign extend
                sal    eax, 8

                mov    ebx, [ esi + 20 ]        ;Load scaled XSize
                idiv   ebx
                mov    XSlope_129, eax                 ;Save XSlope

                mov    eax, [ esi + 16 ]        ;Load Y-size
                mov    edx, [ esi + 16 ]        ;/
                sar    edx, 31                  ;Sign extend
                sal    eax, 8

                mov    ecx, [ esi + 24 ]        ;Load scaled YSize
                idiv   ecx
                mov    YSlope_129, eax          ;Save YSlope

                                                ;ebp = XStart
                                                ;edi = YStart
                                                ;ebx = XSlope
                                                ;ecx = YSlope
                                                ;edx = Y-Loopcounter

                xor    edi, edi
                mov    MapLeft, 0

;--- Clipping...
                mov    ebx, YStart
                cmp    ebx, 0
                jge    UpperOK_129
                neg    ebx
                mov    eax, YSlope_129
                mul    ebx
                add    edi, eax
                sub    [ esi + 24 ], ebx

                mov    YStart, 0

UpperOK_129:
                mov    ebx, YStart
                add    ebx, [ esi + 24 ]
                cmp    ebx, YRes
                jle    LowerOK_129
                sub    ebx, YRes
                sub    [ esi + 24 ], ebx
LowerOK_129:

                mov    ebx, XStart
                cmp    ebx, 0
                jge    LeftOK_129
                neg    ebx
                sar    ebx, 1
                mov    eax, XSlope_129
                mul    ebx

                add    MapLeft, eax
                sub    [ esi + 20 ], ebx
                mov    XStart, 0
LeftOK_129:

                mov    ebx, XStart
                shr    ebx, 1
                add    ebx, [ esi + 20 ]
                cmp    ebx, 320                 ;Hmmm... XRes = Xpixels*2 ?!?
                jle    RightOK_129
                sub    ebx, 320
                sub    [ esi + 20 ], ebx
RightOK_129:





                mov    edx, [ esi + 24 ]        ;Use edx as Y-loopcounter


YLoop_129:
                push   edx
                push   esi
                push   edi

                mov    ecx, [ esi + 20 ]        ;X-loopcounter
                shr    edi, 8
                mov    eax, [ esi + 12 ]
                mul    edi
                shl    eax, 1
                mov    edi, eax
                add    edi, [ esi + 08 ]


                mov    esi, YStart
                mov    esi, ResTable[ esi*4 ]
                add    esi, XStart
                add    esi, CanvasAddr

                mov    edx, MapLeft

                                                ;eax = loader
                                                ;ebx = Xslope
                                                ;ecx = loopcounter
                                                ;edx = xcounter
                                                ;esi = canvasptr
                                                ;edi = mappointer
                                                ;ebp = div

XLoop_129:
                push   ecx
                push   edx

                mov    ecx, edx
                shr    ecx, 8

                mov    edx, [ edi + ecx*2 ]
                mov    ecx, [ esi ]

                mov    eax, 0000000000011111b
                mov    ebx, 0000000000011111b
                and    eax, edx                 ;Load transparency-map
                and    ebx, ecx                 ;Load background

                xor    ebp, ebp
                mov    bl, RBLookUp[ eax + ebx ]
                or     ebp, ebx

                mov    eax, 0000011111100000b
                mov    ebx, 0000011111100000b
                and    eax, edx                 ;Load transparency-map
                and    ebx, ecx                 ;Load background
                shr    eax, 5
                shr    ebx, 5

                mov    bl,  GLookUp[ eax + ebx ]
                and    ebx, 0000000000111111b
                shl    ebx, 5
                or     ebp, ebx

                mov    eax, 01111100000000000b
                mov    ebx, 01111100000000000b
                and    eax, edx                 ;Load transparency-map
                and    ebx, ecx                 ;Load background
                shr    eax, 11
                shr    ebx, 11

                mov    bl, RBLookUp[ eax + ebx ]
                shl    ebx, 11
                or     ebp, ebx

                mov    [ esi ], bp

                pop    edx
                pop    ecx

                add    edx, XSlope_129                 ;Add XSlope
                add    esi, 2

                dec    ecx
                jnz    XLoop_129

                pop    edi
                pop    esi
                pop    edx

                add    edi, YSlope_129
                inc    YStart

                dec    edx
                jnz    YLoop_129

                ret
DMap2D       ENDP

MapMove       PROC
                                                        ;--- Moving global X
                mov    ebx, [ esi + 84 ]
                mov    eax, [ esi + 72 ]                ;XMove
                imul   TimeDiff
                add    ebx, eax                         ;Move it!
                mov    [ esi + 84 ], ebx                ;Save fixed point X
                sar    ebx, 8
                mov    [ esi + 60 ], ebx                ;Save useable X

                                                        ;--- Moving global Y
                mov    ebx, [ esi + 88 ]
                mov    eax, [ esi + 76 ]                ;YMove
                imul   TimeDiff
                add    ebx, eax                         ;Move it!
                mov    [ esi + 88 ], ebx                ;Save fixed point Y
                sar    ebx, 8
                mov    [ esi + 64 ], ebx                ;Save useable Y

                                                        ;--- Moving global Z
                mov    ebx, [ esi + 92 ]
                mov    eax, [ esi + 80 ]                ;ZMove
                imul   TimeDiff
                add    ebx, eax                         ;Move it!
                mov    [ esi + 92 ], ebx                ;Save fixed point Z
                sar    ebx, 8
                mov    [ esi + 68 ], ebx                ;Save useable Z

                                                        ;--- Moving local X
                mov    ebx, [ esi + 120 ]
                mov    eax, [ esi + 108 ]                ;XMove
                imul   TimeDiff
                add    ebx, eax                         ;Move it!
                mov    [ esi + 120 ], ebx                ;Save fixed point X
                sar    ebx, 8
                mov    [ esi + 96 ], ebx                ;Save useable X

                                                        ;--- Moving local Y
                mov    ebx, [ esi + 124 ]
                mov    eax, [ esi + 112 ]                ;YMove
                imul   TimeDiff
                add    ebx, eax                         ;Move it!
                mov    [ esi + 124 ], ebx                ;Save fixed point Y
                sar    ebx, 8
                mov    [ esi + 100 ], ebx                ;Save useable Y

                                                        ;--- Moving local Z
                mov    ebx, [ esi + 128 ]
                mov    eax, [ esi + 116 ]               ;ZMove
                imul   TimeDiff
                add    ebx, eax                         ;Move it!
                mov    [ esi + 128 ], ebx                ;Save fixed point Z
                sar    ebx, 8
                mov    [ esi + 104 ], ebx                ;Save useable Z


                                                        ;XAngle
                xor    edx, edx
                mov    ebx, [ esi + 156 ]               ;fixed point
                mov    eax, [ esi + 144 ]               ;Move
                imul   TimeDiff
                add    ebx, eax                         ;Move it
                cmp    ebx, 360*8*256                   ;Test overflow
                setl   dl                               ;/
                dec    edx                              ;/
                and    edx, 360*8*256                   ;/
                sub    ebx, edx                         ;/
                mov    [ esi + 156 ], ebx               ;Save new fixed point
                shr    ebx, 8                           ;Useable angle
                mov    [ esi + 132 ], ebx               ;/


                                                        ;YAngle
                xor    edx, edx
                mov    ebx, [ esi + 160 ]               ;fixed point
                mov    eax, [ esi + 148 ]               ;Move
                imul   TimeDiff
                add    ebx, eax                         ;Move it
                cmp    ebx, 360*8*256                   ;Test overflow
                setl   dl                               ;/
                dec    edx                              ;/
                and    edx, 360*8*256                   ;/
                sub    ebx, edx                         ;/
                mov    [ esi + 160 ], ebx               ;Save new fixed point
                shr    ebx, 8                           ;Useable angle
                mov    [ esi + 136 ], ebx               ;/


                                                        ;ZAngle
                xor    edx, edx
                mov    ebx, [ esi + 164 ]               ;fixed point
                mov    eax, [ esi + 152 ]               ;Move
                imul   TimeDiff
                add    ebx, eax                         ;Move it
                cmp    ebx, 360*8*256                   ;Test overflow
                setl   dl                               ;/
                dec    edx                              ;/
                and    edx, 360*8*256                   ;/
                sub    ebx, edx                         ;/
                mov    [ esi + 164 ], ebx               ;Save new fixed point
                shr    ebx, 8                           ;Useable angle
                mov    [ esi + 140 ], ebx               ;/


                lea    edi, [ esi + 132 ]
                lea    esi, [ esi + 096 ]
                call   Rotate12

                mov    esi, ObjectOffs
                add    eax, [ esi + 60 ]
                add    ebx, [ esi + 64 ]
                add    ecx, [ esi + 68 ]

                mov    [ esi + 04 ], eax
                mov    [ esi + 08 ], ebx
                mov    [ esi + 12 ], ecx

                ret
MapMove       ENDP

