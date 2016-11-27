//
// Created by evgeny on 23.10.16.
//

//#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

int main(void)  {
    FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    stream = fopen("data/input.txt", "r");
    if (stream == NULL)
        exit(EXIT_FAILURE);
    while ((read = getline(&line, &len, stream)) != -1) {
        printf("Retrieved line of length %zu :\n", read);
        printf("%s", line);
    }
    free(line);
    fclose(stream);
    exit(EXIT_SUCCESS);
}