#include <stdio.h>
#include <stdlib.h>     /* Fixes implicit declaration of exit(), atoi() */
#include <string.h>
#include <strings.h>    /* Fixes implicit declaration of bzero() */
#include <unistd.h>     /* Fixes implicit declaration of read(), write(), close() */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/wait.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

/* Function to handle individual client communication */
void dostuff(int sock)
{
    int n;
    char buffer[256];

    bzero(buffer, 256);
    n = read(sock, buffer, 255);
    if (n < 0) error("ERROR reading from socket");
    
    printf("Here is the message: %s\n", buffer);
    
    n = write(sock, "I got your message", 18);
    if (n < 0) error("ERROR writing to socket");
}

/* Zombie Process Handler to clean up dead child processes */
void signal_handler(int sig)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     /* Prevent Zombies */
     signal(SIGCHLD, signal_handler);

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     
     listen(sockfd, 5);
     clilen = sizeof(cli_addr);
     
     /* Infinite Loop to accept multiple connections */
     while (1) {
         newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         
         if (pid == 0)  {
             /* This is the CHILD process */
             close(sockfd); /* Child doesn't need the listener */
             dostuff(newsockfd);
             exit(0); /* Child must exit when done */
         }
         else {
             /* This is the PARENT process */
             close(newsockfd); /* Parent doesn't need this specific connection */
         }
     } /* End of while */
     
     close(sockfd);
     return 0; 
}