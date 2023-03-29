#!/bin/bash

function info(){
	msg=$1 && echo "" && echo -e "\033[1;32m[*]$msg\033[m"
}

export path_to_build="build"
if [ ! -d "$path_to_build" ]; then mkdir -p "$path_to_build"; fi


export path_to_scripts="scripts"
export path_to_common="src_common"
export path_to_loader="src_loader"
export path_to_main_pic="src_mainpic"

export path_to_asms=${path_to_common}"/asm"

export file_api_defines=${path_to_common}"/api_defines.h"
export file_api_hashes_deinition=${path_to_common}"/api_hashes.h"
export file_mainpic_entry=${path_to_main_pic}"/entry.c"
export file_loader_entry=${path_to_loader}"/loader.c"
export file_macro=${path_to_common}"/macro.h"
export file_common="common.h"
export file_winapis="winapis.txt"
export file_hasher_script=${path_to_scripts}"/hash.py"
export file_bin2sc=${path_to_scripts}"/bin2sc.sh"

export hash_random=$(echo $((RANDOM%90000+10000)))
export hash_seed="5"

export hash_lib_prolog="H_LIB_"
export hash_func_prolog="H_API_"
export x64_asm_compiller="nasm"
export x86_asm_compiller="nasm"
export x86_compiller="i686-w64-mingw32"
export x64_compiller="x86_64-w64-mingw32"
export x64_linker=${x64_compiller}-ld
export x86_linker=${x86_compiller}-ld
export x64_compiller_options=" -fno-builtin -fno-builtin-strlen -Wno-dangling-pointer -Wno-maybe-uninitialized -D_WIN64 -mwindows -Wno-int-to-pointer-cast -Wno-incompatible-pointer-types -s -m64 -Wl,--build-id=none -pipe -Wno-int-conversion -Wl,--gc-sections -Wall -DCOFF_STANDALONE -ffunction-sections -fno-asynchronous-unwind-tables -nostdlib -fno-ident -O2 -c -Wl,--no-seh -g2 "
export x86_compiller_options=" -fno-builtin -fno-builtin-strlen -Wno-dangling-pointer -Wno-maybe-uninitialized -mwindows -Wno-int-to-pointer-cast -Wno-incompatible-pointer-types -s -Wl,--build-id=none -pipe -Wno-int-conversion -Wl,--gc-sections -Wall -DCOFF_STANDALONE -ffunction-sections -fno-asynchronous-unwind-tables -nostdlib -fno-ident -O2 -c -Wl,--no-seh -g2 "

