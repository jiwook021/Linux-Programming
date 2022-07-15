CC = gcc

  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

Target = Process_1
Target2 = Process_2

all:
	$(CC) $(CFLAGS) -o $(Target) $(Target).c -lpthread
	$(CC) $(CFLAGS) -o $(Target2) $(Target2).c -lpthread
		
clean:
	$(RM) $(Target)
	$(RM) $(Target2)



	