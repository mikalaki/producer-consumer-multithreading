/*
 *	File	: prod-cons_multithreading.c
 *
 *	Description	: Modify a pthreads based solution to the producer consumer problem using
 *  multiple consumer and producer threads, and using thread functions as a FIFO
 *  queue elemnts.
 *
 *	Author : Michael Karatzas
 *	Date  : March 2020
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "myFunctions.h"

#define QUEUESIZE 10
#define LOOP 10000
// #define N_OF_FUNCTIONS 5
#define N_OF_ARGS 10
// #define P 4
// #define Q 4
int P; // number of producer Threads
int Q; // number of consumer Threads


FILE *allExecTimes;

//Given struct for defining the workFuction our thread will execute.
struct workFunction {
  void * (*work)(void *);
  void * arg;
};



//Consumer and Producers Threads functions declaration.
void *producer (void *args);
void *consumer (void *args);



//Initialization of the workFunctions' fuctions array
void * (* functions[N_OF_FUNCTIONS])(void *)= {&calc5thPower,&calcCos ,&calcSin,&calcCosSumSin,&calcSqRoot};

//Initialization of the workFunctions' arguments array
int arguments[N_OF_ARGS]={ 0    , 10   , 25   , 30 ,45  ,
                    60   , 75   , 90   ,100 ,120 };


/*The startTimes array, has the same size with the queue.It help us calculate the
waiting time of a workFunction in the queue.When a new item is stored in a queue position (index),
 the time is stored in the same position in the startTimesArray */
struct timeval startTimes[QUEUESIZE] ;

/*we get also time in nanoconds for bigger precision(nanosecond precision) where the execution time is
small (for instance 0.8 usec). Generaly we store the measurements from gettimeofday as we are asked,
but when waiting time is less than 5 microseconds we store the measurement fron clock_gettime
convereted in microseconds (usec)*/
struct timespec startTimes2[QUEUESIZE] ; //getting time is nanoseconds


//Struct defined for the queue implementation
typedef struct {
  struct workFunction buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
} queue;

//queue methods (functions) definition
queue *queueInit (void);
void queueDelete (queue *q);

void queueAdd (queue *q, struct workFunction in);
void queueExec ( queue *q,struct workFunction  workFunc,int currHead);

//variable that holds the number of total fuctions' executions by the consumers
long functionsCounter ;

//variable that hold the mean waiting-time of a function in the queue
double meanWaitingTime;
// long minWaitingTime; // these are computed in matlab
// long maxWaitingTime;

//variable that helps get the termination condition
int terminationStatus;

