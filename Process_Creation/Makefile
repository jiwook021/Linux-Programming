CC = gcc -I ../include
CFLAGS  = -g -Wall
Language = c
output =-o 

path = ../include/
include = error_functions 
Target1 =t_fork
Target2 =fork_file_sharing
Target3 = 
Target4 = 
Target5 = 
Target6 = 


main:
	$(CC) $(CFLAGS) $(Target1).$(Language) $(output) $(Target1) 
	$(CC) $(CFLAGS) $(Target2).$(Language) $(output) $(Target2) 
	$(CC) $(CFLAGS) $(Target3).$(Language) $(output) $(Target3) 
	$(CC) $(CFLAGS) $(Target4).$(Language) $(output) $(Target4)
	$(CC) $(CFLAGS) $(Target5).$(Language) $(output) $(Target5)
	$(CC) $(CFLAGS) $(Target6).$(Language) $(output) $(Target6)

clean:
	$(RM) $(Target1)
	$(RM) $(Target2)
	$(RM) $(Target3)
	$(RM) $(Target4)
	$(RM) $(Target5)
	$(RM) $(Target6)