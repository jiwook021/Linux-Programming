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
pthread_t ClockThread1;
int rClockThread1;

void *res1;
int msgid1=0,msgid2=0;
int fd1; 

char nl[10] = "\n";
char message[10];
char Time_message[10];
char current_time[20] = "Process Start Time: ";
char Message_Received[30] = "s     Message Received: ";

//function call
void  INThandler(int);
void ALRMhandler();
int msleep(long); 
char *my_itoa(int, char*);
static void * threadFunc(void *);

//static void * threadFunc2(void *arg);
//int x2; 
//pthread_t t2;
//int s2;
struct mesg_buffer1 
{
  long mesg_type;
  char mesg_text[100];
  int number;
  int pid;
  int qid;

} message1; 
  
  
struct mesg_buffer2 
{
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
  
  if((-1==msgid1) | (-1==msgid2))
  {
    perror("Msgget on A Failed\n");
  }


  message1.mesg_type = 1;
  message1.number = 1;
  message1.pid = getpid();
  message1.qid = msgid1;

  signal(SIGINT, INThandler);
  signal(SIGALRM, ALRMhandler);

  alarm(5);

  rClockThread1 = pthread_create(&ClockThread1, NULL, threadFunc, &x);
    if (0 != rClockThread1)
      perror("Error pthread_create\n");

  // s2 = pthread_create(&t2, NULL, threadFunc2, &x2);
  //   if (0 != s2)
  //     perror("Error pthread_create\n");

  execlp("./Process_2 &", "./Process_2 &", NULL);  

  fd1 = open("Process_1.log", O_WRONLY | O_CREAT |  O_APPEND, S_IRUSR | S_IWUSR);
    if (-1 == fd1)
      perror("Error opening file\n");
      
  char first[20] = "Process_1 Open\n";
  if(-1 == write(fd1, first, strlen(first)))
  {
    perror("Write Failed");
  }

  while(1)
  {

    if(-1 == clock_gettime( CLOCK_REALTIME, &start)) 
    {
      perror( "clock gettime\n");
    }
    msleep(900);
    message1.number++;
    printf("\n");
    
    if (-1 == msgsnd((int)msgid1, (void*)&message1, (size_t)sizeof(message1)-(size_t)sizeof(message1.mesg_type), (int)0))
    {
      printf("msgnd on A Failed %s\n", strerror(errno));       
    } 
    
    msgrcv((int)msgid2, (void*)&message2, (size_t)sizeof(message2)-(size_t)sizeof(message2.mesg_type), 1, 0); 
   
    
    printf("\n");
        
    if(-1 == clock_gettime( CLOCK_REALTIME, &stop)) 
    {
      perror( "clock gettime" );
      exit( EXIT_FAILURE );
    }
    
    accum = ( stop.tv_sec - start.tv_sec )+ (float)( stop.tv_nsec - start.tv_nsec )/BILLION; 

    printf("A_Timer:%.4lf B_PID:%d B_msqid:%d B_key:%lX B_message: %d", accum, message2.pid , message2.qid, key2, message2.number); // display the message

       
    my_itoa((int)message2.number, message);
    my_itoa((int)Total_time, Time_message);

    if( -1 == write(fd1, current_time, strlen(current_time)))
    {
      perror("Write error");
    }     
    if( -1 == write(fd1, Time_message, strlen(Time_message)))
    {
      perror("Write error");
    }
    if( -1 == write(fd1, Message_Received, strlen(Message_Received)))
    {
      perror("Write error");
    }
    if( -1 == write(fd1, message, strlen(message)))
    {
      perror("Write error");
    }
    if( -1 == write(fd1, nl, strlen(nl)))
    {
      perror("Write error");
    }
  }

  if(-1 == msgctl(msgid1, IPC_RMID, NULL))
  {
    perror("msgctl on msgid1 Failed\n");    
  }
  
  if(-1 == msgctl(msgid2, IPC_RMID, NULL))
  {
    perror("msgctl on msgid2 Failed  \n");    
  }
     
  rClockThread1 = pthread_join(ClockThread1, &res1);
  
  if (0!= rClockThread1)
  {
    perror("Thread Join on Process_1 Failed\n");
  }
  return 0;
   
}

void ALRMhandler()
{
  printf("\nAlarm Signal Generated from A!");
  alarm(5);
}
     
    
void INThandler(int sig)
{

  if(-1 == msgctl(msgid1, IPC_RMID, NULL))
  {
    perror("msgctl on msgid1 Failed\n");    
  }
    if(-1 == msgctl(msgid2, IPC_RMID, NULL))
  {
    perror("msgctl on msgid2 Failed\n");    
  }
  
  if (-1 == close(fd1))
  {
    perror("Close Failed!\n");      
  }

  if ( -1 == kill(message2.pid,SIGTERM))
  {
    perror("Kill on Message2.pid Failed\n");    
  }
 
  exit(0);
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
  if (-1 == clock_gettime(CLOCK_REALTIME, &init_start)) 
    {
      perror( "clock gettime" );
      exit(EXIT_FAILURE);
    }
  while(1)
    {
      if (-1== clock_gettime(CLOCK_REALTIME, &thread_stop)) 
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