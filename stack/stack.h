//
// Created by evgeny on 23.10.16.
//

#ifndef STACK_STACK_H
#define STACK_STACK_H

#endif //STACK_STACK_H



int POISON = -666;

char * DUMP_PATH = "data/dump.txt";

struct Stack_t {
    int count, size;
    double *data;
};

void Stack_ctor(struct Stack_t * this, int size);

void Stack_dtor(struct Stack_t * this);

void Stack_push(struct Stack_t * this, double value);

double Stack_pop(struct Stack_t * this);

bool Stack_OK(struct Stack_t * this);

void Stack_dump(struct Stack_t * this);
