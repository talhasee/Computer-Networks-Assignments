/****************CODE FOR SEQUENTIAL SERVER********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
int sockfd, ret;
struct sockaddr_in serverAddr;

int newSocket;
struct sockaddr_in newAddr;
char buffer[1024];
socklen_t addr_size;
long long factorial(long long n)
{
    long long res = 1, i;
    for (i = 2; i <= n; i++)
        res *= i;
    return res;
}
int create_Socket(int sockfd){
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", 4444);

	if(listen(sockfd, 20) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}
	return sockfd;
}

int main(){
	sockfd = create_Socket(sockfd);
	// FILE *ptr;
	// ptr = fopen("SS.txt", "a");
	
	// else{
		int clientnum = 0;
		for(;;){
			newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
			if(newSocket < 0){
				exit(1);
			}
			FILE *ptr;
			ptr = fopen("SS.txt", "a");
			if(ptr == NULL){
				printf("Failed to open the file\n");
			}
			// printf("Client send data %d\n", ++clientnum);
			printf("Connection accepted from %s: %d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
			int read_size;
			for(;;){
				recv(newSocket, buffer, 1024, 0);
				int n = atoi(buffer);
				if(strlen(buffer) == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Client: %s\n", buffer);
					// if(fputs(buffer, ptr) >= 0)
      				// 	printf("String written to the file successfully...");
					long long temp = atoi(buffer);
					fprintf(ptr, "Factorial - %lld, IP - %s, Port - %d\n", factorial(temp), inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					// printf("FILE written STAT : %d\n", ret);
					//Send factorial to client here
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
			// if(read_size)
			close(newSocket);
			fclose(ptr);
	}
	
	close(sockfd);
	return 0;
}
