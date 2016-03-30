;****************************************************************************
; FILE      : startup.asm
; SUBJECT   : startup code
; PROGRAMMER: (C) Copyright 2008  The Phoenix Team.
;
; This is used for startup of exe files.
;
; Please send comments or bug reports to
;
;    Phoenix Team
;    c/o Peter C. Chapin
;    Vermont Technical College
;    Williston, VT 05495
;    PChapin@vtc.vsc.edu
;****************************************************************************

EXTRN main_:PROC
public _cstart_
public inp_
public outp_
public _small_code_
DGROUP group _DATA,_BSS,STACK

extrn   _edata                  : byte  ; end of DATA (start of BSS)
extrn   _end                    : byte  ; end of BSS (start of STACK)

_TEXT    segment BYTE USE16 public 'CODE'

_cstart_ PROC
    ;initialize stack
    mov  ax, DGROUP
    mov  ss, ax
    mov  ax, OFFSET end_of_stack
    mov  sp, ax
    ;initialize data segment registers
    mov  ax, DGROUP
    mov  es, ax
    mov  ds, ax
    ;zero out _BSS segment
    mov     cx,offset DGROUP:_end   ; end of _BSS segment (start of STACK)
    mov     di,offset DGROUP:_edata ; start of _BSS segment
    sub     cx,di                   ; calc # of bytes in _BSS segment
    mov     al,0                    ; zero the _BSS segment
    rep     stosb                   ; . . .
    ;clear screen
    mov  ah, 0h
    mov  al, 3h
    int  10h
    call main_
_cstart_ ENDP

inp_     PROC
    push dx
    mov  dx, ax
    in   al, dx
    xor  ah, ah
    pop  dx
    ret
inp_     ENDP

outp_     PROC
    push dx
    xchg ax, dx
    out  dx, al
    pop  dx
    ret
outp_     ENDP
_TEXT   ends

_DATA    segment BYTE USE16 public 'CODE'
_small_code_ db 0
_DATA   ends

_BSS    segment word public 'BSS'
_BSS    ends

STACK_SIZE      equ     800h
STACK   segment para stack 'STACK'
        db      (STACK_SIZE) dup(?)
end_of_stack:
STACK   ends

END _cstart_