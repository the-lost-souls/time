;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : V258.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 25.09.1996
;Action          : Vectorobject type 3 (type 258 in the engine)
;
;Include files : math1.inc
;                l258.inc
;
;Notes :
;----------------------------------------------------------------------------

.386
.Model  FLAT

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : LoadV258
;Type         : Procedure
;Last update  : 25.09.1996
;Action       : Loads vectorobject, type 258 into memory
;Optimized    : -
;
;Input variables :   ecx   = Fileoffset to load from
;                  [ edx ] = Filename
;                  [ esi ] = Where to store header
;
;Registers changed : All
;
;Notes : Returns carry flag set if any error. [ edx ] = errormessage
;----------------------------------------------------------------------------

LoadV258      PROC
                push   ecx                      ;Save fileoffset

                mov    ah, 3Dh                  ;Open file
                mov    al, 0                    ;Just read access
                                                ;edx = offset filename
                int    21h

                pop    edx                      ;edx = fileoffset
                jnc    OpenOK258

                mov    edx, LErrTable[ 0*4 ]    ;Err0: error opening file
                jmp    LoadErr258
OpenOK258:
                mov    FHandle, ax              ;Save filehandle

                mov    ah, 42h                  ;Move filepointer
                mov    al, 0                    ;..from start of file
                mov    bx, FHandle              ;Load filehandle
                int    21h
                jnc    SeekOK258

                mov    edx, LErrTable[ 1*4 ]    ;Err1: error moving fpointer
                jmp    LoadErr258
SeekOK258:

                mov    ah, 3Fh                  ;Read from file
                mov    bx, FHandle              ;Load filehandle
                mov    ecx, 4                   ;4 bytes
                mov    edx, OFFSET Info         ;Store info
                int    21h
                jnc    ReadOK1_258
                mov    edx, LErrTable[ 2*4 ]    ;Err2: error reading file
                jmp    LoadErr258
ReadOK1_258:

                mov    ah, 3Fh                  ;Read from file
                mov    bx, FHandle              ;Load filehandle
                mov    ecx, 27*4                ;27*4 (27 DWORDS)
                mov    edx, esi                 ;Store header at [ esi ]
                int    21h

                cmp    eax, ecx                 ;Found everything?
                je     ReadOK2_258

                mov    edx, LErrTable[ 2*4 ]    ;Err2: error reading file
                jmp    LoadErr258
ReadOK2_258:

                cmp    DWORD PTR [ esi ], 258   ;Check object type
                je     ObjectOK258
                mov    edx, LErrTable[ 5*4 ]    ;Err5: wrong object-type
                jmp    LoadErr258
ObjectOK258:


;---------- Allocating buffers...
                mov    eax, [ esi + 04 ]        ;Load NVert
                mov    edx, 68                  ;40*NVert bytes for vertices
                mul    edx
                mov    ebx, eax
                mov    eax, [ esi + 12 ]        ;Load NFace
                shl    eax, 4                   ;16*NFace bytes for faces
                add    ebx, eax                 ;Add size

;...allocating CBuf, RBuf, FBuf, NBuf
                add    ebx, 64                  ;64 bytes for CBuf
                mov    eax, [ esi + 12 ]        ;Load NFace
                mov    edx, 64                  ;64*NFace bytes for FBuf
                mul    edx                      ;/
                add    ebx, eax                 ;Add FBuf-size
                mov    eax, [ esi + 04 ]        ;NFace
                shl    eax, 4                   ;16 bytes for each nvector
                add    ebx, eax                 ;Add NBuf-size
                add    ebx, 32                  ;Secure 32-byte aligning

                mov    edx, ebx                 ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h

                jnc    MemOK258

                mov    edx, LErrTable[ 3*4 ]    ;Err3: error allocating mem
                jmp    LoadErr258
