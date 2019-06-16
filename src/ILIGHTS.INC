;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : ILIGHTS.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 11.09.1996
;Action          : Initializes the light for each frame
;
;Notes : See LIGHTS.DOC for info.
;----------------------------------------------------------------------------

.386
.Model  FLAT

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : InitLights
;Type         : Procedure
;Last update  : 24.06.1996
;Action       : Initializes lights.
;Optimized    : No
;
;Notes : This must be called as an ordinary object and before any of the
;vectorobjects! If the corresponding procedure in LightList is zero, then the
;light isn't initialized.
;----------------------------------------------------------------------------

InitLights    PROC
                mov    ebx, OFFSET Lights
LightL:
                or     DWORD PTR [ ebx ], 0000
                jz     DoneLights

                push   ebx

                mov    ebx, [ ebx ]             ;Load light's offset
                cmp    ebx, 0FFFFh
                je     NoLInit
                mov    eax, [ ebx ]             ;Load light's type
                mov    LightOffs, ebx

                mov    eax, LightList[ eax*4 ]  ;Init-procedure
                or     eax, eax
                jz     NoLInit

                call   eax

NoLInit:
                pop    ebx
                add    ebx, 4
                jmp    LightL

DoneLights:

                ret
InitLights    ENDP



;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : InitL001
;Type         : Procedure
;Last update  : 11.09.1996
;Action       : Creates a 256-length vector out of angles from light002
;Optimized    : No. The rotating-procedure may be replaced
;
;Notes : The datastructure ZVertex just contains a vector pointing in the
;z-direction.
;----------------------------------------------------------------------------

ZVertex       DD 0000, 0000, -256               ;A vector with length 256

InitL002      PROC
                mov    esi, LightOffs

                xor    ebx, ebx                 ;----- Moving light...
                mov    eax, [ esi + 28 ]        ;Load AddX
                imul   TimeDiff
                mov    edx, [ esi + 16 ]        ;Load AngX (fixed point)
                add    edx, eax                 ;Add angX
                cmp    edx, 360*8*256
                setge  bl
                dec    ebx
                and    edx, ebx
                mov    [ esi + 16 ], edx        ;Save fixed point angle
                shr    edx, 8
                mov    [ esi + 04 ], edx        ;Save angle

                xor    ebx, ebx                 ;
                mov    eax, [ esi + 32 ]        ;Load AddY
                imul   TimeDiff
                mov    edx, [ esi + 20 ]        ;Load AngY (fixed point)
                add    edx, eax                 ;Add angY
                cmp    edx, 360*8*256
                setge  bl
                dec    ebx
                and    edx, ebx
                mov    [ esi + 20 ], edx        ;Save fixed point angle
                shr    edx, 8
                mov    [ esi + 08 ], edx        ;Save angle

                xor    ebx, ebx                 ;
                mov    eax, [ esi + 36 ]        ;Load AddZ
                imul   TimeDiff
                mov    edx, [ esi + 24 ]        ;Load AngZ (fixed point)
                add    edx, eax                 ;Add angZ
                cmp    edx, 360*8*256
                setge  bl
                dec    ebx
                and    edx, ebx
                mov    [ esi + 24 ], edx        ;Save fixed point angle
                shr    edx, 8
                mov    [ esi + 12 ], edx        ;Save angle

                                                ;----- Creating vector...
                mov    edi, LightOffs
                add    edi, 4                   ;Point to angles
                mov    esi, OFFSET ZVertex
                call   Rotate12                 ;This may be replaced...(?)

                mov    esi, LightOffs
                mov    [ esi + 40 ], eax        ;Save vector X,
                mov    [ esi + 44 ], ebx        ;Y
                mov    [ esi + 48 ], ecx        ;..and Z

                ret
InitL002      ENDP

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : InitL004
;Type         : Procedure
;Last update  : 11.09.1996
;Action       : Performs any action on any lights of type 004
;Optimized    : -
;
;
;Notes : This one should be fixed later!
;----------------------------------------------------------------------------


InitL004      PROC
;                call   debug
                mov    esi, LightOffs
                add    esi, 516

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

                mov    esi, LightOffs
                add    esi, 516
                add    eax, [ esi + 60 ]
                add    ebx, [ esi + 64 ]
                add    ecx, [ esi + 68 ]

                mov    esi, LightOffs
                mov    [ esi + 04 ], eax
                mov    [ esi + 08 ], ebx
                mov    [ esi + 12 ], ecx


                ret
InitL004      ENDP