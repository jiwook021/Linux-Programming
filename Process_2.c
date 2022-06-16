#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define MAX 100

void  INThandler(int);

#define BILLION  1000000000L;

// structure for message queue
struct mesg_buffer1 {
    long mesg_type;
    char mesg_text[100];
    int number;
    int pid;
    int qid;
} message1;
  
  
  struct mesg_buffer2 {
    long mesg_type;
    char mesg_text[100];
    int number;
    int pid;
    int qid; 
} message2;
  
time_t timer = 0;
long timer1 = 0;
long timer2 = 0;


int msgid1=0,msgid2=0;

int main()
{
  struct timespec start, stop;
  double accum;

   
    long key1 = 0xfffffffe;
    long key2 = 0xfffffffd;
   
    msgid1 = msgget(key1, 0666 | IPC_CREAT);
    msgid2 = msgget(key2, 0666 | IPC_CREAT);
    
     if((msgid1==-1) | (msgid2 ==-1))
    {
      printf("Msgget on B Failed\n");
    }


    message2.mesg_type = 1;
    message2.number = 0;
    message2.pid = getpid();
    message2.qid = msgid2;
    
    if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) 
      {
        perror( "clock gettime" );
        exit( EXIT_FAILURE );
      }

    signal(SIGINT, INThandler);
 
      while(1)
      {  
        if(msgrcv(msgid1, &message1, sizeof(message1), 1, 0)==-1)
        {
         printf("Msgrcv on B Failed\n");    
        } 
        
        if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) 
        {
        perror("clock gettime");
        exit(EXIT_FAILURE);
        }
        
        accum = ( stop.tv_sec - start.tv_sec )
        + ( stop.tv_nsec - start.tv_nsec )/ BILLION; 

        printf("B_Timer:%.2lf A_PID:%d A_msqid:%d A_key:%lX A_message: %d \n", accum, message1.pid , message1.qid , key1 , message1.number); // display the message
        timer1 = time(&timer);    
        
         if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) 
          {
            perror( "clock gettime" );
            exit( EXIT_FAILURE );
          }

        sleep(1);
        message2.number = message2.number+ 2;
        if(msgsnd(msgid2, &message2, sizeof(message2), 0) == -1)
        {
         printf("Msgnd on B Failed\n");       
        } 
      }

    msgctl(msgid1, IPC_RMID, NULL);
    msgctl(msgid2, IPC_RMID, NULL);
    
    return 0;
}

void  INThandler(int sig)
{
  msgctl(msgid1, IPC_RMID, NULL);
  msgctl(msgid2, IPC_RMID, NULL);
  kill(message1.pid,SIGSEGV);    
  exit(0);
}