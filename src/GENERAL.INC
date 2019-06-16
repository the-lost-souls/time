;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : GENERAL.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 23.10.1996
;Action          : Different standalone procedures
;
;Include files : -
;
;Notes :
;----------------------------------------------------------------------------

.386
.Model  FLAT

;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : SetupVGA
;Type         : Procedure
;Last update  : 23.10.96
;Action       : Reserves memory for canvas and sets the linear vga-address
;Optimized    : -
;
;Input variables : XRes, YRes (in bytes, NOT pixels)
;----------------------------------------------------------------------------

CanvasAddr    DD 0                              ;Linear offset of Canvas
CanvasSize    DD 0
XRes          DD 320                            ;Current video-mode's width
YRes          DD 200                            ;/...heigth
ResTable      DD 240 DUP (0)

SetupVGA      PROC
                mov    ax, 0EE02h               ;DOS32: address info
                int    31h                      ;ebx = program segment adress

                mov    eax, 000A0000h           ;
                sub    eax, ebx                 ;Set linear VGA-address
                mov    VgaAddress, eax          ;



                xor    ecx, ecx
VGA_ResTable:
                mov    eax, XRes
                mul    ecx                      ;Counter * XRes
                mov    ResTable[ ecx*4 ], eax   ;Save LookUp-value
                inc    ecx
                cmp    ecx, YRes
                jb     VGA_ResTable

                mov    eax, XRes                ;Calculate memory needed
                mul    YRes                     ;for double-buffer
                                                ;eax = BufferSize
                mov    edx, eax                 ;edx = Blocksize
                mov    ax, 0EE42h               ;DOS32: Allocate memory block
                int    31h

                jnc    VGA_CanvasOK
                mov    eax, 3
                mov    edx, LErrTable[ 3*4 ]
                jmp    VGA_Error
VGA_CanvasOK:
                clc                             ;Everything OK.
                mov    CanvasSize, eax
                mov    CanvasAddr, edx
                ret
VGA_Error:
                stc                             ;Failed!
                ret
SetupVga      ENDP



;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : SetupDOS32
;Type         : Procedure
;Last update  : 23.10.96
;Action       : Sets up different variables
;Optimized    : -
;
;Input variables : TempSize (default 64k)
;----------------------------------------------------------------------------

Program_Base  DD 0
Buffer_es     DW 0                              ;8kb buffer, segment address
Buffer_Flat   DD 0                              ;8kb buffer, linear address
VgaAddress    DD 0                              ;Linear offset of VGA-memory
TempSize      DD 65536*2
TempOffs      DD 0

SetupDOS32    PROC
                mov    ax, 0EE02h               ;DOS32: address info
                int    31h                      ;ebx = program segment adress

                mov    Program_Base, ebx
                mov    Buffer_es, ax

                and    eax, 0FFFFh
                shl    eax, 4                   ;*16
                sub    eax, ebx
                mov    Buffer_flat, eax           ;save near offset to buffer

                mov    eax, 000A0000h           ;
                sub    eax, ebx                 ;Set linear VGA-address
                mov    VgaAddress, eax          ;

                mov    edx, TempSize            ;edx = Blocksize
                mov    ax, 0EE42h               ;DOS32: Allocate memory block
                int    31h

                jnc    D32_TempOK
                mov    eax, 3                   ;Out of memory
                mov    edx, LErrTable[ 3*4 ]
                jmp    D32_Error
D32_TempOK:
                clc                             ;Everything OK.
                mov    TempOffs, edx
                ret
D32_Error:
                stc                             ;Failed!
                ret
SetupDOS32    ENDP



;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : VGAMode
;Type         : Macro
;Last update  : Spring '96
;Action       : Sets the VGA-mode
;Optimized    : :)
;
;Input variables : Mode
;
;Registers changed : ax
;----------------------------------------------------------------------------

VGAMode       MACRO VGA_Mode
                mov    ah, 0                    ;Initialize screenmode
                mov    al, VGA_Mode             ;320x200x256
                int    10h                      ;VGA-services
              ENDM

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : PrintDec
;Type         : Procedure
;Last update  : 13.05.1996
;Action       : Prints the number in eax as unsigned decimal
;Optimized    : -
;
;Input variables : eax = number
;
;Registers changed : eax, ecx, edx
;
;Notes :
;----------------------------------------------------------------------------

PrintDec      PROC
                push   eax
                push   ebx
                push   ecx
                push   edx

                push   DWORD PTR 0FFFFFFFFh

                mov    ecx, 10
PD1:
                xor    edx, edx
                div    ecx
                add    edx, 30h
                push   edx
                or     eax, eax
                jnz    PD1

PD2:
                pop    edx
                cmp    edx, 0FFFFFFFFh
                je     DonePD
                mov    eax, 00000200h
                int    21h
                jmp    PD2
DonePD:
                pop    edx
                pop    ecx
                pop    ebx
                pop    eax

                ret
PrintDec      ENDP


;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : PrintString
;Type         : MACRO
;Last update  : 23.10.1996
;Action       : Prints a string, terminates with '$'
;Optimized    : -
;
;Input variables : String's variablename
;
;Registers changed : eax, edx
;
;----------------------------------------------------------------------------

PrintString   MACRO String
                mov    ax, 0900h
                mov    edx, OFFSET String
                int    21h
              ENDM



;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : PrintCount
;Type         : Procedure
;Last update  : 30.10.1996
;Action       : Prints a string, ecx bytes long
;Optimized    : -
;
;Input variables : [ esi ] = Offset string
;                    ecx   = string length
;
;Registers changed : eax, edx, ecx, esi
;
;----------------------------------------------------------------------------

PrintCount    PROC
PrintC:
                movzx  edx, BYTE PTR [ esi ]
                mov    eax, 00000200h
                int    21h

                inc    esi
                dec    ecx
                jnz    PrintC

                ret
PrintCount    ENDP


;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : PrintZero
;Type         : Procedure
;Last update  : 30.10.1996
;Action       : Prints a string, terminates with 0
;Optimized    : -
;
;Input variables : [ esi ] = Offset string
;
;Registers changed : eax, edx, esi, ecx
;
;----------------------------------------------------------------------------

PrintZero     PROC


PrintZ:
                movzx  edx, BYTE PTR [ esi ]
                or     edx, edx
                jz     DonePZ

                mov    eax, 00000200h           ;Print character
                int    21h

                inc    esi
                dec    ecx
                jnz    PrintZ

DonePZ:

                ret
PrintZero     ENDP


;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : PrintHex
;Type         : Procedure
;Last update  : 30.10.1996
;Action       : Prints a number in hex
;Optimized    : -
;
;Input variables : eax = number
;
;Registers changed : none
;
;----------------------------------------------------------------------------

HexString     DB '0123456789ABCDEF'

PrintHex      PROC
                push   eax
                push   ebx
                push   ecx
                push   edx

                push   DWORD PTR 0FFFFFFFFh

PH1:
                shrd   edx, eax, 4
                shr    eax, 4

                shr    edx, 28
                movzx  edx, BYTE PTR HexString[ edx ]
                push   edx
                or     eax, eax
                jnz    PH1

PH2:
                pop    edx
                cmp    edx, 0FFFFFFFFh
                je     DonePH
                mov    eax, 00000200h
                int    21h
                jmp    PH2
DonePH:
                pop    edx
                pop    ecx
                pop    ebx
                pop    eax

                ret
PrintHex      ENDP

