;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : GOURAD.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 25.09.1996
;Action          : Gourad-filling. With/without clipping
;
;Include files : l1.inc
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
;Last update  : 25.09.1996
;Action       : Draws a gourad-shaded triangle. Uses [ CanvasAddr ]
;Optimized    : Yes (not very good)
;
;Input variables : [ esi ] = offset to triangledata
;
;TriangleData :
;OFFSET                 Description
;-----------------------------------------
;  0                       Color1
;  4                       Color2
;  8                       Color3
; 12                        X1
; 16                        Y1
; 20                        X2
; 24                        Y2
; 28                        X3
; 32                        Y3
;-----------------------------------------
;Total 36 bytes
;
;Registers changed : All.
;
;Notes :
;----------------------------------------------------------------------------

TTri          PROC
                mov    eax, [ esi + 00 ]
                shr    eax, 16
                mov    eax, MapList[ eax*4 ]
                mov    MapOffset, eax

                mov    eax, [ esi + 16 ]        ;U:1 Load Y1
                mov    ebx, [ esi + 24 ]        ;v:1 Load Y2
                mov    ecx, [ esi + 32 ]        ;U:2 Load Y3

                cmp    eax, ebx                 ;v:2 Y1, Y2
                jle    T64_Swap1                    ;U:3
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
T64_Swap1:
                cmp    eax, ecx                 ;Y1, Y3
                jle    T64_Swap2
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

T64_Swap2:
                cmp    ebx, ecx                 ;Y2, Y3
                jle    T64_Swap3
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
T64_Swap3:
                mov    [ esi + 16 ], eax        ;Save Y1
                mov    [ esi + 24 ], ebx        ;Save Y2
                mov    [ esi + 32 ], ecx        ;Save Y3
;-------------------- Order is now (X1,Y1) (X2,Y2) (X3,Y3) with Y1 upper.

;call debug
                                                ;FIRST LINE (1 -> 3)
                mov    eax, [ esi + 28 ]        ;U:1 Load X3
                mov    ebx, [ esi + 12 ]        ;v:1 Load X1
                mov    ecx, [ esi + 32 ]        ;U:2 Y3
                mov    edx, [ esi + 16 ]        ;v:2 Y1

                sub    eax, ebx                 ;U:3 DeltaX1
                sub    ecx, edx                 ;v:3 DeltaY1 (the largest)

                jz     T64_Done                 ;U:4 No poly?
                push   ecx                      ;U:5 Save loopcounter

                sal    eax, 16                  ;U:6 Fixed point
                sal    ebx, 16                  ;U:7 Fixed point
                mov    edx, eax                 ;v:7 Sign extend
                sar    edx, 31                  ;U:8 /

                idiv   ecx                      ;DeltaX/DeltaY (eax:edx/ecx)

                mov    DWORD PTR XSlope1, eax              ;U:1 Save xslope

;---- Red component...
                mov    eax, 000FFh
                mov    edi, 000FFh
                and    eax, [ esi + 08 ]        ;U:2 U3
                and    edi, [ esi + 00 ]        ;v:2 U1
                sub    eax, edi                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sal    eax, 8                   ;U:4 Fixed point

                idiv   ecx

                mov    DWORD PTR USlope1, eax

;---- Green component
                mov    eax, 0FF00h
                mov    edi, 0FF00h
                and    eax, [ esi + 08 ]        ;U:2 V3
                and    edi, [ esi + 00 ]        ;v:2 V1
                sub    eax, edi                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /
                idiv   ecx

                mov    DWORD PTR VSlope1, eax

                                                ;    eax = cslope
                                                ;    edi = ccounter
                                                ;    ecx = loopcounter (DeltaY)
                                                ;    edx = Mempointer
                                                ;    ebx = xcounter

                mov    eax, [ esi + 00 ]
                mov    edi, 0FF00h
                and    edi, eax                 ;edi = GSlope
                and    eax, 0FFh                ;eax = RSlope
                sal    eax, 8

                mov    edx, OFFSET LRBuffer     ;v:3
