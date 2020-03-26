/*
 *	File	: prod-cons_multithreading
 *	Title	: Demo Producer/Consumer.
 *
 *	Short	: Modify a	pthreads solution to the producer consumer problem using
 *  multiple consumer and producers threads, and using thread functions as
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

#define QUEUESIZE 10
#define PI 3.141592654
#define LOOP 50000
#define N_OF_FUNCTIONS 5
#define N_OF_ARGS 10
// #define P 4
// #define Q 4
int P; // number of producer Threads
int Q; // number of consumer Threads

//Given struct for defining the workFuction our thread will execute.
struct workFunction {
  void * (*work)(void *);
  void * arg;
};



//Consumer and Producers Threads functions declaration.
void *producer (void *args);
void *consumer (void *args);

//declaration of the workFuctions that consist the fuctions array
void * PrintNumber(void * arg);
void * PrintCos(void * arg);
void * PrintSin(void * arg);
void * PrintCosSumSin(void * arg);
void * PrintSqRoot(void * arg);

//Initialization of the workFunctions' fuctions array
void * (* functions[N_OF_FUNCTIONS])(void *)= {&PrintNumber,&PrintCos ,&PrintSin,&PrintCosSumSin,&PrintSqRoot};

//Initialization of the workFunctions' arguments array
int arguments[N_OF_ARGS]={ 0    , 10   , 25   , 30 ,45  ,
                    60   , 75   , 90   ,100 ,120 };


/*The startTimes array, has the same size with the queue.It help us calculate the
waiting time of a workFunc in the queue.When a new item is stored in a queue position (index),
 the time is stored in the same position in the startTimesArray */
struct timeval startTimes[QUEUESIZE] ;

//struct timespec startTimes2[QUEUESIZE] ; //getting time is nanoseconds

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

//variable that helps get the termination condition
int terminationStatus;

int main (int argc, char* argv[])
{
  P=atoi(argv[1]);
  Q=atoi(argv[2]);

  queue *fifo; //queue declaration
  pthread_t producers[P], consumers[Q];//threads declaration

  //Initializing to zero the two global variables for the mean waiting-time calculations .
  functionsCounter=0;
  meanWaitingTime=0;


  terminationStatus=0;

  fifo = queueInit (); //queue initialization
  if (fifo ==  NULL) {
    fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }

  /* We first spawn the consumer threads, in order to be able to execute functions
  as soon as queue is not empty , with the help of conds and mutexes*/
  for (size_t i = 0; i < Q; i++)
    pthread_create (&consumers[i], NULL, consumer, fifo);

  //producers threads spawning
  for (size_t i = 0; i < P; i++)
    pthread_create (&producers[i], NULL, producer, fifo);

  //producers' threads joining
  for (size_t i = 0; i < P; i++)
    pthread_join (producers[i], NULL);

  //consumers' threads joining
  for (size_t i = 0; i < Q; i++)
    pthread_join (consumers[i], NULL);

  //queue deletion
  queueDelete (fifo);

  printf("\nPROGRAMM EXECUTION FINISHED. \n");
  printf("For P=%d, and Q=%d ,QUEUESIZE=%d the mean waiting-time is : %lf usec \n \n",P,Q,QUEUESIZE,meanWaitingTime);

  //Getting results in files
  //declaring files' pointers
  FILE *dataFile , *textFile;
  //open the files
  dataFile=fopen("data.csv","a");
  textFile=fopen("consolePrints.txt","a");
  //printing to files
  fprintf(dataFile,"%d,%d,%lf\n",P,Q,meanWaitingTime);
  fprintf(textFile,"For P=%d, and Q=%d ,QUEUESIZE=%d the mean waiting-time is : %lf usec \n ",P,Q,QUEUESIZE,meanWaitingTime,P,Q,meanWaitingTime);


  return 0;
}

