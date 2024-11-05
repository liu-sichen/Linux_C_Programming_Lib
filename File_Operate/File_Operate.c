#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "Log_Trace.h"

#define BUF_SIZE 1024
#define TEST_FILE "/home/yihelsc/testfile.bin"
#define TEST_DATA 0x5A
#define TEST_LEN 0x100000

int ReadFileDirect()
{
    unsigned char *buf;
    int ret = posix_memalign((void **)&buf, 512, TEST_LEN);
    if (ret) {
        perror("posix_memalign failed");
        exit(1);
    }
 
    int file = open(TEST_FILE, O_RDONLY | O_DIRECT, 0755);
    if (file < 0){
        return-1;
    }
 
    ret = read(file, buf, TEST_LEN);
    if (ret < 0) {
		return -1;
    }
     
    free(buf);
    close(file);

	return 0;
}

int WriteFileDirect()
{
    unsigned char *buf;
    int ret = posix_memalign((void **)&buf, 512, TEST_LEN);
    if (ret) {
		return -1;
    }
    memset(buf, TEST_DATA, TEST_LEN);
 
    int file = open(TEST_FILE, O_WRONLY | O_DIRECT | O_CREAT, 0755);
    if (file < 0){
		return -1;
    }
 
    ret = write(file, buf, TEST_LEN);
    if (ret < 0) {
		return -1;
    }
 
    free(buf);
    close(file);

	return 0;
}

void SetFl(int fd, int flags)
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

