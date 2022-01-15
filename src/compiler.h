typedef struct {

    char* source;
    char* current_instruction;
    size_t source_size;

    char* memory;
    char* cell;
    size_t memory_size;

    char** stack;
    char* pointer;
    size_t stack_size;

} Object;


int validate(Object *object);
int interpret(Object *object);
int compile(char *location,Object *object);