MemOK258:

                mov    ebp, eax                 ;ebp = size of allocated mem
                mov    ebx, edx                 ;Make edx available (for mul)
                and    ebx, -32
                add    ebx, 32
                mov    [ esi + 08 ], ebx        ;Save NVertOffs
                mov    eax, [ esi + 04 ]
                mov    edx, 68
                mul    edx
                add    ebx, eax
                mov    [ esi + 16 ], ebx        ;Save NFaceOffs
                mov    edx, [ esi + 12 ]
                shl    edx, 4
                add    ebx, edx
                mov    [ esi + 20 ], ebx        ;Save CBuf-offset
                add    ebx, 64
                mov    [ esi + 28 ], ebx        ;Save FBuf-offset
                mov    eax, [ esi + 12 ]
                mov    edx, 40
                mul    edx
                add    ebx, eax
                mov    [ esi + 32 ], ebx        ;Save NBuf-offset

                                                ;----- Loading vertices
                mov    eax, [ esi + 04 ]        ;Load NVert
                mov    edx, 6                   ;Each vertice in file=6 bytes
                mul    edx
                mov    ecx, eax                 ;ecx = number of bytes to read
                mov    ah, 3Fh                  ;Read from file
                mov    bx, FHandle              ;Load filehandle
                mov    edx, [ esi + 28 ]        ;Use FBuf as temp-buffer
                int    21h

                cmp    eax, ecx                 ;Found everything?
                je     ReadOK3_258

                mov    edx, LErrTable[ 2*4 ]    ;Err2: Error reading file
                jmp    LoadErr258
ReadOK3_258:
                mov    ecx, [ esi + 04 ]        ;Load NVert
                mov    edi, [ esi + 08 ]        ;NVertOffs
                mov    ebx, [ esi + 28 ]        ;Temp-buffer
IVert258:
                movsx  eax, WORD PTR [ ebx ]    ;Load 2-byte X
                mov    [ edi + 04 ], eax        ;Save X
                movsx  eax, WORD PTR [ ebx + 2 ];Load 2-byte Y
                mov    [ edi + 08 ], eax        ;Save Y
                movsx  eax, WORD PTR [ ebx + 4 ];Load 2-byte Z
                mov    [ edi + 12 ], eax        ;Save Z

                mov    eax, [ edi + 04 ]        ;Load X
                mov    edx, [ edi + 08 ]        ;Load Y
                imul   edx
                mov    [ edi ], eax             ;Save constant vxy

                add    ebx, 6
                add    edi, 68
                dec    ecx
                jnz    IVert258

                                                ;----- Loading faces
                mov    eax, [ esi + 12 ]        ;Load NFace
                mov    edx, 7                   ;Each face in file=7 bytes
                mul    edx
                mov    ecx, eax                 ;ecx = number of bytes to read
                mov    ah, 3Fh                  ;Read from file
                mov    bx, FHandle              ;Load filehandle
                mov    edx, [ esi + 28 ]        ;Use FBuf as temp-buffer
                int    21h

                cmp    eax, ecx                 ;Found everything?
                je     ReadOK4_258

                mov    edx, LErrTable[ 2*4 ]    ;Err2: Error reading file
                jmp    LoadErr258
ReadOK4_258:


                mov    ecx, [ esi + 12 ]        ;Load NVert
                mov    edi, [ esi + 16 ]        ;NVertOffs
                mov    ebx, [ esi + 28 ]        ;Temp-buffer
IFace258:
                movzx  eax, BYTE PTR [ ebx ]    ;Load 1-byte color (unsigned)
                mov    [ edi ], eax             ;Save color
                movzx  eax, WORD PTR [ ebx + 1 ];Load 2-byte V0
                mov    [ edi + 04 ], eax        ;Save V0
                movzx  eax, WORD PTR [ ebx + 3 ];Load 2-byte V1
                mov    [ edi + 08 ], eax        ;Save V1
                movzx  eax, WORD PTR [ ebx + 5 ];Load 2-byte V2
                mov    [ edi + 12 ], eax        ;Save V2

                add    ebx, 7
                add    edi, 16
                dec    ecx
                jnz    IFace258

                mov    ah, 3Eh                  ;Close filehandle
                mov    bx, FHandle              ;Load filehandle
                int    21h
                jnc    CloseOK258
                mov    edx, LErrTable[ 4*4 ]    ;Err5: Error closing file
                jmp    LoadErr258
