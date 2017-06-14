/**
 *  @file       user_io.c
 *  @brief      The user I/O related routines on the client side.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       06/01/2017 created.
 *  @date       06/15/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The user I/O related routines on the client side.
 */

#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include "user_io.h"

/**
 *  設定標準輸入 (keyboard) 為 non-block 狀態。
 *
 *  @param      none.
 *
 *  @return     標準輸入 (keyboard) 的 file descriptor: FD。
 **/
int async_keyboard(void) {
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);

    return STDIN_FILENO;
} // async_keyboard()

/**
 *  接收使用者的鍵盤輸入。
 *
 *  @param[in]  fd      鍵盤輸入的 file descriptor
 *  @param[in]  msg_buf 接收鍵盤輸入的文字緩衝區。
 *
 *  @return     由鍵盤讀入的 bytes 總數。
 **/
int user_input(int fd, char *msg_buf) {
    int bytes = 0;

    memset(msg_buf, 0, MSG_LENGTH);

    bytes = read(fd, msg_buf, MSG_LENGTH);

    return bytes;
} // user_input();

/**
 *  將由伺服端接收到訊息輸出到使用者螢幕。
 *
 *  @param[in]  msg_buf 要輸出到螢幕的 struct msg 結構指標。
 *
 *  @return     none.
 **/
void user_output(struct msg *msg_buf) {
    printf("%s 說: %s\n", msg_buf->source, msg_buf->text);
    fflush(stdout);
} // user_output();

// user_io.c
