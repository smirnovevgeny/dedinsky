//
// Created by evgeny on 03.10.16.
//

#include <stdio.h>
#include "input.c"
#include <string.h>
#include <stdbool.h>

int COPY_SIZE = 2;

size_t myStrlen(const char *str) {
    size_t len = 0;
    while (*str != '\0') {
        str++;
        len ++;
    }
    return len;
}

char *myStrncpy(char *buffer, const char *str, size_t n) {
    int i = 0;
    for (i = 0; i < n && *str != '\0'; i++) {
        buffer[i] = *str;
        str++;
    }
    buffer[i] = '\0';
    return buffer;
}

char *myStrncat(char *dest, const char *src, size_t n) {
    size_t dest_len = myStrlen(dest);

    for (size_t i = 0; i < n  && src[i] != '\0'; i++) {
        dest[dest_len + i] = src[i];
    }
    dest[dest_len + n] = '\0';

    return dest;
}

char *myStrstr(const char *haystack, const char *needle) {
    if (myStrlen(haystack) < myStrlen(needle)) {
        return NULL;
    }
    char i, k, j = 0;
    for (i = 0; haystack[i] != '\0'; i++) {
        for (j = i, k = 0; needle[k] != '\0' && haystack[j] == needle[k]; j++, k++);
        if (k > 0 && needle[k] == '\0') {
            return (char *) (haystack + i);
        }
    }
    return NULL;
}

char *myStrtok(char *str, const char *delim) {
    const int NO_SYMBOL = -1;
    static char bufferStore[512], bufferOutput[512];
    if (str != NULL) {
        const size_t strLen = myStrlen(str);
        myStrncpy(bufferStore, str, strLen);
    }
    size_t j = 0;
    size_t first = NO_SYMBOL;
    while (bufferStore[j] != '\0') {
        size_t i = 0;
        for ( ; delim[i] != '\0' && delim[i] != bufferStore[j]; i++);
        if (delim[i] != '\0') {
            if (first == NO_SYMBOL) {
                first = j;
            }
        } else {
            if (first != NO_SYMBOL) {
                break;
            }
        }
        j++;
    }
    myStrncpy(bufferOutput, bufferStore, first);
    if (bufferStore[j] != '\0') {
        myStrncpy(bufferStore, bufferStore + j, 512);
    } else {
        if (j != 0) {
            bufferStore[0] = '\0';
        } else {
            return NULL;
        }
    }
    return bufferOutput;
}

int main() {
    char str[80] = "This is-.www.tutorialspoint.com - website";
    const char s[] = "-.";
    char *token;

    token = myStrtok(str, s);

    while( token != NULL )
    {
        printf("%s\n", token);

        token = myStrtok(NULL, s);
    }
    return 0;
}