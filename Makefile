DIRS = lib 2D_Struct_Array File_Operate List_Directory_Contents \
    Shell Sock_Unix Unbuffered_IO Standard_IO\

all:
	for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE) ) || exit 1; \
	done

clean:
	for i in $(DIRS); do \
		(cd $$i && echo "cleaning $$i" && $(MAKE) clean) || exit 1; \
	done

