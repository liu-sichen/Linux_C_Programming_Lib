#ifndef	_LOG_TRACE_H
#define	_LOG_TRACE_H

#define LOG_LEN_MAX 128

typedef struct {
	int mod;
	int lvl;
	int loc;
} LogPara;


#define LOG_TRACE(para, fmt, ...)  LogTrace(__FILE__, __LINE__, para, fmt, ##__VA_ARGS__)

int LogTrace(const char *file, int line, LogPara *para, const char *fmt, ...);

#endif
