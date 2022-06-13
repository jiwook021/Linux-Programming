#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define MAX 100

void  INThandler(int);
// structure for message queue
struct mesg_buffer1 {
    long mesg_type;
    char mesg_text[100];
    int number;
} message1;
  
  struct mesg_buffer2 {
    long mesg_type;
    char mesg_text[100];
    int number;
} message2;
  
int msgid1,msgid2;

int main()
{
    key_t key1,key2;
   
    msgctl(msgid1, IPC_RMID, NULL);
    // ftok to generate unique key
    key1 = ftok("progfile1", 65);
    // msgget creates a message queue
    msgid1 = msgget(key1, 0666 | IPC_CREAT);




    msgctl(msgid2, IPC_RMID, NULL);
    // ftok to generate unique key
    key2 = ftok("progfile2", 66);
    // msgget creates a message queue
    msgid2 = msgget(key2, 0666 | IPC_CREAT);


  message2.mesg_type = 1;
  message2.number = 0;
  while(1)
  {  
      
    msgrcv(msgid1, &message1, sizeof(message1), 1, 0);  // msgrcv to receive message
    printf("Data Received is : %d \n", message1.number); // display the message
    sleep(1);
  

    message2.number+=2; 
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
  exit(0);
}