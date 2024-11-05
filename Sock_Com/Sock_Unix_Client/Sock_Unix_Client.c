#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define DATA_LEN_MAX 128

int main(int argc, const char *argv[])
{
	int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		printf("socket error errno %d \n", errno);
		return -1;
	}

	struct sockaddr_un client_addr = {0};
	client_addr.sun_family = AF_UNIX; 
	strcpy(client_addr.sun_path, "./local_client");
	socklen_t client_addr_len = sizeof(client_addr);

	if (bind(sockfd, (struct sockaddr *)&client_addr, client_addr_len) == -1) {
		printf("bind error errno %d \n", errno);
		return -1;
	}

	struct sockaddr_un server_addr = {0};
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "./local_server");
	socklen_t server_addr_len = sizeof(server_addr);

	char buff[DATA_LEN_MAX] = {0};
	while (1) {
		printf("Please Input Message: ");
		fgets(buff,sizeof(buff), stdin);
		buff[strlen(buff) - 1] = '\0';

		if (sendto(sockfd, buff,sizeof(buff), 0, (struct sockaddr *)&server_addr, server_addr_len) == -1) {
			printf("sendto error errno %d \n", errno);
			break;
		}

		if (recvfrom(sockfd, buff,sizeof(buff), 0, (struct sockaddr *)&server_addr, &server_addr_len) == -1) {
			printf("recvfrom error errno %d \n", errno);
			break;
		}

		printf("Message from Server [%s] \n", buff);
	}

	close(sockfd);
	remove("./local_client");

	return 0;
}