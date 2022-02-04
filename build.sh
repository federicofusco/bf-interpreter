#!/bin/bash
gcc src/debug.c src/log.c src/stack.c src/compiler.c src/main.c -lm -o bf