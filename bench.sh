#!/bin/bash

#This bash script is used for compiling and benching the prod-cons_multithreading.c programm

#compiling
gcc prod-cons_multithreading.c myFunctions.c -pthread -o program.out -lm -O3

#benching the program
for P in 1 2 4  8  16 32 64
do
  for Q in 1 2 4  8  16 32 64 128 256
  do
    ./program.out "$P"  "$Q"
  done
done
