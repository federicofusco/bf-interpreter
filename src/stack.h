#ifndef STACK_H
#define STACK_H

typedef struct {

    int top;
    void** stack;
    size_t size;

} Stack;

Stack* create_stack(size_t size);
int is_stack_empty(Stack* stack);
int is_stack_full(Stack* stack);
int push_stack(Stack* stack,void* element);
void* pop_stack(Stack* stack);
void destroy_stack(Stack* stack);

#endif