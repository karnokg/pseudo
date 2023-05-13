#!/bin/sh

../pseudo /home/karnokg/Projects/pseudo/examplecodes/final/conditionals.pseudo 2> program.ll
clang-15 program.ll -g -O0 -o program.out ../externfunctions.cpp
lldb-15 ./program.out




