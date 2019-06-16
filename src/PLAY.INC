;----------------------------- ASSEMBLER SOURCE -----------------------------
;Name            : PLAY.INC
;Author          : Cyberfish of The Lost Souls. (Featuring NMS)
;Last update     : 24.09.96
;Action          : Initializing procedures for NMS
;
;Notes : Link with nms.lib
;
;
;----------------------------------------------------------------------------

.386
;Linked with nms.lib

;External procedures:
              EXTRN NMS_Init            :NEAR
              EXTRN NMS_Exit            :NEAR
              EXTRN NMS_Driverinit      :NEAR
              EXTRN NMS_Driverexit      :NEAR
              EXTRN NMS_Moduleinit      :NEAR
              EXTRN NMS_Moduleexit      :NEAR
              EXTRN NMS_Modulestart     :NEAR
              EXTRN NMS_Modulestop      :NEAR
              EXTRN NMS_Autosetup       :NEAR
              EXTRN NMS_Manualsetup     :NEAR
              EXTRN NMS_Activatesync    :NEAR
              EXTRN NMS_Deactivatesync  :NEAR
	      EXTRN NMS_Setmastervol    :NEAR

;External errordata:
              EXTRN NMS_Error:dword
              EXTRN NMS_Errors:dword


;Synchronization variables. Very useful!
              EXTRN NMS_Jmp_Tick        :DWORD
              EXTRN NMS_Jmp_Row         :DWORD
              EXTRN NMS_Jmp_Cmd         :DWORD
              EXTRN NMS_Jmp_End         :DWORD
              EXTRN NMS_Dummy           :NEAR

              EXTRN NMS_ModulePos       :DWORD


;List for soundcard-supports...

              EXTRN GUS_CMDList :DWORD
	      EXTRN SB_CMDList  :DWORD
              EXTRN NS_CMDList  :DWORD
              EXTRN DSK_CMDList :DWORD

Soundcards    DD SB_CMDList
              DD GUS_CMDList
              DD NS_CMDList
              DD DSK_CMDList
              DD -1

;List for dos-extender supports...

	      EXTRN DOS32_CMDList :DWORD
              EXTRN DOS4G_CMDList :DWORD

Systems       DD DOS32_CMDList
              DD DOS4G_CMDList
              DD -1

;List for fileformat-supports...

              EXTRN MOD_CMDList :DWORD
              EXTRN S3M_CMDList :DWORD
              EXTRN DSM_CMDList :DWORD
              EXTRN XM_CMDList  :DWORD

Formats label DWORD
              DD XM_CMDList
              DD S3M_CMDList
              DD MOD_CMDList
              DD DSM_CMDList
	      DD -1


;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : InitPlayer
;Type         : Procedure
;Last update  : 24.09.96
;Action       : Autoinits dosextender and soundcard
;Optimized    : -
;
;
;Registers changed : All.
;
;Notes : NMS_DriverExit and NMS_Exit _MUST_ be called before terminating
;program, even if nosound initialized.
;----------------------------------------------------------------------------

InitPlayer    PROC
                mov    eax, OFFSET Systems      ;Init dos-extender
		call   NMS_Init
		jc     Init_Fail

                mov    eax, OFFSET Soundcards
		call   NMS_AutoSetup            ;Autodetect driver
		jc     Init_Fail

                call   NMS_DriverInit           ;Init driver
		jc     Init_Fail

		jmp    InitDone


Init_Fail:
                mov    edx,nms_error
                mov    edx,nms_errors[edx*4]
                mov    ah,9h
                int    21h

                pop    edx                      ;Dump ret-address
                mov    ax, 4C00h
                int    21h

InitDone:
                ret
InitPlayer    ENDP

;------------------------------- ASSEMBLER ROUTINE ---------------------------
;Name         : InitModule
;Type         : Procedure
;Last update  : 24.09.96
;Action       : Loads, identifies and initializes a module.
;Optimized    : -
;
;
;Input variables : edx = OFFSET FileName
;                  esi = Offset in file to read from
;                  ecx = Number of bytes to read (filesize). If zero then
;                        read to end of file.
;
;
;Notes : Terminates program and prints errormessage if initializing fails.
;----------------------------------------------------------------------------

ModulePtr       DD 0

ModErrTable     DD OFFSET ModError1, OFFSET ModError2, OFFSET ModError3
                DD OFFSET ModError4, OFFSET ModError5

ModError1       DB 0Dh, 0Ah, 'Error opening module'       , 0Dh, 0Ah, '$'
ModError2       DB 0Dh, 0Ah, 'Error allocating memory'    , 0Dh, 0Ah, '$'
ModError3       DB 0Dh, 0Ah, 'Error reading module'       , 0Dh, 0Ah, '$'
ModError4       DB 0Dh, 0Ah, 'Error closing file'         , 0Dh, 0Ah, '$'
ModError5       DB 0Dh, 0Ah, 'Error initializing module'  , 0Dh, 0Ah, '$'

InitModule    PROC
                mov    ah, 3Dh                  ;Open file
                mov    al, 0                    ;Read access
                int    21h
                jnc    ModOpenOK
                mov    eax, 0
                jmp    ModError
ModOpenOK:
                mov    FHandle, ax              ;Save filehandle

                or     ecx, ecx
                jnz    ModLoad

                xor    edx, edx                 ;Don't move!
                mov    ah, 42h                  ;Move filepointer
                mov    al, 2                    ;..from start of file
                mov    bx, FHandle              ;Load filehandle
                int    21h
                mov    ecx, eax                 ;FileSize
                sub    ecx, esi

ModLoad:
                mov    edx, ecx                 ;edx = blocksize
                mov    eax, 0EE42h              ;DOS32: Allocate memory block
                int    31h                      ;Returns: edx=offset

                jnc    ModMemOK
                mov    eax, 1
                jmp    ModError
ModMemOK:
                mov    ModulePtr, edx

;                call   PrintDec                 ;Bytes allocated
;                mov    edx, OFFSET ModAlloc
;                mov    ah, 09
;                int    21h                      ;Display text

                mov    edx, esi                 ;edx = FileOffset
                mov    ah, 42h                  ;Move filepointer
                mov    al, 0                    ;..from start of file
                mov    bx, FHandle              ;Load filehandle
                int    21h

                                                ;ecx = Bytes to read
                mov    ah, 3Fh                  ;Read from file
                mov    bx, FHandle              ;Load filehandle
                mov    edx, ModulePtr           ;Use canvas as temp-buffer
                int    21h
                jnc    ModReadOK
                mov    eax, 2
                jmp    ModError
ModReadOK:

                mov    ah, 3Eh                  ;Close filehandle
                mov    bx, FHandle              ;Load filehandle
                int    21h
                jnc    ModCloseOK
                mov    eax, 3
                jmp    ModError
ModCloseOK:

		mov	eax, OFFSET Formats
                mov     ebx, ModulePtr
                call    nms_moduleinit          ;Init module

		jnc	ModOK
                mov     eax, 4
                jmp     ModError
ModOK:
                ret                             ;Return OK.

ModError:
                call    nms_driverexit          ;Uninit driver
                call    nms_exit                ;Uninit nms

                mov     edx, ModErrTable[ eax*4 ];Display error message
                mov     ah, 09                  ;Write string
                int     21h

                pop     edx                     ;Dump ret address

                mov     ax, 4C00h
                int     21h
InitModule    ENDP
