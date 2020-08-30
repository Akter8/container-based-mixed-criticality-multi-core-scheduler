CC = gcc
flags = -c -Wall
driver = driver
executable = run
output = output.txt

all: $(driver).o cpu.o processor.o job.o utility.o container.o
	$(CC) $(driver).o cpu.o processor.o job.o utility.o container.o -o $(executable)

$(driver).o: $(driver).c
	$(CC) $(flags) $(driver).c

cpu.o: cpu.c
	$(CC) $(flags) cpu.c

processor.o: processor.c
	$(CC) $(flags) processor.c

job.o: job.c
	$(CC) $(flags) job.c

utility.o: utility.c
	$(CC) $(flags) utility.c

container.o: container.c
	$(CC) $(flags) container.c

clean:
	rm -f *.o $(executable) $(output)