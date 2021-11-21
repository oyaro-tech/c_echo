#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 11715

int main(void) {
	struct sockaddr_in address;

	int server_fd = 0;
	int client_fd = 0;
	int opt = 1;
	int addr_len = sizeof(address);

	char buffer[2048] = {0};

	printf("Starting echo server...\n");

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Socket creation failed\n");
		exit(EXIT_FAILURE);
	}
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("Setsockopt failed\n");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) <0 ) {
		perror("Bind failed\n");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 10) < 0 ) {
		perror("Listen failed\n");
		exit(EXIT_FAILURE);
	}

	for(;;) {

		if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addr_len)) < 0) {
			perror("Client connection failed\n");
			exit(EXIT_FAILURE);
		}

		char *client_ip = inet_ntoa(address.sin_addr);
		int msg_size = read(client_fd, buffer, 2048);
		printf("[%s:%d] %s\n", client_ip, ntohs(address.sin_port), buffer);

		send(client_fd, buffer, msg_size, 0);

		close(client_fd);
	}

	close(server_fd);

	return EXIT_SUCCESS;
}
