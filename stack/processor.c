//
// Created by evgeny on 23.10.16.
//

#include <string.h>
#include "stack.c"
#include <math.h>

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
const char * JE = "je";
const char * JNE = "jne";
const char * JA = "ja";
const char * JMP = "jmp";

const char * AX = "ax";
const char * BX = "bx";
const char * CX = "cx";
const char * DX = "dx";

const char * WRONG_ARG_NUMBER = "Incorrect arguments number!";

enum processorCommands {
    pop = 0x30,
    pushValue = 0x20,
    pushRegister = 0x10,
    add = 0x80,
    sub = 0x90,
    mul = 0xA0,
    division = 0xB0,
    Sin = 0x40,
    Cos = 0x50,
    Sqrt = 0x60,
    je = 0xF0,
    jne = 0xE0,
    ja = 0xD0,
    jmp = 0xC0
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

void CPU_pushRegister(struct CPU_t *cpu, int reg);

void CPU_popRegister(struct CPU_t *cpu, int reg);

int registerCode(char *reg);

void CPU_assemble(struct Buffer_t *buffer);

void CPU_evaluate(struct CPU_t *cpu);

void CPU_disassemble(struct CPU_t *cpu);

const char *Reg_disassemble(int reg);

bool isJump(char *command, struct Buffer_t *buffer);

int main() {
    struct CPU_t cpu;
    CPU_ctor(&cpu, 100, 100);

    CPU_assemble(&cpu.buffer);

    CPU_evaluate(&cpu);

    CPU_disassemble(&cpu);


    printf("%lg", Stack_pop(&cpu.stack));
//    for (int i = 0; i < cpu.buffer.currentPosition; i++) {
//        printf("%lg\n", cpu.buffer.data[i]);
//    }

    return 0;
}

void CPU_disassemble(struct CPU_t * cpu) {
    int i = 0;
    int reg = 0;
    FILE * outputFile = fopen("output.txt", "w");

    while (i < cpu -> buffer.currentPosition) {

        FILE *output = fopen("output.txt", "w");
        int current = (int) cpu -> buffer.data[i];
        int args_n = (current & 0b11000000) >> 6;
        int command_number = (current & 0b00110000) >> 4;

        switch (args_n) {
            case 0:
                switch (command_number) {
                    case 1:
                        reg = current & 0b00001111;
                        fprintf(outputFile, "%s %s\n", PUSH, Reg_disassemble(reg));
                        CPU_pushRegister(cpu, reg);
                        break;
                    case 3:
                        reg = current & 0b00001111;
                        fprintf(outputFile, "%s %s\n", POP, Reg_disassemble(reg));
                        break;
                    case 2:
                        i++;
                        fprintf(outputFile, "%s %lg\n", PUSH, cpu -> buffer.data[i]);
                        break;
                    default:
                        assert(WRONG_COMMAND);
                }
                break;
            case 1:
                switch(command_number) {
                    case 0:
                        fprintf(outputFile, "%s\n", SIN);
                        break;
                    case 1:
                        fprintf(outputFile, "%s\n", COS);
                        break;
                    case 2:
                        fprintf(outputFile, "%s\n", SQRT);
                        break;
                    default:
                        assert(WRONG_COMMAND);
                }
                break;
            case 2:
                switch(command_number) {
                    case 0:
                        fprintf(outputFile, "%s\n", ADD);
                        break;

                    case 1:
                        fprintf(outputFile, "%s\n", SUB);
                        break;

                    case 2:
                        fprintf(outputFile, "%s\n", MUL);
                        break;

                    case 3:
                        fprintf(outputFile, "%s\n", DIV);
                        break;

                    default:
                        assert(WRONG_COMMAND);
                }
                break;
            default:
                assert(WRONG_ARG_NUMBER);

        }

        i++;
    }
    fclose(outputFile);

}

const char * Reg_disassemble(int reg) {
    if (reg == ax) {
        return AX;
    }
    if (reg == bx) {
        return BX;
    }
    if (reg == cx) {
        return CX;
    }
    if (reg == dx) {
        return DX;
    }
    assert(WRONG_COMMAND);
}

void CPU_evaluate(struct CPU_t * cpu) {
    int i = 0;
    int reg = 0;
    double top = 0, top2 = 0, result = 0;

    int jump_remain = 0;

    while (i < cpu -> buffer.currentPosition) {
        int current = (int) cpu -> buffer.data[i];
        int args_n = (current & 0b11000000) >> 6;
        int command_number = (current & 0b00110000) >> 4;

        if (jump_remain == 0) {

            switch (args_n) {
                case 0:
                    switch (command_number) {
                        case 1:
                            reg = current & 0b00001111;
                            CPU_pushRegister(cpu, reg);
                            break;
                        case 3:
                            reg = current & 0b00001111;
                            CPU_popRegister(cpu, reg);
                            break;
                        case 2:
                            i++;
                            Stack_push(&cpu->stack, cpu->buffer.data[i]);
                            break;
                        default:
                            assert(WRONG_COMMAND);
                    }
                    break;
                case 1:
                    top = Stack_pop(&cpu->stack);
                    switch (command_number) {
                        case 0:
                            result = sin(top);
                            break;
                        case 1:
                            result = cos(top);
                            break;
                        case 2:
                            result = sqrt(top);
                            break;
                        default:
                            assert(WRONG_COMMAND);
                    }
                    Stack_push(&cpu->stack, result);
                    break;

                case 2:
                    top = Stack_pop(&cpu->stack);
                    top2 = Stack_pop(&cpu->stack);
                    switch (command_number) {
                        case 0:
                            result = top2 + top;
                            break;

                        case 1:
                            result = top2 - top;
                            break;

                        case 2:
                            result = top2 * top;
                            break;

                        case 3:
                            result = top2 / top;
                            break;

                        default:
                            assert(WRONG_COMMAND);
                    }
                    Stack_push(&cpu->stack, result);
                    break;

                case 3:
                    if (command_number > 0) {
                        top = Stack_pop(&cpu->stack);
                        top2 = Stack_pop(&cpu->stack);
                    }
                    bool make_jump = false;
                    switch (command_number) {
                        case 0:
                            make_jump = true;
                            break;
                        case 1:
                            if (top2 > top) {
                                make_jump = true;
                            }
                            break;
                        case 2:
                            if (top2 != top) {
                                make_jump = true;
                            }
                            break;
                        case 3:
                            if (top2 == top) {
                                make_jump = true;
                            }
                            break;
                        default:
                            assert(WRONG_COMMAND);
                    }

                    i++;
                    if (make_jump) {
                        jump_remain = (int) cpu->buffer.data[i];
                    }

                default:
                    assert(WRONG_ARG_NUMBER);

            }
        } else {
            if (args_n < 3) {
                i += args_n;
            } else {
                i += 1;
            }
            jump_remain--;
        }
        i++;
    }
}

void CPU_assemble(struct Buffer_t *buffer) {

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
        if (isJump(command, buffer)) {
            read = fscanf(input, "%lg", &value);
            Buffer_push(buffer, value);
        }
        interpret(buffer, command);

    }

