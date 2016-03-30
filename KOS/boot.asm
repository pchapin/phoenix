        COMMENT *
===========================================================================
FILE          : boot.asm
LAST REVISION : 2003-12-21
SUBJECT       : Bootstrap for the KOS operating system.
PROGRAMMER    : (C) Copyright 2003 by Peter C. Chapin

This file contains the KOS operating system as it is currently defined.
This system is not even remotely close to being complete. It does not
work. Running this program on your computer may cause damage. Don't
blame me.

Please send comments and bug reports (but no blame) to

     Peter C. Chapin
     Vermont Technical College
     Randolph Center, VT 05061
     Peter.Chapin@vtc.vsc.edu
===========================================================================
*

        .386P
        ;; KOS runs on a '386.
        ;; <-- Emacs likes to put two ';' here. I won't fight it.

        INCLUDE SEGMENT.INC

        ;;  Various symbols that look nice.
CR      EQU     13
LF      EQU     10

        ;; ------------------------------------------------------------

        ;; The GDT, (one for now) LDT, and IDT each reside in their own
        ;; segments.
GDT             SEGMENT         USE16

        ;; The first descriptor can't be used. Make it all zeros.
GDT_Slot0       DB      8 DUP (0)

        ;; The KOS (protected mode segments) Code, Data, Stack. These
        ;; are for the OS.

        ;; Code
        ;;   This segment has Base = 00??????h, Limit = 0????h Type = A
        ;;   (execute, ro), DT = 1 (memory), DPL = 0, P = 1, AVL = 0, D
        ;;   = 1, G = 0
GDT_Slot1       DB      000h, 00Fh, 000h, 000h, 000h, 09Ah, 040h, 000h

        ;; Data
        ;;   This segment has Base = 00??????h, Limit = 0????h Type = 2
        ;;   (r/w), DT = 1 (memory), DPL = 0, P = 1, AVL = 0, D = 1, G =
        ;;   0
GDT_Slot2       DB      000h, 000h, 000h, 000h, 000h, 092h, 040h, 000h

        ;; Stack
        ;;   This segment has Base = 00??????h, Limit = 0003FFh Type = 6
        ;;   (r/w, expand down), DT = 1 (memory), DPL = 0, P = 1, AVL =
        ;;   0, D = 1, G = 0
GDT_Slot3       DB      0FFh, 003h, 000h, 000h, 000h, 096h, 040h, 000h

        ;; I need a segment for video RAM if I expect to display
        ;; anything to the user.
        ;;   This segment has Base = 000B8000h, Limit = 00F9Fh Type = 2
        ;;   (r/w), DT = 1 (memory), DPL = 0, P = 1, AVL = 0, D = 1, G =
        ;;   0
GDT_Slot4       DB      09Fh, 00Fh, 000h, 080h, 00Bh, 092h, 040h, 000h

GDT             ENDS

        ;; ------------------------------------------------------------

        ;; The KOS bootstrap is an ordinary 16 bit DOS program. Once KOS
        ;; has initialized itself and is running from its own, protected
        ;; mode space the BootCode, BootData, and BootStack segments
        ;; will never be used again.
        ;;
BootCode        SEGMENT         USE16

                ASSUME  cs:BootCode, ds:BootData

        ;; The KOS bootstrap takes (over) control from DOS at the label 'Boot'
Boot            PROC

        ;; Set the segment registers.
                mov     ax, BootData
                mov     ds, ax
                mov     ax, GDT
                mov     es, ax

        ;; Display an opening message. Let the user know we are on our way.
                mov     ah, 09h
                mov     dx, OFFSET Banner
                int     21h

        ;; Okay... now what? Oh yeah... Set up the descriptor tables. A
        ;; lot of the initialization is hard coded (see below). I need
        ;; to come up with base addresses and limits for some of the
        ;; segments. DOS will help me.

        ;; Tell the user what we are about to do.
                mov     ah, 09h
                mov     dx, OFFSET Init_1
                int     21h

        ;; Let's start with the code segment.
                mov     bx, OFFSET GDT_Slot1
                mov     ecx, 0FFFFFFh ; Fix this limit eventually.
                xor     edx, edx
                mov     dx, _TEXT
                shl     edx, 4
                call    Set_BaseLimit

        ;; Okay, now how about the data segment.
                mov     bx, OFFSET GDT_Slot2
                mov     ecx, 0FFFFFFh ; Fix this limite eventually.
                xor     edx, edx
                mov     dx, _DATA
                shl     edx, 4
                call    Set_BaseLimit

        ;; Now stack...
                mov     bx, OFFSET GDT_Slot3
                mov     ecx, 0FFFFFFh
                xor     edx, edx
                call    Set_BaseLimit

        ;; Now I have to load the GDT register. This is annoying. First
        ;; I have to prepare a six byte data structure in memory to hold
        ;; the linear base address and limit values for the GDT segment.

                mov     dx, 8*5 - 1
                mov     GDT_Limit, dx
                xor     edx, edx
                mov     dx, GDT
                shl     edx, 4
                add     edx, OFFSET GDT_Slot0
                mov     GDT_Base, edx

                mov     edx, OFFSET Start
                mov     KOS_Entry, edx

        ;; Now I can load the GDT register.

                lgdt    PWORD PTR GDT_Descriptor

                ;; Tell the user we did it.
                mov     ah, 09h
                mov     dx, OFFSET Done_Message
                int     21h

                ;;  Now we are ready to switch to protected mode.
                mov     ah, 09h
                mov     dx, OFFSET Init_2
                int     21h

                ;; Disallow interrupts for now.
                cli
                mov     eax, cr0
                or      eax, 1
                mov     cr0, eax
                jmp     FAR PTR flush
