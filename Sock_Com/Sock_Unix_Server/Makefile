include ../../MakeDefine

PROGS = Sock_Unix_Server
SUBDIRLDLIBS = -L../../Lib/Obj -llcpl
SUBDIRINCDIR = -I../../Lib/Include

all : $(PROGS)

% : %.c
	$(CC) $(CFLAGS) $(SUBDIRINCDIR) $@.c -o $@ $(SUBDIRLDLIBS)

clean :
	rm -f $(PROGS) $(TEMPFILES)

