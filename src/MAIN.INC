;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : MAIN.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 10.10.1996
;Action          : Main-objects for engine
;
;Include files : -
;
;Notes : Look MAIN.DOC for detailed info on objects.
;----------------------------------------------------------------------------

.386
.Model  FLAT

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : MainL
;Type         : Macro
;Last update  : 17.10.96
;Action       : The main loop
;Optimized    : -
;
;Input variables : ObjectList. Ending with object type 0000
;
;Notes :
;----------------------------------------------------------------------------

MainL         MACRO
LOCAL MainLoop, ObjectLoop, DoneObjects, Done

MainLoop:
                mov    esi, OFFSET Objects

ObjectLoop:
                or     DWORD PTR [ esi ], 0000
                jz     DoneObjects

                push   esi
                mov    esi, [ esi ]

                cmp    esi, 0FFFFh
                je     FakeObject

                mov    eax, [ esi ]              ;Load object-type
                mov    ObjectOffs, esi           ;Save object's offset
                call   ProcList[ eax*4 ]

FakeObject:
                pop    esi
                add    esi, 4
                jmp    ObjectLoop

DoneObjects:

                mov    ah, 0Bh                   ;
                int    21h                       ;Has somebody pressed a key?
                cmp    al, 00h                   ;
                jne    Done                     ;Probably not...
                jmp    MainLoop

Done:
              ENDM


;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : InitSort
;Type         : Procedure
;Last update  : 13.05.1996
;Action       : Allocates memory for the sorting-tables
;Optimized    : No
;
;Input variables : None
;
;Registers changed : eax, ecx, edx
;
;Notes : Returns eax with number of bytes allocated. May only be used for
;initializing sorting-tables.
;----------------------------------------------------------------------------

SortBuffer      DB 4096 DUP (?)

InitSort      PROC
                mov    ListStart1, OFFSET SortBuffer
                mov    ListEnd1, OFFSET SortBuffer + 1024
                mov    ListStart2, OFFSET SortBuffer + 2048
                mov    ListEnd2, OFFSET SortBuffer + 3072

                ret
InitSort      ENDP

;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : SetupSort
;Type         : Procedure
;Last update  : 13.05.1996
;Action       : Resets the parttables and saves end-part
;Optimized    : No
;
;Input variables : None
;
;Registers changed : eax, ebx, ecx, edx, edi
;
;Notes : MUST be called before Sort!!!
;----------------------------------------------------------------------------

EndPartT      DD 0000                           ;Offset to next
              DD 0000                           ;Draw last! (Z=0)
              DD 0000                           ;PartT 0 = end!

SetupSort     PROC
                mov    eax, ListStart1
                mov    ebx, ListStart2
                mov    edx, ListEnd1
                mov    edi, ListEnd2
                mov    ecx, -256
SortInitL:
                mov    [ ecx*4 + 1024 + edx ], eax
                mov    [ ecx*4 + 1024 + edi ], ebx


                mov    DWORD PTR [ eax ], 0FFFFFFFFh
                mov    DWORD PTR [ ebx ], 0FFFFFFFFh
                add    eax, 4
                add    ebx, 4
;-------------------
                mov    [ ecx*4 + 1028 + edx ], eax
                mov    [ ecx*4 + 1028 + edi ], ebx

                mov    DWORD PTR [ eax ], 0FFFFFFFFh
                mov    DWORD PTR [ ebx ], 0FFFFFFFFh
                add    eax, 4
                add    ebx, 4

                add    ecx, 2
                jnz    SortInitL

                mov    eax, ListStart1
                mov    DWORD PTR [ (255*4) + eax ], OFFSET EndPartT
                mov    eax, ListEnd1
                mov    DWORD PTR [ (255*4) + eax ], OFFSET EndPartT

                ret
SetupSort     ENDP

;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : Sort
;Type         : Procedure
;Last update  : 13.05.1996
;Action       : Sorts the part-tables reset by SetupSort
;Optimized    : No
;
;Input variables : None
;
;Registers changed : All.
;
;Notes : Z-values can't be greater than 65535!
;----------------------------------------------------------------------------

Sort          PROC
                mov    ebx, -1
                mov    edx, ListStart1
                mov    ebp, ListEnd2


RadixL:
                inc    ebx
                mov    esi, [ ebx*4 + edx ]     ;Load start of list1
                cmp    esi, 0FFFFFFFFh
                je     RadixL

                mov    ecx, ListEnd1
                mov    edi, [ ebx*4 + ecx ]     ;Load endoffset of list1

