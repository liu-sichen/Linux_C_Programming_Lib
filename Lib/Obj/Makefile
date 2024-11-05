include ../../MakeDefine

DIRS = Log_Trace obj

LIBS = liblcpl.a
OBJS = Log_Trace.o

all: $(LIBS)

$(LIBS): $(OBJS)
	$(AR) rv $(LIBS) $?

clean:
	rm -f $(TEMPFILES)