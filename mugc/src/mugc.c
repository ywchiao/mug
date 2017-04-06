/**
 *  @file       mugc.c
 *  @brief      The entry point of MUG.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       04/05/2017 last modified.
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

    fd_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

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
    struct pollfd fd[2];
    char buf_send[BUF_MSGS][BUF_SIZE] = {0};
    int msg_buffer = 0;
    int msg_sent = 0;

    if (argc != 3) {
        usage();
    } // fi

    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);

    fd[0].fd = STDIN_FILENO;
    fd[0].events = POLLIN;

    fd_server = socket_to_server(argv[1], strtol(argv[2], NULL, 0));

    fd[1].fd = fd_server;
    fd[1].events = POLLIN | POLLOUT;

    while (true) {
        int c = 0;
        char buf_recv[BUF_SIZE];

        c = poll(fd, 2, -1);

        if (c > 0) {
            if ((fd[0].revents & POLLIN) == POLLIN) {
               read(fd[0].fd, buf_send[msg_buffer], BUF_SIZE);

               msg_buffer = (msg_buffer + 1) % BUF_MSGS;
            } // fi

            if ((fd[1].revents & POLLIN) == POLLIN) {
                while (true) {
                    memset(buf_recv, 0, BUF_SIZE);

                    if (read(fd[1].fd, buf_recv, BUF_SIZE) > 0) {
                        printf("%s", buf_recv);
                    } // fi
                    else {
                        break;
                    } // esle
                } // od
            } // fi

            if ((fd[1].revents & POLLOUT) == POLLOUT) {
                if (msg_sent != msg_buffer) {
                    write(
                        fd[1].fd,
                        buf_send[msg_sent],
                        strlen(buf_send[msg_sent]) + 1
                    );

                    msg_sent = (msg_sent + 1) % BUF_MSGS;
                }// fi
            } // fi
        } // fi
    } // while
} // main

// mugc.c
