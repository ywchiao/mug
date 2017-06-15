/**
 *  @file       server.c
 *  @brief      The server routines of mugd.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/31/2017 created.
 *  @date       06/15/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The server routines of mugd.
 */

#include <poll.h>

#include <stdbool.h>
#include <stdio.h>

#include "mug.h"

#include "logging.h"
#include "msg.h"
#include "msg_io.h"
#include "socket_io.h"
#include "user.h"

#include "server.h"

/**
 *  接受新的客戶端連線。
 *
 *  @param  fd_server 存放 server_socket 的 file descripter (FD).
 *
 *  @return none.
 **/
void guest_new(int fd_server) {
    int fd_guest = -1;

    do {
        fd_guest = socket_accept(fd_server);

        if (fd_guest > 0) {
            char str_buf[MSG_LENGTH * 2];

            user_enter(fd_guest);

            snprintf(
                str_buf, MSG_LENGTH * 2,
                "<- socket[%d] %s",
                fd_guest, "entered."
            );

            logging(str_buf);
        } // fi
    } while (fd_guest != -1);
} // guest_new()

/**
 *  主迴圈
 *
 *  @param  fd_server 存放 server_socket 的 file descripter (FD).
 *
 *  @return none.
 **/
void main_loop(int fd_server) {
    struct pollfd poll_fds[MAX_CLIENTS + 1];

    // 利用 poll() 來監測 socket 活動 (activities)
    poll_fds[0].fd = fd_server;

    // 對 server_socket，只監測 input 事件 (客戶端的連線要求)
    poll_fds[0].events = POLLIN;

    logging_start("./mugd.log");

    while (true) {
        int links = user_counts() + 1;

        // 建立要利用 poll() 監控的 fd 陣列
        for (int i = 1; i < links; i ++) {
            poll_fds[i].fd = user_get_fd(i);
            poll_fds[i].events = POLLIN | POLLOUT;
        } // od

        // 監控是否有新的 *通訊事件* 發生
        poll(poll_fds, links, -1);

        // 處理新的連線要求
        if ((poll_fds[0].revents & POLLIN) == POLLIN) {
            guest_new(fd_server);
        } // fi

        // 將客戶端傳來的訊息， *廣播* 給所有連線客戶端
        for (int i = 1; i < links; i ++) {
            // 檢查 poll_fds[i] 是否準備好可以接收 output 訊息
            if ((poll_fds[i].revents & POLLOUT) == POLLOUT) {
                msg_output(i);
            } // fi
        } // od

        // 接收客戶端送來的訊息
        for (int i = 1; i < links; i ++) {
            // 如果有_客戶_ (某個 fd) 傳送了資料進來，接收它
            if ((poll_fds[i].revents & POLLIN) == POLLIN) {
                if (msg_input(i) == 0) {
                    // 使用者已離線
                    user_left(i);
                } // fi
            } // fi
        } // od

        // 清除已離線的使用者
        user_clear();
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
    int fd_server = -1;

    // 啟動 server_socket
    fd_server = socket_listen(port);

    printf("MUG server started at %s:%d ...\n", "127.0.0.1", port);

    // 主迴圈
    main_loop(fd_server);
} // server_start()

// server.c
