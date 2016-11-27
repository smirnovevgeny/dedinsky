//
// Created by evgeny on 10.10.16.
//

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include "stack.h"

#define MDEBUG

#ifdef MDEBUG
    #define ASSERT_OK(wat) { \
        if (!wat##_OK(this)) { \
            Stack_dump(this); \
            assert(false); \
        } \
    }
#else
    #define ASSERT_OK(wat) { }
#endif


bool Stack_OK(struct Stack_t * this) {
    return this &&
           this -> data &&
           this -> count >= 0 &&
           this -> count <= this -> size;
}

void Stack_ctor(struct Stack_t * this, int size) {

    assert(this && size >= 0);

    this -> data = (double *)malloc(size * sizeof(double));
    this -> count = 0;
    this -> size = size;
}

void Stack_dtor(struct Stack_t * this) {
    ASSERT_OK(Stack);
    free(this -> data);
}

void Stack_push(struct Stack_t * this, double value) {
    ASSERT_OK(Stack);
    this -> count++;
    if (this -> count > this -> size) {
        if (realloc(this,  2 * this -> size * sizeof(double))) {
            this -> size *= 2;
        }
    }
    this -> data[this -> count] = value;
    ASSERT_OK(Stack);
}

double Stack_pop(struct Stack_t * this) {
    ASSERT_OK(Stack);
    --this -> count;
    ASSERT_OK(Stack);
    return this -> data[this -> count];
}

void Stack_dump(struct Stack_t * this) {
    FILE *dumpFile = fopen(DUMP_PATH, "w+");

    fprintf(dumpFile, "Stack [%p] {\n", this);
    printf("\tdata[%d] = [%p] {\n", this -> size, this -> data);

    for (int i = 0; i < this -> size; i++) {
        fprintf(dumpFile, "\t\t");
        if (this -> data[i] != POISON) {
            fprintf(dumpFile, "* [%d] = %lg\n", i, this->data[i]);
        } else {
            fprintf(dumpFile, "  [%d] = %lg POISON!\n", i, this->data[i]);
        }
    }

    fprintf(dumpFile, "\t}\n");
    fprintf(dumpFile, "\tcount = %d\n", this -> count);
    fprintf(dumpFile, "}\n");

    fclose(dumpFile);

}

//int main() {
////    struct Stack_t stack;
////    Stack_ctor(&stack, 2);
//////    Stack_push(&stack,  15);
////    Stack_push(&stack,  148);
//////    ASSERT_OK(Stack);
////    printf("%f\n", Stack_pop(&stack));
////    printf("%f\n", Stack_pop(&stack));
//////    Stack_dtor(&stack);
//}