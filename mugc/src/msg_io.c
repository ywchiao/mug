/**
 *  @file       msg_io.c
 *  @brief      The true msg_io code of mugc.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       06/01/2017 created.
 *  @date       06/02/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The true msg_io code of mugc.
 */

#include <string.h>

#include <unistd.h>

#include "msg_buffer.h"

#include "msg_io.h"

/**
 *  由連線 socket 讀入訊息。
 *
 *  @param[in]  poll_fd 連線 socket 的 pollfd 結構指標
 *  @param[in]  msg_buf 接收 socket 訊息的 struct msg 結構指標
 *
 *  @return     由 poll_fd->fd 讀入的 bytes 總數
 **/
int msg_input(struct pollfd *poll_fd, struct msg *msg_buf) {
    int bytes = 0;

    if ((poll_fd->revents & POLLIN) == POLLIN) {
        memset(msg_buf, 0, sizeof(struct msg));

        bytes = read(poll_fd->fd, msg_buf, sizeof(struct msg));
    } // fi

    return bytes;
} // msg_input()

/**
 *  將準備傳送的訊息推入訊息緩衝區內。
 *
 *  @param[in]  msg_buf 指向準備要傳送的訊息 struct msg 的指標。
 *
 *  @return     none.
 **/
void msg_push(struct msg *msg_buf) {
    struct msg *msg_out = buf4write();

    memcpy(msg_out, msg_buf, sizeof(struct msg));
} // msg_push();

/**
 *  對連線 socket 寫入訊息。
 *
 *  @param[in]  poll_fd 連線 socket 的 pollfd 結構指標
 *
 *  @return     對 poll_fd->fd 寫入的 bytes 總數
 **/
int msg_output(struct pollfd *poll_fd) {
    static int msg_sent = 0;

    int bytes = 0;

    if ((poll_fd->revents & POLLOUT) == POLLOUT) {
        struct msg *msg_buf = NULL;

        msg_buf = buf4read(&msg_sent);

        if (msg_buf) {
            bytes = write(poll_fd->fd, msg_buf, sizeof(struct msg));
        } // fi
    } // fi

    return bytes;
} // msg_output()

// msg_io.c
