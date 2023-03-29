#pragma once

#pragma GCC push_options
#pragma GCC optimize ("O0")
void* _memset(void * dst, int s, size_t count) {
    size_t i = 0;
    char *dst2 = dst;
    for (i=0; i<count; i++) {
        dst2[i] = s;
    }
    return dst;
}
#pragma GCC pop_options