int WriteFileHole(void)
{
	char buf1[] = "abcdefghij";
	char buf2[] = "ABCDEFGHIJ";

	int fd = creat("file.hole", (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
	if (fd  < 0) {
		LOG_TRACE(NULL, "input error");
	}
	SetFl(fd, O_SYNC);
	if (write(fd, buf1, 10) != 10) {
		LOG_TRACE(NULL, "buf1 write error");
	}
	if (lseek(fd, 16384, SEEK_SET) == -1) {
		LOG_TRACE(NULL, "lseek error");
	}
	if (write(fd, buf2, 10) != 10) {
		LOG_TRACE(NULL, "buf2 write error");
	}

	return 0;
}

#define TEST_LOOP 100

int WriteFileStream(unsigned long offset, unsigned char *buf, unsigned int len)
{
	FILE *file = fopen(TEST_FILE, "r+");
	if (file == NULL) {
		return -1;
	}

	if (fseek(file, offset, SEEK_SET)) {
		fclose(file);
		return -1;
	}

	size_t wrSize = fwrite(buf, sizeof(unsigned char), len, file);
	fclose(file);
	if (wrSize != len) {
		return -1;
	}

	return 0;
}

void TestWriteFileStream()
{
	unsigned char *buf = (unsigned char *)malloc(TEST_LEN);
	if (buf == NULL) {
		return;
	}

	memset(buf, TEST_DATA, TEST_LEN);

	FILE *file = fopen(TEST_FILE, "w+");
	fclose(file);
	for (unsigned int i = 0; i < TEST_LOOP; i++) {
		WriteFileStream(i * TEST_LEN, buf, TEST_LEN);
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
		ptr = "** unknown mode **";
	}
	LOG_TRACE(NULL, "%s", ptr);
	return 0;
}

int GetPrivilege(char *name)
{
	if (access(name, R_OK) < 0) {
		LOG_TRACE(NULL, "access error");
	} else {
		LOG_TRACE(NULL, "read access OK");
	}

	if (open(name, O_RDONLY) < 0) {
		LOG_TRACE(NULL, "open error");
	} else {
		LOG_TRACE(NULL, "open for reading OK");
	}

	return 0;
}

int UmaskTest()
{
	umask(0);
	if (creat("foo", (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)) < 0) {
		LOG_TRACE(NULL, "creat error for foo");
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (creat("bar", (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)) < 0) {
		LOG_TRACE(NULL, "creat error for bar");
	}

	struct stat statbuf;
	if (stat("foo", &statbuf) < 0) {
		LOG_TRACE(NULL, "stat error for foo");
	}
	if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
		LOG_TRACE(NULL, "chmod error for foo");
	}
	if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
		LOG_TRACE(NULL, "chmod error for bar");
	}

	return 0;
}

int TestUnlink()
{
	if (open(TEST_FILE, O_RDWR | O_CREAT) < 0) {
		LOG_TRACE(NULL, "open error");
	}

	if (unlink(TEST_FILE) < 0) {
		LOG_TRACE(NULL, "unlink error");
	}
	
	LOG_TRACE(NULL, "file unlinked\n");

	return 0;
}

int KeepAccessTime()
{
	struct stat statbuf;
	if (stat(TEST_FILE, &statbuf) < 0) {
		LOG_TRACE(NULL, "%s: stat error", TEST_FILE);
		return -1;;
	}

	int fd;
	if ((fd = open(TEST_FILE, O_RDWR | O_TRUNC)) < 0) {
		LOG_TRACE(NULL, "%s: open error", TEST_FILE);
		return -1;;
	}

	struct timespec times[2];
	times[0] = statbuf.st_atim;
	times[1] = statbuf.st_mtim;
	if (futimens(fd, times) < 0) {
		LOG_TRACE(NULL, "%s: futimens error", TEST_FILE);
		close(fd);
		return -1;
	}

	close(fd);

	return 0;
}

#define FTW_F 1
#define FTW_D 2
#define FTW_DNR 3
#define FTW_NS 4

typedef int Myfunc(const char *, const struct stat *, int);

//Myfunc myfunc;
//int myftw(char *, Myfunc *);
//int dopath(Myfunc *);

long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int myfunc(const char *pathname, const struct stat *statptr, int type)
{
	switch (type) {
		case FTW_F:
			switch (statptr->st_mode & S_IFMT) {
				case S_IFREG:
					nreg++;
					break;
				case S_IFBLK:
					nblk++;
					break;
				case S_IFCHR:
					nchr++;
					break;
				case S_IFIFO:
					nfifo++;
					break;
				case S_IFLNK:
					nslink++;
					break;
				case S_IFSOCK:
					nsock++;
					break;
				case S_IFDIR:
					LOG_TRACE(NULL, "for S_IFDIR for %s", pathname);
			}
			break;
		case FTW_D:
			ndir++;
			break;
		case FTW_DNR:
			LOG_TRACE(NULL, "can’t read directory %s", pathname);
			break;
		case FTW_NS:
			LOG_TRACE(NULL, "stat error for %s", pathname);
			break;
		default:
			LOG_TRACE(NULL, "unknown type %d for pathname %s", type, pathname);
	}
	return 0;
}

char *fullpath;
size_t pathlen = BUF_SIZE;

int dopath(Myfunc* func)
{
	struct stat statbuf;
	if (lstat(fullpath, &statbuf) < 0) {
		return func(fullpath, &statbuf, FTW_NS);
	}
	if (S_ISDIR(statbuf.st_mode) == 0) {
		return(func(fullpath, &statbuf, FTW_F));
	}
	int ret;
	if ((ret = func(fullpath, &statbuf, FTW_D)) != 0) {
		return ret;
	}
	int n = strlen(fullpath);
	if (n + NAME_MAX + 2 > pathlen) {
		pathlen *= 2;
		if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
			LOG_TRACE(NULL, "realloc failed");
		}
	}
	fullpath[n++] = '/';
	fullpath[n] = 0;

	DIR *dp;
	if ((dp = opendir(fullpath)) == NULL) {
		return(func(fullpath, &statbuf, FTW_DNR));
	}

	struct dirent *dirp;
	while ((dirp = readdir(dp)) != NULL) {
		if (strcmp(dirp->d_name, ".") == 0 || strcmp(dirp->d_name, "..") == 0) {
			continue;
		}
		strcpy(&fullpath[n], dirp->d_name);
		if ((ret = dopath(func)) != 0) {
			break;
		}
	}
	fullpath[n-1] = 0;
	if (closedir(dp) < 0) {
		LOG_TRACE(NULL, "can’t close directory %s", fullpath);
	}

	return(ret);
}

int myftw(char *pathname, Myfunc *func)
{
	fullpath = malloc(BUF_SIZE);
	if (pathlen <= strlen(pathname)) {
		pathlen = strlen(pathname) * 2;
		if ((fullpath = realloc(fullpath, pathlen)) == NULL) {
			LOG_TRACE(NULL, "realloc failed");
		}
	}
	strcpy(fullpath, pathname);
	return dopath(func);
}

