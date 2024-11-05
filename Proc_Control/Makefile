include ../MakeDefine

PROGS = Proc_Control

all : $(PROGS)

% : %.c
	$(CC) $(CFLAGS) $(INCDIR) $@.c -o $@ $(LDLIBS)

clean :
	rm -f $(PROGS) $(TEMPFILES)

