
                .386P

_TEXT           SEGMENT         PUBLIC 'CODE' USE32
                ASSUME          cs:_TEXT

                PUBLIC          Start

        ;;  Here we are!
Start           PROC

        ;; Display stuff in the video RAM.
                mov     ebx, 0
                mov     edx, 0
                mov     ecx, 2000
top_of_loop:
                cmp     BYTE PTR [edx], 0
                jne     Skip
                mov     edx, 0
skip:
                mov     al, [edx]
                mov     es:[ebx], al
                inc     edx
                add     ebx, 2
                loop    top_of_loop

        ;; Stop the program by looping infinitely here. If we get
        ;; "KOS!" all over the screen, then it worked!
stop:
                jmp     stop
Start           ENDP

End_Code:

_TEXT           ENDS

        ;; ------------------------------------------------------------

_DATA           SEGMENT         PUBLIC 'DATA' USE32
                PUBLIC  Data_Start ; What is this symbol used for?
Data_Start:     
Message         DB      "KOS! ", 0
_DATA           ENDS

        ;; ------------------------------------------------------------

KOSStack        SEGMENT         PUBLIC 'STACK' USE32
                DB      256 DUP (?)
KOSStack        ENDS

                END

