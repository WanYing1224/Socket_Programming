#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/un.h>		// Required for Unix Domain Sockets

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, servlen;
     socklen_t clilen;
     struct sockaddr_un serv_addr, cli_addr;
     int n;
     char buffer[256];

     // 1. Create Socket (AF_UNIX) 
     sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     
     // 2. Setup Address Structure 
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sun_family = AF_UNIX;
     strcpy(serv_addr.sun_path, "socket_file"); /* The "file" we use to connect */
     
     // Delete the file if it already exists, or bind will fail 
     unlink("socket_file");

     servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

     // 3. Bind
     if (bind(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0) 
              error("ERROR on binding");

     // 4. Listen 
     listen(sockfd,5);
     printf("Unix Domain Server waiting for connection on file: socket_file\n");

     clilen = sizeof(cli_addr);

     // 5. Accept 
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     
     // 6. Read/Write 
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     
     n = write(newsockfd,"I got your Unix message",23);
     if (n < 0) error("ERROR writing to socket");

     close(newsockfd);
     close(sockfd);
     return 0;
}