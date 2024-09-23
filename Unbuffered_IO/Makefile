include ../MakeDefine

PROGS = Unbuffered_IO

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	rm -f $(PROGS) $(TEMPFILES)