CloseOK258:

                push   ebp                      ;Save "bytes allocated"


;----------------------- CALCULATING NORMAL VECTORS... ---------------------
                mov    ecx, [ esi + 12 ]        ;NFace
                mov    ebp, [ esi + 16 ]        ;Offset faces
                mov    edi, [ esi + 32 ]        ;Offset normalvectors
INormalV258:
                push   ecx

;-------- Making vector of v1
                mov    edx, [ ebp + 08 ]        ;Load v1
                mov    eax, edx
                shl    edx, 6                   ;64 bytes each vertice
                shl    eax, 2
                add    edx, eax
                add    edx, [ esi + 08 ]        ;Add offset vertices
                mov    eax, [ edx + 04 ]        ;Load X
                mov    ebx, [ edx + 08 ]        ;Load Y
                mov    ecx, [ edx + 12 ]        ;Load Z

                mov    edx, [ ebp + 04 ]        ;Load v0
                push   eax
                mov    eax, edx
                shl    edx, 6                   ;64 bytes each vertice
                shl    eax, 2
                add    edx, eax
                pop    eax
                add    edx, [ esi + 08 ]        ;Add offset vertices
                sub    eax, [ edx + 04 ]        ;Subtract X
                sub    ebx, [ edx + 08 ]        ;Subtract Y
                sub    ecx, [ edx + 12 ]        ;Subtract Z

                mov    V1X, eax
                mov    V1Y, ebx
                mov    V1Z, ecx

;-------- Making vector of v2
                mov    edx, [ ebp + 12 ]        ;Load v2
                mov    eax, edx
                shl    edx, 6                   ;64 bytes each vertice
                shl    eax, 2
                add    edx, eax
                add    edx, [ esi + 08 ]        ;Add offset vertices
                mov    eax, [ edx + 04 ]        ;Load X
                mov    ebx, [ edx + 08 ]        ;Load Y
                mov    ecx, [ edx + 12 ]        ;Load Z

                mov    edx, [ ebp + 04 ]        ;Load v0
                push   eax
                mov    eax, edx
                shl    edx, 6                   ;64 bytes each vertice
                shl    eax, 2
                add    edx, eax
                pop    eax
                add    edx, [ esi + 08 ]        ;Add offset vertices
                sub    eax, [ edx + 04 ]        ;Subtract X
                sub    ebx, [ edx + 08 ]        ;Subtract Y
                sub    ecx, [ edx + 12 ]        ;Subtract Z

                mov    V2X, eax
                mov    V2Y, ebx
                mov    V2Z, ecx

;-------- Calculating normalvector
                mov    eax, V1Y
                mov    edx, V2Z
                imul   edx
                mov    ecx, eax
                mov    eax, V1Z
                mov    edx, V2Y
                imul   edx
                sub    ecx, eax
                mov    [ edi + 04 ], ecx        ;Temporary save X

                mov    eax, V1Z
                mov    edx, V2X
                imul   edx
                mov    ecx, eax
                mov    eax, V1X
                mov    edx, V2Z
                imul   edx
                sub    ecx, eax
                mov    [ edi + 08 ], ecx        ;Temporary save Y

                mov    eax, V1X
                mov    edx, V2Y
                imul   edx
                mov    ecx, eax
                mov    eax, V1Y
                mov    edx, V2X
                imul   edx
                sub    ecx, eax
                mov    [ edi + 12 ], ecx        ;Temporary save Z

;------------ Calculating length of vector.
;(X^2) + (Y^2) + (Z^2)

                mov    eax, [ edi + 04 ]        ;Load X
                imul   eax
                mov    ebx, eax
                mov    eax, [ edi + 08 ]        ;Load Y
                imul   eax
                add    ebx, eax
                mov    eax, [ edi + 12 ]        ;Load Z
                imul   eax
                add    ebx, eax

                xor    ecx, ecx
