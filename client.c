#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 11715

int main(void) {
	struct sockaddr_in serv_addr;

	int server_fd = 0;

	char *msg = "Hello from client";
	
	char buffer[2048] = {0};

	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket creation failed\n");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		perror("Invalid address or not supported\n");
		exit(EXIT_FAILURE);
	}

	if(connect(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("Connection failed\n");
		exit(EXIT_FAILURE);
	}

	send(server_fd, msg, strlen(msg), 0);
	printf("Message sent\n");

	read(server_fd, buffer, 2048);
	printf("%s\n", buffer);

	return EXIT_SUCCESS;
}
