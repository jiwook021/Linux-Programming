#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> 

int main()
{
  const char * command0 = "ls -al";
  const char * command1 = "./Process_1 &";
  const char * command2 = "./Process_2";
     
  if(-1==system(command0))
  {
    perror("Error for ls -al");
  }  

  sleep(2);
    
  if(-1==system(command1))
  {
    perror("Error for ./Process_1 &");
  }  
  sleep(2);

  if(-1==system(command2))
  {
    perror("Error for ./Process_2");
  }

return 0;
}