#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#define MAX 100

void  INThandler(int);



// structure for message queue
struct mesg_buffer1 {
    long mesg_type;
    char mesg_text[100];
    int number;
    int pid;
} message1;
  
  
  struct mesg_buffer2 {
    long mesg_type;
    char mesg_text[100];
    int number;
    int pid; 
} message2;
  
time_t timer = 0;
long timer1 = 0;
long timer2 = 0;


int msgid1=0,msgid2=0;

int main()
{
    long key1 = 0xfffffffe;
    long key2 = 0xfffffffd;
   
    msgid1 = msgget(key1, 0666 | IPC_CREAT);
    msgid2 = msgget(key2, 0666 | IPC_CREAT);
    
    message2.mesg_type = 1;
    message2.number = 0;
    message2.pid = getpid();


    timer1 = time(&timer);
  while(1)
  {  
    msgrcv(msgid1, &message1, sizeof(message1), 1, 0);  // msgrcv to receive message
    timer2 = time(&timer);
    printf("Time: %ld PID: %d msqid1: %d key1: %ld message 1 : %d \n", timer2 - timer1, message2.pid , msgid1 , key1 , message1.number); // display the message
    timer1 = time(&timer);    
    sleep(1);
    message2.number = message2.number+ 2;
    msgsnd(msgid2, &message2, sizeof(message2), 0); // msgsnd to send message
    signal(SIGINT, INThandler);
  }
    // to destroy the message queue
    msgctl(msgid1, IPC_RMID, NULL);
    msgctl(msgid2, IPC_RMID, NULL);
  
    
    return 0;
}

void  INThandler(int sig)
{
  signal(sig, SIG_IGN);
  msgctl(msgid1, IPC_RMID, NULL);
  msgctl(msgid2, IPC_RMID, NULL);
  kill(message1.pid,SIGSEGV);
     
  exit(0);
}