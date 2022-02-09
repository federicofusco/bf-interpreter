#ifndef COMPILER_H
#define COMPILER_H

#include "object.h"

extern size_t memory_size;

int compile ( char* script_location );
int validate ( Object* object );
int optimize ( Object* object );
int interpret ( Object* object );

#endif