Sqr1_258:
                inc    ecx
                mov    eax, ecx
                mul    eax

                cmp    eax, ebx
                jb     Sqr1_258
                                                ;ecx = |vector|

                mov    eax, 254 * 65536
                xor    edx, edx
                div    ecx
                mov    ecx, eax

                mov    eax, [ edi + 04 ]        ;Load X
                imul   ecx
                shrd   eax, edx, 16
                mov    [ edi + 04 ], eax        ;Save X

                mov    eax, [ edi + 08 ]        ;Load Y
                imul   ecx
                shrd   eax, edx, 16
                mov    [ edi + 08 ], eax        ;Save Y

                mov    eax, [ edi + 12 ]        ;Load Z
                imul   ecx
                shrd   eax, edx, 16
                mov    [ edi + 12 ], eax        ;Save Z

                mov    eax, [ edi + 04 ]        ;Load X
                imul   DWORD PTR [ edi + 08 ]   ;Constant vxy
                mov    [ edi ], eax             ;Save vxy

                pop    ecx

                add    ebp, 16                  ;Next face
                add    edi, 16
                dec    ecx
                jnz    INormalV258



;-------------------- COMBINING VECTORS FOR SMOOTHING ------------------------


                xor    edx, edx                 ;NVert


CNormalV258:
                push   edx

                xor    eax, eax                 ;Zero vectorcounters
                xor    ebx, ebx
                xor    ecx, ecx

                xor    edi, edi
                mov    ebp, [ esi + 16 ]        ;Offset Faces

ChFace258:

                cmp    [ ebp + 04 ], edx        ;Check v1
                je     AddV258                  ;Add vector
                cmp    [ ebp + 08 ], edx        ;Check v2
                je     AddV258
                cmp    [ ebp + 12 ], edx        ;Check v3
                je     AddV258
                jmp    NotAddV258

AddV258:
                push   edi

                shl    edi, 4                   ;16 bytes each nvector
                add    edi, [ esi + 32 ]

                add    eax, [ edi + 04 ]        ;Add X
                add    ebx, [ edi + 08 ]        ;Add Y
                add    ecx, [ edi + 12 ]        ;Add Z

                pop    edi

NotAddV258:
                add    ebp, 16
                inc    edi
                cmp    edi, [ esi + 12 ]
                jb     ChFace258

                mov    edi, edx
                shl    edi, 6
                lea    edi, [ edi + edx*4 ]
                add    edi, [ esi + 08 ]        ;Offset Vertices
                mov    [ edi + 28 ], eax        ;Temporary save vector
                mov    [ edi + 32 ], ebx        ;/
                mov    [ edi + 36 ], ecx        ;/



;------------ Calculating length of vector.
;(X^2) + (Y^2) + (Z^2)

                mov    eax, [ edi + 28 ]        ;Load X
                imul   eax
                mov    ebx, eax
                mov    eax, [ edi + 32 ]        ;Load Y
                imul   eax
                add    ebx, eax
                mov    eax, [ edi + 36 ]        ;Load Z
                imul   eax
                add    ebx, eax

                xor    ecx, ecx
Sqr2_258:
                inc    ecx
                mov    eax, ecx
                mul    eax

                cmp    eax, ebx
                jb     Sqr2_258
                                                ;ecx = |vector|

                mov    eax, 254 * 65536
                xor    edx, edx
                div    ecx
                mov    ecx, eax

                mov    eax, [ edi + 28 ]        ;Load X
                imul   ecx
                shrd   eax, edx, 16
                mov    [ edi + 28 ], eax        ;Save X

                mov    eax, [ edi + 32 ]        ;Load Y
                imul   ecx
                shrd   eax, edx, 16
                mov    [ edi + 32 ], eax        ;Save Y

                mov    eax, [ edi + 36 ]        ;Load Z
                imul   ecx
                shrd   eax, edx, 16
                mov    [ edi + 36 ], eax        ;Save Z

                mov    eax, [ edi + 28 ]        ;Load X
                imul   DWORD PTR [ edi + 32 ]   ;Constant vxy
                mov    [ edi + 24 ], eax        ;Save vxy

                pop    edx
                inc    edx
                cmp    edx, [ esi + 04 ]
                jb     CNormalV258

