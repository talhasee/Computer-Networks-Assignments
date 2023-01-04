/********MULITHREADED SERVER*******/
 
#include<stdio.h>
#include<string.h>    
#include<stdlib.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    
#include<pthread.h> 
 
// thread function
void *connection_handler(void *);
pthread_mutex_t mutex;

struct Data
{
    int Client_sock;
    char *IP;
    uint16_t port;
};

long long factorial(long long n)
{
    long long res = 1, i;
    for (i = 2; i <= n; i++)
        res *= i;
    return res;
}
int main()
{
    int socketfd , client_sock , c, *tempptr;
    struct sockaddr_in server , client;
     
    //Create socket
    socketfd = socket(AF_INET , SOCK_STREAM , 0);
    if (socketfd == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Declaring Socket Internet Protocol, IP, and Port
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons( 8888 );
     
    //Binding socket with port
    if( bind(socketfd,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("rror in Bindind");
        exit(1);
    }
    puts("Bind to port 8888");
     
    listen(socketfd , 20);

    puts("Listening incoming connections...");
    c = sizeof(struct sockaddr_in);
	pthread_mutex_init(&mutex, NULL);
    while( (client_sock = accept(socketfd, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        pthread_t thread_id;
        tempptr = malloc(sizeof(char));
        *tempptr = client_sock;
        printf("Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        /*char temp[20], temp2[30];
        sprintf(temp, "%d", client_sock);
        sprintf(temp2, "%d", ntohs(server.sin_port));
        char data[4][1024];
        strcpy(data[0], temp);
        strcpy(data[1], inet_ntoa(server.sin_addr));
        strcpy(data[2], temp2);
	*/
        struct Data *temp = malloc(sizeof(struct Data));
        temp->Client_sock = client_sock;
        temp->IP = inet_ntoa(client.sin_addr);
        temp->port = ntohs(client.sin_port);

        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) temp) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        pthread_detach(thread_id); //detached
        puts("Handler assigned");
    }

    pthread_mutex_destroy(&mutex);
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }  
    pthread_exit(0);
}
void *connection_handler(void *socketfd)
{
    //Get the socket descriptor
    //start clock
    struct Data Client = *(struct Data*) socketfd;
    int sock = Client.Client_sock;
    int read_size;
    char *message , clientMessage[2000];
     
    //Receive a message from client
    // printf("Server IP : %s, Port : %d", Client.IP, Client.port);
    FILE *ptr;
    ptr = fopen("TS.txt", "a");
    while( (read_size = recv(sock , clientMessage , 2000 , 0)) > 0 )
    {
        
		clientMessage[read_size] = '\0';
	    printf("Message form Client, CS_ID - %s --- %d\t\n", clientMessage, sock);
        pthread_mutex_trylock(&mutex);
        //fputs(clientMessage, ptr);
        //fputs("\n", ptr);
        long long temp = atoi(clientMessage);
        fprintf(ptr, "Factorial - %lld, IP - %s, Port - %d\n", factorial(temp), Client.IP, Client.port);
        pthread_mutex_unlock(&mutex);

        write(sock , clientMessage , strlen(clientMessage));
        // sprintf(clientMessage, "%s %s", inet_ntoa(server.sin_addr), ntohs(server.sin_port));
        printf("Message - %s", clientMessage);
		bzero(clientMessage, sizeof(clientMessage));
		
    }
    fclose(ptr);
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    //end clock 
    return 0;
} 
