#include <Utils.h>
#include <Macros.h>


SEC( text, B ) UINT_PTR HashString_regular( LPVOID String, UINT_PTR Length) {
    ULONG	Hash  = HASH_SALT;
    PUCHAR Ptr  = String;
    do {
        UCHAR character = *Ptr;
        if ( ! Length ) {
            if ( !*Ptr ) break;
        } else {
            if ( (ULONG) ( Ptr - (PUCHAR)String ) >= Length ) break;
            if ( !*Ptr ) ++Ptr;
        }
        if ( character >= 'a' )
            character -= 0x20;
        Hash = ( ( Hash << 5 ) + Hash ) + character;
        ++Ptr;
    } while ( TRUE );
    return Hash;
}

SEC( text, B ) UINT_PTR HashString_unicode( wchar_t* str ) {
    int hash = HASH_SALT;
    wchar_t val;
    while (*str != 0) {
        val = *str;
        if (val >= 'a' && val <= 'z') {
            val -= 0x20;
        }
        hash = ((hash << 5) + hash) + val;
        str++;
    }
    return hash;
}
