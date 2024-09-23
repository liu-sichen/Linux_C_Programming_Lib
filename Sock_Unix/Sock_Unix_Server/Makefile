include ../../MakeDefine

PROGS = Sock_Unix_Server

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	rm -f $(PROGS) $(TEMPFILES)

