
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

size_t _strlen(const char *s) {
    size_t i = 0;
    while (1) {
        if (s[i] == 0) {
            return i;
        }
        i++;
    }
}

void * _memcpy (void *dst, const void *src, size_t n) {
    size_t i = 0;
    char *dst2 = dst;
    const char *src2 = src;
    for (i=0; i<n; i++) {
        dst2[i] = src2[i];
    }
    return dst;
}


int _strcmp (const char *s1, const char *s2) {
    int i;
    if (_strlen(s1) != _strlen(s2)) {
        return -1;
    }
    for(i=0; i<_strlen(s1); i++) {
        if (s1[i] != s2[i]) {
            return -1;
        }
    }
    return 0;
}

int _strncmp (const char *s1, const char *s2, size_t sz) {
    int i;
    for(i=0; i<sz; i++) {
        if (s1[i] != s2[i]) {
            return -1;
        }
    }
    return 0;
}
