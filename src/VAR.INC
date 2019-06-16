;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : VAR.INC
;Author          : Cyberfish of The Lost Souls
;Last update     : 23.10.96
;Action          : Common variables
;
;----------------------------------------------------------------------------

.386
.Model  FLAT

;----------------------- GENERAL OBJECT DEFINITIONS -------------------------

SortO         DD 0001                           ;Good old radix... :)
ISortO        DD 0002                           ;Initialize sorting arrays
RPartsO       DD 0003                           ;Draw'em!
ClearO        DD 0004, 0000                     ;Blank Canvas
DispO         DD 0005                           ;Display Canvas
InitLO        DD 0006                           ;Init lights each frame
TimerO        DD 0007                           ;Read timer, and update vars


;---------------------------- GLOBAL VARIABLES ------------------------------

                                                ;Main object's varibles
ObjectOffs    DD 0                              ;Offset to current object
LightOffs     DD 0                              ;Offset to current light
PluginOffs    DD 0

StartTime     DD 0000                           ;Timer variables
LastTime      DD 0000                           ;/
TimeDiff      DD 0000                           ;/
Time          DD 0000                           ;/

ListStart1    DD 0                              ;Pointers for sorting-arrays
ListEnd1      DD 0                              ;/
ListStart2    DD 0                              ;/
ListEnd2      DD 0                              ;/





                                                ;Common errormessages
LErrTable     DD OFFSET LoadErr0, OFFSET LoadErr1, OFFSET LoadErr2
              DD OFFSET LoadErr3, OFFSET LoadErr4, OFFSET LoadErr5

LoadErr0      DB 0Ah, 0Dh,'Error opening file.',       0Dh, 0Ah, '$'
LoadErr1      DB 0Ah, 0Dh,'Error moving filepointer.', 0Dh, 0Ah, '$'
LoadErr2      DB 0Ah, 0Dh,'Error reading file.',       0Dh, 0Ah, '$'
LoadErr3      DB 0Ah, 0Dh,'Error allocating memory. ', 0Dh, 0Ah, '$'
LoadErr4      DB 0Ah, 0Dh,'Error closing file.',       0Dh, 0Ah, '$'
LoadErr5      DB 0Ah, 0Dh,'Error wrong objecttype. ', 0Dh, 0Ah, '$'

EOL           DB 0Ah, 0Dh, '$'

                                                ;Variables for vectorobjects
FHandle       DW 0000
Info          DD 0
ObjectType    DD 0

V1X           DD 0                              ;Two vectors of each face
V1Y           DD 0
V1Z           DD 0
V2X           DD 0
V2Y           DD 0
V2Z           DD 0

XMin          DD 0
XMax          DD 0
YMin          DD 0
YMax          DD 0
ZMin          DD 0
Col           DD 0



                                                ;Variables for xTri-routines...
LRBuffer        DD 240*8 DUP (0)                ;Used by all

RSlope          DD 0                            ;Used by gourad64.inc
GSlope          DD 0
BSLope          DD 0
CSlope1         DD 0
CSlope2         DD 0

USlope          DD 0                            ;Used by TTri
VSlope          DD 0
XSlope          DD 0
MapOffset       DD 0


                                                ;Registers for realmode
                                                ;callback

Real_edi        DD 0
Real_esi        DD 0
Real_ebp        DD 0
                DD 0                            ;Reserved
Real_ebx        DD 0
Real_edx        DD 0
Real_ecx        DD 0
Real_eax        DD 0
Real_flags      DW 0
Real_es         DW 0
Real_ds         DW 0
Real_fs         DW 0
Real_gs         DW 0
Real_ip         DW 0
Real_cs         DW 0
Real_sp         DW 0
Real_ss         DW 0