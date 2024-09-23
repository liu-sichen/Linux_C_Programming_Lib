#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "Log_Trace.h"

int main(void)
{
	int c;
	while ((c = getc(stdin)) != EOF) {
		if (putc(c, stdout) == EOF) {
			LOG_TRACE(NULL, "output error");
		}
	}

	if (ferror(stdin)) {
		LOG_TRACE(NULL, "input error");
	}

	return 0;
}

