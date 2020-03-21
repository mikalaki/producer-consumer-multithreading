/*
 *	File	: functions_threading_prod_cons
 *	Title	: Demo Producer/Consumer.
 *	Short	: Modify a	pthreads  solution to the producer consumer problem using
 *  multiple consumer and producers threads, and using functions thread functions as
 *  queue elemnts.
 *	Long 	:
 *	Author : Michael Karatzas
 *	Date  : 20 March 2020
 *	Revised	:
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define QUEUESIZE 5
#define LOOP 1000
#define PI 3.141592654
#define P 1
#define Q 1

struct workFunction {
  void * (*work)(void *);
  void * arg;
};

//Threads functions declaration.
void *producer (void *args);
void *consumer (void *args);

//declaration of the workFuctions that consist the workfuction array and they will feel the queue
void * PrintMyNumber(void * arg);
void * PrintMyCos(void * arg);
void * PrintMySin(void * arg);
void * PrintMycosSumSin(void * arg);
void * PrintMySqRoot(void * arg);

//Initialization of the workFunctions array
void * (* functions[5])(void *)= {&PrintMyNumber,&PrintMyCos ,&PrintMySin,&PrintMycosSumSin,&PrintMySqRoot};

//Arguments' Array
int arguments[10]={ 0    , 10   , 25   , 30 ,45  ,
                    60   , 75   , 90   ,100 ,120 };



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
void queueExec (queue *q);

int main ()
{
  queue *fifo; //queue declaration
  pthread_t producers[P], consumers[Q];//threads declaration

  fifo = queueInit (); //queue initialization
  if (fifo ==  NULL) {
    // fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }
  /* We first spawn the consumer threads, in order to be able to execute functions
  as soon as queue is not empty , with the help of conds and mutexes*/
  for (size_t i = 0; i < Q; i++)
    pthread_create (&consumers[i], NULL, consumer, fifo);
  for (size_t i = 0; i < P; i++)
    pthread_create (&producers[i], NULL, producer, fifo);

  for (size_t i = 0; i < Q; i++)
    pthread_join (consumers[i], NULL);
  for (size_t i = 0; i < P; i++)
    pthread_join (producers[i], NULL);

  queueDelete (fifo);

  return 0;
}

void *producer (void *q)
{
  queue *fifo; //pointer to a queue struct item
  int argIndex, funcIndex;

  fifo = (queue *)q; //casting

  while(1) {
    pthread_mutex_lock (fifo->mut); //locking
    while (fifo->full) {
      // printf ("producer: queue FULL.\n");
      pthread_cond_wait (fifo->notFull, fifo->mut); // when recieves the signal, it waits to lock the mut and continue
    }
    //void queueAdd (queue *q, struct workFunction in)

    // Creating the struct about the workFuction to add in the queue
    struct workFunction  func;

    //getting the random indexes of the workFunc and its arg that will be added to the queue
    argIndex=rand() % 10;
    funcIndex=rand() % 5;

    func.work = functions[funcIndex];
    func.arg = (void *) arguments[argIndex];

    //the function that adds a test function in the queue
    queueAdd (fifo, func);

    pthread_mutex_unlock (fifo->mut); //unlocking
    pthread_cond_signal (fifo->notEmpty); //signal in case cons thread is blocked
  }

  return (NULL);
}

void *consumer (void *q)
{
  queue *fifo;
  // int i;
  //struct workFunction  d;

  fifo = (queue *)q;

  while (1) {
    pthread_mutex_lock (fifo->mut);
    while (fifo->empty) {
      // printf ("consumer: queue EMPTY.\n");
      pthread_cond_wait (fifo->notEmpty, fifo->mut);
    }
    queueExec (fifo);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notFull);
    //printf ("consumer: recieved %d.\n", d);
    // usleep(200000);
  }
  return (NULL);
}

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

void queueAdd (queue *q, struct workFunction in)
{
  q->buf[q->tail] = in;
  q->tail++;
  if (q->tail == QUEUESIZE)
    q->tail = 0;
  if (q->tail == q->head)
    q->full = 1;
  q->empty = 0;

  return;
}

void queueExec (queue *q)
{
  //executing a work function
  struct workFunction out = q->buf[q->head];
  (out.work)(out.arg);

  q->head++;
  if (q->head == QUEUESIZE)
    q->head = 0;
  if (q->head == q->tail)
    q->empty = 1;
  q->full = 0;
  return;

}

//a Work function implementation ( definition)
void * func1(void * arg){
  int i = (int) arg;
  printf("I am function with i=%d \n", i);
}

//Definition of the workFuctions that will used in the queue
void * PrintMyNumber(void * arg){
  int number= (int) arg;
  printf("Hello world, I am number is %d !!\n", number);
}

void * PrintMyCos(void * arg){
  int number;
  float myCos;
  number= (int) arg;
  myCos=cos((float)(  (number* PI) / 180) );
  printf("I am number %d and my cosine is %f ! \n", number,myCos);
}

void * PrintMySin(void * arg){
  int number;
  float mySin;
  number= (int) arg;
  mySin=sin((float)(  (number* PI) / 180) );
  printf("I am number %d and my sine is %f ! \n", number,mySin);
}

void * PrintMycosSumSin(void * arg){
  int number;
  float mySin;
  float myCos;
  number= (int) arg;
  mySin=sin((float)(  (number* PI) / 180)  );
  myCos=cos((float)(  (number* PI) / 180)  );
  printf("I am number %d my cosine is %f, my sine is %f and their sum is %f ! \n", number,myCos,mySin, myCos+mySin);
}

void * PrintMySqRoot(void * arg){
  int number;
  float myRoot;
  number= (int) arg;
  myRoot= sqrt((float)number);
  printf("I am number %d and my square root is %f ! \n", number,myRoot);
}
