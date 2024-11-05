include ../MakeDefine

PROGS = Data_Struct

all : $(PROGS)

% : %.c
	$(CC) $(CFLAGS) $(INCDIR) $@.c -o $@ $(LDLIBS)

clean :
	rm -f $(PROGS) $(TEMPFILES)

