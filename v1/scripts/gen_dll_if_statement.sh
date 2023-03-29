#!/bin/bash

library_name="$1"
upper_name=$(echo "$library_name" | awk '{print toupper($0)}')
upper_name_no_ext="${upper_name%.*}"
upper_name_no_ext_underscored=$(echo "$upper_name_no_ext" | tr '.' '_')
lower_name=$(echo "$library_name" | awk '{print tolower($0)}')

cat << EOF
#ifdef H_LIB_${upper_name_no_ext_underscored}
	if (dll_hash == H_LIB_${upper_name_no_ext_underscored}) {
		char dll_name[] = { $(echo -n "$lower_name" | sed 's/\(.\)/'\''\1'\'', /g')0x00 };
		ptr_loaded_dll = (uint64_t)((tLoadLibraryA)fptr_loadLibary)(dll_name);
	}
#endif
EOF

