;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : VESA.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 29.10.96
;Action          : VESA routines
;
;Include files :
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

VESA_ErrTable   DD 0, OFFSET VESA_Err1, OFFSET VESA_Err2, OFFSET VESA_Err3

VESA_Err1       DB 'VESAErr1: Function failed',0Dh, 0Ah, '$'
VESA_Err2       DB 'VESAErr2: Function not supported', 0Dh, 0Ah, '$'
VESA_Err3       DB 'VESAErr3: Function invalid in current mode', 0Dh, 0Ah, '$'

;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : VESAInfo
;Type         : Procedure
;Last update  : 29.10.1996
;Action       : Gets VESA (2.0) info
;Optimized    : -
;
;Input variables : edi = Offset to put info (normally 256 byte buffer)
;
;Output variables : Returns carry if error.
;                   [ edx ] = error message
;                   eax     = error code
;
;Registers changed : all
;
;Notes : Uses the 8kb buffer set up in realmode space by DOS32.
;----------------------------------------------------------------------------


VESAInfo      PROC
                push   edi

                mov    eax, Buffer_Flat
                mov    BYTE PTR [ eax + 0 ], 'V'
                mov    BYTE PTR [ eax + 1 ], 'B'
                mov    BYTE PTR [ eax + 2 ], 'E'
                mov    BYTE PTR [ eax + 3 ], '2'

                mov    ax, Buffer_es            ;Segment address to rm-buffer
                mov    Real_es, ax              ;/
                mov    Real_edi, 0
                mov    Real_eax, 04F00h

                mov    ax, 300h                 ;DOS32: Real mode call back
                xor    ebx, ebx                 ;Zero flags etc.
                mov    bl, 10h                  ;Interrupt number
                xor    cx, cx
                mov    edi, OFFSET Real_edi
                int    31h

                pop    edi

                mov    eax, Real_eax

                or     ah, ah                   ;Error?
                jz     VESA_OK

                mov    edx, VESA_ErrTable[ eax*4 ]
                stc
                ret
VESA_OK:

                mov    esi, Buffer_Flat
                mov    ecx, 1024/4
                rep    movsd

                clc
                ret
VESAInfo      ENDP

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : VESAModeInfo
;Type         : Procedure
;Last update  : 29.10.96
;Action       : Gets information on VESA-modes
;Optimized    : -
;
;
;Input variables : eax = VESA Mode
;                  edi = Where to put info (256-byte buffer)
;
;
;Registers changed : All
;
;Notes : The mode should be
;
;
;----------------------------------------------------------------------------


VESAModeInfo  PROC
                push   edi

                mov    Real_ecx, eax            ;SVGA-mode
                mov    Real_edi, 0              ;Info-buffer
                mov    Real_eax, 04F01h

                mov    ax, 300h                 ;DOS32: Real mode call back
                xor    ebx, ebx                 ;Zero flags etc.
                mov    bl, 10h                  ;Interrupt number
                xor    cx, cx
                mov    edi, OFFSET Real_edi     ;Offset  realmode registers
                int    31h

                pop    edi

                mov    eax, Real_eax
                cmp    ah, 0
                je     VESAModeOK

                mov    edx, VESA_ErrTable[ eax*4 ]
                stc

                ret
VESAModeOK:

                mov    esi, Buffer_Flat
                mov    ecx, 256/4
                rep    movsd

                clc
                ret
VESAModeInfo  ENDP


;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : VESAMode
;Type         : Macro
;Last update  : 23.10.1996
;Action       : Sets VESA-mode
;Optimized    : -
;
;Input variables : Mode
;
;Registers changed : ax, bx
;----------------------------------------------------------------------------

VESAMode      MACRO VESA_Mode
                mov    ah, 4Fh
                mov    al, 02
                mov    bx, VESA_Mode
                int    10h
              ENDM
