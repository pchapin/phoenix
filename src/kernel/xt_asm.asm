        COMMENT *
===========================================================================
FILE          : xt_asm.asm
LAST REVISION : 2008-02-24
SUBJECT       : Implementation of thread creation function.
AUTHOR        : (C) Copyright 2008 by the Phoenix Contributors

Please send comments and bug reports to

     Phoenix Team
     c/o Peter C. Chapin
     Vermont Technical College
     Williston, VT 05495
     PChapin@vtc.vsc.edu
===========================================================================
*
        .MODEL SMALL
        .386
        PUBLIC create_process_asm_
        PUBLIC timer_ISR_asm_
        EXTRN schedule_:PROC
        .CODE

timer_ISR_asm_ PROC near
        push ax
        push cx
        push dx
        push bx
        push bp
        push si
        push di
        push ds
        push es

        mov ax, DGROUP
        mov ds, ax
        mov es, ax

        ;don't turn interrupt on
        mov dx, ss
        mov ax, sp
        call schedule_

        mov ss, dx
        mov sp, ax

        mov dx, 20h
        mov al, 20h
        out dx, al

        pop es
        pop ds
        pop di
        pop si
        pop bp
        pop bx
        pop dx
        pop cx
        pop ax
        iret
timer_ISR_asm_ ENDP


        ; This procedure takes two agruments:
        ;     Address to the start routine for the thread in cs:ax
        ;     Address to the botton of the thread's stack in ds:dx
        ;
        ; NOTE: This procedure assumes ss == ds and cs != ds.
        ;
create_process_asm_ PROC near
        push    bx        ; Preserve registers as per Open Watcom C compiler requirements.
        push    cx

        mov     bx, sp    ; Save stack pointer of caller.
        mov     sp, dx    ; Point sp at new thread stack.

        mov     cx, 202h  ; Initial flag register for thread.
        push    cx
        push    cs        ; Thread starting address in cs:ax
        push    ax

        mov     cx, 0     ; Initial value for GPRs. The weird order below matches OW.
        push    cx        ; ax
        push    cx        ; cx
        push    cx        ; dx
        push    cx        ; bx
        push    cx        ; bp
        push    cx        ; si
        push    cx        ; di

        mov     cx, ds    ; Thread uses same ds as creator.
        push    cx        ; ds
        push    cx        ; es

        mov     sp, bx    ; All done. Restore creator's stack pointer
        pop     cx        ; Restore creator's registers.
        pop     bx
        ret
create_process_asm_ ENDP

        END
