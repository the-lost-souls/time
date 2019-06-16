;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : MATH1.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 20.06.1996
;Action          : Several calculation-routines
;
;Include files : general.inc
;
;Object files  : trig1-8.obj (SineTable)
;
;Notes :
; * F3DTo2D                     Macro           - Projects a 3D-coordinate
; * RConst6                     Macro           - Calculates rotating-consts
; * Rotate6                     Macro           - Rotates a coordinate
; * Rotate12                    Procedure       - Rotates a coordinate
;
;----------------------------------------------------------------------------

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         :
;Type         :
;Last update  :
;Action       :
;Optimized    :
;
;Input variables :
;
;
;Registers changed :
;
;
;Notes :
;
;
;----------------------------------------------------------------------------

.386
.Model  FLAT

EXTRN           SineTable : DWORD

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : F3DTo2D
;Type         : Macro
;Last update  : 14.04.1996
;Action       : Projects a 3D-coordinate to it's 2D
;Optimized    : Yes(?)
;
;Input variables : eax = X   ebx = Y   ecx = Z
;                  ebx = XOut eax = YOut
;
;Registers changed : eax, edx, ebx
;
;Notes : Difficult to optimize any more. Several RC's... :(
;----------------------------------------------------------------------------

F3DTo2D       MACRO

                sal    eax, 8                   ;U:1 Scale
                mov    edx, eax                 ;U:2 Sign extend 1:2
                sar    edx, 31                  ;U:3 Sign extend 2:2

                idiv   ecx                      ;U:4 (X*256)/Z

                mov    edx, ebx                 ;U:1 Sign extend 1:2
                mov    ebx, eax                 ;U:2 Save 2D-X

                mov    eax, edx                 ;U:3 Load 3D-Y
                sal    eax, 8                   ;U:4 Scale
                sar    edx, 31                  ;U:5 Sign extend 2:2

                idiv   ecx

;ebx = 2D-X
;eax = 2D-Y
              ENDM


;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : RConst6
;Type         : Proc
;Last update  : 25.04.1996
;Action       : Calculates the constants for a following rotate-routine
;Optimized    : ...?
;
;Input variables : [ esi ] = Where to put the constants (DWORDs)
;                  [ edi ] = Angles (DWORDs)
;
; Constants - structure :
; Constant    OFFSET
; m[0][0]        0
; m[1][0]        4
; m[2][0]        8
; m[0][1]       12
; m[1][1]       16
; m[2][1]       20
; m[0][2]       24
; m[1][2]       28
; m[2][2]       32
; mxy0          36
; mxy1          40
; mxy2          44
;--------------------------------
;Total : 48 bytes
;
;Registers changed : eax, ebx, ecx, edx, edi, ebp
;
;Notes :
;----------------------------------------------------------------------------

RConst6       PROC
                mov    ebx, [ edi + 0 ]         ;Load X-angle
                mov    ecx, [ edi + 4 ]         ;Load Y-angle
                mov    ebp, [ edi + 8 ]         ;Load Z-angle

;----------------------------- Constant : m[0][0] ----------------------------
;cos(za) * cos(ya) + sin(za) * sin(xa) * sin(ya)

                mov    eax, SineTable[ ebp*4 + (90*8*4) ] ;cos(za)
                mov    edx, SineTable[ ecx*4 + (90*8*4) ] ;cos(ya)

                imul   edx                      ;cos(za) * cos(ya)
                sar    eax, 8                   ;65536/256 = 256
                mov    edi, eax                 ;Save result

                mov    eax, SineTable[ ebp*4 ]  ;Sine of ZA
                mov    edx, SineTable[ ebx*4 ]  ;Sine of XA
                imul   edx                      ;sin(za) * sin(xa)
                sar    eax, 8                   ;65536/256 = 256
                mov    edx, SineTable[ ecx*4 ]  ;Sine of YA
                imul   edx                      ;sin(za) * sin(xa) * sin(ya)
                sar    eax, 8                   ;65536/256 = 256
                add    edi, eax                 ;Addition
                mov    [ esi ], edi             ;Save m[0][0]

;----------------------------- Constant : m[1][0] ----------------------------
;sin(za) * cos(xa)

                mov    eax, SineTable[ ebp*4 ]  ;sin(za)
                mov    edx, SineTable[ ebx*4 + (90*8*4) ] ;cos(xa)

                imul   edx                      ;sin(za) * cos(xa)
                sar    eax, 8                   ;
                mov    [ esi + 4 ], eax         ;Save m[1][0]

