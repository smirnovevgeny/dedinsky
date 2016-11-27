//
// Created by evgeny on 23.10.16.
//

#include <string.h>
#include "stack.c"

char * INPUT_FILE = "data/input.txt";
int LONGEST_COMMAND = 4;
char * WRONG_COMMAND = "Wrong command!";

const char * POP = "pop";
const char * PUSH = "push";
const char * ADD = "add";
const char * SUB = "sub";
const char * MUL = "mul";
const char * DIV = "div";
const char * COS = "cos";
const char * SIN = "sin";
const char * SQRT = "sqrt";

const char * AX = "ax";
const char * BX = "bx";
const char * CX = "cx";
const char * DX = "dx";

enum processorCommands {
    pop = 0x30,
    pushValue = 0x10,
    pushRegister = 0x20,
    add = 0x80,
    sub = 0x90,
    mul = 0xA0,
    division = 0xB0,
    sin = 0x40,
    cos = 0x50,
    sqrt = 0x60
};

enum registerTypes {
    ax = 0x01,
    bx = 0x02,
    cx = 0x04,
    dx = 0x08
};


struct Buffer_t {
    double *data;
    int size, currentPosition;
};

void Buffer_ctor(struct Buffer_t * this, int size) {
    this -> data = malloc(sizeof(double) * size);
    this -> currentPosition = 0;
    this -> size = size;
};

void Buffer_push(struct Buffer_t * this, double value) {
    this -> data[this -> currentPosition ++] = value;
    if (this -> currentPosition > this -> size) {
        if (realloc(this,  2 * this -> size * sizeof(double))) {
            this -> size *= 2;
        }
    }
}

struct CPU_t {
    struct Stack_t stack;
    struct Buffer_t buffer;
    double ax, bx, cx, dx;
//    добавить pc, stackPointer, si, di
};

void CPU_ctor(struct CPU_t * this, int bufferSize, int stackSize) {
    Stack_ctor( &this -> stack, stackSize);
    Buffer_ctor(&this -> buffer, bufferSize);
}


void interpret(struct Buffer_t *buffer, char *command);

void CPU_pushRegister(struct CPU_t *cpu, char *reg);

void CPU_popRegister(struct CPU_t *cpu, char *reg);

int registerCode(char *reg);

void Buffer_read(struct Buffer_t *buffer);

int main() {
    struct CPU_t cpu;
    CPU_ctor(&cpu, 100, 100);

    Buffer_read(&cpu.buffer);

    for (int i = 0; i < cpu.buffer.size; i++) {
        int current = cpu.buffer.data[i];

    }


    for (int i = 0; i < cpu.buffer.currentPosition; i++) {
        printf("%lg\n", cpu.buffer.data[i]);
    }

    return 0;
}

void Buffer_read(struct Buffer_t *buffer) {

    FILE *input = fopen(INPUT_FILE, "r");
    char *command = malloc(sizeof(char) * LONGEST_COMMAND);

    double value = 0;
    int read = 0;
    char * reg = malloc(sizeof(char) * 2);

    while ((read = fscanf(input, "%s", command)) != -1) {
        if (!strcmp(command, PUSH)) {
            read = fscanf(input, "%lg", &value);
            if (read != 0) {
                Buffer_push(buffer, pushValue);
                Buffer_push(buffer, value);
            } else {
                read = fscanf(input, "%s", reg);
                Buffer_push(buffer, pushRegister + registerCode(reg));
            }
            continue;
        }
        if (!strcmp(command, POP)) {
            read = fscanf(input, "%s", reg);
            Buffer_push(buffer, pop + registerCode(reg));
            continue;
        }
        interpret(buffer, command);
    }


}

int registerCode(char *reg) {
    if (!strcmp(reg, AX)) {
        return ax;
    }
    if (!strcmp(reg, BX)) {
        return bx;
    }
    if (!strcmp(reg, CX)) {
        return cx;
    }
    if (!strcmp(reg, DX)) {
        return dx;
    }
    assert(WRONG_COMMAND);

}

void CPU_popRegister(struct CPU_t *cpu, char *reg) {
    if (!strcmp(reg, AX)) {
        cpu -> ax = Stack_pop(&cpu -> stack);
    }
    if (!strcmp(reg, BX)) {
        cpu -> bx = Stack_pop(&cpu -> stack);
    }
    if (!strcmp(reg, CX)) {
        cpu -> cx = Stack_pop(&cpu -> stack);
    }
    if (!strcmp(reg, DX)) {
        cpu -> dx = Stack_pop(&cpu -> stack);
    }
    assert(WRONG_COMMAND);
}

void CPU_pushRegister(struct CPU_t * cpu, char *reg) {
    if (!strcmp(reg, AX)) {
        Stack_push(&cpu -> stack, cpu -> ax);
    }
    if (!strcmp(reg, BX)) {
        Stack_push(&cpu -> stack, cpu -> bx);
    }
    if (!strcmp(reg, CX)) {
        Stack_push(&cpu -> stack, cpu -> cx);
    }
    if (!strcmp(reg, DX)) {
        Stack_push(&cpu -> stack, cpu -> dx);
    }
    assert(WRONG_COMMAND);
}


void interpret(struct Buffer_t *buffer, char *command) {

    if (!strcmp(command, ADD)) {
        Buffer_push(buffer, add);
        return;
    }
    if (!strcmp(command, MUL)) {
        Buffer_push(buffer, mul);
        return;
    }
    if (!strcmp(command, SUB)) {
        Buffer_push(buffer, sub);
        return;
    }
    if (!strcmp(command, DIV)) {
        Buffer_push(buffer, division);
        return;
    }
    if (!strcmp(command, COS)) {
        Buffer_push(buffer, cos);
        return;
    }
    if (!strcmp(command, SIN)) {
        Buffer_push(buffer, sin);
        return;
    }
    if (!strcmp(command, SQRT)) {
        Buffer_push(buffer, sqrt);
        return;
    }
    assert(WRONG_COMMAND);

}