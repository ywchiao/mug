/**
 *  @file       server.c
 *  @brief      The server routines of mugd.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/31/2017 created.
 *  @date       06/08/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The server routines of mugd.
 */

#include <poll.h>

#include <stdbool.h>
#include <stdio.h>

#include "logging.h"
#include "msg.h"
#include "msg_io.h"
#include "socket_io.h"
#include "user.h"

#include "server.h"

/**
 *  接受新的客戶端連線，同時放入 poll_fds 中，由 poll() 監控
 *
 *  @param[in]  poll_fds 存放已連線客戶端 pollfd 結構的陣列指標
 *  @param[in]  clients  存放客戶端相關資料 user 結構的陣列指標
 *  @param[in]  n_guests 目前已連線 (過) 的客戶端總數
 *
 *  @return     目前已連線的客戶端總數
 **/
int guest_new(struct pollfd *poll_fds, struct user *clients, int n_guests) {
    int fd_guest = -1;
    int fd_server = poll_fds[0].fd;

    do {
        fd_guest = socket_accept(fd_server);

        if (fd_guest > 0) {
            poll_fds[n_guests].fd = fd_guest;

            clients[n_guests].nick[0] = 0;
            clients[n_guests].msg_idx = 0;

            // 設定對 *新*連線，n_guests，要監測它的
            // input 和 output 事件
            poll_fds[n_guests].events = POLLIN | POLLOUT;

            n_guests ++;
        } // fi
    } while (fd_guest != -1);

    return n_guests;
} // guest_new()

/**
 *  主迴圈
 *
 *  @param  fd_socket 存放 server_socket 的 file descripter (FD).
 *
 *  @return none.
 **/
void main_loop(int fd_socket) {
    int n_guests = 1;
    struct pollfd poll_fds[MAX_GUESTS];
    struct user clients[MAX_GUESTS];

    // 利用 poll() 來監測 socket 活動 (activities)
    poll_fds[0].fd = fd_socket;

    // 對 server_socket，只監測 input 事件 (客戶端的連線要求)
    poll_fds[0].events = POLLIN;

    logging_start("./mugd.log");

    while (true) {
        int sockets = 0;

        // 監控是否有新的 *通訊事件* 發生
        poll(poll_fds, n_guests, -1);

        sockets = n_guests;

        // 處理新的連線要求
        if ((poll_fds[0].revents & POLLIN) == POLLIN) {
            char str_buf[MSG_LENGTH * 2];

            n_guests = guest_new(poll_fds, clients, n_guests);

            snprintf(
                str_buf, MSG_LENGTH * 2,
                "<- socket[%d] %s\n",
                poll_fds[sockets].fd, "entered."
            );

            logging(str_buf);
        } // fi

        // 接收客戶端送來的訊息
        msg_input(poll_fds, clients, sockets);

        // 將客戶端傳來的訊息， *廣播* 給所有連線客戶端
        msg_output(poll_fds, clients, sockets);
    } // while
} // main_loop()

/**
 *  啟動伺服，進入程式主迴圈。
 *
 *  @param  port 伺服使用的 _通訊埠_ (port) 號碼
 *
 *  @return none.
 **/
void server_start(int port) {
    int fd_socket = -1;

    // 啟動 server_socket
    fd_socket = socket_listen(port);

    printf("MUG server started at %s:%d ...\n", "127.0.0.1", port);

    main_loop(fd_socket);
} // server_start()

// server.c
