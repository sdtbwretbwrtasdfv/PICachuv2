
#include <windows.h>

#ifdef _WIN64
    #define PPEB_PTR __readgsqword( 0x60 )
#else
    #define PPEB_PTR __readfsdword( 0x30 )
#endif


#define WIN32_FUNC( x )     __typeof__( x ) * x
#define SEC( s, x )         __attribute__( ( section( "." #s "$" #x "" ) ) )
#define RVA_2_VA(T, B, R)   ( T )( ( PBYTE ) B + R )
#define GET_SYMBOL( x )     ( ULONG_PTR )( GetRIP( ) - ( ( ULONG_PTR ) & GetRIP - ( ULONG_PTR ) x ) )
#define C_PTR( x )          ( ( PVOID ) ( x ) )
#define U_PTR( x )          ( ( UINT_PTR ) ( x ) )


#define GET_SYMBOL_STR(name) (char*)GET_SYMBOL(name)
#define CHAR(name, symbol_name) char * name = GET_SYMBOL_STR(symbol_name)
