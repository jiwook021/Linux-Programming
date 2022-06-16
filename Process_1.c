#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h> 

#define MAX 100

void  INThandler(int);

time_t timer = 0;
long timer1 = 0;
long timer2 = 0;

#define BILLION  1000000000L;

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

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
  

int msgid1=0,msgid2=0;
  
int main()
{
  long key1 = 0xfffffffe;
  long key2 = 0xfffffffd;
   
  struct timespec start, stop;
  double accum;

    msgid1 = msgget(key1, 0666 | IPC_CREAT);
    msgid2 = msgget(key2, 0666 | IPC_CREAT);
    
    if((msgid1==-1) | (msgid2 ==-1))
    {
      printf("Msgget on A Failed\n");
    }
    message1.mesg_type = 1;
    message1.number = 1;
    message1.pid = getpid();
    message1.qid = msgid1;
    signal(SIGINT, INThandler);
    
      while(1)
      {
        if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) 
        {
          perror( "clock gettime" );
          exit( EXIT_FAILURE );
        }
        msleep(900);
        message1.number++;
        if (msgsnd(msgid1, &message1, sizeof(message1), 0) == -1)
        {
        printf("msgnd on A Failed\n");       
        } 
        if(msgrcv(msgid2, &message2, sizeof(message2), 1, 0)==-1)
        {
        printf("Msgrcv on A Failed\n");    
        } 
        
        if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) 
        {
        perror( "clock gettime" );
        exit( EXIT_FAILURE );
        }
        accum = ( stop.tv_sec - start.tv_sec )+ (float)( stop.tv_nsec - start.tv_nsec )/BILLION; 

        printf("A_Timer:%.4lf B_PID:%d B_msqid:%d B_key:%lX B_message: %d \n", accum, message2.pid , message2.qid, key2, message2.number); // display the message
      }
    
    msgctl(msgid1, IPC_RMID, NULL);
    msgctl(msgid2, IPC_RMID, NULL);
     
    return 0;
}

void INThandler(int sig)
{
  msgctl(msgid1, IPC_RMID, NULL);
  msgctl(msgid2, IPC_RMID, NULL);
  kill(message2.pid,SIGSEGV);
   
  exit(0);
}