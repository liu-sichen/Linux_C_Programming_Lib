include ../MakeDefine

PROGS = File_Operate
ADDFLAGS = -D_GNU_SOURCE

all : $(PROGS)

% : %.c
	$(CC) $(CFLAGS) $(ADDFLAGS) $(INCDIR) $@.c -o $@ $(LDLIBS)

clean :
	rm -f $(PROGS) $(TEMPFILES)

