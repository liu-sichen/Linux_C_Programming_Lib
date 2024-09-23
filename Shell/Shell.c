#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUFF_LEN 128

int main()
{
	char buff[BUFF_LEN] = {0};
	while (1) {
		printf("yihelscBash>");
		fflush(stdout);

		fgets(buff, BUFF_LEN, stdin);
		buff[strlen(buff) - 1] = 0;

		system(buff);
	}
}