;----------------------------- Constant : m[2][0] ----------------------------
;sin(za) * sin(xa) * cos(ya) - cos(za) * sin(ya)

                mov    eax, SineTable[ ebp*4 ]  ;sin(za)
                mov    edx, SineTable[ ebx*4 ]  ;sin(xa)
                imul   edx                      ;sin(za) * sin(xa)
                sar    eax, 8
                mov    edx, SineTable[ ecx*4 + (90*8*4) ] ;cos(ya)
                imul   edx                      ;sin(za) * sin(xa) * cos(ya)
                sar    eax, 8
                mov    edi, eax                 ;Save result

                mov    eax, SineTable[ ebp*4 + (90*8*4) ] ;cos(za)
                mov    edx, SineTable[ ecx*4 ]  ;sin(ya)

                imul   edx                      ;cos(za) * -sin(ya)
                sar    eax, 8
                sub    edi, eax                 ;Subtraction
                mov    [ esi + 8 ], edi         ;Save m[2][0]

;----------------------------- Constant : m[0][1] ----------------------------
;cos(za) * sin(xa) * sin(ya) - sin(za) * cos(ya)

                mov    eax, SineTable[ ebp*4 + (90*8*4) ] ;cos(za)
                mov    edx, SineTable[ ebx*4 ]  ;sin(xa)
                imul   edx                      ;cos(xa) * sin(xa)
                sar    eax, 8

                mov    edx, SineTable[ ecx*4 ]  ;sin(ya)
                imul   edx                      ;cos(xa) * sin(xa) * sin(ya)
                sar    eax, 8
                mov    edi, eax                 ;Save result

                mov    eax, SineTable[ ebp*4 ]  ;sin(za)
                mov    edx, SineTable[ ecx*4 + (90*8*4) ] ;cos(ya)
                imul   edx                      ;sin(za) * cos(ya)
                sar    eax, 8

                sub    edi, eax                 ;Subtraction
                mov    [ esi + 12 ], edi        ;Save m[0][1]

;----------------------------- Constant : m[1][1] ----------------------------
;cos(za) * cos(xa)

                mov    eax, SineTable[ ebp*4 + (90*8*4) ] ;cos(za)
                mov    edx, SineTable[ ebx*4 + (90*8*4) ] ;cos(xa)
                imul   edx                      ;cos(za) * cos(xa)
                sar    eax, 8
                mov    [ esi + 16 ], eax        ;Save m[1][1]

;----------------------------- Constant : m[1][2] ----------------------------
;sin(za) * sin(ya) + cos(za) * sin(xa) * cos(ya)

                mov    eax, SineTable[ ebp*4 ]  ;sin(za)
                mov    edx, SineTable[ ecx*4 ]  ;sin(ya)
                imul   edx                      ;sin(za) * sin(ya)
                sar    eax, 8
                mov    edi, eax                 ;Save result

                mov    eax, SineTable[ ebp*4 + (90*8*4) ] ;cos(za)
                mov    edx, SineTable[ ebx*4 ]  ;sin(xa)
                imul   edx                      ;sin(za) * sin(ya) * cos(za)
                sar    eax, 8
                mov    edx, SineTable[ ecx*4 + (90*8*4) ] ;cos(ya)
                imul   edx                      ;sin(xa) * cos(ya)
                sar    eax, 8
                add    edi, eax                 ;Sixth constant done
                mov    [ esi + 20 ], edi        ;Save it

;----------------------------- Constant : m[2][0] ----------------------------
;cos(xa) * sin(ya)

                mov    eax, SineTable[ ebx*4 + (90*8*4) ] ;cos(xa)
                mov    edx, SineTable[ ecx*4 ]  ;sin(ya)
                imul   edx                      ;cos(xa) * sin(ya)
                sar    eax, 8
                mov    [ esi + 24 ], eax        ;Save m[2][0]

;----------------------------- Constant : m[2][1] ----------------------------
;-sin(xa)

                mov    eax, SineTable[ ebx*4 ]  ;sin(xa)
                neg    eax                      ;-sin(xa)
                mov    [ esi + 28 ], eax        ;Save m[2][1]

;----------------------------- Constant : m[2][2] ----------------------------
;cos(xa) * cos(ya)

                mov    eax, SineTable[ ebx*4 + (90*8*4) ] ;cos(xa)
                mov    edx, SineTable[ ecx*4 + (90*8*4) ] ;cos(ya)
                imul   edx                      ;cos(xa) * cos(ya)
                sar    eax, 8
                mov    [esi + 32 ], eax         ;Save m[2][2]

