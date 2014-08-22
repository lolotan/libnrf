CC = gcc
CFLAG = -c -Wall
EXECUTABLE = nrftest
MAIN = main

all: $(EXECUTABLE)

$(EXECUTABLE): nrflib.o spi.o gpio.o timer.o $(MAIN).o
	$(CC) spi.o gpio.o timer.o nrflib.o $(MAIN).o -o $(EXECUTABLE)

$(MAIN).o: $(MAIN).c
	$(CC) $(CFLAG) $(MAIN).c

nrflib.o: nrflib.c spi.o gpio.o timer.o
	$(CC) $(CFLAG) nrflib.c
	
spi.o: spi.c
	$(CC) $(CFLAG) spi.c

gpio.o: gpio.c
	$(CC) $(CFLAG) gpio.c
	
timer.o: timer.c
	$(CC) $(CFLAG) timer.c
	
clean:
	rm -rf *.o $(EXECUTABLE)
