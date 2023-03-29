#pragma once

size_t _strlen(unsigned char *s) {
    size_t i = 0;
    while (1) {
        if (s[i] == 0) {
            return i;
        }
        i++;
    }
}
