#pragma once

void * _memcpy (void *dst, const void *src, size_t n) {
    size_t i = 0;
    char *dst2 = dst;
    const char *src2 = src;
    for (i=0; i<n; i++) {
        dst2[i] = src2[i];
    }
    return dst;
}