ListL:
                mov    eax, [ esi + 4 ]
                shr    eax, 8                   ;All z-coords are positive
                and    eax, 255
                mov    ecx, 255
                sub    ecx, eax

                mov    eax, [ ecx*4 + ebp ]
                mov    [ eax ], esi
                mov    [ ecx*4 + ebp ], esi

                cmp    esi, edi
                mov    esi, [ esi ]
                jne    ListL
                cmp    ebx, 255
                jb     RadixL

                ret
Sort          ENDP

;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : RunParts
;Type         : Procedure
;Last update  : 13.05.1996
;Action       : Runs all the parts in the parttables
;Optimized    : No
;
;Notes : The loop stops when it has reached the EndMark-part
;----------------------------------------------------------------------------


RunParts      PROC
                mov    ebx, -1
                mov    edi, ListStart2

PartL:
                inc    ebx
                mov    esi, [ ebx*4 + edi ] ;Load start of list
                cmp    esi, 0FFFFFFFFh
                je     PartL

ListL2:
                mov    eax, [ esi + 08 ]        ;Load Parttype
                or     eax, eax
                jz     DoneParts

                mov    eax, PartList[ eax*4 ]

                push   esi
                push   ebx

                add    esi, 12                  ;Dump partheader

                call   eax

                pop    ebx
                pop    esi

                mov    edi, Listend2
                cmp    esi, [ ebx*4 + edi ]
                mov    esi, [ esi ]
                jne    ListL2

                mov    edi, ListStart2
                cmp    ebx, 255
                jb     PartL

DoneParts:
                ret
RunParts      ENDP

;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : ClearCanvas
;Type         : Procedure
;Last update  : 13.06.1996
;Action       : Clears the canvas with given color
;Optimized    : No
;
;Input variables : [ ObjectOffs + 04 ] = color
;
;Registers changed : eax, ebx, ecx
;
;Notes : Only clears 64000 bytes.
;----------------------------------------------------------------------------

ClearCanvas   PROC
                mov    ecx, ObjectOffs
                mov    eax, XRes
                mul    YRes
                mov    ebx, eax
                shr    ebx, 5
                mov    eax, [ ecx + 04 ]        ;Load color
                mov    ecx, CanvasAddr
BlankLoop:
              I = 0
              REPT 8
                mov    [ ecx + (I*4)  ], eax
              I = I + 1
              ENDM

                add    ecx, 32
                dec    ebx
                jnz    BlankLoop

                ret
ClearCanvas   ENDP

;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : DisplayCanvas
;Type         : Procedure
;Last update  : 10.10.1996
;Action       : Copies Canvas to VGAAdress
;Optimized    : Yes
;
;Registers changed : eax, ebx, ecx, esi, edi
;
;Notes : Copies 64000 bytes to the screen.
;----------------------------------------------------------------------------

FillVGA       MACRO
Local FillL

FillL:
                mov    eax, [ esi + 00 ]
                mov    ebx, [ esi + 04 ]
                mov    [ edi + 00 ], eax
                mov    [ edi + 04 ], ebx
                mov    eax, [ esi + 08 ]
                mov    ebx, [ esi + 12 ]
                mov    [ edi + 08 ], eax
                mov    [ edi + 12 ], ebx
                mov    eax, [ esi + 16 ]
                mov    ebx, [ esi + 20 ]
                mov    [ edi + 16 ], eax
                mov    [ edi + 20 ], ebx
                mov    eax, [ esi + 24 ]
                mov    ebx, [ esi + 28 ]
                mov    [ edi + 24 ], eax
                mov    [ edi + 28 ], ebx

                add    esi, 32
                add    edi, 32
                dec    ecx
                jnz    FillL


              ENDM

DisplayCanvas PROC
                mov    ecx, 64000/32
                mov    esi, CanvasAddr
                mov    edi, VGAAddress

                FillVGA

                ret
DisplayCanvas ENDP


;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : DisplayVESA
;Type         : Procedure
;Last update  : 10.10.1996
;Action       : Copies Canvas to vidememory (using VESA bank switching)
;Optimized    : No
;
;Registers changed : -
;
;Notes : Copies XRes*YRes bytes to the videomem.
;----------------------------------------------------------------------------