flush:
                ;; (Re)set the segment registers.
                mov     ax, 0010h
                mov     ds, ax
                mov     fs, ax
                mov     gs, ax

                mov     ax, 0018h
                mov     ss, ax
                mov     esp, 0

                mov     ax, 0020h
                mov     es, ax

                jmp     FWORD PTR KOS_Entry
                ; jmp     FAR PTR Start

        ;; If we get here, something is wrong. Return to (Yuck) DOS.
                mov     ax, 4C01h
                int     21h
Boot            ENDP


        ;; +++++
        ;; Get_BaseLimit
        ;;
        ;; The following procedure looks up the Base address and Limit
        ;; fields of a given selector.
        ;;
        ;; ENTRY:  bx contains the selector. (Currently only GDT supported).
        ;; EXIT : ecx contains the limit.
        ;;        edx contains the base address.
        ;;
Get_BaseLimit   PROC
                push    ax
                push    bx
                push    es

        ;; Figure out the offset and segment address of the GDT.
                and     bx, 0FFF8h
                mov     ax, GDT
                mov     es, ax

        ;; Get the base address out of the descriptor.
                mov     dh, es:[bx+7]
                mov     dl, es:[bx+4]
                shl     edx, 16
                mov     dx, es:[bx+2]

        ;; Get the limit value out of the descriptor.
                xor     ecx, ecx
                mov     cl, es:[bx+6]
                and     cl, 0Fh
                shl     ecx, 16
                mov     cx, es:[bx+0]
                bt      WORD PTR es:[bx+6], 6
                jnc     skip_mult
                shl     ecx, 12

skip_mult:
                pop     es
                pop     bx
                pop     ax

                ret
Get_BaseLimit   ENDP


        ;; +++++
        ;; Set_BaseLimit
        ;;
        ;; The following procedure defines the Base address and Limit
        ;; fields of a given selector.
        ;;
        ;; ENTRY:  bx contains the selector. (Currently only GDT supported).
        ;;        ecx contains the limit. (Only 20 bit limits supported)
        ;;        edx contains the base address.
        ;; EXIT : Descriptor updated.
        ;;
Set_BaseLimit   PROC
                push    ax
                push    bx
                push    ecx
                push    edx
                push    es

        ;; Figure out the offset and segment address of the GDT.
                and     bx, 0FFF8h
                mov     ax, GDT
                mov     es, ax

        ;; Stash the base address into the descriptor.
                mov     es:[bx+2], dx
                shr     edx, 16
                mov     es:[bx+4], dl
                mov     es:[bx+7], dh

        ;; Stash the limit value into the descriptor.
                mov     es:[bx+0], cx
                shr     ecx, 16
                and     BYTE PTR es:[bx+6], 0F0h
                or      BYTE PTR es:[bx+6], cl

                pop     es
                pop     edx
                pop     ecx
                pop     bx
                pop     ax
                ret
Set_BaseLimit   ENDP

BootCode        ENDS


        ;; ------------------------------------------------------------

        ;;  Data used by the KOS bootstrap.
BootData        SEGMENT         USE16

Banner          DB      CR, LF
                DB      "***** KOS Loader *****", CR, LF
                DB      "WARNING: This software does not work!", CR, LF
                DB      "Send comments to: Peter.Chapin@vtc.vsc.edu", CR, LF
                DB      CR, LF, "Initializing KOS v0.0... ", "$"

Init_1          DB      CR, LF, "Initializing the GDT... ", "$"
Init_2          DB              "Jumping to protected mode... ", "$"

Done_Message    DB      "Done!", CR, LF, "$"

GDT_Descriptor  LABEL   PWORD
GDT_Limit       DW      0
GDT_Base        DD      0

KOS_Entry       DD      0
                DW      0008h

BootData        ENDS


        ;; ------------------------------------------------------------

        ;; The stack used by the KOS bootstrap.
BootStack       SEGMENT         'STACK' USE16
                DB      256 DUP (?)
BootStack       ENDS

                END     Boot

