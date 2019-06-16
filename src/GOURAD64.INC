;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : GOURAD64.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 23.10.1996
;Action          : Gourad-filling, 64k. With/without clipping
;
;Notes :
; * GTri                        Procedure       - Gourad triangle
; * GTriC                       Procedure       - Gourad triangle, clipped
;
;----------------------------------------------------------------------------

.386

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : GTri
;Type         : Procedure
;Last update  : 23.10.1996
;Action       : Draws a gourad-shaded triangle. Uses [ CanvasAddr ]
;Optimized    : Yes (not very good)
;
;Input variables : [ esi ] = offset to triangledata
;
;TriangleData :
;OFFSET                 Description
;-----------------------------------------
;  0                        R1, G1, B1, 0
;  4                        R2, G2, B2, 0
;  8                        R3, G3, B3, 0
; 12                        X1
; 16                        Y1
; 20                        X2
; 24                        Y2
; 28                        X3
; 32                        Y3
;-----------------------------------------
;Total 36 bytes
;
;Notes :
;----------------------------------------------------------------------------

GTri          PROC
                mov    eax, [ esi + 16 ]        ;U:1 Load Y1
                mov    ebx, [ esi + 24 ]        ;v:1 Load Y2
                mov    ecx, [ esi + 32 ]        ;U:2 Load Y3

                cmp    eax, ebx                 ;v:2 Y1, Y2
                jle    G64_Swap1                    ;U:3
                                                ;Swapping (X1,Y1) and (X2,Y2)
                mov    edx, [ esi + 12 ]        ;Load X1
                mov    ebp, eax                 ;Save Y1
                mov    edi, [ esi + 20 ]        ;Load X2
                mov    eax, ebx                 ;Save Y2 -> Y1
                mov    [ esi + 12 ], edi        ;Save X2 -> X1
                mov    [ esi + 20 ], edx        ;Save X1 -> X2
                mov    ebx, ebp                 ;Save Y1 -> Y2

                mov    edx, [ esi + 00 ]        ;Load Col1
                mov    edi, [ esi + 04 ]        ;Col2
                mov    [ esi + 00 ], edi
                mov    [ esi + 04 ], edx
G64_Swap1:
                cmp    eax, ecx                 ;Y1, Y3
                jle    G64_Swap2
                                                ;Swapping (X1,Y1) and (X3,Y3)
                mov    edx, [ esi + 12 ]        ;Load X1
                mov    ebp, eax                 ;Save Y1
                mov    edi, [ esi + 28 ]        ;Load X3
                mov    eax, ecx                 ;Save Y3 -> Y1
                mov    [ esi + 12 ], edi        ;Save X3 -> X1
                mov    [ esi + 28 ], edx        ;Save X1 -> X3
                mov    ecx, ebp                 ;Save Y1 -> Y3

                mov    edx, [ esi + 00 ]        ;Load Col1
                mov    edi, [ esi + 08 ]        ;Col2
                mov    [ esi + 00 ], edi
                mov    [ esi + 08 ], edx

G64_Swap2:
                cmp    ebx, ecx                 ;Y2, Y3
                jle    G64_Swap3
                                                ;Swapping (X2,Y2) and (X3,Y3)
                mov    edx, [ esi + 20 ]        ;Load X2
                mov    ebp, ebx                 ;Save Y2
                mov    edi, [ esi + 28 ]        ;Load X3
                mov    ebx, ecx                 ;Save Y3 -> Y2
                mov    [ esi + 20 ], edi        ;Save X3 -> X2
                mov    [ esi + 28 ], edx        ;Save X2 -> X3
                mov    ecx, ebp                 ;Save Y2 -> Y3

                mov    edx, [ esi + 04 ]        ;Load Col1
                mov    edi, [ esi + 08 ]        ;Col2
                mov    [ esi + 04 ], edi
                mov    [ esi + 08 ], edx
G64_Swap3:
                mov    [ esi + 16 ], eax        ;Save Y1
                mov    [ esi + 24 ], ebx        ;Save Y2
                mov    [ esi + 32 ], ecx        ;Save Y3
;-------------------- Order is now (X1,Y1) (X2,Y2) (X3,Y3) with Y1 upper.

                                                ;FIRST LINE (1 -> 3)
                mov    eax, [ esi + 28 ]        ;U:1 Load X3
                mov    ebx, [ esi + 12 ]        ;v:1 Load X1
                mov    ecx, [ esi + 32 ]        ;U:2 Y3
                mov    edx, [ esi + 16 ]        ;v:2 Y1

                sub    eax, ebx                 ;U:3 DeltaX1
                sub    ecx, edx                 ;v:3 DeltaY1 (the largest)

                jz     G64_Done                 ;U:4 No poly?
                push   ecx                      ;U:5 Save loopcounter

                sal    eax, 16                  ;U:6 Fixed point
                sal    ebx, 16                  ;U:7 Fixed point
                mov    edx, eax                 ;v:7 Sign extend
                sar    edx, 31                  ;U:8 /

                idiv   ecx                      ;DeltaX/DeltaY (eax:edx/ecx)

                mov    DWORD PTR G64_XSlope1, eax              ;U:1 Save xslope