;Converting all vertices in facedefinitions to linear offsets to vertices

                mov    ecx, [ esi + 12 ]        ;NFace
                mov    edi, [ esi + 16 ]        ;Offset faces
                mov    ebx, [ esi + 08 ]        ;Offset vertices
CVert258:
                mov    eax, [ edi + 04 ]
                mov    edx, 68
                mul    edx
                add    eax, ebx
                mov    [ edi + 04 ], eax

                mov    eax, [ edi + 08 ]
                mov    edx, 68
                mul    edx
                add    eax, ebx
                mov    [ edi + 08 ], eax

                mov    eax, [ edi + 12 ]
                mov    edx, 68
                mul    edx
                add    eax, ebx
                mov    [ edi + 12 ], eax

                add    edi, 16
                dec    ecx
                jnz    CVert258

                pop    ebp                      ;ebp = bytes allocated

;---------------------------- NORMALVECTORS DONE... -------------------------

                clc                             ;No errors! :)
                ret                             ;Go home
LoadErr258:
                stc                             ;Error!
                ret
LoadV258      ENDP


;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : V258
;Type         : Procedure
;Last update  : 04.09.1996
;Action       : Rotates, projects and stores the faces into the part-tables
;Optimized    : Yes (Quit good)
;
;Registers changed : All
;
;Notes :
;----------------------------------------------------------------------------

V258          PROC

;-------------------------- INCREASING ANGLES... ----------------------------
                mov    esi, ObjectOffs

                xor    ebx, ebx
                mov    eax, [ esi + 60 ]        ;Load AddX
                imul   TimeDiff                 ;Implement timer
                mov    edx, [ esi + 48 ]        ;Load AngX (fixed point)
                add    edx, eax                 ;Add angX
                cmp    edx, 360*8*256           ;Test if exceeded limit
                setge  bl                       ;Possibly zero angle
                dec    ebx                      ;/
                and    edx, ebx                 ;/
                mov    [ esi + 48 ], edx
                shr    edx, 8
                mov    [ esi + 36 ], edx

                xor    ebx, ebx
                mov    eax, [ esi + 64 ]        ;Load AddY
                imul   TimeDiff
                mov    edx, [ esi + 52 ]        ;Load AngY (fixed point)
                add    edx, eax                 ;Add AngY
                cmp    edx, 360*8*256
                setge  bl
                dec    ebx
                and    edx, ebx
                mov    [ esi + 52 ], edx
                shr    edx, 8
                mov    [ esi + 40 ], edx

                xor    ebx, ebx
                mov    eax, [ esi + 68 ]        ;Load AddZ
                imul   TimeDiff
                mov    edx, [ esi + 56 ]
                add    edx, eax                 ;Add AngZ
                cmp    edx, 360*8*256
                setge  bl
                dec    ebx
                and    edx, ebx
                mov    [ esi + 56 ], edx
                shr    edx, 8
                mov    [ esi + 44 ], edx
;--------------------- OBJECT'S ROTATING CONSTANTS... ------------------------
                mov    edi, esi
                add    edi, 36                  ;Point to angles
                mov    esi, [ esi + 20 ]        ;Offset to constant-buffer

                                                ;esi = offset constant-buffer
                                                ;edi = offset angles (X, Y, Z)
                call   RConst6                  ;Calculate constants
                                                ;esi is unchanged

;------------------------- ROTATING COORDINATES... --------------------------

                mov    esi, ObjectOffs
                mov    ecx, [ esi + 04 ]        ;Load NVert
                mov    edi, [ esi + 08 ]        ;Offset Vertices
                mov    esi, [ esi + 20 ]        ;Offset CBuf

