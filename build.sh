#!/bin/bash

# Checks that the correct number of arguments has been passed
if [[ $# -ne 0 ]] 
then
	gcc src/stack.c src/main.c src/compiler.c src/log.c src/debug.c src/object.c -lm -o $1 -O5
else
	echo "Usage: ./build.sh [OUTPUT FILE]"
fi