;---- Red component...
                mov    eax, 0FFh
                mov    edi, 0FFh
                and    eax, [ esi + 08 ]        ;U:2 C3
                and    edi, [ esi + 00 ]        ;v:2 C1
                sub    eax, edi                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sal    eax, 19                  ;U:4 Fixed point

                idiv   ecx

                mov    DWORD PTR G64_RSlope1, eax

;---- Green component
                mov    eax, 0FF00h
                mov    edi, 0FF00h
                and    eax, [ esi + 08 ]        ;U:2 C3
                and    edi, [ esi + 00 ]        ;v:2 C1
                sub    eax, edi                 ;U:3 DeltaC1

                sar    eax, 3

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /
                idiv   ecx

                mov    DWORD PTR G64_GSlope1, eax

;---- Blue component
                mov    eax, 0FF0000h
                mov    edi, 0FF0000h
                and    eax, [ esi + 08 ]        ;U:2 C3
                and    edi, [ esi + 00 ]        ;v:2 C1
                sub    eax, edi                 ;U:3 DeltaC1

                sar    eax, 8

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /
                idiv   ecx

                mov    DWORD PTR G64_BSlope1, eax
                                                ;    eax = cslope
                                                ;    edi = ccounter
                                                ;    ecx = loopcounter (DeltaY)
                                                ;    edx = Mempointer
                                                ;    ebx = xcounter

                mov    eax, [ esi + 00 ]
                mov    edi, 0FF00h
                and    edi, eax                 ;edi = GSlope
                shr    edi, 3
                mov    ebp, 0FF0000h
                and    ebp, eax                 ;ebp = BSlope
                shr    ebp, 8
                and    eax, 0FFh                ;eax = RSlope
                sal    eax, 19

                mov    edx, OFFSET LRBuffer     ;v:3
G64_L1Loop:
                mov    [ edx ], ebx             ;U:1 Save xvalue
                add    ebp, 12345678h           ;v:1
G64_BSlope1 = $ - 4
                mov    [ edx + 04 ], eax        ;U:2 Save RValue
                add    ebx, 12345678h           ;v:2
G64_XSlope1 = $ - 4
                mov    [ edx + 08 ], edi        ;U:3 Save GValue
                add    eax, 12345678h           ;v:3
G64_RSlope1 = $ - 4
                mov    [ edx + 12 ], ebp        ;U:4 Save BValue
                add    edi, 12345678h           ;v:4
G64_GSlope1 = $ - 4
                add    edx, 32                  ;U:5
                dec    ecx                      ;v:5
                jnz    G64_L1Loop               ;U:6


G64_DumpL1:

                                                ;SECOND LINE (1 -> 2)
                mov    edi, OFFSET LRBuffer     ;U:1 Use other column

                mov    eax, [ esi + 20 ]        ;v:1 Load X2
                mov    ebx, [ esi + 12 ]        ;U:2 Load X1
                mov    ecx, [ esi + 24 ]        ;v:2 Y2
                mov    edx, [ esi + 16 ]        ;U:3 Y1

                sub    eax, ebx                 ;v:3 DeltaX2
                sub    ecx, edx                 ;U:4 DeltaY2
                jz     G64_DumpL2               ;v:4 No line?

                sal    eax, 16                  ;U:1 Fixed point
                sal    ebx, 16                  ;/
                mov    edx, eax                 ;U:2 Sign extend
                sar    edx, 31                  ;U:3 /

                idiv   ecx                      ;DeltaX/DeltaY (eax:edx/ecx)

                mov    DWORD PTR G64_XSlope2, eax                 ;U:2 Save xslope

