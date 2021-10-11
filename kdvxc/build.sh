#!/bin/bash

if ! test -d bin
then
	mkdir bin
fi

gcc src/*.c -o bin/kdvxs.o -Llib -Iinc 