DisplayVESA   PROC
                mov   eax, XRes                 ;Calculate canvassize
                mul   YRes
                mov   ebp, eax
                xor   edx, edx
                cld

                mov   esi, CanvasAddr           ;Read from here
VESALoop:
                push  edx

                mov   ah, 4Fh                   ;SVGA services
                mov   al, 05h                   ;SVGA videomem window control
                mov   bh, 0                     ;SVGA bank select
                mov   bl, 0                     ;window A
                                                ;edx = bank
                int   10h

                mov    ecx, 65536/32
                mov    edi, VGAAddress

                FillVGA

                pop   edx
                inc   edx                       ;Next bank

                sub   ebp, 16384*4
                cmp   ebp, 16384*4
                jg    VESALoop

                mov   ah, 4Fh                   ;SVGA services
                mov   al, 05h                   ;SVGA videomem window control
                mov   bh, 0                     ;SVGA bank select
                mov   bl, 0                     ;window A
                                                ;edx = bank
                int   10h

                mov   ecx, ebp
                shr   ecx, 5                    ;32 bytes each loop
                mov   edi, VGAAddress

                FillVGA

                ret
DisplayVESA   ENDP

;----------------------------------------------------------------------------
;Name         : ReadTimer
;Type         : Procedure
;Last update  : 04.10.1996
;Action       : Gets a value from the timer with resolution of about 1/4100
;Optimized    : -
;
;Output variables : eax = timervalue
;
;Registers changed : eax
;
;Notes : ...
;----------------------------------------------------------------------------

ReadTimer     PROC
                mov  ah,0                  ;get tick count from Dos and use For hi 3 Bytes
                int  01ah                  ;lo order count in DX, hi order in CX

                mov  al, dh
                mov  ah, cl
                shl  eax, 16
                mov  ah, dl
                mov  al,00000001b          ;read lo Byte straight from timer chip
                out  43h,al                ;latch count
                in   al,40h                ;read in lo Byte (and discard)
                in   al,40h                ;hi Byte into al

                mov  ebx, 0100h
                xor  edx, edx
                mov  dl, al
                sub  ebx, edx
                xor  al, al
                add  eax, ebx

                mov  edx, LastTime
                mov  LastTime, eax
                sub  eax, edx

                cmp  eax, 0
                jge  TimerOK
                mov  eax, 0
TimerOK:

                mov  TimeDiff, eax
                add  Time, eax

                ret
ReadTimer     ENDP

;----------------------------------------------------------------------------
;Name         : InitTimer
;Type         : Procedure
;Last update  : 04.10.1996
;Action       : Initializes the timer, and the timervariables
;Optimized    : -
;
;Output variables : eax = timervalue
;
;Registers changed : eax
;
;Notes : ...
;----------------------------------------------------------------------------

InitTimer     PROC
                mov  al,00000000b
                out  43h,al                     ;Port number of timer control
                xor  al,al
                out  40h,al                     ;Port number of timer 0
                out  40h,al

                                                ;Get initial timer-value
                mov  ah,0                       ;get tick count from Dos and use For hi 3 Bytes
                int  01ah                       ;lo order count in DX, hi order in CX

                mov  al, dh
                mov  ah, cl
                shl  eax, 16
                mov  ah, dl

                mov  al,00000100b               ;read lo Byte straight from timer chip
                out  43h,al                     ;latch count
                in   al,40h                     ;read in lo Byte (and discard)
                in   al,40h                     ;hi Byte into al
                neg  al                         ;make it so counting up instead of down

                mov  StartTime, eax
                mov  LastTime, eax
                mov  Time, 0

                ret
InitTimer     ENDP

;------------------------------- ASSEMBLER ROUTINE --------------------------
;Name         : DisplayLFB
;Type         : Procedure
;Last update  : 30.10.1996
;Action       : Copies Canvas to vidememory (using linear framebuffer)
;Optimized    : No
;
;Registers changed : -
;
;Notes : Copies XRes*YRes bytes to the videomem.
;----------------------------------------------------------------------------

DisplayLFB    PROC
                mov   eax, XRes                 ;Calculate canvassize
                mul   YRes

                mov   esi, CanvasAddr           ;Read from here
                mov   edi, VGAAddress
                mov   ecx, eax                  ;ecx = CanvasSize
                shr   ecx, 5                    ;/ div 32
                FillVGA

                ret
DisplayLFB    ENDP