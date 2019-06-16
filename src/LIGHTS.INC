;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : LIGHTS.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 04.09.1996
;Action          : Lightcalculations
;
;Include files : -
;
;Notes :
;
;
;----------------------------------------------------------------------------

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         :
;Type         :
;Last update  :
;Action       :
;Optimized    :
;
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
;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : L001
;Type         : Macro
;Last update  : 04.09.1996
;Action       : Calculates a light-value, type 1
;Optimized    : No
;
;
;Input variables : LightOffs = Offset lightdata type 001
;                  [ edi ]   = Offset normalvector (XYZ) with length 256
;
;
;Registers changed : eax, ebx, edx, esi
;
;
;Notes : Takes the dotproduct of normalvector and lightvector and divides it
;by the vectors' lengths. Returns the cosine of angle between vectors
;multiplied by 256.
;----------------------------------------------------------------------------

L001          MACRO
                mov    esi, LightOffs

                mov    eax, [ esi + 40 ]        ;Load Lightvector0 (X)
                imul   DWORD PTR [ edi + 00 ]   ;LV0*NV0
                mov    ebx, eax                 ;Save result

                mov    eax, [ esi + 44 ]        ;Load LV1 (Y)
                imul   DWORD PTR [ edi + 04 ]   ;LV1*NV1
                add    ebx, eax

                mov    eax, [ esi + 48 ]        ;Load LV2 (Z)
                imul   DWORD PTR [ edi + 08 ]   ;LV2*NV2
                add    ebx, eax                 

                sar    ebx, 8                   ;Divide by length

              ENDM



;edi = Offset rotated normalvector and position
;
;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : L002
;Type         : Macro
;Last update  : 04.09.1996
;Action       : Calculates a lightvalue, type 002
;Optimized    : Yes
;
;
;Input variables : LightOffs = Offset lightdata type 002
;                  [ edi ]   = Normalvector with position
;
;Registers changed : eax, ebx, ecx, edx, ebp, esi
;
;
;Notes : See lights.doc for mathematical explanation.
;----------------------------------------------------------------------------

L002          MACRO
LOCAL Sw1, Sw2, Sw3, OutRange, InRange
                mov    esi, LightOffs

                mov    ebx, [ edi + 12 ]        ;NVector's Xpos
                mov    eax, [ esi + 04 ]        ;Light's Xpos
                sub    eax, ebx                 ;Creating vector
                mov    [ esi + 28 ], eax

                imul   DWORD PTR [ edi + 00 ]   ;NVector's X
                mov    ebp, eax                 ;U:1

                mov    ebx, [ edi + 16 ]        ;v:1 NVector's Ypos
                mov    eax, [ esi + 08 ]        ;U:2 Light's Ypos
                sub    eax, ebx                 ;U:3 Creating vector
                mov    [ esi + 32 ], eax        ;U:4

                imul   DWORD PTR [ edi + 04 ]   ;U:5 NVector's Y
                add    ebp, eax

                mov    ebx, [ edi + 20 ]        ;NVector's Zpos
                mov    eax, [ esi + 12 ]        ;Light's Zpos
                sub    eax, ebx                 ;Creating vector
                mov    [ esi + 36 ], eax        ;NVector's Z

                imul   DWORD PTR [ edi + 08 ]   ;NVector's Z
                add    ebp, eax                 ;ebp = nv dot lv


                                                ;Finding |lv|


                mov    eax, [ esi + 28 ]        ;Lightvector X
                mov    ebx, [ esi + 32 ]        ;/Y
                mov    ecx, [ esi + 36 ]        ;/Z

                xor    edx, edx
                cmp    eax, 0                   ;Get absolute values
                setg   dl
                dec    edx
                xor    eax, edx

                xor    edx, edx
                cmp    ebx, 0
                setg   dl
                dec    edx
                xor    ebx, edx

                xor    edx, edx
                cmp    ecx, 0
                setg   dl
                dec    edx
                xor    ecx, edx

                cmp    eax, ebx                 ;Get vector's length
                jg     Sw1
                mov    edx, eax                 ;X <-> Y
                mov    eax, ebx
                mov    ebx, edx
Sw1:
                cmp    eax, ecx
                jg     Sw2
                mov    edx, eax                 ;X <-> Z
                mov    eax, ecx
                mov    ecx, edx
Sw2:
                cmp    ebx, ecx
                jg     Sw3
                mov    edx, ebx                 ;Y <-> Z
                mov    ebx, ecx
                mov    ecx, edx
Sw3:


                shr    ebx, 1
                add    ecx, eax
                add    ecx, ebx                 ;ecx = |lv|

                cmp    ecx, [ esi + 44 ]        ;BlackRange
                jae    OutRange

                mov    eax, ebp
                mov    edx, ebp
                sar    edx, 31

                or     ecx, 1
                idiv   ecx
                mov    ebx, eax                 ;ebx = colorvalue

                mov    edx, [ esi + 40 ]        ;MaxRange
                cmp    ecx, edx
                jbe    InRange

                sub    ecx, edx
                mov    eax, [ esi + 48 ]        ;Load
                mul    ecx
                sar    eax, 3
                sub    ebx, eax
                jmp    InRange

OutRange:
                xor    ebx, ebx
InRange:

              ENDM