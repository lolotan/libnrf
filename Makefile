CC = gcc
CFLAG = -c -Wall -Wextra
PROJECT = nrf
NRFSTATICLIB=lib$(PROJECT).a
EXECUTABLE = $(PROJECT)test
MAIN = main
OBJECTS = nrflib.o spi.o gpio.o timer.o

all: $(EXECUTABLE) $(NRFSTATICLIB)

$(EXECUTABLE): $(NRFSTATICLIB) $(MAIN).o
	$(CC) $(MAIN).o -static -L. -lnrf -o $(EXECUTABLE)
	
$(NRFSTATICLIB): $(OBJECTS)
	ar rcs $@ $^

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
	rm -rf *.o *.a $(EXECUTABLE)
