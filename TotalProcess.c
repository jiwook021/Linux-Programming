#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{

    const char * command1 = "./Process_1 &";
    system(command1);
    sleep(2);
    const char * command2 = "./Process_2";
    system(command2);
}