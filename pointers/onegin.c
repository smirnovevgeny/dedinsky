//
// Created by evgeny on 09.10.16.
//

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

const char *FILE_INPUT = "data/onegin.txt";
const char *FILE_OUTPUT = "data/output.txt";
const char *FILE_OUTPUT_REVERSED = "data/output_reversed.txt";


int lineCount(char *str, size_t n);


int myStrcmp(const char * a, const char * b, bool reversed);

void printToFile(const char *file_name, char *array[], size_t n);

int cstring_cmp(const void *a, const void *b);

int cstring_cmp_reversed(const void *a, const void *b);

char * readLines(const char *file_name, size_t *symbols_n);

size_t getSymbolsNumber(FILE *file);

void stringToList(char *string, char *list[], size_t n);

const char *strEnd(const char *string);

int main() {

    size_t symbols_n;
    char *input_string = readLines(FILE_INPUT, &symbols_n);

    int lines_number = lineCount(input_string, symbols_n);
    char *lines[lines_number];

    stringToList(input_string, lines, symbols_n);

    qsort(lines, (size_t) lines_number, sizeof(char *), cstring_cmp);
    printToFile(FILE_OUTPUT, lines, (size_t) lines_number);

    qsort(lines, (size_t) lines_number, sizeof(char *), cstring_cmp_reversed);
    printToFile(FILE_OUTPUT_REVERSED, lines, (size_t) lines_number);

    return 0;
}

void stringToList(char *string, char *list[], size_t n) {
    int current_line = 0;
    int start_position = 0;
    for (int symbol = 0; symbol < n; symbol++) {
        if (string[symbol] == '\n') {
            string[symbol] = '\0';
            list[current_line] = string + start_position;
            current_line++;
            start_position = symbol + 1;
        }
    }
}


char * readLines(const char *file_name, size_t * symbols_n) {
    FILE *input_file;
    input_file = fopen(file_name, "r");

    *symbols_n = getSymbolsNumber(input_file);

    char *input_string = malloc((size_t) (*symbols_n + 1)* sizeof(char));
    input_string[0] = '\0';
    input_string++;

    fread(input_string, sizeof(char), *symbols_n, input_file);
    fclose(input_file);
    return input_string;
}

size_t getSymbolsNumber(FILE *file) {
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    size_t symbols_n = file_size / sizeof(char);
    rewind(file);
    return symbols_n;
}

void printToFile(const char *file_name, char *array[], size_t n) {
    FILE *output = fopen(file_name, "w");
    for (int line_n = 0; line_n < n; line_n++) {
        fputs(array[line_n], output);
        fputc('\n', output);
    }
    fclose(output);
}


int lineCount(char *str, size_t n) {
    int lineNumber = 0;
    for (int i = 0; i < n && *str != '\0'; i++) {
        if (str[i] == '\n') {
            lineNumber++;
        }
    }
    return lineNumber;
}

void strReverse(char *str) {
    size_t len = strlen(str);
    char save;
    for (size_t i = 0; i < len / 2 ; i++) {
        save = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = save;
    }
}

int myStrcmp(const char * a, const char * b, bool reversed) {

    int diff = *a - *b;

    if (diff == 0) {
        if (*a == '\0') {
            return 0;
        } else {
            reversed ? --a, --b : ++a, ++b;
            return myStrcmp(a, b, reversed);
        }
    } else {
        int punct_a = ispunct(*a);
        int punct_b = ispunct(*b);

        if (punct_a) {
            reversed ? a-- : a++;
        }

        if (punct_b) {
            reversed ? b-- : b++;
        }

        if (punct_a ||  punct_b) {
            myStrcmp(a, b, reversed);
        } else {
            return diff;
        }
    }
}

int cstring_cmp(const void *a, const void *b)
{
    const char **element_a = (const char **)a;
    const char **element_b = (const char **)b;
    return myStrcmp(*element_a, *element_b, false);
}

int cstring_cmp_reversed(const void *a, const void *b)
{
    const char **element_a = (const char **)a;
    const char **element_b = (const char **)b;
    return myStrcmp(strEnd(*element_a), strEnd(*element_b), true);
}

const char *strEnd(const char *string) {
    return string + strlen(string) - 1;
}