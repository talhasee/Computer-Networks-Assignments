/*
	CODE FOR SEQUENTIAL CLIENT SENDING REQUESTS
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888

int clientSocket, ret;
struct sockaddr_in serverAddr;
char buffer[1024];

int create_socket(int clientSocket){
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // or INADDR_ANY
	return clientSocket;
}
int main(){
	
	printf("[+]Connected to Server.\n");
	int i = 0; //, j = 0;
	for(int j = 0; j < 10; j++){
		clientSocket = create_socket(clientSocket);
		ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
		if(ret < 0){
			printf("[-]Error in connection..\n");
			exit(1);
		}
		i = 0;
		for(i = 1; i <= 20; i++){
			// printf("Client: \t");
			// scanf("%s", &buffer[0]);
			sprintf(buffer, "%d", i);
			send(clientSocket, buffer, strlen(buffer), 0);

			// if(strcmp(buffer, ":exit") == 0){
			// 	close(clientSocket);
			// 	printf("[-]Disconnected from server.\n");
			// 	exit(1);
			// }
			bzero(buffer, sizeof(buffer));
			if(recv(clientSocket, buffer, 1024, 0) < 0){
				printf("[-]Error in receiving data.\n");
			}else{
				printf("Server: \t%s\n", buffer);
			}
		}
		close(clientSocket);
	}

	return 0;
}
