#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    int serverlen;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    
    portno = atoi(argv[2]);
    
    // 1. Create socket with SOCK_DGRAM (UDP)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    server = gethostbyname(argv[1]);
	
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    // 2. Setup the server address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    
    // 3. Send message using sendto (No connect!)
    serverlen = sizeof(serv_addr);
    n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &serv_addr, serverlen);
	
    if (n < 0) 
         error("ERROR sending to socket");
         
    // 4. Receive reply
    bzero(buffer,256);
	
    // Don't necessarily care who sent the reply for this simple test, so address can be NULL/0
    n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *) &serv_addr, &serverlen);
	
    if (n < 0) 
         error("ERROR receiving from socket");
         
    printf("Server reply: %s\n", buffer);
    
    close(sockfd);
    return 0;
}