;---- Red component...
                mov    eax, 0FFh
                mov    edx, 0FFh
                and    eax, [ esi + 04 ]        ;U:2 C3
                and    edx, [ esi + 00 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sal    eax, 19                  ;U:4 Fixed point

                idiv   ecx

                mov    DWORD PTR G64_RSlope2, eax

;---- Green component
                mov    eax, 0FF00h
                mov    edx, 0FF00h
                and    eax, [ esi + 04 ]        ;U:2 C3
                and    edx, [ esi + 00 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sar    eax, 3                  ;U:4 Fixed point

                idiv   ecx

                mov    DWORD PTR G64_GSlope2, eax

;---- Blue component
                mov    eax, 0FF0000h
                mov    edx, 0FF0000h
                and    eax, [ esi + 04 ]        ;U:2 C3
                and    edx, [ esi + 00 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1


                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sar    eax, 8                   ;U:4 Fixed point

                idiv   ecx

                mov    DWORD PTR G64_BSlope2, eax
                                                ;    edi = ccounter
                                                ;    ecx = loopcounter (DeltaY)
                                                ;    edx = Mempointer
                                                ;    ebx = xcounter

                mov    eax, [ esi + 00 ]
                mov    edx, 0FF00h
                and    edx, eax                 ;edx = GSlope
                shr    edx, 3
                mov    ebp, 0FF0000h
                and    ebp, eax                 ;ebp = BSlope
                shr    ebp, 8
                and    eax, 0FFh                ;eax = RSlope
                shl    eax, 19

G64_L2Loop:
                mov    [ edi + 16  ], ebx       ;U:1 Save xvalue
                mov    [ edi + 20  ], eax       ;v:1 Save cvalue
                mov    [ edi + 24  ], edx       ;v:1 Save cvalue
                mov    [ edi + 28  ], ebp       ;v:1 Save cvalue

                add    ebx, 12345678h
G64_XSlope2 = $ - 4
                add    ebp, 12345678h
G64_BSlope2 = $ - 4
                add    eax, 12345678h           ;U:2 Increase xcounter
G64_RSlope2 = $ - 4
                add    edx, 12345678h
G64_GSlope2 = $ - 4

                add    edi, 32                  ;U:3
                dec    ecx                      ;v:3
                jnz    G64_L2Loop               ;U:4
                                                ;eax = cslope
                                                ;edx = ccounter
                                                ;ecx = loopcounter (DeltaY)
                                                ;ebx = xcounter
                                                ;ebp = xslope
                                                ;edi = mempointer
G64_DumpL2:

                                                ;THIRD LINE (2 -> 3)
                mov    eax, [ esi + 28 ]        ;Load X3
                mov    ebx, [ esi + 20 ]        ;Load X2
                mov    ecx, [ esi + 32 ]        ;Y3
                mov    edx, [ esi + 24 ]        ;Y2
                sub    eax, ebx                 ;DeltaX3
                sub    ecx, edx                 ;DeltaY3
                jz     G64_DumpL3               ;No line?

                sal    eax, 16                  ;Fixed point
                sal    ebx, 16
                mov    edx, eax                 ;Sign extend
                sar    edx, 31                  ;/

                idiv   ecx                      ;DeltaX/DeltaY (eax:edx/ecx)

                mov    DWORD PTR G64_XSlope3, eax

;---- Red component...
                mov    eax, 0FFh
                mov    edx, 0FFh
                and    eax, [ esi + 08 ]        ;U:2 C3
                and    edx, [ esi + 04 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sal    eax, 19                  ;U:4 Fixed point

                idiv   ecx

                mov    DWORD PTR G64_RSlope3, eax

;---- Green component
                mov    eax, 0FF00h
                mov    edx, 0FF00h
                and    eax, [ esi + 08 ]        ;U:2 C3
                and    edx, [ esi + 04 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sar    eax, 3                  ;U:4 Fixed point

                idiv   ecx

                mov    DWORD PTR G64_GSlope3, eax

;---- Blue component
                mov    eax, 0FF0000h
                mov    edx, 0FF0000h
                and    eax, [ esi + 08 ]        ;U:2 C3
                and    edx, [ esi + 04 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1


                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sar    eax, 8                   ;U:4 Fixed point

                idiv   ecx

                mov    DWORD PTR G64_BSlope3, eax

                                                ;    eax = cslope
                                                ;    edi = ccounter
                                                ;    ecx = loopcounter (DeltaY)
                                                ;    edx = Mempointer
                                                ;    ebx = xcounter

                mov    eax, [ esi + 04 ]
                mov    edx, 0FF00h
                and    edx, eax                 ;edi = GSlope
                sar    edx, 3
                mov    ebp, 0FF0000h
                and    ebp, eax                 ;ebp = BSlope
                sar    ebp, 8
                and    eax, 0FFh                ;eax = RSlope
                sal    eax, 19

G64_L3Loop:
                mov    [ edi + 16 ], ebx   ;U:1 Save xvalue
                mov    [ edi + 20 ], eax   ;v:1 Save cvalue
                mov    [ edi + 24 ], edx   ;U:2 Save cvalue
                mov    [ edi + 28 ], ebp   ;v:2 Save cvalue

                add    eax, 12345678h           ;U:3 Increase xcounter
G64_RSlope3 = $ - 4
                add    edx, 12345678h           ;v:3
G64_GSlope3 = $ - 4
                add    ebp, 12345678h           ;U:4
G64_BSlope3 = $ - 4
                add    ebx, 12345678h           ;v:4
G64_XSlope3 = $ - 4
                add    edi, 32                  ;U:3
                dec    ecx                      ;v:3
                jnz    G64_L3Loop               ;U:4

G64_DumpL3:

;----------------------------------------------------------------------------
                                                ;FILLING LINES...

                mov    edi, [ esi + 16 ]        ;Load Y1
                mov    edi, ResTable[ edi*4 ]
                add    edi, CanvasAddr

                mov    ebx, OFFSET LRBuffer
                mov    ebp, OFFSET LRBuffer + 16
                pop    edx                      ;Restore loopcounter

G64_LLoop:
                mov    esi, [ ebx ]             ;U:1 Load Xleft
                mov    ecx, [ ebp ]             ;v:1 Load Xright
                sar    esi, 16                  ;U:3
                sar    ecx, 16                  ;U:4
                sub    ecx, esi                 ;U:5
                jz     G64_DoneLine             ;v:5
                jg     G64_LROK
                neg    ecx
                mov    esi, [ ebp ]
                sar    esi, 16
                xchg   ebx, ebp
G64_LROK:

                push   edx                      ;U:1
                push   ebx                      ;v:1
                push   edi                      ;U:2
                push   ebp                      ;v:2

                mov    edx, [ ebx + 04 ]        ;U:3 ---- Red
                mov    eax, [ ebp + 04 ]        ;v:3 Load CRight

                shl    esi, 1                   ;U:4
                sub    eax, edx                 ;v:4 DeltaC

                add    esi, edi                 ;U:5 esi = mempointer
                mov    edx, eax                 ;v:5
                sar    edx, 31                  ;U:6 Sign extend
                mov    edi, 0111111111110000000000000b ;v:6

                idiv   ecx

                and    edi, eax

                mov    edx, [ ebx + 12 ]        ;---- Blue
                mov    eax, [ ebp + 12 ]
                sub    eax, edx

                mov    edx, eax                 ;U:5
                sar    edx, 31                  ;U:6 Sign extend

                idiv   ecx
                and    eax, 01111111111111b
                or     edi, eax
                mov    DWORD PTR CSlope1_, edi

                mov    edx, [ ebx + 08 ]        ;---- Green
                mov    eax, [ ebp + 08 ]
                sub    eax, edx

                mov    edx, eax                 ;U:5
                sar    edx, 31                  ;U:6 Sign extend

                idiv   cx
                mov    DWORD PTR CSlope2_, eax

                mov    eax, [ ebx + 04 ]        ;Red start
                mov    edx, [ ebx + 12 ]
                and    eax, 0111111111110000000000000b
                and    edx, 01111111111111b
                or     eax, edx

                mov    ebp, [ ebx + 08 ]

                                                ;eax = cslope
                                                ;ecx = loopcounter
                                                ;ebx = ccounter
                                                ;esi = xleft

                mov    edx, DWORD PTR CSlope1_

G64_HLoop:
                mov    ebx, eax                 ;U:1
                mov    edi, 00000011111100000b  ;v:1
                shr    ebx, 8                   ;U:2
                and    edi, ebp                 ;v:2
                and    ebx, 01111100000011111b  ;U:3
                or     ebx, edi                 ;U:4

                mov    [ esi ], bx              ;U:4
                add    eax, 12345678h           ;U:1
CSlope1_ = $ - 4
                add    ebp, 12345678h           ;v:1
CSlope2_ = $ - 4

                add    esi, 2                   ;v:3
                dec    ecx                      ;U:6
                jnz    G64_HLoop                    ;v:6

G64_DoneLine2:
                pop    ebp
                pop    edi
                pop    ebx
                pop    edx
G64_DoneLine:

                add    ebx, 32
                add    ebp, 32
                add    edi, XRes

                dec    edx
                jnz    G64_LLoop

G64_Done:


                ret
GTri          ENDP







;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : GTriC
;Type         : Procedure
;Last update  : 23.10.1996
;Action       : Gouradshaded triangle, 64k, clipped
;Optimized    : No
;
;Input variables : [ esi ] = offset to triangledata
;
;----------------------------------------------------------------------------

GTriC         PROC
                mov    eax, [ esi + 16 ]        ;U:1 Load Y1
                mov    ebx, [ esi + 24 ]        ;v:1 Load Y2
                mov    ecx, [ esi + 32 ]        ;U:2 Load Y3

                cmp    eax, ebx                 ;v:2 Y1, Y2
                jle    G64C_Swap1                    ;U:3
                                                ;Swapping (X1,Y1) and (X2,Y2)
                mov    edx, [ esi + 12 ]        ;Load X1
                mov    ebp, eax                 ;Save Y1
                mov    edi, [ esi + 20 ]        ;Load X2
                mov    eax, ebx                 ;Save Y2 -> Y1
                mov    [ esi + 12 ], edi        ;Save X2 -> X1
                mov    [ esi + 20 ], edx        ;Save X1 -> X2
                mov    ebx, ebp                 ;Save Y1 -> Y2

                mov    edx, [ esi + 00 ]        ;Load Col1
                mov    edi, [ esi + 04 ]        ;Col2
                mov    [ esi + 00 ], edi
                mov    [ esi + 04 ], edx
G64C_Swap1:
                cmp    eax, ecx                 ;Y1, Y3
                jle    G64C_Swap2
                                                ;Swapping (X1,Y1) and (X3,Y3)
                mov    edx, [ esi + 12 ]        ;Load X1
                mov    ebp, eax                 ;Save Y1
                mov    edi, [ esi + 28 ]        ;Load X3
                mov    eax, ecx                 ;Save Y3 -> Y1
                mov    [ esi + 12 ], edi        ;Save X3 -> X1
                mov    [ esi + 28 ], edx        ;Save X1 -> X3
                mov    ecx, ebp                 ;Save Y1 -> Y3

                mov    edx, [ esi + 00 ]        ;Load Col1
                mov    edi, [ esi + 08 ]        ;Col2
                mov    [ esi + 00 ], edi
                mov    [ esi + 08 ], edx

G64C_Swap2:
                cmp    ebx, ecx                 ;Y2, Y3
                jle    G64C_Swap3
                                                ;Swapping (X2,Y2) and (X3,Y3)
                mov    edx, [ esi + 20 ]        ;Load X2
                mov    ebp, ebx                 ;Save Y2
                mov    edi, [ esi + 28 ]        ;Load X3
                mov    ebx, ecx                 ;Save Y3 -> Y2
                mov    [ esi + 20 ], edi        ;Save X3 -> X2
                mov    [ esi + 28 ], edx        ;Save X2 -> X3
                mov    ecx, ebp                 ;Save Y2 -> Y3

                mov    edx, [ esi + 04 ]        ;Load Col1
                mov    edi, [ esi + 08 ]        ;Col2
                mov    [ esi + 04 ], edi
                mov    [ esi + 08 ], edx
G64C_Swap3:
                mov    [ esi + 16 ], eax        ;Save Y1
                mov    [ esi + 24 ], ebx        ;Save Y2
                mov    [ esi + 32 ], ecx        ;Save Y3
;-------------------- Order is now (X1,Y1) (X2,Y2) (X3,Y3) with Y1 upper.

                                                ;FIRST LINE (1 -> 3)
                mov    eax, [ esi + 28 ]        ;U:1 Load X3
                mov    ebx, [ esi + 12 ]        ;v:1 Load X1
                mov    ecx, [ esi + 32 ]        ;U:2 Y3
                mov    edx, [ esi + 16 ]        ;v:2 Y1

                sub    eax, ebx                 ;U:3 DeltaX1
                sub    ecx, edx                 ;v:3 DeltaY1 (the largest)

                jz     G64C_Done                ;U:4 No poly?

                sal    eax, 16                  ;U:6 Fixed point
                sal    ebx, 16                  ;U:7 Fixed point
                mov    edx, eax                 ;v:7 Sign extend
                sar    edx, 31                  ;U:8 /

                idiv   ecx                      ;DeltaX/DeltaY (eax:edx/ecx)

                mov    XSlope, eax              ;U:1 Save xslope

;---- Red component...
                mov    eax, 0FFh
                mov    edi, 0FFh
                and    eax, [ esi + 08 ]        ;U:2 R3
                and    edi, [ esi + 00 ]        ;v:2 R1
                sub    eax, edi                 ;U:3 DeltaR1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sal    eax, 19                  ;U:4 Fixed point

                idiv   ecx

                mov    RSlope, eax

;---- Green component
                mov    eax, 0FF00h
                mov    edi, 0FF00h
                and    eax, [ esi + 08 ]        ;U:2 G3
                and    edi, [ esi + 00 ]        ;v:2 G1
                sub    eax, edi                 ;U:3 DeltaG1

                sar    eax, 3

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /
                idiv   ecx

                mov    GSlope, eax

;---- Blue component
                mov    eax, 0FF0000h
                mov    edi, 0FF0000h
                and    eax, [ esi + 08 ]        ;U:2 B3
                and    edi, [ esi + 00 ]        ;v:2 B1
                sub    eax, edi                 ;U:3 DeltaB1

                sar    eax, 8

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /
                idiv   ecx

                mov    BSlope, eax


                mov    eax, [ esi + 00 ]
                mov    edi, 0FF00h
                and    edi, eax                 ;edi = GSlope
                shr    edi, 3
                mov    ebp, 0FF0000h
                and    ebp, eax                 ;ebp = BSlope
                shr    ebp, 8
                and    eax, 0FFh                ;eax = RSlope
                sal    eax, 19

;---- Clipping...
                cmp    DWORD PTR [ esi + 16 ], 0
                jge    G64C_Y1UpperOK

                push   ecx
                push   eax

                mov    ecx, DWORD PTR [ esi + 16 ]
                neg    ecx

                mov    eax, ecx
                imul   RSlope
                add    [ esp + 00 ], eax        ;Update RCounter

                mov    eax, ecx
                imul   GSlope
                add    edi, eax

                mov    eax, ecx
                imul   BSlope
                add    ebp, eax

                mov    eax, ecx
                imul   XSlope
                add    ebx, eax

                sub    [ esp + 04 ], ecx
                pop    eax
                pop    ecx

G64C_Y1UpperOK:
                cmp    DWORD PTR [ esi + 32 ], 200
                jle    G64C_Y1LowerOK

                mov    edx, [ esi + 32 ]
                sub    edx, 200
                sub    ecx, edx

G64C_Y1LowerOK:
                push   ecx                      ;Save loopcounter

;---- Setting up LRBuffer...

                mov    edx, OFFSET LRBuffer     ;v:3
G64C_L1Loop:
                mov    [ edx ], ebx             ;U:1 Save xvalue
                mov    [ edx + 04 ], eax        ;v:1 Save RValue
                mov    [ edx + 08 ], edi        ;Save GValue
                mov    [ edx + 12 ], ebp        ;Save BValue

                add    eax, RSlope              ;U:2 Increase xcounter
                add    edi, GSlope
                add    ebp, BSlope
                add    ebx, XSlope
                add    edx, 32                  ;U:3
                dec    ecx                      ;v:3
                jnz    G64C_L1Loop              ;U:4


G64C_DumpL1:

                                                ;SECOND LINE (1 -> 2)
                mov    edi, OFFSET LRBuffer     ;U:1 Use other column

                mov    eax, [ esi + 20 ]        ;v:1 Load X2
                mov    ebx, [ esi + 12 ]        ;U:2 Load X1
                mov    ecx, [ esi + 24 ]        ;v:2 Y2
                mov    edx, [ esi + 16 ]        ;U:3 Y1

                sub    eax, ebx                 ;v:3 DeltaX2
                sub    ecx, edx                 ;U:4 DeltaY2
                jz     G64C_DumpL2              ;v:4 No line?

                sal    eax, 16                  ;U:1 Fixed point
                sal    ebx, 16                  ;/
                mov    edx, eax                 ;U:2 Sign extend
                sar    edx, 31                  ;U:3 /

                idiv   ecx                      ;DeltaX/DeltaY (eax:edx/ecx)

                mov    XSlope, eax                 ;U:2 Save xslope

;---- Red component...
                mov    eax, 0FFh
                mov    edx, 0FFh
                and    eax, [ esi + 04 ]        ;U:2 C3
                and    edx, [ esi + 00 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sal    eax, 19                  ;U:4 Fixed point

                idiv   ecx

                mov    RSlope, eax

;---- Green component
                mov    eax, 0FF00h
                mov    edx, 0FF00h
                and    eax, [ esi + 04 ]        ;U:2 C3
                and    edx, [ esi + 00 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sar    eax, 3                   ;U:4 Fixed point

                idiv   ecx

                mov    GSlope, eax

;---- Blue component
                mov    eax, 0FF0000h
                mov    edx, 0FF0000h
                and    eax, [ esi + 04 ]        ;U:2 C3
                and    edx, [ esi + 00 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1


                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sar    eax, 8                   ;U:4 Fixed point

                idiv   ecx

                mov    BSlope, eax
                                                ;    edi = ccounter
                                                ;    ecx = loopcounter (DeltaY)
                                                ;    edx = Mempointer
                                                ;    ebx = xcounter

                mov    eax, [ esi + 00 ]
                mov    edx, 0FF00h
                and    edx, eax                 ;edx = GSlope
                shr    edx, 3
                mov    ebp, 0FF0000h
                and    ebp, eax                 ;ebp = BSlope
                shr    ebp, 8
                and    eax, 0FFh                ;eax = RSlope
                shl    eax, 19

;---- Clipping...
                cmp    DWORD PTR [ esi + 24 ], 0
                jle    G64C_DumpL2

                cmp    DWORD PTR [ esi + 16 ], 0
                jge    G64C_Y2UpperOK

                push   eax
                push   edx
                push   ecx

                mov    ecx, [ esi + 16 ]
                neg    ecx

                mov    eax, ecx
                imul   RSlope
                add    [ esp + 08 ], eax        ;Update RCounter

                mov    eax, ecx
                imul   GSlope
                add    [ esp + 04 ], eax        ;Update GCounter

                mov    eax, ecx
                imul   BSlope
                add    ebp, eax

                mov    eax, ecx
                imul   XSlope
                add    ebx, eax

                sub    [ esp + 00 ], ecx
                pop    ecx
                pop    edx
                pop    eax

G64C_Y2UpperOK:
                cmp    DWORD PTR [ esi + 24 ], 200
                jle    G64C_Y2LowerOK

                sub    ecx, [ esi + 24 ]
                add    ecx, 200
G64C_Y2LowerOK:


;---- Setting up LRBuffer

G64C_L2Loop:
                mov    [ edi + 16  ], ebx       ;U:1 Save xvalue
                mov    [ edi + 20  ], eax       ;v:1 Save cvalue
                mov    [ edi + 24  ], edx       ;v:1 Save cvalue
                mov    [ edi + 28  ], ebp       ;v:1 Save cvalue

                add    eax, RSlope              ;U:2 Increase xcounter
                add    edx, GSlope
                add    ebp, BSlope
                add    ebx, XSlope
                add    edi, 32                  ;U:3
                dec    ecx                      ;v:3
                jnz    G64C_L2Loop              ;U:4
                                                ;eax = cslope
                                                ;edx = ccounter
                                                ;ecx = loopcounter (DeltaY)
                                                ;ebx = xcounter
                                                ;ebp = xslope
                                                ;edi = mempointer
G64C_DumpL2:

                                                ;THIRD LINE (2 -> 3)
                mov    eax, [ esi + 28 ]        ;Load X3
                mov    ebx, [ esi + 20 ]        ;Load X2
                mov    ecx, [ esi + 32 ]        ;Y3
                mov    edx, [ esi + 24 ]        ;Y2
                sub    eax, ebx                 ;DeltaX3
                sub    ecx, edx                 ;DeltaY3
                jz     G64C_DumpL3              ;No line?

                sal    eax, 16                  ;Fixed point
                sal    ebx, 16
                mov    edx, eax                 ;Sign extend
                sar    edx, 31                  ;/

                idiv   ecx                      ;DeltaX/DeltaY (eax:edx/ecx)

                mov    XSlope, eax

;---- Red component...
                mov    eax, 0FFh
                mov    edx, 0FFh
                and    eax, [ esi + 08 ]        ;U:2 R3
                and    edx, [ esi + 04 ]        ;v:2 R1
                sub    eax, edx                 ;U:3 DeltaR3

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sal    eax, 19                  ;U:4 Fixed point

                idiv   ecx

                mov    RSlope, eax

;---- Green component
                mov    eax, 0FF00h
                mov    edx, 0FF00h
                and    eax, [ esi + 08 ]        ;U:2 C3
                and    edx, [ esi + 04 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sar    eax, 3                   ;U:4 Fixed point

                idiv   ecx

                mov    GSlope, eax

;---- Blue component
                mov    eax, 0FF0000h
                mov    edx, 0FF0000h
                and    eax, [ esi + 08 ]        ;U:2 C3
                and    edx, [ esi + 04 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1


                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sar    eax, 8                   ;U:4 Fixed point

                idiv   ecx

                mov    BSlope, eax

                                                ;    eax = cslope
                                                ;    edi = ccounter
                                                ;    ecx = loopcounter (DeltaY)
                                                ;    edx = Mempointer
                                                ;    ebx = xcounter

                mov    eax, [ esi + 04 ]
                mov    edx, 0FF00h
                and    edx, eax                 ;edx = GSlope
                sar    edx, 3
                mov    ebp, 0FF0000h
                and    ebp, eax                 ;ebp = BSlope
                sar    ebp, 8
                and    eax, 0FFh                ;eax = RSlope
                sal    eax, 19

;---- Clipping...
                cmp    DWORD PTR [ esi + 24 ], 200
                jge    G64C_DumpL3

                cmp    DWORD PTR [ esi + 24 ], 0
                jge    G64C_Y3UpperOK

                push   eax
                push   edx
                push   ecx

                mov    ecx, [ esi + 24 ]
                neg    ecx

                mov    eax, ecx
                imul   RSlope
                add    [ esp + 08 ], eax        ;Update RCounter

                mov    eax, ecx
                imul   GSlope
                add    [ esp + 04 ], eax        ;Update GCounter

                mov    eax, ecx
                imul   BSlope
                add    ebp, eax

                mov    eax, ecx
                imul   XSlope
                add    ebx, eax

                sub    [ esp + 00 ], ecx
                pop    ecx
                pop    edx
                pop    eax

G64C_Y3UpperOK:

                cmp    DWORD PTR [ esi + 32 ], 200
                jle    G64C_Y3LowerOK

                sub    ecx, [ esi + 32 ]
                add    ecx, 200
G64C_Y3LowerOK:


;---- Setting up LRBuffer

G64C_L3Loop:
                mov    [ edi + 16 ], ebx        ;U:1 Save xvalue
                mov    [ edi + 20 ], eax        ;v:1 Save cvalue
                mov    [ edi + 24 ], edx        ;U:2 Save cvalue
                mov    [ edi + 28 ], ebp        ;v:2 Save cvalue

                add    eax, RSlope              ;U:3 Increase xcounter
                add    edx, GSlope              ;v:3
                add    ebp, BSlope              ;U:4
                add    ebx, XSlope              ;v:4
                add    edi, 32                  ;U:3
                dec    ecx                      ;v:3
                jnz    G64C_L3Loop              ;U:4

G64C_DumpL3:




;---------------------------- FILLING LINES ------------------------------------------------

                xor    edx, edx
                mov    edi, [ esi + 16 ]        ;Load Y1
                cmp    edi, 0
                setl   dl
                dec    edx
                and    edi, edx

                mov    edi, ResTable[ edi*4 ]
                add    edi, CanvasAddr

                mov    ebx, OFFSET LRBuffer
                mov    ebp, OFFSET LRBuffer + 16
                pop    edx                      ;Restore loopcounter

G64C_LLoop:
                mov    esi, [ ebx ]             ;U:1 Load Xleft
                mov    ecx, [ ebp ]             ;v:1 Load Xright
                sar    esi, 16                  ;U:3
                sar    ecx, 16                  ;U:4
                sub    ecx, esi                 ;U:5
                jz     G64C_DoneLine            ;v:5
                jg     G64C_LROK
                neg    ecx
                mov    esi, [ ebp ]
                sar    esi, 16
                xchg   ebx, ebp
G64C_LROK:

                push   edx                      ;U:1
                push   ebx                      ;v:1
                push   edi                      ;U:2
                push   ebp                      ;v:2

                mov    edx, [ ebx + 04 ]        ;U:3 ---- Red
                mov    eax, [ ebp + 04 ]        ;v:3 Load CRight

                sub    eax, edx                 ;v:4 DeltaC

                mov    edx, eax                 ;v:5
                sar    edx, 31                  ;U:6 Sign extend
                mov    edi, 0111111111110000000000000b ;v:6

                idiv   ecx

                and    edi, eax

                mov    edx, [ ebx + 12 ]        ;---- Blue
                mov    eax, [ ebp + 12 ]
                sub    eax, edx

                mov    edx, eax                 ;U:5
                sar    edx, 31                  ;U:6 Sign extend

                idiv   cx
                and    eax, 01111111111111b
                or     edi, eax
                mov    CSlope1, edi

                mov    edx, [ ebx + 08 ]        ;---- Green
                mov    eax, [ ebp + 08 ]
                sub    eax, edx

                mov    edx, eax                 ;U:5
                sar    edx, 31                  ;U:6 Sign extend

                idiv   cx
                mov    CSlope2, eax

                mov    eax, [ ebx + 04 ]        ;Red start
                mov    edx, [ ebx + 12 ]
                and    eax, 0111111111110000000000000b
                and    edx, 01111111111111b
                or     eax, edx

;---- Clipping...
                lea    edx, [ esi + ecx ]
                cmp    edx, 0
                jle    G64C_DoneLine2

                cmp    edx, 320
                jle    G64C_RightOK
                sub    edx, 320
                sub    ecx, edx
G64C_RightOK:


                cmp    esi, 320
                jge    G64C_DoneLine2

                cmp    esi, 0
                jge    G64C_LeftOK

                push   eax
                push   ebp

                mov    ebp, esi
                neg    ebp

                mov    eax, ebp
                imul   CSlope1
                add    [ esp + 04 ], eax

                mov    eax, ebp
                imul   CSlope2
                add    [ ebx + 08 ], eax

                sub    ecx, ebp
                xor    esi, esi

                pop    ebp
                pop    eax
G64C_LeftOK:

                shl    esi, 1
                add    esi, [ esp + 04 ]        ;= pushed edi (esi=mempointer)

                mov    ebp, [ ebx + 08 ]

                                                ;eax = cslope
                                                ;ecx = loopcounter
                                                ;ebx = ccounter
                                                ;esi = xleft

                mov    edx, CSlope1

G64C_HLoop:
                mov    ebx, eax                 ;U:1
                mov    edi, 00000011111100000b  ;v:1
                shr    ebx, 8                   ;U:2
                and    edi, ebp                 ;v:2
                and    ebx, 01111100000011111b  ;U:3

                or     ebx, edi                 ;U:4

                mov    [ esi ], bx              ;U:4

                add    eax, edx                 ;U:1
                add    ebp, CSlope2             ;v:1
                add    esi, 2                   ;v:3

                dec    ecx                      ;U:6
                jnz    G64C_HLoop               ;v:6

G64C_DoneLine2:
                pop    ebp
                pop    edi
                pop    ebx
                pop    edx
G64C_DoneLine:

                add    ebx, 32
                add    ebp, 32
                add    edi, XRes

                dec    edx
                jnz    G64C_LLoop

G64C_Done:


                ret
GTriC         ENDP