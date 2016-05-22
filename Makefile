PROJECT = libnrf
NRFSTATICLIB=$(PROJECT).a
OBJECTS = $(PROJECT).o

all: $(NRFSTATICLIB)
	
$(NRFSTATICLIB): $(OBJECTS)
	$(AR) rcs $@ $^

$(OBJECTS): libnrf.c 
	$(CC) $(CFLAGS) $(LDFLAGS) libnrf.c
	
clean:
	rm -rf *.o *.a
