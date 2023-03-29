#!/bin/bash
path_to_bin=$2
target=$path_to_bin
output=$4
arch=$6


cat << 'EOF' > tmp.py
#!/usr/bin/env python3
import sys

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("usage: %s file.bin\n" % (sys.argv[0],))
        sys.exit(0)
    shellcode = "\""
    ctr = 1
    maxlen = 1000000000000
    for b in open(sys.argv[1], "rb").read():
        shellcode += ",0x{:02x}".format(b)
        if ctr == maxlen:
            shellcode += "\" +\n\""
            ctr = 0
        ctr += 1
    shellcode += "\""
    print(shellcode)
EOF


asm_stub=$(python3 tmp.py $target  | sed 's/",/asm(prelog.byte /')
asm_stub2=$(echo "$asm_stub" | sed 's/"/prolog/')
asm_stub3=$(echo $asm_stub2 | sed 's/prolog/\\n\\t"/' | sed 's/prelog/"/')
echo "int main() {" >> tmp.c
echo "${asm_stub3}">> tmp.c
echo "\"ret\n\t\");">> tmp.c
echo "return 0;">> tmp.c
echo "}">> tmp.c

if [ "$arch" = "x64" ]; then
    x86_64-w64-mingw32-gcc -Os -s -ffunction-sections tmp.c -o "$output"
fi

if [ "$arch" = "x86" ]; then
	i686-w64-mingw32-gcc -Os -s -ffunction-sections tmp.c -o $output
fi

rm tmp.c
rm tmp.py
