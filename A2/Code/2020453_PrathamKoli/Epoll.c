/****************CODE FOR EPOLL SERVER********************/
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <stdlib.h>

#define PORT 8888
#define SIZE 1024
#define MAX_CLIENTS 10
#define MAX_EVENTS 10
const int zero = 0;
struct sockaddr_in cliaddr;

int create_Socket(int sockfd){
	pid_t childpid;
    struct sockaddr_in serverAddr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("Error in connection.\n");
		exit(1);
	}
	printf("*********Server created.********\n");
    //Declaring Internet protocol, port and IP address
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);
	serverAddr.sin_family = AF_INET;
	
	if(bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) >= 0){
        printf("Bind to port %d\n", 8888);
    }
    else{
		printf("Error in binding.\n");
		return 1;
	}

	if(listen(sockfd, 20) == 0){
		printf("Listening....\n");
	}else{
		printf("Error in binding.\n");
        return 1;
	}
	return sockfd;
}

int main()
{
    int SERVERSOCK = create_Socket(0);

    struct epoll_event ev, events[MAX_EVENTS];
    int EPfd;
    EPfd = epoll_create1(0);
    if (EPfd < 0)
    {
        perror("epoll_create1");
    }
    ev.data.fd = SERVERSOCK;
    ev.events = EPOLLIN;
    
    if (epoll_ctl(EPfd, EPOLL_CTL_ADD, SERVERSOCK, &ev) < -1)
    {
        printf("ERROR in epoll_ct\n");
        return 1;
    }
    int m = 0;
    int CL_Sockets[MAX_CLIENTS];
    
    while(m < MAX_EVENTS)
    {
        CL_Sockets[m] = 0;
        m++;
    }
    int CLIENTSOCKET, FD;
    for(;;)
    {
        FILE *ptr;
        // ptr = fopen("EP.txt", "a");
        //Assigning wait values
        FD = epoll_wait(EPfd, events, MAX_EVENTS, -1);
        if (FD == -1)
        {
            printf("Error in epoll_wait\n");
            return 1;
        }
        int j = 0;
        while(j < FD)
        {
            if (events[j].data.fd == SERVERSOCK)
            {
                // struct sockaddr_in cliaddr;
                int addrlen = sizeof(cliaddr);
                CLIENTSOCKET = accept(SERVERSOCK, (struct sockaddr *)&cliaddr, &addrlen);
                if (CLIENTSOCKET == -1)
                {
                    perror("accept");
                }
                int k = 0;
                while(k < MAX_CLIENTS)
                {
                    
                    if (CL_Sockets[k] == zero)
                    {
                        CL_Sockets[k] = CLIENTSOCKET;
                        // Updating descriptors
                        //and flags
                        ev.data.fd = CL_Sockets[k];
                        ev.events = EPOLLIN | EPOLLET;
                        

                        if (epoll_ctl(EPfd, EPOLL_CTL_ADD, CL_Sockets[k], &ev) == -1)
                        {
                            printf("Error in epoll_ctl line 117\n");
                            return 1;
                        }
                        break;
                    }
                    k++;
                }
            }
            int l = 0;
            while(l < MAX_CLIENTS)
            {
                char buf[SIZE];
                if ((CL_Sockets[l] > 0 )&& (events[j].data.fd == CL_Sockets[l]))
                {   
                    

                    struct SEND{
                        struct sockaddr_in data;
                        int num;
                    };
                    struct SEND *BUF = malloc(sizeof(struct SEND));
                    // int BUF_SIZE = read(CL_Sockets[i], buf, SIZE - 1);
                    int BUF_SIZE = read(CL_Sockets[l], buf, sizeof(buf));
                    printf("BUF_SIZE %d\n", BUF_SIZE);
                    // write(CL_Sockets[i], buf, strlen(buf));
                    // sprintf(buf, "%d", BUF->num);
                    if (BUF_SIZE < 0)      epoll_ctl(EPfd, EPOLL_CTL_DEL, CL_Sockets[l], NULL);
                    else if (BUF_SIZE == zero)  epoll_ctl(EPfd, EPOLL_CTL_DEL, CL_Sockets[l], NULL);
                    else
                    {
                        ptr = fopen("EP.txt", "a");
                        write(CL_Sockets[l], buf, strlen(buf));
                        printf("Client Data : %s IP: %s Port : %d", buf, inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                        
                        fprintf(ptr, "Factorial: %s IP : %s Port : %d\n", buf, inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
                        
                        buf[BUF_SIZE] = '\0';
                        bzero(buf, sizeof(buf));
                        printf("From client: %s\n", buf);
                        fclose(ptr);
                    }
                }
                l++;
            }
            j++;
        }
        printf("BAHAR AAGYA\n");
        // fclose(ptr);
    }

    printf("server end\n");

    close(SERVERSOCK);

    return 0;
}

