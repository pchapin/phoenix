
;
; This file defines the various segments in use by this program.
;

_TEXT           SEGMENT         PUBLIC 'CODE' USE32
                EXTRN           Start:PROC
_TEXT           ENDS

_DATA           SEGMENT         PUBLIC 'DATA' USE32
                EXTRN           Data_Start:BYTE
_DATA           ENDS

DGROUP          GROUP           _DATA

KOSStack        SEGMENT         PUBLIC 'STACK' USE32
KOSStack        ENDS

