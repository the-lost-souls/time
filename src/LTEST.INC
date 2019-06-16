;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : L258.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 04.09.1996
;Action          : Lightroutines for V258-object type
;
;Include files : lights.inc
;
;Notes :
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

LightList258  DD 0000
              DD 0000
              DD L002_258
              DD 0000
              DD L004_258

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : SetLights258
;Type         : Macro
;Last update  : 04.08.1996
;Action       : Goes through the lights and returns calculated color
;Optimized    : No
;
;
;Input variables : -
;
;Output variables : DD Col = calculated color
;
;Registers changed : eax, ebx, ecx, edx, ebp
;
;Notes : edi and esi are pushed onto the stack so all registers are
;available for procedures.
;----------------------------------------------------------------------------

SetLights258  MACRO
                push   esi
                push   edi

                mov    esi, OFFSET Lights

L258Loop:
                or     DWORD PTR [ esi ], 0000
                jz     DoneL258

                push   esi
                mov    esi, [ esi ]
                cmp    esi, 0FFFFh
                je     DumpLight258
                mov    LightOffs, esi           ;Save object's offset

                mov    ebx, [ esi ]             ;Load object-type
                mov    eax, LightList258[ ebx*4 ] ;Load object's procedure

                or     eax, eax
                jz     DumpLight258

                call   eax
DumpLight258:
                pop    esi
                add    esi, 4                   ;Next light...
                jmp    L258Loop

DoneL258:

                pop    edi
                pop    esi
              ENDM


;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : L002_258
;Type         : Procedure
;Last update  : 04.08.1996
;Action       : Calculates a colorvalue and adds it to variable Col
;Optimized    : No
;
;Input variables : -
;
;Registers changed : All
;
;Notes : This just sets up for macro L001 (lights.inc)
;----------------------------------------------------------------------------

L002_258      PROC
                mov    edi, [ esp + 08 ]        ;Load verticeoffset
                add    edi, 40                  ;Point to nvector
                L001                            ;Calculate

                sub    ebx, 10                  ;Avoid overflow

                xor    eax, eax                 ;Avoid ebx becoming negative
                cmp    ebx, 0                   ;/
                setl   al                       ;/
                dec    eax                      ;/
                and    ebx, eax                 ;/

                mov    esi, LightOffs
                add    esi, 64
                mov    eax, ebx
                shr    eax, 3
                mov    edx, [ esi + eax*4 ]     ;Load R

                add    esi, 32*4
                mov    eax, ebx
                shr    eax, 2
                mov    eax, [ esi + eax*4 ]     ;Load G
                shl    eax, 8
                or     edx, eax

                add    esi, 64*4
                mov    eax, ebx
                shr    eax, 3
                mov    eax, [ esi + eax*4 ]     ;Load B
                shl    eax, 16
                or     edx, eax

                add    Col, edx                 ;Add color

                ret
L002_258      ENDP


;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : L004_258
;Type         : Procedure
;Last update  : 04.08.1996
;Action       : Calculates a colorvalue and adds it to Col
;Optimized    : No
;
;Input variables : -
;
;
;Registers changed : All
;
;Notes : Just setting up for macro L002 (lights.inc)
;----------------------------------------------------------------------------

L004_258      PROC
                mov    edi, [ esp + 08 ]        ;Load verticeoffset
                add    edi, 40                  ;Point to nvector
                L002                            ;Calculate

                sub    ebx, 20

                xor    eax, eax                 ;Avoid ebx becoming negative
                cmp    ebx, 0                   ;/
                setl   al                       ;/
                dec    eax                      ;/
                and    ebx, eax                 ;/

                mov    esi, LightOffs
                add    esi, 64
                mov    eax, ebx
                shr    eax, 3
                mov    edx, [ esi + eax*4 ]     ;Load R

                add    esi, 32*4
                mov    eax, ebx
                shr    eax, 2
                mov    eax, [ esi + eax*4 ]     ;Load G
                shl    eax, 8
                or     edx, eax

                add    esi, 64*4
                mov    eax, ebx
                shr    eax, 3
                mov    eax, [ esi + eax*4 ]     ;Load B
                shl    eax, 16
                or     edx, eax

                add    Col, edx                 ;Add color

                ret
L004_258      ENDP