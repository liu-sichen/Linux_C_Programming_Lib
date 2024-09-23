#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define BUFF_SIZE 4096

int main(int argc, char *argv[])
{
	int n;
	char buf[BUFF_SIZE];
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
