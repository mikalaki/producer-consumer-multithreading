/*
 *	File	: myFunctions.c
 *
 *	Description	: This file contains the definitions of the functions that are going
 *  to be used in the workFunction struct.
 *
 *	Author : Michael Karatzas
 *	Date  : March 2020
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "myFunctions.h"
#define FUNC_REPS 10

//Definition of the workFuctions that will used in the queue.

void * PrintNumber(void * arg){
  int number= (int) arg;
  printf("Hello world, the arg number is %d !!\n", number);

}

void * calcCos(void * arg){
  int number;
  float myCos;
  number= (int) arg;
  for(int i=1;i<=FUNC_REPS;i++)
    myCos=cos((float)(  (i*number* PI) / 180) );


  //printf("The arg number is %d and its cosine is %f ! \n", number,myCos);
  //usleep(5000);
}

void * calcSin(void * arg){
  int number;
  float mySin;
  number= (int) arg;
  for(int i=1;i<=FUNC_REPS;i++)
    mySin=sin((float)(  (i* number* PI) / 180) );
  //printf("The arg number is %d and its sine is %f ! \n", number,mySin);
  //usleep(5000);
}

void * calcCosSumSin(void * arg){
  int number;
  float mySin;
  float myCos;
  number= (int) arg;
  mySin=sin((float)(  (number* PI) / 180)  );
  myCos=cos((float)(  (number* PI) / 180)  );
  //printf("The arg number is %d its cosine is %f, its sine is %f and their sum is %f ! \n", number,myCos,mySin, myCos+mySin);
  //usleep(5000);
}

void * calcSqRoot(void * arg){
  int number;
  float myRoot;
  number= (int) arg;
  myRoot= sqrt((float)number);
  //printf("The arg number is %d and its square root is %f ! \n", number,myRoot);
  //usleep(5000);
}