    fclose(input);

}

bool isJump(char * command, struct Buffer_t *buffer) {
    if (!strcmp(command, JMP)) {
        Buffer_push(buffer, jmp);
        return true;
    }

    if (!strcmp(command, JA)) {
        Buffer_push(buffer, ja);
        return true;
    }

    if (!strcmp(command, JE)) {
        Buffer_push(buffer, je);
        return true;
    }

    if (!strcmp(command, JNE)) {
        Buffer_push(buffer, jne);
        return true;
    }
    return false;
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

void CPU_pushRegister(struct CPU_t *cpu, int reg) {
    if (reg == ax) {
        cpu -> ax = Stack_pop(&cpu -> stack);
    }
    if (reg == bx) {
        cpu -> bx = Stack_pop(&cpu -> stack);
    }
    if (reg == cx) {
        cpu -> cx = Stack_pop(&cpu -> stack);
    }
    if (reg == dx) {
        cpu -> dx = Stack_pop(&cpu -> stack);
    }
    assert(WRONG_COMMAND);
}

void CPU_popRegister(struct CPU_t * cpu, int reg) {
    if (reg == ax) {
        Stack_push(&cpu -> stack, cpu -> ax);
    }
    if (reg == bx) {
        Stack_push(&cpu -> stack, cpu -> bx);
    }
    if (reg == cx) {
        Stack_push(&cpu -> stack, cpu -> cx);
    }
    if (reg == dx) {
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
        Buffer_push(buffer, Cos);
        return;
    }
    if (!strcmp(command, SIN)) {
        Buffer_push(buffer, Sin);
        return;
    }
    if (!strcmp(command, SQRT)) {
        Buffer_push(buffer, Sqrt);
        return;
    }
    assert(WRONG_COMMAND);

}