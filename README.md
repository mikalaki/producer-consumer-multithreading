# producer-consumer-multithreading
This repo is about the first assigment on the real-time embedded systems class . We are called to modify a solution of producer consumer problem,which use threads , in order to be able to support multiple producer and multiple consumer threads, and having functions as queue items. The main programm of the assigment is prod-cons_multithreading.c <br>
**Statististis about the waiting time , are stored in stats folder**
## Compiling and Execution of the Program
### Execution Method 1
For executing the program you have first to compile it ,by executing the command bellow inside the project folder :<br>
`gcc prod-cons_multithreading.c myFunctions.c -pthread -o program.out -lm -O3`<br>
after compilation finish you are able to execute the programm with the following command<br>
`./program.out p q` , where **p is the number of producer threads** you want to have and **q the number of consumers**,for instance if you want to have 4 producer threads and 4 consumers threads you have to run the command:<br>
`./program.out 4 4`

### Execution Method 2-Benchmarking
If you want to run the programm for a compination of **p** and **q** values, you can edit the *bench.sh* file and set the p and q values , for which you want to run the programm, in the two for loops . After editing the file , by running :<br>
`./bench.sh` <br>
in the terminal , the programm gets compiled and execute for the parameters you set.

# Project Description,<br>
## **(!!!BELLOW IS NOT THE REPORT FOR THE UNIVERSITY ASSIGMENT, just a rοugh summary in English!!!)**<br>
**The report is uploaded on elearning platform. A copy off it is the [report.pdf](report.pdf)  **
## Goal  
 In this project , we are called to modify a producer-consumer solution that uses pthreads. The original program spawns one producer and one consumer thread , the original programm's code is locate in the prod-cons_default.c file . The purpose is the new program to be executed by **p** producer threads and **q** consumer threads. In the queue , the producer threads will add functions , using the `struct workFunction` in a FIFO queue and the role of the consumers threads is to execute these functions in parallel.
 The `struct workFunction` is defined as : <br> 
```
struct workFunction { 
  void * (*work)(void *);
  void * arg;  
}
   ```
   
   <br>
   
   Furthermore we are asked to take measurements about the average waiting time of a workFunction item in the FIFO queue ,   and find for what number of consumers **q** , we get the smallest one . 
   ## Measurements 
   Here are the tables with my measurements : <br>
   **Measurements for QUEUESIZE =10 , time is in microseconds (usec) :**
   ![measurements for queuesize=10](/stats/QUEUESIZE10/meanWaitingTime10.png)
   <br>
   **Measurements for QUEUESIZE =1000 , time is in microseconds (usec) :**
   ![measurements for queuesize=1000](/stats/QUEUESIZE1000/meanWaitingTime1000.png)
   <br> 
   
   ## Conclusion
   The conclusion about the mean waiting time, that  we get for the measurements in the tables above , is that:<br>
   **1. For a constant number of producer threads p , as the number of consumers q is rising , the mean waiting number of a function is getting greater, until some point in reaches a small value (about 1.4-2 microseconds) and after by rising the number of consumer q we get some small differences.** <br>
   **2. Bigger QUEUESIZE means bigger queue  and bigger mean waiting time ,as more functions can be in a pending state inside our FIFO queue** <br>
   The conlusion number 1 , can be justified , also by the bellow graphs: <br>
**Graph for QUEUESIZE =10 :**<br>
![graph for queuesize=10](/stats/QUEUESIZE10/graphs/meanWaitingTimeQUEUESIZE10.bmp)
<br> 
**Graph for QUEUESIZE =1000 :**<br>
![graph for queuesize=1000](/stats/QUEUESIZE1000/graphs/meanWaitingTimeQUEUESIZE1000.bmp) 
<br>
The above conlcusions and measurements arise from the fact that we used functions in the myFunctions.c file that are not time costly , if we used "heavier functions", the average waiting time would be greater ,of course , but the conclusions about the relation between p,q and mean waiting time would stay about the same.

