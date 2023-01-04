#include<stdio.h>
#include<string.h>
#include<sys/select.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
#define MAX 1024
#define PORT 8888
#define MAXCON 11
double total = 0;
pthread_mutex_t mutex;
FILE *ptr;
long factorial(long x);
int main()
{
    int Sockfd, cliAddrLength, flag = 1, *newclient_Socket, returnS;
    struct sockaddr_in serverAddr, cliAddr; 
    cliAddrLength = sizeof(struct sockaddr_in);
    
    Sockfd = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
    if (Sockfd < 0) {
        perror("Server not created\n");
        exit(0);
    }
    int sockopt = setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    if (sockopt < 0) {
        perror("ERROR in setsockopt\n");
        exit(0);
    }
    printf("Server Created\n");
    memset(&serverAddr, '\0', sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(8888);
    if(!(bind(Sockfd,(struct sockaddr *)&serverAddr , sizeof(serverAddr)) < 0)) {
        printf("Bind to port 8888\n");
    }else{
        perror("Bind failed!\n");
        return 1;
    }
    
	if ((listen(Sockfd, 20)) == 0) {
        printf("Listening for connections\n");
    }
    else{
		perror("Listen failed!\n");
		exit(0);
    }
    int ClientCONN[MAXCON + 1];
    fd_set fdread;
    int m = 0;
    while(m < MAXCON){
        ClientCONN[m] = -1;
        m++;
    }
    m = 0 ;
    ClientCONN[m] = Sockfd;
    pthread_mutex_init(&mutex, NULL);
    for(;;){  
        FD_ZERO(&fdread);
        m = 0;
        while(m < MAXCON){
            if (ClientCONN[m] >= 0) {
                int sd = ClientCONN[m];
                FD_SET(sd, &fdread);
            }
            m++;
        }
        returnS = select(FD_SETSIZE, &fdread, NULL, NULL, NULL);
        if (returnS >= 0) {
            if (FD_ISSET(Sockfd, &fdread)) {
                int c = sizeof(struct sockaddr_in);
                int client_Socket = accept(Sockfd, (struct sockaddr*) &cliAddr, &cliAddrLength);
                if (client_Socket >= 0) {
                    printf("Accepted connection from IP : %s Port : %d\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
                    m = 0;
                    while(m < MAXCON){
                        if (ClientCONN[m] < 0) {
                            ClientCONN[m] = client_Socket;
                            break;
                        }
                        m++;
                    }
                }
                else {
                    printf("Accept failed Successfully!!!!......\n");
                    exit(0);
                }
                returnS--;
                if (returnS == 0){ 
                    continue;
                }
            }
            m = 1;
            while(m < MAXCON) {
               char buffer[MAX]; 
                if ((ClientCONN[m] > 0) && (FD_ISSET(ClientCONN[m], &fdread))) {
                     
                    int client_Socket = ClientCONN[m];
                    
                    memset(buffer, '\0', sizeof(buffer));
                    int returnS = read(client_Socket, buffer, sizeof(buffer));
                    if (returnS == 0) {
                        printf("Disconnected from IP: %s Port: %d\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
                        close(ClientCONN[m]);
                        ClientCONN[m]=-1;
                    }
                    if (returnS > 0) {
                        printf("Client sent: %s\n", buffer);
                        long long temp = atoi(buffer);
                        long fact = factorial(temp);
                        bzero(buffer, sizeof(buffer));
                        sprintf(buffer, "%ld", fact);
                        // pthread_mutex_lock(&mutex);
                        ptr = fopen("SLS.txt", "a");
                        fprintf(ptr, "Factorial %ld IP:  %s Port: %d\n", factorial(temp), inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
                        fclose(ptr);
                        // pthread_mutex_unlock(&mutex);
                        write(client_Socket, buffer, sizeof(buffer));
                        bzero(buffer, sizeof(buffer));
                    }
                    if (returnS == -1) {
                        perror("ERROR IN READING");
                        break;
                    }
                }
                returnS--;
                if (returnS == 0) {
                    m++;
                    continue;
                }
                m++;
            }
        }
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
long factorial(long x) {
    long fact = 1;
    for (int i = 2; i <= x; i++) {
        fact *= i;
    }
    return fact;
}
