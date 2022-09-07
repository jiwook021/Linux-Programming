CC = g++
CFLAGS  = -g -Wall -std=c++17
Language = cpp -lpthread
output =-o 

Target1 = sleep
Target2 = threads
Target3 = shared_lock

main:
	$(CC) $(CFLAGS) $(Target1).$(Language) $(output) $(Target1)
	$(CC) $(CFLAGS) $(Target2).$(Language) $(output) $(Target2)
	$(CC) $(CFLAGS) $(Target3).$(Language) $(output) $(Target3)

clean:
	$(RM) $(Target1)
	$(RM) $(Target2)
	$(RM) $(Target3)
