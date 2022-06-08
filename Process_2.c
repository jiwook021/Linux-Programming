#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define MAX 100

void  INThandler(int);
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
    int number;
} message;
  
int msgid;
 
int main()
{
    key_t key;
  
    msgctl(msgid, IPC_RMID, NULL);
  
    // ftok to generate unique key
    key = ftok("progfile", 65);
  
    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);

  message.number = 0;
  while(1)
  {  
      
    msgrcv(msgid, &message, sizeof(message), 1, 0);  // msgrcv to receive message
    printf("Data Received is : %d \n", message.number); // display the message


    message.number++; 
    msgsnd(msgid, &message, sizeof(message), 0); // msgsnd to send message
    sleep(1);

    signal(SIGINT, INThandler);

  }
    // to destroy the message queue
    msgctl(msgid, IPC_RMID, NULL);
  
    return 0;
}

void  INThandler(int sig)
{

  signal(sig, SIG_IGN);
  msgctl(msgid, IPC_RMID, NULL);
  exit(0);
}