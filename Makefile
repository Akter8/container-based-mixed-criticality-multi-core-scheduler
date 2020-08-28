CC = gcc
flags = -c -Wall
driver = driver
executable = run

all: $(driver).o job.o utility.o
	$(CC) $(driver).o job.o utility.o -o $(executable)

$(driver).o: $(driver).c
	$(CC) $(flags) $(driver).c

job.o: job.c
	$(CC) $(flags) job.c

utility.o: utility.c
	$(CC) $(flags) utility.c

clean:
	rm -f *.o $(executable)