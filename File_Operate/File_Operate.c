#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>

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

int main()
{
	WriteFileDirect();
	ReadFileDirect();
    //TestWriteFileStream();
    return 0;
}
