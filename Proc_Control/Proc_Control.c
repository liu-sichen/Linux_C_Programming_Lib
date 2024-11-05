#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "Log_Trace.h"

int main(void)
{
	LOG_TRACE(NULL, "hello world from process ID %ld\n", (long)getpid());
	LOG_TRACE(NULL, "uid = %d, gid = %d\n", getuid(), getgid());
	return 0;
}


