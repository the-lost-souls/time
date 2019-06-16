Vector1       DD 27 DUP (0)
              DD 779355

Vector2       DD 27 DUP (0)
              DD 844706

Vector3       DD 27 DUP (0)
              DD 64

Vector4       DD 27 DUP (0)
              DD 810697


BackO         DD 9
              DD 0

BackO2        DD 9
              DD 0

BackO3        DD 9
              DD 0

FadeO         DD 10
              DD 0

CrossO        DD 11
              DD 0                           ;work
              DD 0                           ;org1
              DD 0                           ;org2
              DD 0                           ;org3


TimingO       DD 08

Map1          DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 0                        ;Y-pos (3D)
              DD 00                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 0700  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0300  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 00 *256                       ;120 Local fixed point, X
              DD 0100  *256                       ;124 /Y
              DD 00 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 000                        ;144 Angle X-move
              DD 000                        ;148 /Y
              DD 0000                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z

Map2          DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD -3                        ;076 /Y
              DD 25                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 0200  *256                  ;088 /Y
              DD 0200  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 0100 *256                   ;120 Local fixed point, X
              DD 050  *256                   ;124 /Y
              DD -100 *256                   ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0020                        ;144 Angle X-move
              DD 0030                        ;148 /Y
              DD 0050                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z

Map3          DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0500  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD -150 *256                       ;120 Local fixed point, X
              DD 100  *256                       ;124 /Y
              DD 000 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0050                        ;144 Angle X-move
              DD 0030                        ;148 /Y
              DD 0010                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z

Map4          DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0500  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD -150 *256                       ;120 Local fixed point, X
              DD 050  *256                       ;124 /Y
              DD 0150 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0020                        ;144 Angle X-move
              DD 0010                        ;148 /Y
              DD 0070                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z

Map5          DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0500  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 0150 *256                   ;120 Local fixed point, X
              DD -100 *256                 ;124 /Y
              DD 0100 *256                    ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0050                        ;144 Angle X-move
              DD 0010                        ;148 /Y
              DD 0040                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z

Map6          DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0500  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 0150 *256                       ;120 Local fixed point, X
              DD -150  *256                       ;124 /Y
              DD -100 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0080                        ;144 Angle X-move
              DD 0030                        ;148 /Y
              DD 0010                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z



;-------------------------- OBJECTS FIRST PART... ---------------------------

Map7          DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0400  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD -100 *256                       ;120 Local fixed point, X
              DD 00  *256                       ;124 /Y
              DD 00 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0025                        ;144 Angle X-move
              DD 0020                        ;148 /Y
              DD 0025                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z

Map8          DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0400  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 100 *256                       ;120 Local fixed point, X
              DD 00  *256                       ;124 /Y
              DD 00 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0040                        ;144 Angle X-move
              DD 0005                        ;148 /Y
              DD 0010                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z

Map9          DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0400  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 00 *256                       ;120 Local fixed point, X
              DD -100  *256                       ;124 /Y
              DD 00 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0010                        ;144 Angle X-move
              DD 0025                        ;148 /Y
              DD 0030                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z

Map10         DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0400  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 00 *256                       ;120 Local fixed point, X
              DD 100  *256                       ;124 /Y
              DD 00 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0025                        ;144 Angle X-move
              DD 0040                        ;148 /Y
              DD 0010                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z

Map11         DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0400  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 00 *256                       ;120 Local fixed point, X
              DD 00  *256                       ;124 /Y
              DD -100 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0025                        ;144 Angle X-move
              DD 0025                        ;148 /Y
              DD 0040                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z

Map12         DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0400  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 00 *256                       ;120 Local fixed point, X
              DD 00  *256                       ;124 /Y
              DD 100 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0025                        ;144 Angle X-move
              DD 0025                        ;148 /Y
              DD 0010                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z


;-------------------------------- LIGHT VARIABLES ---------------------------

LightList     DD 0000                  ;Reserved...
              DD 0000                  ;Init for L001
              DD InitL002              ;Light2, angle-defined.
              DD 0000                  ;Init for L002
              DD InitL004              ;Light4, point-defined


Lights        DD Light1
              DD 0FFFFh
              DD 0FFFFh
              DD 0000

Light1        DD 0004                        ;00
              DD -200                        ;04 X
              DD 0                           ;08 Y
              DD 200                         ;12 Z
              DD 00                          ;16 Fixed point position, X
              DD 00                          ;20 /Y
              DD 00                          ;24 /Z
              DD 0                           ;28 Vectorcoordinates for vector
              DD 0                           ;32 from light to face.
              DD 0                           ;36
              DD 600                         ;40 Start fading range
              DD 800                        ;44 Maxrange of light (light=0)
              DD 5                           ;48 Decreasement in sector.

              DD 31                          ;52 Colors
              DD 0                           ;56
              DD 0                           ;60
              DD 32 DUP (0)                  ;64
              DD 64 DUP (0)
              DD 32 DUP (0)

              DD 0000                        ;576 Global X
              DD 0000                        ;580 /Y
              DD 0000                        ;584 /Z
              DD -15                        ;588 Global X-move
              DD 0000                        ;592 /Y
              DD 0                         ;596 /Z
              DD -200  *256                   ;600 Global fixed point, X
              DD 000  *256                   ;604 /Y
              DD 300  *256                  ;608 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 000                        ;116 /Z
              DD 00 *256                   ;120 Local fixed point, X
              DD 00 *256                   ;124 /Y
              DD 00 *256                   ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 000                        ;144 Angle X-move
              DD 000                        ;148 /Y
              DD 000                        ;152 /Z
              DD 000                        ;156 Angle fixed point, X
              DD 000                        ;160 /Y
              DD 0000                        ;164 /Z

