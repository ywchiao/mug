/**
 *  @file       mugc.c
 *  @brief      The entry point of MUG.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       05/25/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The entry point of MUG.
 */

#include "msg.h"
#include "msg_buffer.h"

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

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    inet_pton(AF_INET, ip, &(server_addr.sin_addr));

    fd_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    connect(
        fd_socket,
        (struct sockaddr *)&server_addr,
        sizeof(server_addr)
    );

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
    int msg_sent = 0;
    int state = STATE_CONNECTING;

    if (argc != 3) {
        usage();
    } // fi

    fd_server = socket_to_server(argv[1], strtol(argv[2], NULL, 0));

    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);

    fd[0].fd = STDIN_FILENO;
    fd[0].events = POLLIN;

    fd[1].fd = fd_server;
    fd[1].events = POLLIN | POLLOUT;

    while (true) {
        int c = 0;
        struct msg *buf_msg;

        c = poll(fd, 2, -1);

        if (c > 0) {
            if ((fd[0].revents & POLLIN) == POLLIN) {
                if (state == STATE_CONNECTING) {
                    char buf_dump[MSG_LENGTH];

                    read(fd[0].fd, buf_dump, MSG_LENGTH);
                } // fi
                else {
                    buf_msg = buf4write();

                    buf_msg->type = MSG_MESSAGE;

                    if (state & STATE_LOGIN) {
                        buf_msg->type = MSG_NICKNAME;
                        state &= (~STATE_LOGIN);
                    } // fi

                    read(fd[0].fd, buf_msg->text, MSG_LENGTH);
                } // esle
            } // fi

            if ((fd[1].revents & POLLIN) == POLLIN) {
                struct msg msg_buf = {0} ;

                memset(&msg_buf, 0, sizeof(struct msg));

                if (read(fd[1].fd, &msg_buf, sizeof(struct msg)) > 0) {
                    printf("%s 說: %s\n", msg_buf.source, msg_buf.text);
                    fflush(stdout);

                    if (msg_buf.type == MSG_WELCOME) {
                        state |= STATE_LOGIN;
                    } // fi
                    else {
                        state |= STATE_CHATTING;
                    } // esle
                } // fi
            } // fi

            if ((fd[1].revents & POLLOUT) == POLLOUT) {
                buf_msg = buf4read(&msg_sent);

                if (buf_msg) {
                    write(fd[1].fd, buf_msg, sizeof(struct msg));
                }// fi
            } // fi
        } // fi
    } // while
} // main

// mugc.c
