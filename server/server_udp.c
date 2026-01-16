#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, portno;
     int n;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     // 1. Create socket with SOCK_DGRAM (UDP)
     sockfd = socket(AF_INET, SOCK_DGRAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     // 2. Bind is still required for the server
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
              
     printf("UDP Server is running on port %d...\n", portno);

     // 3. Infinite Loop (No listen/accept needed)
     while(1) {
         clilen = sizeof(cli_addr);
         bzero(buffer,256);
         
         // Receive message AND capture client address
         n = recvfrom(sockfd, buffer, 255, 0, (struct sockaddr *) &cli_addr, &clilen);
         if (n < 0) error("ERROR on recvfrom");
         
         printf("Received packet from client: %s\n", buffer);
         
         // Send reply to the captured client address
         n = sendto(sockfd, "I got your UDP message", 22, 0, (struct sockaddr *) &cli_addr, clilen);
         if (n < 0) error("ERROR on sendto");
     }
     
     close(sockfd);
     return 0;
}