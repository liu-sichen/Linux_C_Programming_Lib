include ../MakeDefine

PROGS = Standard_IO

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $@.c -o $@ $(LDLIBS) 

clean:
	rm -f $(PROGS) $(TEMPFILES)

