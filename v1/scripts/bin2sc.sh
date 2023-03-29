#!/bin/bash

if test "$#" -ne 2; then
        echo "This is shellcode extraction script" && echo ""
	echo "Options:"
	echo "        -bin path_to_your.bin         (C shellcode from raw/bin file)"
	echo "        -exe path_to_pic.exe          (C shellcode from exe file)"
	echo "        -exe_to_bin path_to_pic.exe   (Create raw file from PE)"
	echo "        -bin_inline path_to_bin.bin   (Create exe file from bin by ASM inline)"

else
	if [ "$1" == "-bin" ]; then
		echo ".bin to shellcode"
        	path_to_bin=$2
        	shellcode=$(hexdump -v -e '"\\""x" 1/1 "%02x" ""' ${path_to_bin})
        	echo "char shellcode [] = \"$shellcode\";" >> $path_to_bin.h
	fi

        if [ "$1" == "-exe" ]; then
               echo ".exe to shellcode"
			path_to_exe=$2
			objcopy -O binary --only-section=.text $path_to_exe $(echo $path_to_exe | sed 's/.exe/.bin/')
			path_to_bin=$(echo $path_to_exe | sed 's/.exe/.bin/')
			shellcode=$(hexdump -v -e '"\\""x" 1/1 "%02x" ""' ${path_to_bin})
			echo "char shellcode [] = \"$shellcode\";" >> $path_to_bin.h
			rm $(echo $path_to_exe | sed 's/.exe/.bin/')
        fi
        if [ "$1" == "-exe_to_bin" ]; then
                echo ".exe to .bin"
		path_to_bin=$2
		objcopy -O binary --only-section=.text $path_to_bin $(echo $path_to_bin | sed 's/.exe/.bin/')
        fi

        if [ "$1" == "-bin_inline" ]; then
			path_to_bin=$2
			target=$path_to_bin

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
			x86_64-w64-mingw32-gcc tmp.c -o $(echo $target | sed 's/\..*/\.exe/')
			rm tmp.c
			rm tmp.py
        fi
fi
