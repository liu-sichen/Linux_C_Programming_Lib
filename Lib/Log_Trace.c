#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "Log_Trace.h"

int LogTrace(const char *file, int line, LogPara *para, const char *fmt, ...)
{

	char logStr[LOG_LEN_MAX] = {0};

	sprintf(logStr, "File: %s, Line: %d info: ", file, line);
	
	va_list args;
	va_start(args, fmt);
	vsprintf(logStr + strlen(logStr), fmt, args);
	va_end(args);

	printf("%s \n", logStr);

	return 0;
}

