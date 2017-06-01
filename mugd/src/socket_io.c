/**
 *  @file       socket_io.c
 *  @brief      The socket related routines.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/31/2017 created.
 *  @date       05/31/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The socket related routines.
 */

#include <errno.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "msg.h"

/**
 *  設定並傳回一個 ServerSocket 的 file descripter (FD).
 *
 *  @param[in]  port        伺服器通訊埠 (port) 號碼
 *  @param[in]  poll_fds    存放連線客戶端 pollfd 結構陣列的指標
 *
 *  @return     ServerSocket 的 FileDescripter (FD)
 **/
int socket_listen(int port) {
    int fd_socket;
    struct sockaddr_in server_addr;

    // 建立一個 NONBLOCK 的 STREAM (串流) socket
    fd_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    memset(&server_addr, 0, sizeof(server_addr));

    // 設定預定的主機地址，通訊埠 (port)
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);

    // 將 fd_socket 和伺服地址資訊 *綁定*
    bind(fd_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 開始 *聆聽* (listen) 連線要求
    listen(fd_socket, 10);

    return fd_socket;
} // socket_listen()

/**
 *  接受一個新的連線要求，同時送出歡迎訊息
 *
 *  @param[in]  fd_server Server_socket 的 file descriptor: fd.
 *
 *  @return     新連線的 file descriptor; 或 -1, 沒有更多的連線要求
 **/
int socket_accept(int fd_server) {
    struct msg greeting;
    char *welcome = "Welcome to the world of MUG.\nWhat is your name?";
    int fd_guest = -1;

    memset(&greeting, 0, sizeof(struct msg));

    greeting.type = MSG_WELCOME;
    strcpy(greeting.source, "伺服");
    strcpy(greeting.text, welcome);

    fd_guest = accept(fd_server, (struct sockaddr *)NULL, NULL);

    if (fd_guest > 0) {
        write(fd_guest, &greeting, sizeof(struct msg));
        printf("send -> %s\n", greeting.text);
    } // fi
    else if (!((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
        // if not no further connection requests (EAGAIN || EWOULDBLOCK)
        // then something wrong!!!
        fprintf(
            stderr,
            "Function: %s@%s:line %d: %s\n",
            __func__, __FILE__, __LINE__,
            strerror(errno)
        );

        exit(-1);
    } // esle

    return fd_guest;
} // socket_accept()

// socket_io.c
