#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

int main(int args, char **argv) {
    int sock_fd;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("bind socket error");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);

    struct hostent *p_host = gethostbyname("www.baidu.com");
    printf("h_name: %s\n", p_host->h_name);
    printf("h_length: %d\n", p_host->h_length);
    printf("h_addrtype: %d\n", p_host->h_addrtype);
    const char *addr = inet_ntoa(*(struct in_addr *) p_host->h_addr_list[0]);
    // server_addr.sin_addr.s_addr = inet_addr("180.101.49.11");
    server_addr.sin_addr.s_addr = inet_addr(addr);

    int conn_stat = connect(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (conn_stat < 0) {
        printf("%d\n", errno);
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    // Only this line has been changed. Everything is same.
    char *hello = "GET / HTTP/1.1\nHost: www.baidu.com\nUser-Agent: pedro/7.54.0\nAccept: */*\n\r\n";
    write(sock_fd, hello, strlen(hello));
    char buf[2048];
    read(sock_fd, buf, sizeof(buf));
    fputs(buf, stdout);
    close(sock_fd);
    return 0;
}