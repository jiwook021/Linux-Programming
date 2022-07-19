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

struct timespec start, stop, init_start, thread_stop;
double accum,Total_time;
char * strerror (int errnum);
int x; 
pthread_t t1;
int s1;
void *res1;
int msgid1=0,msgid2=0;
int fd1; 


char nl[10] = "\n";
char message[10];
char Time_message[10];
char current_time[20] = "Process Start Time: ";
char Message_Received[30] = "s     Message Received: ";

void  INThandler(int);

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

  do 
  {
        res = nanosleep(&ts, &ts);
  } 
  while (res && errno == EINTR);
    return res;
}

char *my_itoa(int num, char *str)
{
  if(str == NULL)
  {
    return NULL;
  }
  
  sprintf(str, "%d", num);
  return str;
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
      msleep(900);
      
    Total_time = ( thread_stop.tv_sec - init_start.tv_sec )+ (float)( thread_stop.tv_nsec - init_start.tv_nsec )/BILLION; 
    printf("\nA_Total_Timer:%.4lf", Total_time); 
    }
  return (void *) arg;
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
  

int main()
{

  long key1 = 0xfffffffe;
  long key2 = 0xfffffffd;
   
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
      
  s1 = pthread_create(&t1, NULL, threadFunc, &x);
    if (s1 != 0)
      printf("Error pthread_create");

  fd1 = open("Process_1.log", O_WRONLY | O_CREAT |  O_APPEND, S_IRUSR | S_IWUSR);
    if (fd1 == -1)
      printf("Error opening file");
      
  char first[20] = "Process_1 Open\n";
  write(fd1, first, strlen(first));

  while(1)
  {
    if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) 
    {
      perror( "clock gettime" );
      exit( EXIT_FAILURE );
    }
    msleep(900);
    message1.number++;
    printf("\n");
    
    if (msgsnd((int)msgid1, (void*)&message1, (size_t)sizeof(message1)-(size_t)sizeof(message1.mesg_type), (int)0) == -1)
    {
      printf("msgnd on A Failed %s\n", strerror(errno));       
    } 
    
    if(msgrcv((int)msgid2, (void*)&message2, (size_t)sizeof(message2)-(size_t)sizeof(message2.mesg_type), 1, 0) == -1)
    {
      printf("Msgrcv on A Failed  %s\n", strerror(errno));    
    } 
    
    printf("\n");
        
    if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) 
    {
      perror( "clock gettime" );
      exit( EXIT_FAILURE );
    }
    
    accum = ( stop.tv_sec - start.tv_sec )+ (float)( stop.tv_nsec - start.tv_nsec )/BILLION; 

    printf("A_Timer:%.4lf B_PID:%d B_msqid:%d B_key:%lX B_message: %d", accum, message2.pid , message2.qid, key2, message2.number); // display the message

       
    my_itoa((int)message2.number, message);
    my_itoa((int)Total_time, Time_message);

    write(fd1, current_time, strlen(current_time));     
    write(fd1, Time_message, strlen(Time_message));
    write(fd1, Message_Received, strlen(Message_Received));
    write(fd1, message, strlen(message));
    write(fd1, nl, strlen(nl));
  }

  if(-1 == msgctl(msgid1, IPC_RMID, NULL))
  {
    printf("msgctl on msgid1 Failed  %s\n", strerror(errno));    
  }
  
  if(-1 == msgctl(msgid2, IPC_RMID, NULL))
  {
    printf("msgctl on msgid2 Failed  %s\n", strerror(errno));    
  }
     
  s1 = pthread_join(t1, &res1);
  
  if (s1 != 0)
  {
    printf("Thread Join on Process_1 Failed");
  }
  return 0;
   
}
   
     
    
void INThandler(int sig)
{

  if(-1 == msgctl(msgid1, IPC_RMID, NULL))
  {
    printf("msgctl on msgid1 Failed  %s\n", strerror(errno));    
  }
    if(-1 == msgctl(msgid2, IPC_RMID, NULL))
  {
    printf("msgctl on msgid2 Failed  %s\n", strerror(errno));    
  }
  
  if (-1 == close(fd1))
  {
    printf("Close Failed! %s\n", strerror(errno));      
  }

  if ( -1 == kill(message2.pid,SIGTERM))
  {
    printf("Kill on Message2.pid Failed  %s\n", strerror(errno));    
  }
 

   
  exit(0);
}