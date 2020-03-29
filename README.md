# producer-consumer-multithreading
This is repo is about the first assigment on the real-time embedded systems class . We are called to modify a solution of producer consumer problem,which use threads , in order to be able to support multiple producer and multiple consumer threads, and having functions as queue items. 
## Compiling and Execution of the Program
###Execution Method 1
For executing the program you have first to compile it ,by executing the command bellow inside the project folder :<br>
`gcc prod-cons_multithreading.c myFunctions.c -pthread -o program.out -lm -O3`<br>
after compilation finish you are able to execute the programm with the following command<br>
`./program.out p q` , where **p is the number of producer threads** you want to have and **q the number of consumers**,for instance if you want to have 4 producer threads and 4 consumers threads you have to run the command:<br>
`./program.out 4 4`



