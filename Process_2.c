#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include <signal.h>
#include <errno.h> 
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define MAX 100


#define BILLION  1000000000L;

char * strerror (int errnum);
void  INThandler(int);

int msgid1=0,msgid2=0;
int x; 
pthread_t t2;
int s2;
void *res2;
int fd2; 

struct timespec start, stop, init_start, thread_stop;
double accum,Total_time;


char current_time[20] = "Process Start Time: ";
char Message_Received[30] = "s     Message Received: ";
char nl[10] = "\n";
char message[10];
char Time_message[10];

char *my_itoa(int num, char *str)
{
  if(str == NULL)
  {
    return NULL;
  }
  sprintf(str, "%d", num);

  return str;
}

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
    } 
    while (res && errno == EINTR);

    return res;
}

static void * threadFunc(void *arg)
{
   if (clock_gettime(CLOCK_REALTIME, &init_start) == -1 ) 
    {
      perror( "clock gettime" );
      exit( EXIT_FAILURE );
    }
  while(1)
  {
    if (clock_gettime(CLOCK_REALTIME, &thread_stop) == -1 ) 
    {
      perror( "clock gettime" );
      exit( EXIT_FAILURE );
    }
 
    msleep(500);
    
    Total_time = ( thread_stop.tv_sec - init_start.tv_sec )+ (float)( thread_stop.tv_nsec - init_start.tv_nsec )/BILLION; 
    printf("\nB_Total_Timer:%.4lf", Total_time);
    
  }
  return (void *) arg;
}
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
  
int main()
{
 
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



    s2 = pthread_create(&t2, NULL, threadFunc, &x);
    if (s2 != 0)
      printf("Error pthread_create");

    fd2 = open("Process_2.log", O_WRONLY | O_CREAT |  O_APPEND, S_IRUSR | S_IWUSR);
    if (fd2 == -1)
      printf("Error opening file");
      
    char first[20] = "Process_2 Open\n";
    write(fd2, first, strlen(first));

    while(1)
    {  
      if(msgrcv((int)msgid1, (void*)&message1, (size_t)sizeof(message1)-(size_t)sizeof(message1.mesg_type), 1, 0)==-1)
      {
        printf("Msgrcv on B Failed  %s\n", strerror(errno));    
      } 
        
      printf("\n");
 
      if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) 
      {
      perror("clock gettime");
      exit(EXIT_FAILURE);
      }
        
      accum = ( stop.tv_sec - start.tv_sec )+ (float)( stop.tv_nsec - start.tv_nsec )/ BILLION; 
      printf("B_Timer:%.4lf A_PID:%d A_msqid:%d A_key:%lX A_message: %d", accum, message1.pid , message1.qid , key1 , message1.number); // display the message
 
      if(clock_gettime( CLOCK_REALTIME, &start) == -1 ) 
      {
        perror( "clock gettime" );
        exit( EXIT_FAILURE );
      }

      msleep(600);

      message2.number = message2.number+ 2;
  
      if(msgsnd((int)msgid2, (void*)&message2, (size_t)sizeof(message2)-(size_t)sizeof(message2.mesg_type), (int)0) == -1)
      {
        printf("Msgnd on B Failed  %s\n", strerror(errno));       
      } 

      my_itoa((int)message1.number, message);
      my_itoa((int)Total_time, Time_message);
   
      write(fd2, current_time, strlen(current_time));       
      write(fd2, Time_message, strlen(Time_message));
      write(fd2, Message_Received, strlen(Message_Received));
      write(fd2, message, strlen(message));
      write(fd2, nl, strlen(nl));
      printf("\n");
 
      }

    if(-1 == msgctl(msgid1, IPC_RMID, NULL))
    {
      printf("msgctl on msgid1 Failed  %s\n", strerror(errno));    
    }
        if(-1 == msgctl(msgid2, IPC_RMID, NULL))
    {
      printf("msgctl on msgid2 Failed  %s\n", strerror(errno));    
    }
 
    s2 = pthread_join(t2, &res2);
    if (s2 != 0)
    {
      printf("Thread Join on Process_2 Failed");
    }
 
    return 0;
}

void  INThandler(int sig)
{
    if(-1 == msgctl(msgid1, IPC_RMID, NULL))
    {
      printf("msgctl on msgid1 Failed  %s\n", strerror(errno));    
    }
    
    if(-1 == msgctl(msgid2, IPC_RMID, NULL))
    {
      printf("msgctl on msgid2 Failed  %s\n", strerror(errno));    
    }
    if ( -1 == kill(message1.pid,SIGSEGV))
    {     
      printf("Kill on Message1.pid Failed  %s\n", strerror(errno));    
    }
  exit(0);
}