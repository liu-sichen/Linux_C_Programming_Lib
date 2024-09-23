include ../MakeDefine

LIBS = liblcpl.a
OBJS = Log_Trace.o

all: $(LIBS)

$(LIBS): $(OBJS)
	$(AR) rv $(LIBS) $?

clean:
	rm -f $(TEMPFILES)