int main (int argc, char* argv[])
{



  P=atoi(argv[1]);
  Q=atoi(argv[2]);


  //Getting results in files
  //declaring files' pointers
  FILE  *dataFileMean, *dataFileMax , *dataFileMin , *textFile;


  char filename[sizeof "allTimesPXXX_QXXX.csv"];

  //Giving to the file the proper name
  sprintf(filename, "allTimesP%03d_Q%03d.csv", P,Q);

  //Open the file where all the function waiting times of the current execution are stored
  allExecTimes = fopen(filename,"a");



  queue *fifo; //queue declaration
  pthread_t producers[P], consumers[Q];//threads declaration

  //Initializing to zero the two global variables for the mean waiting-time calculations .
  functionsCounter=0;
  meanWaitingTime=0;

  // //Initialize max waiting time equal to zero.
  // maxWaitingTime=0;
  //
  // //Initialize min waiting time equal to zero.
  // minWaitingTime=INFINITY;


  terminationStatus=0;

  fifo = queueInit (); //queue initialization
  if (fifo ==  NULL) {
    fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }

  /* We first spawn the consumer threads, in order to be able to execute functions
  as soon as queue is not empty , with the help of conds and mutexes*/
  for (int i = 0; i < Q; i++)
    pthread_create (&consumers[i], NULL, consumer, fifo);

  //producers threads spawning
  for (int i = 0; i < P; i++)
    pthread_create (&producers[i], NULL, producer, fifo);

  //producers' threads joining
  for (int i = 0; i < P; i++)
    pthread_join (producers[i], NULL);

  //consumers' threads joining
  for (int i = 0; i < Q; i++)
    pthread_join (consumers[i], NULL);

  //queue deletion
  queueDelete (fifo);

  printf("\nPROGRAMM EXECUTION FINISHED. \n");

  //printf("For P=%d, and Q=%d ,QUEUESIZE=%d the min waiting-time is : %ld nsec \n \n",P,Q,QUEUESIZE,minWaitingTime);
  // printf("\nFor P=%d, and Q=%d ,QUEUESIZE=%d the min waiting-time is : %f usec \n",P,Q,QUEUESIZE,((float)minWaitingTime )/1000);
  // printf("For P=%d, and Q=%d ,QUEUESIZE=%d the max waiting-time is : %ld usec  \n",P,Q,QUEUESIZE,maxWaitingTime);
  printf("For P=%d, and Q=%d ,QUEUESIZE=%d the mean waiting-time is : %lf usec \n \n",P,Q,QUEUESIZE,meanWaitingTime);


  //open the files that store mean , min and max waiting time of all executions
  dataFileMean=fopen("dataΜΕΑΝ.csv","a");
  // dataFileMin=fopen("dataMIN.csv","a");
  // dataFileMax=fopen("dataMAX.csv","a");
  textFile=fopen("consolePrints.txt","a");

  //printing to files the mean , min and max waiting time of the executions
  fprintf(dataFileMean,"%d,%d,%lf\n",P,Q,meanWaitingTime);
  // fprintf(dataFileMin,"%d,%d,%lf\n",P,Q,((float)minWaitingTime )/1000);
  // fprintf(dataFileMax,"%d,%d,%ld\n",P,Q,maxWaitingTime);


  fprintf(textFile,"\nFor P=%d, and Q=%d ,QUEUESIZE=%d the mean waiting-time is : %lf usec \n ",P,Q,QUEUESIZE,meanWaitingTime);
  // fprintf(textFile,"For P=%d, and Q=%d ,QUEUESIZE=%d the min waiting-time is : %f usec \n ",P,Q,QUEUESIZE,((float)minWaitingTime )/1000);
  // fprintf(textFile,"For P=%d, and Q=%d ,QUEUESIZE=%d the max waiting-time is : %ld usec \n \n",P,Q,QUEUESIZE,maxWaitingTime);


  fclose(allExecTimes);
  fprintf(textFile,"FunctionsCounter: %ld \n ",functionsCounter);

  return 0;
}

//producers' threads function
void *producer (void *q)
{
  queue *fifo; //pointer to a queue struct item
  int argIndex, funcIndex;

  fifo = (queue *)q;

  //producers' loop
  for (int i = 0; i < LOOP; i++)
  {
    pthread_mutex_lock (fifo->mut);

    while (fifo->full) {
      //printf ("producer: queue FULL.\n");
      pthread_cond_wait (fifo->notFull, fifo->mut);
    }

    // Declaring the workFuction that is going to be added in the queue.
    struct workFunction  func;

    //getting the workFuctions' function and argument randomly.
    argIndex=rand() % N_OF_ARGS;
    funcIndex=rand() % N_OF_FUNCTIONS;


    func.work = functions[funcIndex];
    func.arg = (void *) arguments[argIndex];

    //the function that adds a test function in the queue
    queueAdd (fifo, func);

    //Check if we are in the last rep of producer
    if(i< LOOP-1){
      pthread_mutex_unlock (fifo->mut); //unlocking
      pthread_cond_signal (fifo->notEmpty); //signal in case cons thread is blocked
    }
    /*If we are in the last repetion of a producer in the loop, we update the terminationStatus value.
    If terminationStatus variable gets value equal to P (producers' number), means that all producers finished
    adding functions in the queue. */
    else{
      terminationStatus++; //updating terminationStatus
      pthread_mutex_unlock (fifo->mut);
      pthread_cond_signal (fifo->notEmpty);
    }
  }

  return (NULL);
}

//consumers' threads function
void *consumer (void *q)
{
  queue *fifo;
  fifo = (queue *)q;

  while (1) {

    pthread_mutex_lock (fifo->mut);
    /*When all the producers finished adding new workFunction items in the queue (terminationStatus==P)
     and the fifo queue is empty , the termination condition is met and the
     consumer Threads return.*/
    while (fifo->empty ) {
      //printf ("consumer: queue EMPTY.\n");

      //Termination condition,terminationStatus ==P means that all producers finished adding workFuncs in the queue.
      if(terminationStatus ==P){

        //Unlock the mutex before termination in order all the producers threads to terminate.
        pthread_mutex_unlock (fifo->mut);
        pthread_cond_signal (fifo->notEmpty);
        //printf("All produced functions are executed, producer's function,unlocks and returns. \n");
        return;
      }

      pthread_cond_wait (fifo->notEmpty, fifo->mut);

    }
    //variable to store the function the producer thread will execute, after unlocking
    struct workFunction execFunc;
    //The head value that corresponds to the queue position of the workFunction thread will execute
    long currHead;

    currHead=fifo->head;
    execFunc=fifo->buf[currHead];

    queueExec (fifo,execFunc,currHead);


  }
  return (NULL);
}

