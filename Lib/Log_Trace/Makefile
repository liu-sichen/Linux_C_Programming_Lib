include ../../MakeDefine

OBJS = Log_Trace.o
LIBINCDIR = -I../Include
OBJDIR = ../Obj/
all : $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) $(LIBINCDIR) -c $< -o $(OBJDIR)$@

clean :
	rm -f $(TEMPFILES)

