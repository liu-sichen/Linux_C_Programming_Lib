include ../MakeDefine

PROGS = List_Directory_Contents

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	rm -f $(PROGS) $(TEMPFILES)

