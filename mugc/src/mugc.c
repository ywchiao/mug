/**
 *  @file       mugc.c
 *  @brief      The entry point of MUG.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       03/23/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The entry point of MUG.
 */

#include "mugc.h"

int main(int argc, char *argv[]) {
    int fd_socket;

    char buf_send[BUF_SIZE];
    char buf_recv[BUF_SIZE];

    struct sockaddr_in server_addr;

    fd_socket=socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(strtol(argv[2], NULL, 0));

    // argv[1]: ip address, for example: 127.0.0.1
    inet_pton(AF_INET, argv[1], &(server_addr.sin_addr));

    connect(fd_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    while (true) {
        memset(buf_send, 0, BUF_SIZE);
        memset(buf_recv, 0, BUF_SIZE);

        read(fd_socket, buf_recv, BUF_SIZE);
        printf("%s\n", buf_recv);

        fgets(buf_send, BUF_SIZE, stdin);
        write(fd_socket, buf_send, strlen(buf_send)+1);
    } // while
} // main

// mugc.c