T64_L1Loop:
                mov    [ edx ], ebx             ;U:1 Save xvalue
                mov    [ edx + 04 ], eax        ;U:2 Save U
                mov    [ edx + 08 ], edi        ;U:3 Save V
                add    edx, 24                  ;v:3

                add    ebx, 12345678h           ;v:1
Xslope1 = $ - 4
                add    eax, 12345678h           ;v:2 Increase xcounter
Uslope1 = $ - 4
                add    edi, 12345678h           ;U:4
Vslope1 = $ - 4


                dec    ecx                      ;v:4
                jnz    T64_L1Loop               ;U:5


T64_DumpL1:

                                                ;SECOND LINE (1 -> 2)
                mov    edi, OFFSET LRBuffer     ;U:1 Use other column

                mov    eax, [ esi + 20 ]        ;v:1 Load X2
                mov    ebx, [ esi + 12 ]        ;U:2 Load X1
                mov    ecx, [ esi + 24 ]        ;v:2 Y2
                mov    edx, [ esi + 16 ]        ;U:3 Y1

                sub    eax, ebx                 ;v:3 DeltaX2
                sub    ecx, edx                 ;U:4 DeltaY2
                jz     T64_DumpL2                   ;v:4 No line?

                sal    eax, 16                  ;U:1 Fixed point
                sal    ebx, 16                  ;/
                mov    edx, eax                 ;U:2 Sign extend
                sar    edx, 31                  ;U:3 /

                idiv   ecx                      ;DeltaX/DeltaY (eax:edx/ecx)

                mov    DWORD PTR XSlope2, eax                 ;U:2 Save xslope

