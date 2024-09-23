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
	
	struct sockaddr_un server_addr = {0};
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "./local_server");
	socklen_t server_addr_len = sizeof(server_addr);

	if (bind(sockfd, (struct sockaddr *)&server_addr, server_addr_len) == -1) {
		printf("bind error errno %d \n", errno);
		return -1;
	}

	struct sockaddr_un client_addr = {0};
	socklen_t client_addr_len = sizeof(client_addr);

	char buff[DATA_LEN_MAX];
	while (1) {
		memset(buff, 0, sizeof(buff));
		if (recvfrom(sockfd, buff,sizeof(buff), 0, (struct sockaddr *)&client_addr, &client_addr_len) == -1) {
			printf("recvfrom error errno %d \n", errno);
			break;
		}

		printf("client (%s) data [%s] \n", client_addr.sun_path, buff);
		
		strcat(buff, "--ack");

		if (sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&client_addr, client_addr_len) == -1) {
			printf("sendto error errno %d \n", errno);
			break;
		}
	}

	close(sockfd);
	remove("./local_server");

	return 0;
}