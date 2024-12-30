#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include "Log_Trace.h"

#define ALIGN_SIZE 512
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define STDIO_MAX_SIZE 4096

int ReadFileDirect(const char *fileName, unsigned char **data, unsigned int len)
{
	if ((data == NULL) || ((len % ALIGN_SIZE) != 0)) {
		LOG_TRACE(NULL, "arg wrong");
		return -1;
	}

    unsigned char *buf;
    int ret = posix_memalign((void **)&buf, ALIGN_SIZE, len);
    if (ret != 0) {
        LOG_TRACE(NULL, "posix_memalign failed ret is %d", ret);
        return -1;
    }
 
    int file = open(fileName, O_RDONLY | O_DIRECT, FILE_MODE);
    if (file < 0){
		LOG_TRACE(NULL, "open failed ret is %d", ret);
        return -1;
    }
 
    ret = read(file, buf, len);
    if (ret != len) {
		LOG_TRACE(NULL, "read failed ret is %d", ret);
		return -1;
    }
     
    *data = buf;
    close(file);

	return 0;
}

int WriteFileDirect(const char *fileName, unsigned char *data, unsigned int len)
{
	if ((data == NULL) || ((len % ALIGN_SIZE) != 0)) {
		LOG_TRACE(NULL, "arg wrong");
		return -1;
	}

    unsigned char *buf;
    int ret = posix_memalign((void **)&buf, ALIGN_SIZE, len);
    if (ret != 0) {
		LOG_TRACE(NULL, "posix_memalign failed ret is %d", ret);
		return -1;
    }
    memcpy(buf, data, len);
 
    int file = open(fileName, O_WRONLY | O_DIRECT | O_CREAT, FILE_MODE);
    if (file < 0){
		LOG_TRACE(NULL, "open failed ret is %d", ret);
		return -1;
    }
 
    ret = write(file, buf, len);
    if (ret != len) {
		LOG_TRACE(NULL, "write failed ret is %d", ret);
		return -1;
    }
 
    free(buf);
    close(file);

	return 0;
}

int OpenTmpFile(const char *fileName)
{
	if (open(fileName, O_RDWR | O_CREAT) < 0) {
		LOG_TRACE(NULL, "open error");
	}

	if (unlink(fileName) < 0) {
		LOG_TRACE(NULL, "unlink error");
	}
	
	LOG_TRACE(NULL, "file unlinked\n");

	return 0;
}

void SetFile(int fd, int flags)
{
	int val = fcntl(fd, F_GETFL, 0);
	if (val < 0) {
		LOG_TRACE(NULL, "fcntl F_GETFL error");
		return;
	}

	LOG_TRACE(NULL, "FL is %x \n", val);

	val |= flags;
	if (fcntl(fd, F_SETFL, val) < 0) {
		LOG_TRACE(NULL, "fcntl F_SETFL error");
	}
	return;
}

int GetFileType(char *name)
{
	struct stat buf;
	if (lstat(name, &buf) < 0) {
		LOG_TRACE(NULL, "lstat error");
		return -1;
	}

	char *ptr;
	if (S_ISREG(buf.st_mode)) {
		ptr = "regular";
	} else if (S_ISDIR(buf.st_mode)) {
		ptr = "directory";
	} else if (S_ISCHR(buf.st_mode)) {
		ptr = "character special";
	} else if (S_ISBLK(buf.st_mode)) {
		ptr = "block special";
	} else if (S_ISFIFO(buf.st_mode)) {
		ptr = "fifo";
	} else if (S_ISLNK(buf.st_mode)) {
		ptr = "symbolic link";
	} else if (S_ISSOCK(buf.st_mode)) {
		ptr = "socket";
	} else {
		ptr = "unknown mode";
	}

	LOG_TRACE(NULL, "%s", ptr);
	return 0;
}

int ListDir(const char *dirName)
{
	DIR *dp;
	struct dirent *dirp;

	if ((dp = opendir(dirName)) == NULL) {
		LOG_TRACE(NULL, "opendir fail", dirName);
		return -1;
	}

	while ((dirp = readdir(dp)) != NULL) {
		LOG_TRACE(NULL, "%s", dirp->d_name);
	}

	closedir(dp);
	return 0;
}

int AddPermission(const char *name, unsigned int umaskVal, unsigned int permission)
{
	umask(umaskVal);

	struct stat statbuf;
	if (lstat(name, &statbuf) < 0) {
		LOG_TRACE(NULL, "lstat fail");
		return -1;
	}
	if (chmod("foo", statbuf.st_mode | permission) < 0) {
		LOG_TRACE(NULL, "chmod fail");
		return -1;
	}

	return 0;
}

int FwriteFile(const char *fileName, unsigned long offset, unsigned char *buf, unsigned int len)
{
	FILE *file = fopen(fileName, "r+");
	if (file == NULL) {
		LOG_TRACE(NULL, "fopen failed");
		return -1;
	}

	if (fseek(file, offset, SEEK_SET)) {
		LOG_TRACE(NULL, "fseek failed");
		fclose(file);
		return -1;
	}

	size_t wrSize = fwrite(buf, sizeof(unsigned char), len, file);
	fclose(file);
	if (wrSize != len) {
		LOG_TRACE(NULL, "fwrite failed");
		return -1;
	}

	return 0;
}

int FreadFile(const char *fileName, unsigned long offset, unsigned char *buf, unsigned int len)
{
	FILE *file = fopen(fileName, "r");
	if (file == NULL) {
		LOG_TRACE(NULL, "fopen failed");
		return -1;
	}

	if (fseek(file, offset, SEEK_SET)) {
		LOG_TRACE(NULL, "fseek failed");
		fclose(file);
		return -1;
	}

	size_t wrSize = fread(buf, sizeof(unsigned char), len, file);
	fclose(file);
	if (wrSize != len) {
		LOG_TRACE(NULL, "fread failed");
		return -1;
	}

	return 0;
}

int GetPutStdin()
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

int GetsPutsStdin()
{
	char buf[STDIO_MAX_SIZE];
	while (fgets(buf, STDIO_MAX_SIZE, stdin) != NULL) {
		if (fputs(buf, stdout) == EOF) {
			LOG_TRACE(NULL, "output error");
		}
	}

	if (ferror(stdin)) {
		LOG_TRACE(NULL, "input error");
	}

	return 0;
}

int main(int argc, char *argv[])
{
    return 0;
}