;------------------------------ Constant : mxy0 ------------------------------
;m[0][0] * m[1][0]

                mov    eax, [ esi ]             ;Load m[0][0]
                mov    edx, [ esi + 4 ]         ;Load m[1][0]
                imul   edx
                sar    eax, 8
                mov    [ esi + 36 ], eax        ;Save mxy0

;------------------------------ Constant : mxy1 ------------------------------
;m[0][1] * m[1][1]

                mov    eax, [ esi + 12 ]        ;Load m[0][1]
                mov    edx, [ esi + 16 ]        ;Load m[1][1]
                imul   edx
                sar    eax, 8
                mov    [ esi + 40 ], eax        ;Save mxy1

;------------------------------ Constant : mxy2 ------------------------------
;m[0][2] * m[1][2]

                mov    eax, [ esi + 24 ]        ;Load m[0][2]
                mov    edx, [ esi + 28 ]        ;Load m[1][2]
                imul   edx
                sar    eax, 8
                mov    [ esi + 44 ], eax        ;Save mxy2

                ret
RConst6       ENDP

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : Rotate6
;Type         : Macro
;Last update  : 18.04.1996
;Action       : Rotation, 6 imuls
;Optimized    : No
;
;Input variables : [ esi ] = Offset to constants from RConst6
;                  [ edi ] = Offset to vertex (Const, X, Y, Z in DWORDs)
;
;Output variables : eax = X  ebx = Y  ecx = Z
;
;Registers changed : eax, ebx, ecx, edx, ebp
;
;Notes : Yihaaa. This better be fast! :)
;----------------------------------------------------------------------------

Rotate6       MACRO
                mov    ebx, [ edi + 12 ]        ;U:1 Load v[2] = Z
                mov    ecx, [ edi +  0 ]        ;v:1 Load constant vxy

;----------- X-coordinate

                mov    eax, [ edi +  4 ]        ;U:2 Load v[0] = X
                sal    eax, 8                   ;U:3
                mov    ebp, [ esi +  4 ]        ;v:2 Load m[1][0]
                add    eax, ebp                 ;v:4
                mov    edx, [ edi +  8 ]        ;v:3 Load v[1] = Y
                sal    edx, 8                   ;U:4
                mov    ebp, [ esi +  0 ]        ;U:5 Load m[0][0]
                add    edx, ebp                 ;v:6

                imul   edx                      ;U:7
                shrd   eax, edx, 8              ;?
                mov    ebp, [ esi + 36 ]        ;Load mxy0
                sub    eax, ebp                 ; - mxy0
                mov    ebp, eax
                mov    eax, [ esi +  8 ]        ;Load m[2][0]
                imul   ebx
                add    ebp, eax
                sar    ebp, 8
                sub    ebp, ecx                 ;-vxy

                push   ebp

;----------- Y-coordinate

                mov    eax, [ edi +  4 ]        ;U:1 Load v[0] = X
                sal    eax, 8                   ;U:2
                mov    ebp, [ esi + 16 ]        ;v:2 Load m[1][1]
                add    eax, ebp                 ;U:3
                mov    edx, [ edi +  8 ]        ;v:3 Load v[1] = Y
                sal    edx, 8                   ;U:4
                mov    ebp, [ esi + 12 ]        ;v:4 Load m[0][1]
                add    edx, ebp                 ;U:5

                imul   edx
                shrd   eax, edx, 8
                mov    ebp, [ esi + 40 ]        ;Load mxy1
                sub    eax, ebp                 ; - mxy1
                mov    ebp, eax
                mov    eax, [ esi + 20 ]        ;Load m[2][1]
                imul   ebx
                add    ebp, eax                 ;
                sar    ebp, 8
                sub    ebp, ecx                 ; - vxy0

                push   ebp

;----------- Z-coordinate

                mov    eax, [ edi +  4 ]        ;Load v[0] = X
                sal    eax, 8
                mov    ebp, [ esi + 28 ]        ;Load m[1][2]
                add    eax, ebp
                mov    edx, [ edi +  8 ]        ;Load v[1] = Y
                sal    edx, 8
                mov    ebp, [ esi + 24 ]        ;Load m[0][2]
                add    edx, ebp

                imul   edx
                shrd   eax, edx, 8
                mov    ebp, [ esi + 44 ]        ;Load mxy2
                sub    eax, ebp                 ; - mxy2
                mov    ebp, eax
                mov    eax, [ esi + 32 ]        ;Load m[2][2]
                imul   ebx
                add    ebp, eax                 ;
                sar    ebp, 8
                sub    ebp, ecx                 ; - vxy0

                mov    ecx, ebp                 ;ecx = Z
                pop    ebx                      ;Restore Y
                pop    eax                      ;Restore X

              ENDM







