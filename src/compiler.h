#ifndef COMPILER_H
#define COMPILER_H

#include "stack.h"

typedef struct {

    char* source;
    char* current_instruction;
    size_t source_size;

    unsigned char* memory;
    unsigned char* cell;
    size_t memory_size;

    Stack* stack;

} Object;

int validate(Object* object);
void optimize(Object* unoptimized_object);
void compile(char* location);
int interpret(Object* object);

#endif