RVert258:
                push   ecx
                                                ;[ esi ] = coordinate
                                                ;[ edi ] = constant


;ROTATING COORDINATE...
;X-coordinate...
                mov    eax, [ edi + 04 ]        ;v:1 Load v[0] = X
                shl    eax, 8                   ;U:2
                mov    edx, [ edi + 08 ]        ;v:2 Load v[1] = Y
                shl    edx, 8                   ;U:3-4
                add    eax, [ esi + 04 ]        ;v:3-4
                add    edx, [ esi + 00 ]        ;U:5-6

                imul   edx                      ;U:7
                shrd   eax, edx, 8              ;U:?

                sub    eax, [ esi + 36 ]        ;U:1-2 - mxy0
                mov    ebx, eax                 ;U:3
                mov    eax, [ esi + 08 ]        ;U:4 Load m[2][0]

                imul   DWORD PTR [ edi + 12 ]   ;U:4 v[2] = Z
                add    ebx, eax                 ;U:1

;Y-coordinate...
                mov    edx, [ edi +  8 ]        ;v:1 Load v[1] = Y
                sal    edx, 8                   ;U:2
                mov    eax, [ edi +  4 ]        ;v:2 Load v[0] = X
                sal    eax, 8                   ;U:3-4
                add    edx, [ esi + 12 ]        ;U:3-4 m[0][1]
                add    eax, [ esi + 16 ]        ;v:5-6  m[1][1]
                imul   edx                      ;U:7

                shrd   eax, edx, 8              ;U:?
                sub    eax, [ esi + 40 ]        ;U:1 - mxy1
                mov    ecx, eax                 ;U:2
                mov    eax, [ esi + 20 ]        ;U:3 Load m[2][1]
                imul   DWORD PTR [ edi + 12 ]   ;U:4 v[2] = Z

                add    ecx, eax                 ;U:1

;Z-coordinate...
                mov    edx, [ edi +  8 ]        ;v:1 Load v[1] = Y
                sal    edx, 8                   ;U:2
                mov    eax, [ edi +  4 ]        ;v:2 Load v[0] = X
                sal    eax, 8                   ;U:3-4
                add    edx, [ esi + 24 ]        ;v:3-4 Load m[0][2]
                add    eax, [ esi + 28 ]        ;U:5-6 Load m[1][2]
                imul   edx                      ;U:7

                shrd   eax, edx, 8              ;U:?
                sub    eax, [ esi + 44 ]        ;U:1 - mxy2
                mov    ebp, eax                 ;U:2
                mov    eax, [ esi + 32 ]        ;U:3 Load m[2][2]
                imul   DWORD PTR [ edi + 12 ]   ;U:4 v[2] = Z

                sar    ebx, 8                   ;U:1 X-Fix up
                sar    ecx, 8                   ;U:2 Y-Fix up
                add    ebp, eax                 ;v:2 From rotateZ
                sar    ebp, 8                   ;U:3 Z-fixup
                mov    edx, ObjectOffs          ;v:3
                sub    ebx, [ edi + 00 ]        ;U:4-5 Subtract vxy from X
                sub    ecx, [ edi + 00 ]        ;v:4-5 Subtract vxy from Y
                sub    ebp, [ edi + 00 ]        ;U:6-7 Subtract vxy from Z

                add    ebx, [ edx + 72 ]        ;v:6-7 Add object's xpos
                add    ecx, [ edx + 76 ]        ;U:8-9 Add object's ypos
                add    ebp, [ edx + 80 ]        ;v:8-9 Add object's zpos

                                                ;ebx = rotated X
                                                ;ecx = rotated Y
                                                ;ebp = rotated Z

                or     ecx, 1                   ;U:10 Avoid div by zero
                mov    [ edi + 52 ], ebx        ;v:10 Save 3D-X
                mov    [ edi + 60 ], ebp        ;U:11 Save 3D-Z
                mov    [ edi + 56 ], ecx        ;v:11 Save 3D-Y