function temp_cleanup(){
	info "Cleanning..."
	rm $path_to_build/*.*
	if [ -f $file_api_hashes_deinition ]; then rm $file_api_hashes_deinition; fi
	if [ -f $file_mainpic_entry ]; then rm $file_mainpic_entry; fi
	if [ -f $file_api_defines ]; then rm $file_api_defines; fi
	if [ -f $path_to_loader/shellcode.h ]; then rm $path_to_loader/shellcode.h; fi
}
function postcompile_clean_up(){
	#temp_cleanup
	rm $path_to_build/*.o
	#rm $path_to_build/*.h

}
function create_api_hashes(){
	IFS=$OLDIFS
	info "Building API hashes file"
	echo "#pragma once"  >> $file_api_hashes_deinition
	echo "/* This file creates automaticly, do not change it manualy */"  >> $file_api_hashes_deinition
	echo "/* seed1 : $hash_random, seed2: $hash_seed*/"  >> $file_api_hashes_deinition
	for api in $(cat $file_winapis| sed 's/->.*//' | sort | uniq); do
		echo "#define $hash_lib_prolog"$(echo $api | tr '[:lower:]' '[:upper:]' | sed 's/\..*//' ) $(python3 $file_hasher_script $api $hash_random $hash_seed) >> $file_api_hashes_deinition;
	done
	for api in $(cat $file_winapis|sed 's/.*->//'| sed 's/<-.*//'); do
		initial_api=$(echo $api)
		echo "#define $hash_func_prolog"$(echo $api | tr '[:lower:]' '[:upper:]' | sed 's/.*->//' ) $(python3 $file_hasher_script $initial_api $hash_random $hash_seed) >>  $file_api_hashes_deinition;
	done
}
function create_api_defines(){
	IFS=$OLDIFS
	echo "/* This file creates automaticly, do not change it manualy */"  >> $file_api_defines
	echo -e "#pragma once" >> $file_api_defines
	echo -e "#include <stdint.h>" >> $file_api_defines
	echo -e "#include \"windows.h\"" >> $file_api_defines
	echo -e "#include \"wininet.h\"" >> $file_api_defines
	echo -e "#include \"$file_common\"" >> $file_api_defines
	IFS=$'\n'
	for api in $(cat $file_winapis | sort | uniq); do
		apiname=$(echo $api|sed 's/.*->//'|sed 's/<-.*//')
		type=$(echo $api| sed 's/.*<-//' | sed 's/<<.*//')
		params=$(echo $api| sed 's/.*<<//')
		echo "$type __cdecl _$apiname $params;" >> $file_api_defines;
	done
	for api in $(cat $file_winapis | sort | uniq); do
		apiname=$(echo $api|sed 's/.*->//'|sed 's/<-.*//')
		type=$(echo $api| sed 's/.*<-//' | sed 's/<<.*//')
		params=$(echo $api| sed 's/.*<<//')
		echo "typedef $type(WINAPI* t$apiname)$params;" >> $file_api_defines;
	done
}
function create_mainpic_entry(){
	IFS=$OLDIFS
	echo -e "#include <windows.h>">>$file_mainpic_entry
	echo -e "#include \"../src_common/common.h\"">>$file_mainpic_entry
	echo -e "#include \"main.h\"">>$file_mainpic_entry
	echo -e >>$file_mainpic_entry
	echo -e "typedef struct {">>$file_api_defines
	IFS=$'\n'
	for api in $(cat $file_winapis | sort | uniq); do
		apiname=$(echo $api|sed 's/.*->//'|sed 's/<-.*//')
		type=$(echo $api| sed 's/.*<-//' | sed 's/<<.*//')
		params=$(echo $api| sed 's/.*<<//')
		echo -e "\tD_API( _$apiname );">>$file_api_defines;
	done
	echo -e "} API ;">>$file_api_defines
	echo "">>$file_mainpic_entry
	echo "int Execute(LPCWSTR PasswdFromLoader){">>$file_mainpic_entry
	echo -e "\tAPI Api;">>$file_mainpic_entry
	echo -e "">>$file_mainpic_entry
	for api in $(cat $file_winapis | sort | uniq); do
		api_dll=$(echo $api|sed 's/->.*//' | tr '[:lower:]' '[:upper:]')
		api_name=$(echo $api|sed 's/.*->//'|sed 's/<-.*//')
		api_name_upper=$(echo $api|sed 's/.*->//'|sed 's/<-.*//' |  tr '[:lower:]' '[:upper:]')
		api_type=$(echo $api| sed 's/.*<-//' | sed 's/<<.*//')
		api_params=$(echo $api| sed 's/.*<<//');
		dll_hash_name=$(echo $hash_lib_prolog$api_dll | sed 's/\..*//')
          	api_hash_name=$(echo $hash_func_prolog$api_name_upper | sed 's/\..*//')
		echo -e "\tApi._$api_name = (t$api_name)getFunctionPtr($dll_hash_name, $api_hash_name);">>$file_mainpic_entry
	done
	echo -e "\tMmain(Api, PasswdFromLoader);">>$file_mainpic_entry
	echo -e "\treturn 0;">>$file_mainpic_entry
	echo -e "}">>$file_mainpic_entry
}
function build_asms(){
	IFS=$OLDIFS
	info "Build adjust-stack"
	$x64_asm_compiller -f win64 $path_to_asms/stack_x64.asm -o $path_to_build/stack_x64.o
	$x86_asm_compiller -f win32 $path_to_asms/stack_x86.asm -o $path_to_build/stack_x86.o

}
function build_api_resolvings(){
	IFS=$OLDIFS
	info "Build ApiResolve $arch"
	if [ "$arch" == "x64" ]; then
		${x64_compiller}-gcc ${x64_compiller_options} ${path_to_common}/api_resolve.c -DSALT="$hash_random" -c -o ${path_to_build}/api_resolve_x64.o
    	fi
	if [ "$arch" == "x86" ]; then
		${x86_compiller}-gcc ${x86_compiller_options} ${path_to_common}/api_resolve.c  -DSALT="$hash_random" -c -o ${path_to_build}/api_resolve_x86.o

	fi
}
function build_mainpic(){
	IFS=$OLDIFS
	info "Build PIC for arch: $arch"
	info "Build mainpic object"
	if [ "$arch" == "x64" ]; then
		${x64_compiller}-gcc ${x64_compiller_options} ${file_mainpic_entry} -c -o ${path_to_build}/mainpic_x64.o
    	fi
	if [ "$arch" == "x86" ]; then
		${x86_compiller}-gcc ${x86_compiller_options} ${file_mainpic_entry} -c -o ${path_to_build}/mainpic_x86.o

	fi
}
function link_pic(){
	IFS=$OLDIFS
	if [ "$arch" == "x64" ]; then
		 ${x64_linker} -s ${path_to_build}/*_x64.o -o ${path_to_build}/pic_x64.exe
	fi
	if [ "$arch" == "x86" ]; then
	 	 ${x86_linker} -s ${path_to_build}/*_x86.o -o ${path_to_build}/pic_x86.exe
	fi
}
function dump_shellcode(){
	${file_bin2sc} -exe ${path_to_build}/pic_${arch}.exe
	${file_bin2sc} -exe_to_bin ${path_to_build}/pic_${arch}.exe
	mv ${path_to_build}/pic_${arch}.exe ${path_to_build}/original_pic_${arch}.exe
	${file_bin2sc} -bin_inline ${path_to_build}/pic_${arch}.bin
	mv ${path_to_build}/pic_${arch}.exe ${path_to_build}/inlined_pic_${arch}.exe
}

function build_loader(){
	IFS=$OLDIFS
	mkdir ${path_to_build}/tmp
	mv ${path_to_build}/mainpic_*.o  ${path_to_build}/tmp/

	info "Compiling loader object arch: $arch"
	if [ "$arch" == "x64" ]; then
		cp ${path_to_build}/pic_${arch}.bin.h ${path_to_loader}/shellcode.h
		${x64_compiller}-gcc ${x64_compiller_options} ${file_loader_entry} -c -o ${path_to_build}/loader_x64.o
		rm ${path_to_loader}/shellcode.h
    	fi
	if [ "$arch" == "x86" ]; then
		cp ${path_to_build}/pic_${arch}.bin.h ${path_to_loader}/shellcode.h
		${x86_compiller}-gcc ${x86_compiller_options} ${file_loader_entry} -c -o ${path_to_build}/loader_x86.o
		rm ${path_to_loader}/shellcode.h

	fi
	if [ "$arch" == "x64" ]; then
		 ${x64_linker} -s ${path_to_build}/*_x64.o -o ${path_to_build}/loader_x64.exe
	fi
	if [ "$arch" == "x86" ]; then
	 	 ${x86_linker} -s ${path_to_build}/*_x86.o -o ${path_to_build}/loader_x86.exe
	fi
	mv ${path_to_build}/tmp/* ${path_to_build}/
	rm -rf ${path_to_build}/tmp/
}

function main(){
	OLDIFS=$IFS && IFS=$'\n'
	temp_cleanup
	create_api_hashes
	create_api_defines
	create_mainpic_entry
	build_asms
	arch="x64" && build_api_resolvings
	arch="x86" && build_api_resolvings
	arch="x64" && build_mainpic
	arch="x86" && build_mainpic
	arch="x64" && link_pic
	arch="x86" && link_pic
	arch="x64" && dump_shellcode
	arch="x86" && dump_shellcode
	#arch="x64" && build_loader
	#arch="x86" && build_loader
	postcompile_clean_up


}


if test "$#" -eq 1; then
	if [ $1 = "clean" ]
	then
		temp_cleanup
	fi
else
	main
fi

if [ -f $path_to_scripts/testings.sh ]; then $path_to_scripts/testings.sh; fi
