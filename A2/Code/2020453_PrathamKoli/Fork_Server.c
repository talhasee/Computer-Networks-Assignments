/**************SERVER USING CLIENT*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>

#define PORT 8888
#define SIZE 1024
long long factorial(long long n)
{
    long long res = 1, i;
    for (i = 2; i <= n; i++)
        res *= i;
    return res;
}
int main(){
	char buffer[SIZE];
	int SocketFD, ret, newSocket;
	struct sockaddr_in serverAddr, NewServerAddr;
	socklen_t addr_size;	
	pid_t pid;
	SocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(SocketFD < 0){
		printf("Error in socket making!!!!....\n");
		exit(1);
	}

	printf("Socket Created..........\n");
	
	//Declaring Server IP, Port, Internet Protocol
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//Binding socket with port
	ret = bind(SocketFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}

	printf("Bind to port %d\n", 8888);

	if(listen(SocketFD, 20) == 0){
		printf(" Listening for Connections....\n");
	}else{
		printf(" Error in binding Socker.\n");
	}

	//Declaring Semaphore 
	sem_t sem;
	sem_init(&sem, 1, 1);
	while(1){
		newSocket = accept(SocketFD, (struct sockaddr*)&NewServerAddr, &addr_size);
		if(newSocket < 0){
			perror("Connection Accepting Error");
			exit(1);
		}
		printf("Connection accepted from IP: %s Port:%d\n", inet_ntoa(NewServerAddr.sin_addr), ntohs(NewServerAddr.sin_port));
		FILE *ptr;
		ptr = fopen("FS.txt", "a");
		if(ptr == NULL){
			printf("Failed to open the file\n");
		}
		if((pid = fork()) == 0){
			close(SocketFD);
			for(;;){
				bzero(buffer, sizeof(buffer));
				recv(newSocket, buffer, SIZE, 0);
				if(strlen(buffer) == 0){
					printf("Disconnected from IP: %s Port: %d\n", inet_ntoa(NewServerAddr.sin_addr), ntohs(NewServerAddr.sin_port));
					break;
				}else{
					long long temp;
					temp = atoi(buffer);
					sem_wait(&sem);
					fprintf(ptr, "Factorial %lld IP : %s Port : %d\n", factorial(temp), inet_ntoa(NewServerAddr.sin_addr), ntohs(NewServerAddr.sin_port));
					sem_post(&sem);
					printf("Client Send: %s\n", buffer);
					send(newSocket, buffer, SIZE, 0);
					bzero(buffer, sizeof(buffer));
				}
			}
			fclose(ptr);
		}
	}
	sem_destroy(&sem);
	close(newSocket);
	return 0;
}