int traversesDir()
{
	int ret;
	ret = myftw("/home/yihelsc", myfunc); /* does it all */
	ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;
	if (ntot == 0) {
		ntot = 1;
	}
	LOG_TRACE(NULL, "regular files = %7ld, %5.2f %%\n", nreg, nreg * 100.0 / ntot);
	LOG_TRACE(NULL, "directories = %7ld, %5.2f %%\n", ndir, ndir * 100.0 / ntot);
	LOG_TRACE(NULL, "block special = %7ld, %5.2f %%\n", nblk, nblk * 100.0 / ntot);
	LOG_TRACE(NULL, "char special = %7ld, %5.2f %%\n", nchr, nchr * 100.0 / ntot);
	LOG_TRACE(NULL, "FIFOs = %7ld, %5.2f %%\n", nfifo, nfifo * 100.0 / ntot);
	LOG_TRACE(NULL, "symbolic links = %7ld, %5.2f %%\n", nslink, nslink * 100.0 / ntot);
	LOG_TRACE(NULL, "sockets = %7ld, %5.2f %%\n", nsock, nsock * 100.0 / ntot);
	return ret;
}

int TestChdir()
{
	if (chdir("/home/test_user") < 0) {
		LOG_TRACE(NULL, "chdir failed");
	}
	char *ptr = malloc(BUF_SIZE);
	if (getcwd(ptr, BUF_SIZE) == NULL) {
		LOG_TRACE(NULL, "getcwd failed");
	}
	LOG_TRACE(NULL, "cwd = %s\n", ptr);

	return 0;
}

int PrintDevId()
{
	struct stat buf;
	if (stat("/dev/sda1", &buf) < 0) {
		LOG_TRACE(NULL, "stat error");
		return -1;
	}
	LOG_TRACE(NULL, "dev = %d/%d", major(buf.st_dev), minor(buf.st_dev));
	if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)) {
		LOG_TRACE(NULL, " (%s) rdev = %d/%d", (S_ISCHR(buf.st_mode)) ? "character" : "block", major(buf.st_rdev), minor(buf.st_rdev));
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
#include <errno.h>

int ReadWriteStdin()
{
	int n;
	char buf[BUF_SIZE];
	while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
		if (write(STDOUT_FILENO, buf, n) != n) {
			printf("write error errno is %d \n", errno);
			return -1;
		}
	}

	if (n < 0) {
		printf("read error errno is %d \n", errno);
		return -1;
	}

	return 0;
}

int ListDir()
{
	DIR *dp;
	struct dirent *dirp;

	if ((dp = opendir("/home/yihelsc")) == NULL) {
		printf("can’t open %s errno is %d \n", "/home/yihelsc", errno);
		return -1;
	}

	while ((dirp = readdir(dp)) != NULL) {
		printf("%s\n", dirp->d_name);
	}

	closedir(dp);
	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		LOG_TRACE(NULL, "Please Input: File_Operate Cmd");
		return -1;
	}
	if (strcmp(argv[1], "WriteFileDirect") == 0) {
		WriteFileDirect();
		return 0;
	}

	if (strcmp(argv[1], "PrintDevId") == 0) {
		PrintDevId();
		return 0;
	}

	LOG_TRACE(NULL, "Please Input: File_Operate Cmd. Cmd is below:");
	LOG_TRACE(NULL, "WriteFileDirect");
	LOG_TRACE(NULL, "ReadFileDirect");
	LOG_TRACE(NULL, "TestWriteFileStream");
	LOG_TRACE(NULL, "WriteFileHole");
	LOG_TRACE(NULL, "GetFileType");
	LOG_TRACE(NULL, "GetPrivilege");
	LOG_TRACE(NULL, "UmaskTest");
	LOG_TRACE(NULL, "TestUnlink");
	LOG_TRACE(NULL, "KeepAccessTime");
	LOG_TRACE(NULL, "traversesDir");
	LOG_TRACE(NULL, "TestChdir");
	LOG_TRACE(NULL, "PrintDevId");
	//ReadFileDirect();
	//TestWriteFileStream();
	//WriteFileHole();
	//GetFileType("/etc/passwd");
	//GetFileType("/dev/tty");
	//GetPrivilege("/etc/shadow");
	//UmaskTest();
    //TestUnlink();
    //KeepAccessTime();
    //traversesDir();
    //TestChdir();
    return 0;
}
