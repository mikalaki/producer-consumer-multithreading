#!/bin/bash
#This bash script is used for compiling and benching the prod-cons_multithreading.c programm

#compiling
gcc prod-cons_multithreading.c -pthread -o program.out -lm -O3

#benching the program
for P in 1 2 4 6 8 10 12 16
do
  for Q in 1 2 4 6 8 10 12 16 32 64
  do
    ./program.out "$P"  "$Q"
  done
done
