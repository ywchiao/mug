/**
 *  @file       mugd.c
 *  @brief      The entry point of MUG server (daemon).
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       03/08/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The entry point of MUG server (daemon).
 */

#include "mugd.h"

int main(int argc, char *argv[]) {
    int fd_socket;
    int fd_comm;
    char buffer[BUF_SIZE];
    struct sockaddr_in server_addr;

    char *welcome = "Welcome to the world of MUG.\n";

    printf("MUG server started at %s:%s ...\n", "127.0.0.1", argv[1]);

    fd_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(strtol(argv[1], NULL, 0));

    bind(fd_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(fd_socket, 10);

    fd_comm = accept(fd_socket, (struct sockaddr *)NULL, NULL);

    write(fd_comm, welcome, strlen(welcome) + 1);
    printf("send -> %s", welcome);

    while (true) {
        memset(buffer, 0, BUF_SIZE);
 
        read(fd_comm, buffer, BUF_SIZE);
        printf("recv <- %s", buffer);

        write(fd_comm, buffer, strlen(buffer) + 1);
        printf("send -> %s", buffer);
    } // while
} // main()

// mugd.c
