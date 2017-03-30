/**
 *  @file       mugd.c
 *  @brief      The entry point of MUG server (daemon).
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       03/30/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The entry point of MUG server (daemon).
 */

#include "mugd.h"

/**
 *  Set up and return the ServerSocket file descripter.
 *
 *  @param[in]  port 伺服器通訊埠 (port) 號碼
 *
 *  @return     ServerSocket 的 FileDescripter (FD)
 **/
int socket_server(int port) {
    int fd_socket;
    struct sockaddr_in server_addr;

    fd_socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);

    bind(fd_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(fd_socket, 10);

    return fd_socket;
} // socket_server()

/**
 *  Printout the usage.
 *
 *  @param  none.
 *
 *  @return none.
 **/
void usage(void) {
    printf("usage: \n");
    printf("    mugd port\n");

    exit(0);
} // usage()

/**
 *  MugD 伺服端程式進入點。
 *
 *  @param[in]  argc 命令列參數個數。
 *  @param[in]  argv 命令列參數陣列；每個參數都是一個 string。
 *
 *  @return     程式結束狀態
 **/
int main(int argc, char *argv[]) {
    char *welcome = "Welcome to the world of MUG.\n";
    int fd_server;
    int fd_client;

    if (argc != 2) {
        usage();
    } // fi

    fd_server = socket_server(strtol(argv[1], NULL, 0));

    printf("MUG server started at %s:%s ...\n", "127.0.0.1", argv[1]);

    fd_client = accept(fd_server, (struct sockaddr *)NULL, NULL);

    write(fd_client, welcome, strlen(welcome) + 1);
    printf("send -> %s", welcome);

    while (true) {
        char buffer[BUF_SIZE];

        memset(buffer, 0, BUF_SIZE);

        read(fd_client, buffer, BUF_SIZE);
        printf("recv <- %s", buffer);

        write(fd_client, buffer, strlen(buffer) + 1);
        printf("send -> %s", buffer);
    } // while
} // main()

// mugd.c
