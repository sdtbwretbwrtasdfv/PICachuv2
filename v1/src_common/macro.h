#pragma once


/* Cast as a function / variable / string in a specific region of memory */
#define D_SEC( x )	__attribute__(( section( ".text$" #x ) ))

/* Cast as a pointer with the specified typedef */
#define D_API( x )	__typeof__( x ) * x

/* Cast as a pointer-wide integer */
#define U_PTR( x )	( ( ULONG_PTR) x )

/* Cast as a pointer */
#define C_PTR( x )	( ( PVOID ) x )

#define stack_char(str) ({ \
    const char *s = (str); \
    size_t len = sizeof(s); \
    char buf[len]; \
    _memcpy(buf, s, len); \
    buf[len - 1] = '\0'; \
    buf; \
})
