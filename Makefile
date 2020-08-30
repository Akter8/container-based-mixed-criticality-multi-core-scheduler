CC = gcc
flags = -c -Wall
driver = driver
executable = run

all: $(driver).o job.o utility.o container.o
	$(CC) $(driver).o job.o utility.o container.o -o $(executable)

$(driver).o: $(driver).c
	$(CC) $(flags) $(driver).c

job.o: job.c
	$(CC) $(flags) job.c

utility.o: utility.c
	$(CC) $(flags) utility.c

container.o: container.c
	$(CC) $(flags) container.c

clean:
	rm -f *.o $(executable)