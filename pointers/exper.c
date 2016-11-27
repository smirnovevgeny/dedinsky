//
// Created by evgeny on 23.10.16.
//

//#define _GNU_SOURCE
#include <stdio.h>
//#include <stdlib.h>

const char *FILE_INPUT = "data/onegin.txt";

int main() {
    FILE *input_file;
    input_file = fopen(FILE_INPUT, "r");
    perror("null!!");
    fclose(input_file);
    return 0;

}