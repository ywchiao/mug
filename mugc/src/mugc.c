/**
 *  @file       mugc.c
 *  @brief      The entry point of MUG.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       03/30/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The entry point of MUG.
 */

#include "mugc.h"

/**
 *  Connect to server ip:port and return the socket.
 *
 *  @param[in]  ip   伺服器 ip 地址
 *  @param[in]  port 伺服器通訊埠 (port) 號碼
 *
 *  @return     通訊 socket 的 File Descripter, FD
 **/
int socket_to_server(char *ip, int port) {
    int fd_socket;
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family=AF_INET;
    server_addr.sin_port = htons(port);

    inet_pton(AF_INET, ip, &(server_addr.sin_addr));

    fd_socket = socket(AF_INET, SOCK_STREAM, 0);

    connect(fd_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    return fd_socket;
} // socket_to_server()

/**
 *  Printout the usage.
 *
 *  @param  none.
 *
 *  @return none.
 **/
void usage(void) {
    printf("usage: \n");
    printf("    mugc ip port\n");

    exit(0);
} // usage()

/**
 *  MugC 客戶端程式進入點。
 *
 *  @param[in]  argc 命令列參數個數。
 *  @param[in]  argv 命令列參數陣列；每個參數都是一個 string。
 *
 *  @return     程式結束狀態
 **/
int main(int argc, char *argv[]) {
    int fd_server;

    if (argc != 3) {
        usage();
    } // fi

    fd_server = socket_to_server(argv[1], strtol(argv[2], NULL, 0));

    while (true) {
        char buf_send[BUF_SIZE];
        char buf_recv[BUF_SIZE];

        memset(buf_send, 0, BUF_SIZE);
        memset(buf_recv, 0, BUF_SIZE);

        read(fd_server, buf_recv, BUF_SIZE);
        printf("%s\n", buf_recv);

        fgets(buf_send, BUF_SIZE, stdin);
        write(fd_server, buf_send, strlen(buf_send)+1);
    } // while
} // main

// mugc.c
