#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
	DIR *dp;
	struct dirent *dirp;
	if (argc != 2) {
		printf("usage: test directory_name \n");
		return -1;
	}

	if ((dp = opendir(argv[1])) == NULL) {
		printf("can’t open %s errno is %d \n", argv[1], errno);
		return -1;
	}

	while ((dirp = readdir(dp)) != NULL) {
		printf("%s\n", dirp->d_name);
	}

	closedir(dp);
	return 0;
}
