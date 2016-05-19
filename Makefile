PROJECT = nrf
NRFSTATICLIB=lib$(PROJECT).a
OBJECTS = nrflib.o

all: $(NRFSTATICLIB)
	
$(NRFSTATICLIB): $(OBJECTS)
	$(AR) rcs $@ $^

nrflib.o: nrflib.c 
	$(CC) $(CFLAGS) $(LDFLAGS) nrflib.c
	
clean:
	rm -rf *.o *.a
