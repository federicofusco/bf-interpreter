#ifndef DEBUG_H
#define DEBUG_H

#include "object.h"

extern int debug_enabled;

void debug_cells ( Object* object );
void debug_placeholder ( int* n );
void debug_brackets ( Object* object );
void debug_breakpoint ( Object* object );

#endif