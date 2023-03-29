#!/bin/bash

file_winapis="winapis.defs"
file_hasher_script="Scripts/Hasher.py"

hashes_file="Include/Hashes.h"
core_file="Include/Core.h"
entry_file="Source/Entry.c"
GREEN='\033[0;32m'
RESET='\033[0m'

hash_random=$1
echo -e "${GREEN}[*]${RESET} Prebuilding using seed $hash_random"

function pre_clean(){
	rm $hashes_file
	rm $core_file
	rm $entry_file

}
function create_hashes(){
	echo '/* This file creates automaticly, do not change it manualy */' >> $hashes_file
	echo "/* seed1 : $hash_random */" >> $hashes_file
	for dll in $(cat $file_winapis| sed 's/->.*//' | sort | uniq); do
		echo $(python3 $file_hasher_script -t dll $dll -s $hash_random) >> $hashes_file;
	done
	for api in $(cat $file_winapis|sed 's/.*->//'| sed 's/<-.*//'); do
		echo $(python3 $file_hasher_script -t func $api -s $hash_random) >> $hashes_file;
	done
}
function create_core(){
	echo -e '/* This file creates automaticly, do not change it manualy */' >> $core_file
	echo -e '#include <Includes.h>' >> $core_file
	echo -e '#include <Macros.h>' >> $core_file
	echo -e '#include <Hashes.h>' >> $core_file
	echo -e 'UINT_PTR GetRIP( VOID );' >> $core_file
	echo -e '\ttypedef struct {' >> $core_file
	echo -e '\t\tstruct {' >> $core_file
	for api in $(cat $file_winapis|sed 's/.*->//'| sed 's/<-.*//'); do
		echo -e "\t\t\tWIN32_FUNC( $api );"  >> $core_file
	done
	echo -e '\t\t} Win32;' >> $core_file
	echo -e '\t\tstruct {' >> $core_file
	for dll in $(cat $file_winapis| sed 's/->.*//' | sort | uniq) ; do
		dllname=$(echo $dll | sed 's/.dll//' | awk '{ print toupper(substr($0,1,1)) tolower(substr($0,2)) }')
		echo -e "\t\t\tHMODULE $dllname;" >> $core_file
	done
	echo -e '\t\t} Modules;' >> $core_file
	echo -e '} INSTANCE, *PINSTANCE;' >> $core_file
}
function create_entry(){
	echo -e '/* This file creates automaticly, do not change it manualy */' >> $entry_file
	echo -e '#include <Core.h>'  >> $entry_file
	echo -e '#include <Win32.h>'  >> $entry_file
	echo -e 'SEC( text, B ) VOID Entry( LPCWSTR PasswdFromLoader ) {'  >> $entry_file
		echo -e '\tINSTANCE Instance = { };'  >> $entry_file
		echo -e '\tInstance.Modules.Kernel32   	= LdrModulePeb( H_LIB_KERNEL32 );'  >> $entry_file
		echo -e '\tInstance.Win32.LoadLibraryA 	= LdrFunction( Instance.Modules.Kernel32,  H_FUNC_LOADLIBRARYA);'  >> $entry_file
		for dll in $(cat $file_winapis| sed 's/->.*//' | sort | uniq); do
			dllname=$(echo $dll | sed 's/.dll//' | awk '{ print toupper(substr($0,1,1)) tolower(substr($0,2)) }')
			if [ "$dllname" != "Kernel32" ]; then
			    echo -e "\tInstance.Modules."$dllname" = Instance.Win32.LoadLibraryA( GET_SYMBOL( \"$dllname\" ) );"  >> $entry_file
			fi
			echo -e "\tif ( Instance.Modules."$dllname" != NULL ) {"  >> $entry_file
			for api in $(cat $file_winapis | grep $dll | sed 's/.*->//'| sed 's/<-.*//'); do
				func_hashname=$(cat $hashes_file | grep -i $api | sed 's/.*H_FUNC/H_FUNC/' | sed 's/ .*//')
				echo -e "\t\tInstance.Win32."$api" = LdrFunction( Instance.Modules."$dllname",  $func_hashname);"  >> $entry_file

			done
			echo -e '\t}'  >> $entry_file
		done
		echo -e '\tMain(Instance, PasswdFromLoader);'  >> $entry_file
		echo -e '\tInstance.Win32.ExitProcess(0);'  >> $entry_file
		echo -e "}"  >> $entry_file
}

pre_clean
create_hashes
create_core
create_entry
