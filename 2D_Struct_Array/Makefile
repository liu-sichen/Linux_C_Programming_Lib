include ../MakeDefine

PROGS = 2D_Struct_Array

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	rm -f $(PROGS) $(TEMPFILES)

