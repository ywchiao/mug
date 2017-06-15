/**
 *  @file       msg_io.c
 *  @brief      The msg I/O routines.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/31/2017 created.
 *  @date       06/15/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The msg I/O routines.
 */

#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include "logging.h"

#include "msg.h"
#include "msg_buffer.h"

#include "user.h"

#include "msg_io.h"

static char str_msg[MSG_LENGTH * 2];
static char str_buf[MSG_LENGTH * 2];

/**
 *  讀取指定的客戶端傳來的訊息。
 *
 *  @param[in]  idx     連線客戶端索引
 *
 *  @return     讀入的訊息長度，單位是 _bytes_
 **/
int msg_input(int idx) {
    int bytes = 0;
    int fd = user_get_fd(idx);
    struct msg *msg_buffer = NULL;

    msg_buffer = buf4write();

    // 將 _訊息暫存區_ (msg_buffer) 的記憶體空間清空
    memset(msg_buffer, 0, sizeof(struct msg));

    // bytes 代表 read(...) 讀到的字元 (character) 數
    bytes = read(fd, msg_buffer, sizeof(struct msg));

    // 如果 bytes == 0，代表客戶端 (client) 已離線
    if (bytes > 0) {
        // 移除字串尾端的 "\r\n" 字元
        msg_buffer->text[strcspn(msg_buffer->text, "\r\n")] = '\0';

        // 客戶端傳來它的_暱稱_ (nickname)
        if (msg_buffer->type == MSG_NICKNAME) {
            // 將客戶 nickname 保留下來
            user_set_nickname(idx, msg_buffer->text);
        } // fi

        printf(
            "recv <- %d:%s:%s\n",
            fd, msg_buffer->source, msg_buffer->text
        );

        // 將客戶 nickname 放入 _訊息物件_ 的 source 欄
        strcpy(msg_buffer->source, user_get_nickname(idx));

        msg_2str(msg_buffer, str_msg, MSG_LENGTH *2);

        snprintf(
            str_buf, MSG_LENGTH * 2,
            "<- socket[%d] %s", fd, str_msg
        );

        logging(str_buf);
    } // esle

    return bytes;
} // msg_input()

/**
 *  將暫存的訊息傳送給指定的客戶端。
 *
 *  @param[in]  idx     連線客戶端索引
 *
 *  @return     none.
 **/
void msg_output(int idx) {
    // msg_idx 代表特定客戶端 *預計接收* 的訊息索引
    int msg_idx = user_msg_idx(idx);
    int fd = user_get_fd(idx);

    if (!is_empty(msg_idx)) {
        struct msg *message = NULL;

        // 如果有新的訊息等待傳送
        message = buf4read(user_next_msg(idx));

        write(fd, message, sizeof(struct msg));

        printf(
            "send -> %d:%d:%s:%s\n",
            msg_idx, fd, user_get_nickname(idx), message->text
        );

        msg_2str(message, str_msg, MSG_LENGTH *2);

        snprintf(
            str_buf, MSG_LENGTH * 2,
            "-> socket[%d] %s", fd, str_msg
        );

        logging(str_buf);
    } // fi
} // msg_output()

// msg_io.c