;----------------------------- DONE ROTATION ---------------------------------

                                                ;Projecting...

                mov    eax, ebx                 ;U:12 Load X
                mov    edx, ebx                 ;v:12 Load X for sign extend
                sal    eax, 8                   ;U:13 Distance...
                sar    edx, 31                  ;U:14 Sign extend

                idiv   ebp                      ;U:15 (X*256)/Z

                add    eax, 160                 ;U:1 Center 2D-X
                mov    edx, ecx                 ;v:1 Load 3D-Y for sign extend
                sar    edx, 31                  ;U:2 Sign extend
                mov    [ edi + 16 ], eax        ;v:2 Save 2D-X
                mov    eax, ecx                 ;U:3 Load 3D-Y
                sal    eax, 8                   ;U:4 Scale

                idiv   ebp                      ;U:5 (Y*256)/Z

                add    eax, 100                 ;Center
                mov    [ edi + 20 ], eax        ;Save 2D-Y


                push   edi
                add    edi, 24                  ;Point to normalvector
                Rotate6
                pop    edi
                mov    [ edi + 40 ], eax        ;Save rotated nv
                mov    [ edi + 44 ], ebx
                mov    [ edi + 48 ], ecx

;--------------------------- LIGHT CALCULATIONS ------------------------------
                mov    Col, 0
                SetLights258

;----------------------------- LIGHTS DONE -----------------------------------

                mov    eax, Col
;                shr    eax, 3                   ;64-color gradient
;                cmp    eax, 31
;                jb     ColOK258
;                mov    eax, 31
;ColOK258:
                mov    [ edi + 64 ], eax        ;Save color


DumpVert258:

                add    edi, 68

                pop    ecx
                dec    ecx
                jnz    RVert258                 ;Finish?


;--------------------------- SETTING UP FACES... -----------------------------
                mov    esi, ObjectOffs
                mov    edi, [ esi + 28 ]        ;Offset to FBuf
                mov    esi, [ esi + 16 ]        ;Offset to faces
                xor    ecx, ecx                 ;ecx = FaceCounter

