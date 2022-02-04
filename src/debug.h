#ifndef DEBUG_H
#define DEBUG_H

#include "compiler.h"

void debug_cells(Object* object);
void debug_placeholder(int* n);
void debug_breakpoint(Object* object);

#endif