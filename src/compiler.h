typedef struct {

    char* source;
    size_t size;

} Object;

int validate (Object *object);
int compile(char *location,Object *object);