//Queue Initialization Function
queue *queueInit (void)
{
  queue *q;

  q = (queue *)malloc (sizeof (queue));
  if (q == NULL) return (NULL);

  q->empty = 1;
  q->full = 0;
  q->head = 0;
  q->tail = 0;
  q->mut = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
  pthread_mutex_init (q->mut, NULL);
  q->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notFull, NULL);
  q->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notEmpty, NULL);

  return (q);
}

//Queue Destroy Function
void queueDelete (queue *q)
{
  pthread_mutex_destroy (q->mut);
  free (q->mut);
  pthread_cond_destroy (q->notFull);
  free (q->notFull);
  pthread_cond_destroy (q->notEmpty);
  free (q->notEmpty);
  free (q);
}

//Method that adds a new workFunction in the queue
void queueAdd (queue *q, struct workFunction in)
{
  //A new workFunction is added in the queue
  q->buf[q->tail] = in;

  // THE BEGINNING of the WAITING TIME is after the workFunction is added in the queue.
  gettimeofday(&startTimes[q->tail],NULL);
  clock_gettime(CLOCK_MONOTONIC, &startTimes2[q->tail]);

  q->tail++;
  if (q->tail == QUEUESIZE)
    q->tail = 0;
  if (q->tail == q->head)
    q->full = 1;
  q->empty = 0;

  return;
}

void queueExec ( queue *q,struct workFunction  workFunc,int currHead)
{


  ////TIME CALCULATIONS/////
  //variable to store the waiting time of the current workFunc
  long currWaitingTime =0 ;
  long currWaitingTime2=0 ;

  //variable to get the time that workFunction is getting out of the queue( before execution)
  struct timeval endTime;
  struct timespec endTime2;




  //The END of the waiting time , is the moment exactly before the function is executed.
  gettimeofday(&endTime,NULL);
  clock_gettime(CLOCK_MONOTONIC, &endTime2);

  //calculating waiting time in microseconds.
  currWaitingTime= (endTime.tv_sec*1e6 -(startTimes[currHead] ).tv_sec*1e6);
  currWaitingTime+= (endTime.tv_usec-(startTimes[currHead] ).tv_usec);




  //calculating waiting time in nanoseconds.
  currWaitingTime2=(endTime2.tv_sec-(startTimes2[currHead ]).tv_sec) * 1e9  ;
  currWaitingTime2+=(endTime2.tv_nsec-(startTimes2[currHead ]).tv_nsec  );


  // //Check if the current waiting time is the min waiting time.
  // if(currWaitingTime2 < minWaitingTime){
  //   minWaitingTime=currWaitingTime2;
  //
  // }
  //
  // //Check if the current waiting time is the max waiting time.
  // if(currWaitingTime > maxWaitingTime){
  //   maxWaitingTime=currWaitingTime;
  //
  // }

  if(currWaitingTime!=0)
    fprintf(allExecTimes,"%ld\n ",currWaitingTime);
  else
    fprintf(allExecTimes, "%lf\n", ((float)currWaitingTime2)/1000);

  //updating global variables that are used for calculating the mean waiting time.
  ++functionsCounter;


  //Updating Head Value for the next consumer thread,before unlocking the mutex
  q->head++;
  if (q->head == QUEUESIZE)
    q->head = 0;
  if (q->head == q->tail)
    q->empty = 1;
  q->full = 0;

  //Updating the mean waiting time of a function value
  meanWaitingTime= (meanWaitingTime*(functionsCounter-1) + (double)currWaitingTime )/(functionsCounter) ;




  //Unlocking the mutex
  pthread_mutex_unlock (q->mut);
  pthread_cond_signal (q->notFull);

  /*Executing the workFunction function after unlocking the mutex , leads to parallel
  execution of workFunction functions*/
  (workFunc.work)((workFunc.arg));

  return;

}
