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
AddX            DD 5
AddY            DD 3

Bump2D        PROC
                mov    esi, ObjectOffs

                mov    eax, AddX
                mov    ebx, AddY
                add    DWORD PTR [ esi + 4 ], eax         ;Add x
                add    DWORD PTR [ esi + 8 ], ebx         ;Add y

                cmp    DWORD PTR [ esi + 4 ], 320
                jl     BumpXL
                neg    AddX
BumpXL:
                cmp    DWORD PTR [ esi + 8 ], 200
                jl     BumpYL
                neg    AddY
BumpYL:
                cmp    DWORD PTR [ esi + 4 ], 0
                jg     BumpXG
                neg    AddX
BumpXG:
                cmp    DWORD PTR [ esi + 8 ], 0
                jg     BumpYG
                neg    AddY
BumpYG:

                                                ;Storing part
                mov    eax, [ esi + 16 ]        ;Load...
                mov    edi, OFFSET BumpPart

                and    eax, 255
                mov    ebx, 255
                sub    ebx, eax

                mov    ecx, ListEnd1
                mov    eax, [ebx*4 + ecx ]      ;Load offset to end
                mov    [ eax ], edi             ;Update link
                mov    [ ebx*4 + ecx ], edi     ;Update end-pointer

                ret
Bump2D        ENDP


;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : DBump2D
;Type         : Procedure
;Last update  : 09.09.1996
;Action       : Draws the 2D-bump. Called from RParts
;Optimized    : Yes
;
;Notes : Look BUMP2D.DOC for explanation.
;----------------------------------------------------------------------------

NX              DD 0
NY              DD 0
YCounter        DD 0
MapC            DD 0

DBump2D       PROC
                mov    eax, ColorMap
                mov    MapC, eax

                mov    ebp, [ esi ]             ;Load mappointer
                mov    edi, CanvasAddr
                add    edi, 321*2
                add    ebp, 321

                mov    eax, 1
                sub    eax, [ esi - 16 ]        ;
                mov    NY, eax

                mov    YCounter, 198

YLoop:
                mov    ecx, [ esi - 20 ]        ;X
                push   esi
                mov    esi, 318
XLoop:

                xor    edx, edx
                xor    eax, eax

                mov    al, [ ebp + esi + 1 ]
                mov    dl, [ ebp + esi - 1 ]
                sub    eax, edx                 ;eax = X

                xor    ebx, ebx
                mov    bl, [ ebp + esi + 320]
                mov    dl, [ ebp + esi - 320]
                sub    ebx, edx                 ;ebx = Y

                push   ebp
                push   edi

                mov    ebp, NY

                add    eax, ecx                 ;Add NX
                add    ebx, ebp

                mov    edx, eax                 ;Sign extend
                sar    edx, 31                  ;/
                mov    edi, ebx                 ;Sign extend
                sar    edi, 31                  ;/

                xor    eax, edx
                xor    ebx, edi

                add    eax, 128
                add    ebx, 128
                cmp    eax, 255
                jl     C1OK
                mov    eax, 254

C1OK:                                           ;eax = Col1
                cmp    ebx, 255
                jl     C2OK
                mov    ebx, 254
C2OK:                                           ;eax = Col2



                mul    ebx

                pop    edi

                xor    ebp, ebp
                shr    eax, 10

                neg    eax
                add    eax, 64


                mov    ebx, MapC
;------- G
                mov    edx, [ ebx + esi*2 ]     ;Load color-value
                and    edx, 0000011111100000b
                shl    edx, 2

                add    edx, BumpTables2
                mov    dx, [ edx + eax*2 ]
                shl    edx, 5
                or     bp, dx

                shr    eax, 1

                mov    edx, [ ebx + esi*2 ]     ;Load color-value
                and    edx, 0000000000011111b
                shl    edx, 6

                add    edx, BumpTables
                mov    dx, [ edx + eax*2 ]
                or     bp, dx

;------- R

                mov    edx, [ ebx + esi*2 ]     ;Load color-value
                and    edx, 01111100000000000b
                shr    edx, 5

                add    edx, BumpTables
                mov    dx, [ edx + eax*2 ]
                shl    dx, 11
                or     bp, dx

                mov    [ edi + esi*2 ], bp

                pop    ebp



                dec    ecx
                dec    esi
                jnz    XLoop

                pop    esi

                add    edi, 320*2
                add    MapC, 320*2
                add    ebp, 320
                inc    NY

                dec    YCounter
                jnz    YLoop


                ret
DBump2D       ENDP