;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : Rotate12
;Type         : Procedure
;Last update  : 20.06.1996
;Action       : Rotation, 12 imuls
;Optimized    : No
;
;Input variables : [ esi ] = Offset to vertex
;                  [ edi ] = Offset to angles
;
;Output variables : eax = X  ebx = Y  ecx = Z
;
;Registers changed : eax, ebx, ecx, edx, ebp
;
;Notes : Slow one. May be used if it's a small amount of coordinates.
;----------------------------------------------------------------------------

Rotate12      PROC
                                                ;----- Rotating X-axis
                mov    ebp, [ edi + 00 ]        ;Load XAngle

                mov    ebx, SineTable[ ebp*4 ]  ;sin(XAngle)
                mov    ecx, SineTable[ ebp*4 + (90*4*8) ] ;cos(XAngle)

                mov    eax, DWORD PTR [ esi + 04 ] ;Load Y
                imul   ecx                      ;y*cos(XAngle)
                mov    ebp, eax                 ;Save result

                mov    eax, DWORD PTR [ esi + 08 ] ;Load Z
                imul   ebx                      ;z*sin(XAngle)
                sub    ebp, eax                 ;ebp = newY
                sar    ebp, 8                   ;Fixed point...
                push   ebp                      ;Save newY

                mov    eax, DWORD PTR [ esi + 04 ] ;Load Y
                imul   ebx                      ;y*sin(XAngle)
                mov    ebx, eax                 ;Save result

                mov    eax, DWORD PTR [ esi + 08 ] ;Load Z
                imul   ecx                      ;z*cos(XAngle)
                add    ebx, eax                 ;ebx = newZ
                sar    ebx, 8                   ;Fixed point...



                                                ;----- Rotating Y-axis
                mov    ebp, [ edi + 04 ]        ;Load YAngle

                mov    eax, SineTable[ ebp*4 + (90*4*8) ] ;cos(YAngle)
                imul   ebx                      ;z*cos(YAngle)
                mov    ecx, eax                 ;Save result

                mov    ebp, SineTable[ ebp*4 ]  ;sin(YAngle)
                mov    eax, [ esi + 00 ]        ;Load X
                imul   ebp                      ;x*sin(YAngle)
                sub    ecx, eax                 ;ecx = Final Z
                sar    ecx, 8                   ;Fixed point...
                push   ecx                      ;Save it!

                mov    eax, ebp                 ;sin(YAngle)
                imul   ebx                      ;z*sin(YAngle)
                mov    ebx, eax                 ;Save result

                mov    eax, [ esi + 00 ]        ;Load X
                mov    edx, [ edi + 04 ]        ;Load YAngle (...again :( )
                mov    edx, SineTable[ edx*4 + (90*4*8) ] ;cos(YAngle)

                imul   edx                      ;x*cos(YAngle)
                add    ebx, eax                 ;ebx = newX
                sar    ebx, 8                   ;Fixed point...



                                                ;----- Rotating Z-axis
                mov    ebp, [ edi + 08 ]        ;Load ZAngle

                mov    eax, SineTable[ ebp*4 + (90*4*8) ] ;cos(ZAngle)
                imul   ebx                      ;x*cos(ZAngle)
                mov    ecx, eax                 ;Save result

                mov    eax, SineTable[ ebp*4 ]  ;sin(ZAngle)
                imul   DWORD PTR [ esp + 4 ]    ;y*sin(ZAngle)
                sub    ecx, eax                 ;ecx = Final X
                sar    ecx, 8                   ;Fixed point...

                mov    eax, SineTable[ ebp*4 ]  ;sin(ZAngle)
                imul   ebx                      ;y*cos(ZAngle)
                mov    ebx, eax                 ;Save result

                mov    eax, SineTable[ ebp*4 + (90*4*8) ] ;cos(ZAngle)
                imul   DWORD PTR [ esp + 4 ]    ;x*sin(ZAngle)
                add    ebx, eax                 ;ebx = Final Y
                sar    ebx, 8                   ;Fixed point...

                mov    eax, ecx                 ;eax = Final X
                pop    ecx                      ;ecx = Final Z

                pop    edx                      ;Dump tempvariable

                ret
              ENDP