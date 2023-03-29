$(shell mkdir -p Bin)

ProjectName 		:= $(shell basename `pwd`)

CCX64			= x86_64-w64-mingw32-gcc
CCX86			= i686-w64-mingw32-gcc

CFLAGS			=  -Os -fno-asynchronous-unwind-tables -nostdlib
CFLAGS 			+= -fno-ident -fpack-struct=8 -falign-functions=1
CFLAGS  			+= -s -ffunction-sections -falign-jumps=1 -w
CFLAGS			+= -falign-labels=1 -fPIC -Wl,-TScripts/Linker.ld
CFLAGS			+= -Wl,-s,--no-seh,--enable-stdcall-fixup

GREEN       := $(shell tput -Txterm setaf 2)
RESET       := $(shell tput -Txterm sgr0)

EXECUTABLE_X64	= Bin/$(ProjectName).x64.exe
RAWBINARY_X64	= Bin/$(ProjectName).x64.bin
EXECUTABLE_X86	= Bin/$(ProjectName).x86.exe
RAWBINARY_X86	= Bin/$(ProjectName).x86.bin

hash_random := $(shell /bin/bash -c 'echo $$((RANDOM % 90000 + 10000))')
#hash_random := 5382

all: prebuild x64 x86 tests

prebuild: clean
	@ Scripts/prebuild.sh $(hash_random)

x64: clean
	@ echo "$(GREEN)[*]$(RESET) Compile x64 executable..."
	@ nasm -f win64 Source/Asm/x64/asm.s -o Bin/asm.x64.o
	@ $(CCX64) Source/*.c Bin/asm.x64.o -DHASH_SALT='($(hash_random))' -o $(EXECUTABLE_X64) $(CFLAGS) $(LFLAGS) -IInclude -masm=intel
	@ echo "$(GREEN)[*]$(RESET) Extract shellcode: $(RAWBINARY_X64)"
	@ python3 Scripts/extract.py -f $(EXECUTABLE_X64) -o $(RAWBINARY_X64)
	@ rm $(EXECUTABLE_X64)

x86: clean
	@ echo "$(GREEN)[*]$(RESET) Compile x86 executable..."
	@ nasm -f win32 Source/Asm/x86/asm.s -o Bin/asm.x86.o
	@ $(CCX86) Source/*.c Bin/asm.x86.o -DHASH_SALT='($(hash_random))' -o $(EXECUTABLE_X86) $(CFLAGS) $(LFLAGS) -IInclude -masm=intel
	@ echo "$(GREEN)[*]$(RESET) Extract shellcode: $(RAWBINARY_X86)"
	@ python3 Scripts/extract.py -f $(EXECUTABLE_X86) -o $(RAWBINARY_X86)
	@ rm $(EXECUTABLE_X86)

tests: clean
	@ echo "$(GREEN)[*]$(RESET) Inlining x64 shellcode..."
	@ Scripts/bin2sc.sh -bin $(RAWBINARY_X64) -o $(basename $(RAWBINARY_X64))_inlined.exe -arch x64
	@ echo "$(GREEN)[*]$(RESET) Inlining x86 shellcode..."

	@ Scripts/bin2sc.sh -bin $(RAWBINARY_X86) -o $(basename $(RAWBINARY_X86))_inlined.exe -arch x86


clean:
	@ rm -rf Bin/*.o
	@ rm -rf Bin/*.bin
	@ rm -rf Bin/*.exe
