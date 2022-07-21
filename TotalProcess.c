#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

    const char * command0 = "ls -al"; 
    const char * command1 = "./Process_1 &";
    const char * command2 = "./Process_2";
     
    system(command0);
   
    sleep(2);
    system(command1);
   
    
    sleep(2);

    // if (return1==-1||return1 ==127)
    // {
    //     exit(0);
    // }
    
    system(command2);

    // if (return2==-1||return2 ==127)
    // { 
    //     printf("Error for Process_2");
    //     exit(0);
    // }
}