/*
 *	File	: myFunctions.h
 *
 *	Description	: This file contains the declarations of the functions that are going
 *  to be used in the workFunction struct.
 *
 *	Author : Michael Karatzas
 *	Date  : March 2020
 *
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define PI 3.141592654
#define N_OF_FUNCTIONS 5

/* Declaration of the workFuctions that consist the fuctions array*/

//This function prints the integer value of the argument .
void * PrintNumber(void * arg);

//This function calculates 10 cosines
void * calcCos(void * arg);

//This function calculates 10 sines.
void * calcSin(void * arg);

//This function calculates the sum of argument's sine and cosine and prints it.
void * calcCosSumSin(void * arg);

//This function calculates argument's square root and prints it.
void * calcSqRoot(void * arg);





#endif
