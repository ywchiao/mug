/**
 *  @file       msg_io.c
 *  @brief      The msg I/O routines.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/31/2017 created.
 *  @date       06/08/2017 last modified.
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

#include "msg_io.h"

static char str_msg[MSG_LENGTH * 2];
static char str_buf[MSG_LENGTH * 2];

/**
 *  將客戶端送來的的訊息， *暫存* 在訊息暫存區 (msg_buffer)
 *
 *  @param[in]  poll_fds    存放已連線客戶端 pollfd 結構的陣列指標
 *  @param[in]  clients     存放客戶端相關資料 user 結構的陣列指標
 *  @param[in]  guests      目前的連線客戶端總數
 *
 *  @return     下一個可用的訊息暫存區索引 (index)
 **/
void msg_input(struct pollfd *poll_fds, struct user *clients, int guests) {
    // 處理聊天訊息
    for (int i = 1; i < guests; i ++) {
        // 如果有 _客戶_ (某個 fd) 傳送了資料進來，接收它
        if ((poll_fds[i].revents & POLLIN) == POLLIN) {
            int count = 0;
            struct msg *msg_buffer = NULL;

            msg_buffer = buf4write();

            // 將 _訊息暫存區_ (msg_buffer) 的記憶體空間清空
            memset(msg_buffer, 0, sizeof(struct msg));

            // count 代表 read(...) 讀到的字元 (character) 數
            count = read(poll_fds[i].fd, msg_buffer, sizeof(struct msg));

            // 如果 count == 0，代表客戶端 (client) 已離線
            if (count == 0) {
                // 關閉 (close) 相關的 socket
                close(poll_fds[i].fd);

                // 同時標定 fd = -1，以備後續清理
                poll_fds[i].fd = -1;
            } // fi
            else {
                // 客戶端傳來它的 _暱稱_ (nickname)
                if (msg_buffer->type == MSG_NICKNAME) {
                    char *nick = msg_buffer->text;

                    // 移除字串尾端的 "\r\n" 字元
                    nick[strcspn(nick, "\r\n")] = '\0';

                    // 將客戶 nickname 保留下來
                    strcpy(clients[i].nick, nick);
                } // fi

                printf(
                    "recv <- %d:%s:%s",
                    poll_fds[i].fd,
                    msg_buffer->source,
                    msg_buffer->text
                );

                // 將客戶 nickname 放入 _訊息物件_ 的 source 欄
                strcpy(msg_buffer->source, clients[i].nick);

                msg_2str(msg_buffer, str_msg, MSG_LENGTH *2);

                snprintf(
                    str_buf, MSG_LENGTH * 2,
                    "<- socket[%d] %s\n",
                    poll_fds[i].fd, str_msg
                );

                logging(str_buf);
            } // esle
        } // fi
    } // od
} // msg_input()

/**
 *  將收到的訊息， *廣播* (broadcast) 給群裡的所有人
 *
 *  @param[in]  poll_fds    存放已連線客戶端 pollfd 結構的陣列指標
 *  @param[in]  clients     存放客戶端相關資料 user 結構的陣列指標
 *  @param[in]  guests      目前的連線客戶端總數
 *
 *  @return     none.
 **/
void msg_output(struct pollfd *poll_fds, struct user *clients, int guests) {
    // 處理訊息 *群播* (multicast)
    // 將聊天訊息傳遞給聊天室的所有人
    for (int i = 0; i < guests; i ++) {
        // 檢查 poll_fds[i] 是否準備好可以接收 output 訊息
        if ((poll_fds[i].revents & POLLOUT) == POLLOUT) {
            // idx 代表特定客戶端 *預計接收* 的訊息索引
            struct msg *message = NULL;

            message = buf4read(&clients[i].msg_idx);

            // 如果有新的訊息等待傳送
            if (message != NULL) {
                write(
                    poll_fds[i].fd,
                    message,
                    sizeof(struct msg)
                );

                printf(
                    "send -> %d:%d:%s:%s\n",
                    clients[i].msg_idx,
                    poll_fds[i].fd,
                    clients[i].nick,
                    message->text
                );

                msg_2str(message, str_msg, MSG_LENGTH *2);

                snprintf(
                    str_buf, MSG_LENGTH * 2,
                    "-> socket[%d] %s\n",
                    poll_fds[i].fd, str_msg
                );

                logging(str_buf);
            } // fi
        } // fi
    } // od
} // msg_output()

// msg_io.c