Faces258:
                push   ecx
                                                ;Rotating normalvector...
                mov    ebx, ObjectOffs
                mov    ebp, [ ebx + 20 ]        ;Offset CBuf
                shl    ecx, 4                   ;16 bytes each nvector
                add    ecx, [ ebx + 32 ]        ;Offset NBuf

                mov    eax, [ ecx +  4 ]        ;Load v[0] = X
                sal    eax, 8
                add    eax, [ ebp + 28 ]        ;Add m[1][2]
                mov    edx, [ ecx +  8 ]        ;Load v[1] = Y
                sal    edx, 8
                add    edx, [ ebp + 24 ]        ;Add m[0][2]

                imul   edx
                shrd   eax, edx, 8
                sub    eax, [ ebp + 44 ]        ;Subtract mxy2
                mov    ebx, eax
                mov    eax, [ ebp + 32 ]        ;Load m[2][2]
                imul   DWORD PTR [ ecx + 12 ]
                add    ebx, eax                 ;
                sar    ebx, 8
                mov    eax, [ ecx ]             ;v:1 Load constant vxy
                sub    ebx, eax                 ;Subtract vxy0

                cmp    ebx, 20                   ;Polygon visible?
                jg     Dump258

                mov    edx, [ esi + 04 ]        ;Load Vertex0
                mov    ecx, [ edx + 60 ]        ;Load Z0

                cmp    ecx, 10                  ;Polygon behind user?
                jle    Dump258


                mov    eax, [ edx + 16 ]        ;Load X0
                mov    ebx, [ edx + 20 ]        ;Load Y0
                mov    edx, [ edx + 64 ]        ;Load color

                mov    [ edi + 24 ], eax        ;Save X0
                mov    [ edi + 28 ], ebx        ;Save Y0
                mov    [ edi + 12 ], edx        ;Save color


                mov    ZMin, ecx
                mov    XMin, eax                ;Initial max/min values
                mov    YMin, ebx                ;/

                mov    edx, [ esi + 08 ]        ;Load Vertex1 (placed here to
                                                ;avoid AGI
                mov    XMax, eax                ;Initial max/min values
                mov    YMax, ebx                ;/

                                                ;----- Second vertex...
                mov    ecx, [ edx + 60 ]        ;Load Z1

                cmp    ecx, 10                  ;Polygon behind user?
                jle    Dump258

                mov    eax, [ edx + 16 ]        ;Load X1
                mov    ebx, [ edx + 20 ]        ;Load Y1
                mov    edx, [ edx + 64 ]        ;Load color

                mov    [ edi + 32 ], eax        ;Save X1
                mov    [ edi + 36 ], ebx        ;Save Y1
                mov    [ edi + 16 ], edx        ;Save color

                cmp    eax, XMin                ;Get max/min
                jg     XMin1_258
                mov    XMin, eax
XMin1_258:
                cmp    eax, XMax
                jl     XMax1_258
                mov    XMax, eax
XMax1_258:

                mov    edx, [ esi + 12 ]        ;Load Vertex2 (placed here to
                                                ;avoid AGI

                cmp    ebx, YMin
                jg     YMin1_258
                mov    YMin, ebx
YMin1_258:
                cmp    ebx, YMax
                jl     YMax1_258
                mov    YMax, ebx
YMax1_258:
                cmp    ecx, ZMin
                jg     ZMin1_258
                mov    ZMin, ecx
ZMin1_258:
                                                ;----- Third vertex...

                mov    ecx, [ edx + 60 ]        ;Load Z2

                cmp    ecx, 10                  ;Polygon behind user?
                jle    Dump258

                mov    eax, [ edx + 16 ]        ;Load X2
                mov    ebx, [ edx + 20 ]        ;Load Y2
                mov    edx, [ edx + 64 ]        ;Load color

                mov    [ edi + 40 ], eax        ;Save X2
                mov    [ edi + 44 ], ebx        ;Save Y2
                mov    [ edi + 20 ], edx        ;Save color

                cmp    eax, XMin                ;Get max/min
                jg     XMin2_258
                mov    XMin, eax
XMin2_258:
                cmp    eax, XMax
                jl     XMax2_258
                mov    XMax, eax
XMax2_258:
                cmp    ebx, YMin
                jg     YMin2_258
                mov    YMin, ebx
YMin2_258:
                cmp    ebx, YMax
                jl     YMax2_258
                mov    YMax, ebx
YMax2_258:
                cmp    ecx, ZMin
                jg     ZMin2_258
                mov    ZMin, ecx
ZMin2_258:

                cmp    XMin, 320
                jge    Dump258
                cmp    XMax, 0
                jle    Dump258
                cmp    YMin, 200
                jge    Dump258
                cmp    YMax, 0
                jle    Dump258

                mov    eax, 0004                ;=gourad, with clipping
                                                ;Testing if clipping needed
                cmp    XMin, 0
                jl     Clip258

                cmp    XMax, 320
                jg     Clip258

                cmp    YMin, 0
                jl     Clip258

                cmp    YMax, 200
                jg     Clip258
                mov    eax, 0003                ;=gourad, no clipping

Clip258:

                mov    [ edi + 08 ], eax        ;Save Part-type

                mov    eax, ZMin                ;Load...
                mov    [ edi + 04 ], eax        ;...and save ZMin

                and    eax, 255
                mov    ebx, 255
                sub    ebx, eax

                mov    ecx, ListEnd1
                mov    eax, [ ebx*4 + ecx ]     ;Load offset to end
                mov    [ eax ], edi             ;Update link
                mov    [ ebx*4 + ecx ], edi     ;Update end-pointer

                add    edi, 48

Dump258:
                add    esi, 16

                mov    ebx, ObjectOffs
                pop    ecx
                inc    ecx
                cmp    ecx, [ ebx + 12 ]
                jb     Faces258

                ret
V258          ENDP