//producers' threads function
void *producer (void *q)
{
  queue *fifo; //pointer to a queue struct item
  int argIndex, funcIndex;

  fifo = (queue *)q;

  //producers' loop
  for (size_t i = 0; i < LOOP; i++)
  {
    pthread_mutex_lock (fifo->mut);

    while (fifo->full) {
      printf ("producer: queue FULL.\n");
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
    /*When all the producers finished adding new workFunc items in the queue (terminationStatus==P)
     and the fifo queue is empty , the termination condition is met and the
     consumer Threads return.*/
    while (fifo->empty ) {
      printf ("consumer: queue EMPTY.\n");

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
    //The head value that corresponds to the queue position of the workFunc thread will execute
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

//Method that adds a new workFunc in the queue
void queueAdd (queue *q, struct workFunction in)
{
  //A new workFunc is added in the queue
  q->buf[q->tail] = in;

  // THE BEGINNING of the WAITING TIME is after the workFunction is added in the queue.
  gettimeofday(&startTimes[q->tail],NULL);
  //clock_gettime(CLOCK_MONOTONIC, &startTimes2[q->tail]);

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
  //variable to store the waiting time of the current workFunc
  long currWaitingTime =0 ;
  //long currWaitingTime2=0 ;

  //variable to get the time that workFunc is getting out of the queue( before execution)
  struct timeval endTime;
  //struct timespec endTime2;




  //The END of the waiting time , is the moment exactly before the function is executed.
  gettimeofday(&endTime,NULL);
  //clock_gettime(CLOCK_MONOTONIC, &endTime2);

  //calculating waiting time in microseconds.
  currWaitingTime= (endTime.tv_sec*1e6 -(startTimes[currHead] ).tv_sec*1e6);
  currWaitingTime+= (endTime.tv_usec-(startTimes[currHead] ).tv_usec);

  // //calculating waiting time in nanoseconds.
  // currWaitingTime2=(endTime2.tv_sec-(startTimes2[currHead ]).tv_sec) * 1e9  ;
  // currWaitingTime2+=(endTime2.tv_nsec-(startTimes2[currHead ]).tv_nsec  );

  //updating global variables that are used for calculating the mean waiting time.
  ++functionsCounter;

  //Updating the mean waiting time of a function value
  meanWaitingTime= (meanWaitingTime*(functionsCounter-1) + (double)currWaitingTime )/(functionsCounter) ;
  printf("\n \nThe waiting time of the current function is : %ld usec.\n",currWaitingTime );
  //printf("The waiting time of the current function is : %ld nsec.\n",currWaitingTime2 );
  printf("The mean waiting time of a function is : %lf usec.\n",meanWaitingTime );
  printf("functionsCounter : %ld \n ",functionsCounter );

  //Updating Head Value for the next consumer thread,before unlocking the mutex
  q->head++;
  if (q->head == QUEUESIZE)
    q->head = 0;
  if (q->head == q->tail)
    q->empty = 1;
  q->full = 0;


  //Unlocking the mutex
  pthread_mutex_unlock (q->mut);
  pthread_cond_signal (q->notFull);

  /*Executing the workFunc function after unlocking the mutex , leads to parallel
  execution of workFunc functions*/
  (workFunc.work)((workFunc.arg));

  return;

}



//Definition of the workFuctions that will used in the queue.
//These are just some simple functions that do mathematical computations.
void * PrintNumber(void * arg){
  int number= (int) arg;
  printf("Hello world, the arg number is %d !!\n", number);

}

void * PrintCos(void * arg){
  int number;
  float myCos;
  number= (int) arg;
  myCos=cos((float)(  (number* PI) / 180) );


  printf("The arg number is %d and its cosine is %f ! \n", number,myCos);
  //usleep(5000);
}

void * PrintSin(void * arg){
  int number;
  float mySin;
  number= (int) arg;
  mySin=sin((float)(  (number* PI) / 180) );
  printf("The arg number is %d and its sine is %f ! \n", number,mySin);
  //usleep(5000);
}

void * PrintCosSumSin(void * arg){
  int number;
  float mySin;
  float myCos;
  number= (int) arg;
  mySin=sin((float)(  (number* PI) / 180)  );
  myCos=cos((float)(  (number* PI) / 180)  );
  printf("The arg number is %d its cosine is %f, its sine is %f and their sum is %f ! \n", number,myCos,mySin, myCos+mySin);
  //usleep(5000);
}

void * PrintSqRoot(void * arg){
  int number;
  float myRoot;
  number= (int) arg;
  myRoot= sqrt((float)number);
  printf("The arg number is %d and its square root is %f ! \n", number,myRoot);
  //usleep(5000);
}