#!/usr/bin/env bash
echo "Compiling"
gcc -Wall -Wextra -o main main.c -I. -DENABLE_MSG -g

if [[ $? != 0 ]]; then
	echo "Compilation Failed"
else
	echo "Compiled Successfully"
fi