;---- Red component...
                mov    eax, 0FFh
                mov    edx, 0FFh
                and    eax, [ esi + 04 ]        ;U:2 C3
                and    edx, [ esi + 00 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sal    eax, 8                   ;U:4 Fixed point

                idiv   ecx

                mov    DWORD PTR USlope2, eax

;---- Green component
                mov    eax, 0FF00h
                mov    edx, 0FF00h
                and    eax, [ esi + 04 ]        ;U:2 C3
                and    edx, [ esi + 00 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                idiv   ecx

                mov    DWORD PTR VSlope2, eax

                                                ;    edi = ccounter
                                                ;    ecx = loopcounter (DeltaY)
                                                ;    edx = Mempointer
                                                ;    ebx = xcounter

                mov    eax, [ esi + 00 ]
                mov    edx, 0FF00h
                and    edx, eax                 ;edx = GSlope
                and    eax, 0FFh                ;eax = RSlope
                shl    eax, 8

T64_L2Loop:
                mov    [ edi + 12  ], ebx       ;U:1 Save xvalue
                mov    [ edi + 16  ], eax   ;v:1 Save cvalue
                mov    [ edi + 20  ], edx   ;v:1 Save cvalue
                add    edi, 24                  ;U:3

                add    eax, 12345678h           ;U:2 Increase xcounter
USlope2 = $ - 4
                add    ebx, 12345678h
XSlope2 = $ - 4
                add    edx, 12345678h
VSlope2 = $ - 4

                dec    ecx                      ;v:3
                jnz    T64_L2Loop               ;U:4
                                                ;eax = cslope
                                                ;edx = ccounter
                                                ;ecx = loopcounter (DeltaY)
                                                ;ebx = xcounter
                                                ;ebp = xslope
                                                ;edi = mempointer
T64_DumpL2:

                                                ;THIRD LINE (2 -> 3)
                mov    eax, [ esi + 28 ]        ;Load X3
                mov    ebx, [ esi + 20 ]        ;Load X2
                mov    ecx, [ esi + 32 ]        ;Y3
                mov    edx, [ esi + 24 ]        ;Y2
                sub    eax, ebx                 ;DeltaX3
                sub    ecx, edx                 ;DeltaY3
                jz     T64_DumpL3               ;No line?

                sal    eax, 16                  ;Fixed point
                sal    ebx, 16
                mov    edx, eax                 ;Sign extend
                sar    edx, 31                  ;/

                idiv   ecx                      ;DeltaX/DeltaY (eax:edx/ecx)

                mov    DWORD PTR XSlope3, eax

;---- Red component...
                mov    eax, 0FFh
                mov    edx, 0FFh
                and    eax, [ esi + 08 ]        ;U:2 C3
                and    edx, [ esi + 04 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sal    eax, 8                   ;U:4 Fixed point

                idiv   ecx

                mov    DWORD PTR USlope3, eax

;---- Green component
                mov    eax, 0FF00h
                mov    edx, 0FF00h
                and    eax, [ esi + 08 ]        ;U:2 C3
                and    edx, [ esi + 04 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                idiv   ecx

                mov    DWORD PTR VSlope3, eax

                                                ;    eax = cslope
                                                ;    edi = ccounter
                                                ;    ecx = loopcounter (DeltaY)
                                                ;    edx = Mempointer
                                                ;    ebx = xcounter

                mov    eax, [ esi + 04 ]
                mov    edx, 0FF00h
                and    edx, eax                 ;edi = GSlope
                and    eax, 0FFh                ;eax = RSlope
                sal    eax, 8

T64_L3Loop:
                mov    [ edi + 12 ], ebx   ;U:1 Save xvalue
                mov    [ edi + 16 ], eax   ;v:1 Save cvalue
                mov    [ edi + 20 ], edx   ;U:2 Save cvalue

                add    eax, 12345678h           ;U:3 Increase xcounter
USlope3 = $ - 4
                add    edx, 12345678h           ;v:3
VSlope3 = $ - 4
                add    ebx, 12345678h           ;v:4
XSlope3 = $ - 4
                add    edi, 24                  ;U:3
                dec    ecx                      ;v:3
                jnz    T64_L3Loop               ;U:4

T64_DumpL3:

;----------------------------------------------------------------------------
                                                ;FILLING LINES...

;call debug

                mov    edi, [ esi + 16 ]        ;Load Y1
                mov    edi, ResTable[ edi*4 ]
                add    edi, CanvasAddr

                mov    ebx, OFFSET LRBuffer
                mov    ebp, OFFSET LRBuffer + 12
                pop    edx                      ;Restore loopcounter

T64_LLoop:
                mov    esi, [ ebx ]             ;U:1 Load Xleft
                mov    ecx, [ ebp ]             ;v:1 Load Xright
                sar    esi, 16                  ;U:3
                sar    ecx, 16                  ;U:4
                sub    ecx, esi                 ;U:5
                jz     T64_DoneLine             ;v:5
                jg     T64_LROK
                neg    ecx
                mov    esi, [ ebp ]
                sar    esi, 16
                xchg   ebx, ebp
T64_LROK:

                push   edx                      ;U:1
                push   ebx                      ;v:1
                push   edi                      ;U:2
                push   ebp                      ;v:2

;                call   debug

                mov    edx, [ ebx + 04 ]        ;U:3 ---- U
                mov    eax, [ ebp + 04 ]        ;v:3 Load CRight

                shl    esi, 1                   ;U:4
                sub    eax, edx                 ;v:4 DeltaU

                add    esi, edi                 ;U:5 esi = mempointer
                mov    edx, eax                 ;v:5
                sar    edx, 31                  ;U:6 Sign extend

                idiv   ecx

                mov    DWORD PTR USlope, eax              ;edi = Uslope

                mov    edx, [ ebx + 08 ]        ;---- V
                mov    eax, [ ebp + 08 ]
                sub    eax, edx

                mov    edx, eax                 ;U:5
                sar    edx, 31                  ;U:6 Sign extend

                idiv   ecx
                mov    DWORD PTR VSlope, eax

                mov    eax, [ ebx + 04 ]        ;Red start
                mov    ebp, [ ebx + 08 ]
                                                ;eax = cslope
                                                ;ecx = loopcounter
                                                ;ebx = ccounter
                                                ;esi = xleft

                mov    edi, MapOffset
                mov    ebx, ebp                 ;U:1
T64_HLoop:
                mov    bl, ah
                mov    dx, [ edi + ebx*2 ]
                mov    [ esi ], dx              ;U:4

                add    eax, USlope      ;U:1
USlopex = $ - 4
                add    ebp, VSlope      ;v:1
VSlopex = $ - 4
                add    esi, 2

                dec    ecx                      ;U:6
                mov    ebx, ebp                 ;U:1
                jnz    T64_HLoop                ;v:6

T64_DoneLine2:
                pop    ebp
                pop    edi
                pop    ebx
                pop    edx
T64_DoneLine:

                add    ebx, 24
                add    ebp, 24
                add    edi, XRes

                dec    edx
                jnz    T64_LLoop

T64_Done:


                ret
TTri          ENDP


;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : GTri
;Type         : Procedure
;Last update  : 25.09.1996
;Action       : Draws a gourad-shaded triangle. Uses [ CanvasAddr ]
;Optimized    : Yes (not very good)
;
;Input variables : [ esi ] = offset to triangledata
;
;TriangleData :
;OFFSET                 Description
;-----------------------------------------
;  0                       Color1
;  4                       Color2
;  8                       Color3
; 12                        X1
; 16                        Y1
; 20                        X2
; 24                        Y2
; 28                        X3
; 32                        Y3
;-----------------------------------------
;Total 36 bytes
;
;Registers changed : All.
;
;Notes :
;----------------------------------------------------------------------------

TTriC         PROC
                mov    eax, [ esi + 00 ]
                shr    eax, 16
                mov    eax, MapList[ eax*4 ]
                mov    MapOffset, eax

                mov    eax, [ esi + 16 ]        ;U:1 Load Y1
                mov    ebx, [ esi + 24 ]        ;v:1 Load Y2
                mov    ecx, [ esi + 32 ]        ;U:2 Load Y3

                cmp    eax, ebx                 ;v:2 Y1, Y2
                jle    T64C_Swap1                    ;U:3
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
T64C_Swap1:
                cmp    eax, ecx                 ;Y1, Y3
                jle    T64C_Swap2
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

T64C_Swap2:
                cmp    ebx, ecx                 ;Y2, Y3
                jle    T64C_Swap3
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
T64C_Swap3:
                mov    [ esi + 16 ], eax        ;Save Y1
                mov    [ esi + 24 ], ebx        ;Save Y2
                mov    [ esi + 32 ], ecx        ;Save Y3
;-------------------- Order is now (X1,Y1) (X2,Y2) (X3,Y3) with Y1 upper.

;call debug
                                                ;FIRST LINE (1 -> 3)
                mov    eax, [ esi + 28 ]        ;U:1 Load X3
                mov    ebx, [ esi + 12 ]        ;v:1 Load X1
                mov    ecx, [ esi + 32 ]        ;U:2 Y3
                mov    edx, [ esi + 16 ]        ;v:2 Y1

                sub    eax, ebx                 ;U:3 DeltaX1
                sub    ecx, edx                 ;v:3 DeltaY1 (the largest)

                jz     T64C_Done                ;U:4 No poly?

                sal    eax, 16                  ;U:6 Fixed point
                sal    ebx, 16                  ;U:7 Fixed point
                mov    edx, eax                 ;v:7 Sign extend
                sar    edx, 31                  ;U:8 /

                idiv   ecx                      ;DeltaX/DeltaY (eax:edx/ecx)

                mov    XSlope, eax              ;U:1 Save xslope

;---- U-coordinate...
                mov    eax, 000FFh
                mov    edi, 000FFh
                and    eax, [ esi + 08 ]        ;U:2 U3
                and    edi, [ esi + 00 ]        ;v:2 U1
                sub    eax, edi                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sal    eax, 8                   ;U:4 Fixed point

                idiv   ecx

                mov    USlope, eax

;---- V-coordinate...
                mov    eax, 0FF00h
                mov    edi, 0FF00h
                and    eax, [ esi + 08 ]        ;U:2 V3
                and    edi, [ esi + 00 ]        ;v:2 V1
                sub    eax, edi                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /
                idiv   ecx

                mov    VSlope, eax

;---------- Clipping...

                mov    edx, [ esi + 32 ]        ;Load Y3
                cmp    edx, 200
                jle    T64C_Y1LowerOK

                sub    edx, 200
                sub    ecx, edx

T64C_Y1LowerOK:



                                                ;    eax = UCounter
                                                ;    edx = VCounter
                                                ;    ebx = XCounter
                                                ;    ecx = loopcounter(DeltaY)
                                                ;    edi = Mempointer

                mov    edx, [ esi + 00 ]
                mov    eax, [ esi + 16 ]        ;Load Y1

                cmp    eax, 0
                jge    T64C_ClipUpper1

                neg    eax
                imul   XSlope
                add    ebx, eax

                mov    eax, [ esi + 16 ]
                neg    eax                      ;
                imul   USlope

                sar    eax, 8
                and    eax, 0FFh
                mov    edi, eax

                mov    eax, [ esi + 16 ]
                neg    eax
                imul   VSlope

                and    eax, 0FF00h
                or     eax, edi
                mov    edx, [ esi + 00 ]
                add    edx, eax

                add    ecx, [ esi + 16 ]



T64C_ClipUpper1:

                push   ecx

                mov    eax, 000FFh              ;eax = USlope
                and    eax, edx                 ;edi = VSlope
                and    edx, 0FF00h                 ;
                sal    eax, 8                   ;Fixed point



                mov    edi, OFFSET LRBuffer     ;v:3
T64C_L1Loop:
                mov    [ edi ], ebx             ;U:1 Save xvalue
                mov    [ edi + 04 ], eax        ;v:1 Save RValue
                mov    [ edi + 08 ], edx        ;Save GValue

                add    eax, USlope              ;U:2 Increase xcounter
                add    edx, VSlope
                add    ebx, XSlope
                add    edi, 24                  ;U:3
                dec    ecx                      ;v:3
                jnz    T64C_L1Loop                   ;U:4


T64C_DumpL1:

                                                ;SECOND LINE (1 -> 2)
                mov    edi, OFFSET LRBuffer     ;U:1 Use other column

                mov    eax, [ esi + 20 ]        ;v:1 Load X2
                mov    ebx, [ esi + 12 ]        ;U:2 Load X1
                mov    ecx, [ esi + 24 ]        ;v:2 Y2
                mov    edx, [ esi + 16 ]        ;U:3 Y1

                sub    eax, ebx                 ;v:3 DeltaX2
                sub    ecx, edx                 ;U:4 DeltaY2
                jz     T64C_DumpL2                   ;v:4 No line?

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

                sal    eax, 8                   ;U:4 Fixed point

                idiv   ecx

                mov    USlope, eax

;---- Green component
                mov    eax, 0FF00h
                mov    edx, 0FF00h
                and    eax, [ esi + 04 ]        ;U:2 C3
                and    edx, [ esi + 00 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                idiv   ecx

                mov    VSlope, eax

                cmp    DWORD PTR [ esi + 24 ], 0
                jle    T64C_DumpL2

                mov    edx, [ esi + 24 ]        ;Load Y3
                cmp    edx, 200
                jle    T64C_Y2LowerOK

                sub    edx, 200
                sub    ecx, edx

T64C_Y2LowerOK:

                mov    edx, [ esi + 00 ]        ;Load mapping coordinates
                mov    eax, [ esi + 16 ]        ;Load Y1
                cmp    eax, 0
                jge    T64C_ClipUpper2

                neg    eax
                imul   XSlope
                add    ebx, eax

                push   ebx

                mov    eax, [ esi + 16 ]
                neg    eax
                imul   USlope

                mov    bl, ah

                mov    eax, [ esi + 16 ]
                neg    eax
                imul   VSlope

                mov    bh, ah
                mov    edx, [ esi + 00 ]
                add    edx, ebx

                add    ecx, [ esi + 16 ]

                pop    ebx

T64C_ClipUpper2:


                                                ;    edi = ccounter
                                                ;    ecx = loopcounter (DeltaY)
                                                ;    edx = Mempointer
                                                ;    ebx = xcounter

                mov    eax, 0FFh
                and    eax, edx                 ;eax = USlope
                and    edx, 0FF00h              ;edx = VSlope
                sal    eax, 8

T64C_L2Loop:
                mov    [ edi + 12  ], ebx       ;U:1 Save xvalue
                mov    [ edi + 16  ], eax   ;v:1 Save cvalue
                mov    [ edi + 20  ], edx   ;v:1 Save cvalue

                add    eax, USlope              ;U:2 Increase xcounter
                add    edx, VSlope
                add    ebx, XSlope
                add    edi, 24                  ;U:3
                dec    ecx                      ;v:3
                jnz    T64C_L2Loop              ;U:4
                                                ;eax = cslope
                                                ;edx = ccounter
                                                ;ecx = loopcounter (DeltaY)
                                                ;ebx = xcounter
                                                ;ebp = xslope
                                                ;edi = mempointer
T64C_DumpL2:

                                                ;THIRD LINE (2 -> 3)
                mov    eax, [ esi + 28 ]        ;Load X3
                mov    ebx, [ esi + 20 ]        ;Load X2
                mov    ecx, [ esi + 32 ]        ;Y3
                mov    edx, [ esi + 24 ]        ;Y2
                sub    eax, ebx                 ;DeltaX3
                sub    ecx, edx                 ;DeltaY3
                jz     T64C_DumpL3              ;No line?

                sal    eax, 16                  ;Fixed point
                sal    ebx, 16
                mov    edx, eax                 ;Sign extend
                sar    edx, 31                  ;/

                idiv   ecx                      ;DeltaX/DeltaY (eax:edx/ecx)

                mov    XSlope, eax

;---- Red component...
                mov    eax, 0FFh
                mov    edx, 0FFh
                and    eax, [ esi + 08 ]        ;U:2 C3
                and    edx, [ esi + 04 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                sal    eax, 8                   ;U:4 Fixed point

                idiv   ecx

                mov    USlope, eax

;---- Green component
                mov    eax, 0FF00h
                mov    edx, 0FF00h
                and    eax, [ esi + 08 ]        ;U:2 C3
                and    edx, [ esi + 04 ]        ;v:2 C1
                sub    eax, edx                 ;U:3 DeltaC1

                mov    edx, eax                 ;U:5 Sign extend
                sar    edx, 31                  ;U:6 /

                idiv   ecx

                mov    VSlope, eax

;-------------

                cmp    DWORD PTR [ esi + 24 ], 200
                jge    T64C_DumpL3


                mov    edx, [ esi + 32 ]        ;Load Y3
                cmp    edx, 200
                jle    T64C_Y3LowerOK

                sub    edx, 200
                sub    ecx, edx

T64C_Y3LowerOK:

                mov    edx, [ esi + 04 ]        ;Load mapping coordinates
                mov    eax, [ esi + 24 ]        ;Load Y1
                cmp    eax, 0
                jge    T64C_ClipUpper3

                push   edi

                neg    eax
                imul   XSlope
                add    ebx, eax

                mov    eax, [ esi + 24 ]
                neg    eax
                imul   USlope

                mov    edi, eax
                sar    edi, 8

                mov    eax, [ esi + 24 ]
                neg    eax
                imul   VSlope

                and    edi, 0FFh
                and    eax, 0FF00h
                or     edi, eax
                mov    edx, [ esi + 04 ]
                add    edx, edi

                add    ecx, [ esi + 24 ]

                pop    edi

T64C_ClipUpper3:


                                                ;    edi = ccounter
                                                ;    ecx = loopcounter (DeltaY)
                                                ;    edx = Mempointer
                                                ;    ebx = xcounter

                mov    eax, 0FFh
                and    eax, edx                 ;eax = USlope
                and    edx, 0FF00h              ;edx = VSlope
                sal    eax, 8

                                                ;    eax = cslope
                                                ;    edi = ccounter
                                                ;    ecx = loopcounter (DeltaY)
                                                ;    edx = Mempointer
                                                ;    ebx = xcounter

T64C_L3Loop:
                mov    [ edi + 12 ], ebx   ;U:1 Save xvalue
                mov    [ edi + 16 ], eax   ;v:1 Save cvalue
                mov    [ edi + 20 ], edx   ;U:2 Save cvalue

                add    eax, USlope              ;U:3 Increase xcounter
                add    edx, VSlope              ;v:3
                add    ebx, XSlope              ;v:4
                add    edi, 24                  ;U:3
                dec    ecx                      ;v:3
                jnz    T64C_L3Loop              ;U:4

T64C_DumpL3:

;----------------------------------------------------------------------------
                                                ;FILLING LINES...

;call debug

                xor    eax, eax
                mov    edi, [ esi + 16 ]        ;Load Y1
                cmp    edi, 0
                setl   al
                dec    eax
                and    edi, eax
                mov    edi, ResTable[ edi*4 ]
                add    edi, CanvasAddr

                mov    ebx, OFFSET LRBuffer
                mov    ebp, OFFSET LRBuffer + 12
                pop    edx                      ;Restore loopcounter

T64C_LLoop:
                mov    esi, [ ebx ]             ;U:1 Load Xleft
                mov    ecx, [ ebp ]             ;v:1 Load Xright
                sar    esi, 16                  ;U:3
                sar    ecx, 16                  ;U:4
                sub    ecx, esi                 ;U:5
                jz     T64C_DoneLine                 ;v:5
                jg     T64C_LROK
                neg    ecx
                mov    esi, [ ebp ]
                sar    esi, 16
                xchg   ebx, ebp
T64C_LROK:

                push   edx                      ;U:1
                push   ebx                      ;v:1
                push   edi                      ;U:2
                push   ebp                      ;v:2

                mov    edx, [ ebx + 04 ]        ;U:3 ---- U
                mov    eax, [ ebp + 04 ]        ;v:3 Load CRight

                sub    eax, edx                 ;v:4 DeltaU


                mov    edx, eax                 ;v:5
                sar    edx, 31                  ;U:6 Sign extend

                idiv   ecx

                mov    USlope, eax              ;edi = Uslope

                mov    edx, [ ebx + 08 ]        ;---- V
                mov    eax, [ ebp + 08 ]
                sub    eax, edx

                mov    edx, eax                 ;U:5
                sar    edx, 31                  ;U:6 Sign extend

                idiv   ecx
                mov    VSlope, eax

;------- Clipping...

                cmp    esi, 320                 ;Line outside screen
                jge    T64C_DoneLine2

                lea    eax, [ esi + ecx ]
                cmp    eax, 0
                jle    T64C_DoneLine2

                cmp    eax, 320
                jle    T64C_RightOK
                sub    eax, 320
                sub    ecx, eax
T64C_RightOK:

                cmp    esi, 0
                jge    T64C_LeftOK

                neg    esi
                mov    eax, USlope
                imul   esi
                mov    ebp, eax

                mov    eax, VSlope
                imul   esi

                add    [ ebx + 04 ], ebp
                add    [ ebx + 08 ], eax

                sub    ecx, esi
                xor    esi, esi


T64C_LeftOK:

                shl    esi, 1
                add    esi, edi                 ;U:5 esi = mempointer

                mov    eax, [ ebx + 04 ]        ;Red start
                mov    ebp, [ ebx + 08 ]
                                                ;eax = cslope
                                                ;ecx = loopcounter
                                                ;ebx = ccounter
                                                ;esi = xleft

                mov    edi, MapOffset
                mov    ebx, ebp                 ;U:1
T64C_HLoop:
                mov    bl, ah
                mov    dx, [ edi + ebx*2 ]
                mov    [ esi ], dx              ;U:4

                add    eax, USlope              ;U:1
                add    ebp, VSlope              ;v:1
                add    esi, 2

                dec    ecx                      ;U:6
                mov    ebx, ebp                 ;U:1
                jnz    T64C_HLoop               ;v:6

T64C_DoneLine2:
                pop    ebp
                pop    edi
                pop    ebx
                pop    edx
T64C_DoneLine:

                add    ebx, 24
                add    ebp, 24
                add    edi, XRes

                dec    edx
                jnz    T64C_LLoop

T64C_Done:


                ret
TTriC         ENDP





