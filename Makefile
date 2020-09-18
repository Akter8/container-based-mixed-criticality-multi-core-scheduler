# The compiler.
CC = gcc
# Set any flags we want.
flags = -c -Wall
# Driver function file name.
driver = driver
# The name of the executable.
executable = run
# The name of the output file.
output = output.txt


all: $(driver).o cpu.o processor.o job.o utility.o container.o scheduler.o priority_queue.o
	$(CC) $(driver).o cpu.o processor.o job.o utility.o container.o scheduler.o priority_queue.o -o $(executable)

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

scheduler.o: scheduler.c
	$(CC) $(flags) scheduler.c

priority_queue.o: priority_queue.c
	$(CC) $(flags) priority_queue.c

clean:
	rm -f *.o $(executable) $(output)