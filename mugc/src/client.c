/**
 *  @file       client.c
 *  @brief      The true client code of mugc.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       06/01/2017 created.
 *  @date       06/02/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The true client code of mugc.
 */

#include <poll.h>

#include <stdbool.h>

#include "msg.h"

#include "msg_io.h"
#include "socket_io.h"
#include "user_io.h"

#include "client.h"

/**
 *  Main loop. 整個程式的主迴圈。
 *
 *  @param[in]  poll_fds 存放 pollfd 結構的陣列指標
 *
 *  @return     none.
 **/
void main_loop(struct pollfd *poll_fds) {
    int state = STATE_CONNECTING;
    struct msg msg_buf = {0} ;

    while (true) {
        poll(poll_fds, 2, -1);

        if (user_input(&poll_fds[0], msg_buf.text) > 0) {
            if (state != STATE_CONNECTING) {
                msg_buf.type = MSG_MESSAGE;

                if (state & STATE_LOGIN) {
                    msg_buf.type = MSG_NICKNAME;
                    state &= (~STATE_LOGIN);
                } // fi

                msg_push(&msg_buf);
            } // esle
        } // fi

        if (msg_input(&poll_fds[1], &msg_buf) > 0) {
            user_output(&msg_buf);

            if (msg_buf.type == MSG_WELCOME) {
                state |= STATE_LOGIN;
            } // fi
            else {
                state |= STATE_CHATTING;
            } // esle
        } // fi

        msg_output(&poll_fds[1]);
    } // while
} // main_loop()

/**
 *  依使用者指定的 ip, port 嘗試和伺服器建立連線，啟動 main loop。
 *
 *  @param[in]  ip      伺服 ip 地址
 *  @param[in]  port    通訊埠號碼
 *
 *  @return     none.
 **/
void client_start(char *ip, int port) {
    struct pollfd fd[2];

    fd[0].fd = async_keyboard();
    fd[0].events = POLLIN;

    fd[1].fd = socket_connect(ip, port);
    fd[1].events = POLLIN | POLLOUT;

    main_loop(fd);
} // client_start()

// client.c