Light2        DD 0004                        ;00
              DD -200                        ;04 X
              DD 0                           ;08 Y
              DD 200                         ;12 Z
              DD 00                          ;16 Fixed point position, X
              DD 00                          ;20 /Y
              DD 00                          ;24 /Z
              DD 0                           ;28 Vectorcoordinates for vector
              DD 0                           ;32 from light to face.
              DD 0                           ;36
              DD 600                         ;40 Start fading range
              DD 800                        ;44 Maxrange of light (light=0)
              DD 5                           ;48 Decreasement in sector.

              DD 0                          ;52 Colors
              DD 63                           ;56
              DD 0                           ;60
              DD 32 DUP (0)                  ;64
              DD 64 DUP (0)
              DD 32 DUP (0)

              DD 0000                        ;572 Global X
              DD 0000                        ;578 /Y
              DD 0000                        ;582 /Z
              DD 0015                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 200  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0400  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 00 *256                   ;120 Local fixed point, X
              DD 00 *256                   ;124 /Y
              DD 00 *256                   ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0025                        ;144 Angle X-move
              DD 0010                        ;148 /Y
              DD 004                        ;152 /Z
              DD 000                        ;156 Angle fixed point, X
              DD 000                        ;160 /Y
              DD 0000                        ;164 /Z


Light3        DD 0004                        ;00
              DD -200                        ;04 X
              DD 0                           ;08 Y
              DD 200                         ;12 Z
              DD 00                          ;16 Fixed point position, X
              DD 00                          ;20 /Y
              DD 00                          ;24 /Z
              DD 0                           ;28 Vectorcoordinates for vector
              DD 0                           ;32 from light to face.
              DD 0                           ;36
              DD 600                         ;40 Start fading range
              DD 800                        ;44 Maxrange of light (light=0)
              DD 5                           ;48 Decreasement in sector.

              DD 0                          ;52 Colors
              DD 0                           ;56
              DD 31                           ;60
              DD 32 DUP (0)                  ;64
              DD 64 DUP (0)
              DD 32 DUP (0)

              DD 0000                        ;572 Global X
              DD 0000                        ;578 /Y
              DD 0000                        ;582 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0500  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 00 *256                   ;120 Local fixed point, X
              DD 00 *256                   ;124 /Y
              DD 00 *256                   ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0010                        ;144 Angle X-move
              DD 0010                        ;148 /Y
              DD 0025                        ;152 /Z
              DD 000                        ;156 Angle fixed point, X
              DD 000                        ;160 /Y
              DD 0000                        ;164 /Z

Map13         DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 0                        ;Y-pos (3D)
              DD 00                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0500  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 00 *256                       ;120 Local fixed point, X
              DD 00  *256                       ;124 /Y
              DD 150 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0020                        ;144 Angle X-move
              DD 0020                        ;148 /Y
              DD 0010                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z

Map14         DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0500  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 150 *256                       ;120 Local fixed point, X
              DD 00  *256                       ;124 /Y
              DD -150 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0030                        ;144 Angle X-move
              DD 0030                        ;148 /Y
              DD 0010                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z

Map15         DD 129                         ;Object type
              DD 0                        ;X-pos (3D)
              DD 150                        ;Y-pos (3D)
              DD 200                         ;Z-pos (3D)

              DD 0000                        ;Offset to next part
              DD 0000                        ;Z-position (sorting...)
              DD 0007                        ;Part-type

              DD 0000                        ;X-pos (2D)
              DD 0000                        ;Y-pos (2D)
              DD 0000                        ;Pointer to map
              DD 64                         ;Map XSize
              DD 64                         ;Map YSize
              DD 320                          ;Map scaled X-size
              DD 200                          ;Map scaled X-size
              DD MapMove

              DD 0000                        ;060 Global X
              DD 0000                        ;064 /Y
              DD 0000                        ;068 /Z
              DD 0000                        ;072 Global X-move
              DD 0000                        ;076 /Y
              DD 0000                        ;080 /Z
              DD 000  *256                  ;084 Global fixed point, X
              DD 000  *256                  ;088 /Y
              DD 0500  *256                  ;092 /Z

              DD 0000                        ;096 Local X
              DD 0000                        ;100 /Y
              DD 0000                        ;104 /Z
              DD 0000                        ;108 Local X-move
              DD 0000                        ;112 /Y
              DD 0000                        ;116 /Z
              DD 150 *256                       ;120 Local fixed point, X
              DD -150  *256                       ;124 /Y
              DD 150 *256                       ;128 /Z

              DD 0000                        ;132 Angle X
              DD 0000                        ;136 /Y
              DD 0000                        ;140 /Z
              DD 0020                        ;144 Angle X-move
              DD 0040                        ;148 /Y
              DD 0010                        ;152 /Z
              DD 0000                        ;156 Angle fixed point, X
              DD 0000                        ;160 /Y
              DD 0000                        ;164 /Z


;------------------------------ 2D BUMP -------------------------------------

Bump          DD 128                         ;0  Object type
              DD 0000                        ;4  LightX
              DD 0000                        ;8  LightY
BumpPart      DD 0000                        ;12 Offset to next part
              DD 10000                       ;16 Z-value
              DD 0008                        ;20 Part-type
              DD 0000                        ;24 Pointer to map

ColorMap      DD 0
BumpTables    DD 0
BumpTables2   DD 0

