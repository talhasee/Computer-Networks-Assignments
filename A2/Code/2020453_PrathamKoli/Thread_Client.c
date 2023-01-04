#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h> 
#define MAX_SIZE 1024

int clientSocket, ret;
struct sockaddr_in serverAddr;
char buffer[1024];

#define PORT 8888
void* connection_handler(void *sock_desc);
int create_Socket(int clientSocket){
	
	//Create a socket
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");
	
	//Prepare the sockaddr structure
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// inet_aton(ipAddr, &serverAddr.sin_addr.s_addr);
	return clientSocket;
}

int main(){
	
	// pthread_t thread_id;
	pthread_t th[20];
	for(int j = 1; j <= 10; j++){
		int sock_desc, new_socket, c, *new_sock, i;
		
		if(pthread_create(th+j, NULL, &connection_handler, NULL) < 0){
			perror("Could not create thread");
			return 1;
		}
	}
	// for (int i = 1; i <= 12; i++) {
    //     if (pthread_join(th[i], NULL) != 0) {
    //         return 2;
    //     }
    //     printf("Thread %d has finished execution\n", i);
    // }
	pthread_exit(NULL);
	return 0;
}
/**********Connection Handler for each thread**************/
void *connection_handler(void *threadid){
	int threadnum = (int)threadid;
	int sock_desc;

	struct sockaddr_in serv_addr;
	char sbuff[MAX_SIZE], rbuff[MAX_SIZE];

	if((sock_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Failed creating socket \n");
		exit(1);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

	if(connect(sock_desc, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		printf("Failed to connect to Server\n");
		exit(1);
	}
	// int tid;
	// tid = pthread_getthreadid_np();
	printf("Connected succesfully client: %ld\n", pthread_self());
	int i = 1;	
	while(i <= 20){
		printf("For thread : \t%ld \n", pthread_self());
		// fgets(sbuff, MAX_SIZE, stdin);
		sprintf(sbuff, "%d", i);
		send(sock_desc, sbuff, strlen(sbuff), 0);
		int stat = -1;
		if((stat = recv(sock_desc, rbuff, MAX_SIZE, 0)) < 0){
			printf("error");
		}
		else
			printf("Server : %s\n", rbuff);
		// printf("stat - %d\n", stat);
		//	fputs(rbuff, stdout);
		bzero(rbuff, MAX_SIZE);
		// sleep(1);
		i++;
	}
	// for(int i = 1; i <= 20; i++){
	// 	sprintf(buffer, "%d", i);
	// 	send(sock_desc, buffer, strlen(buffer), 0);

	// 	bzero(buffer, sizeof(buffer));
	// 	if(recv(sock_desc, buffer, 1024, 0) < 0){
	// 		printf("Error in receiving data. \n");
	// 	}
	// 	else{
	// 		printf("Server: \t%s\n", buffer);
	// 	}
	// }
	close(sock_desc);
	return 0;
}
	//clientSocket = create_Socket(clientSocket);
	// ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	// if(ret < 0){
	// 	printf("[-]Error in connection.\n");
	// 	exit(1);
	// }
	// printf("[+]Connected to Server.\n");
	// puts("");
	// int i = 0;
	// while(i != 1){
	// 		printf("Client: \t");
	// 		scanf("%s", &buffer[0]);
	// 		send(clientSocket, buffer, strlen(buffer), 0);

	// 		if(strcmp(buffer, ":exit") == 0){
	// 			close(clientSocket);
	// 			printf("[-]Disconnected from server.\n");
	// 			exit(1);
	// 		}

	// 		if(recv(clientSocket, buffer, 1024, 0) < 0){
	// 			printf("[-]Error in receiving data.\n");
	// 		}else{
	// 			printf("Server: \t%s\n", buffer);
	// 		}
	// 		i += 1;
	// }
