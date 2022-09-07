CC = g++
CFLAGS  = -g -Wall -std=c++17
Language = cpp -lpthread
output =-o 

Target1 = sleep
Target2 = threads
Target3 = shared_lock
Target4 = avoid_deadlock
Target5 = sync_cout
Target6 = call_once

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