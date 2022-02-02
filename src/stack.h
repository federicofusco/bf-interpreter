#ifndef STACK_H
#define STACK_H

typedef struct {

    int top;
    char** stack;
    size_t size;

} Stack;

Stack* create_stack(size_t size);
int is_stack_empty(Stack* stack);
int is_stack_full(Stack* stack);
void push_stack(Stack* stack,char* element);
char* pop_stack(Stack* stack);

#endif