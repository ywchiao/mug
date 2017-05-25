/**
 *  @file       msg_buffer.c
 *  @brief      The ring-buffer of messages.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/24/2017 created.
 *  @date       05/25/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The ring-buffer of messages.
 */

#include "msg_buffer.h"

static int head = 0;
static int tail = 0;
static struct msg buffers[BUF_MSGS];

/**
 *  傳回下一個可供 _讀取_ 的訊息物件。
 *
 *  將已讀取訊息索引 (*index) 和暫存訊息的索引 (head) 作比較；
 *    如果有尚未讀取的訊息 ->
 *      傳回那個訊息，同時更新已讀取訊息索引 (*index);
 *    如果訊息均已讀取 ->
 *      傳回 NULL;
 *
 *  @param  index 目前客戶端已讀取訊息的素引
 *
 *  @return 下一個可讀的暫存訊息物件指標 (struct msg *)
 **/
struct msg *buf4read(int *index) {
    struct msg *buf = NULL;
    int idx = *index;

    if (idx != head) {
        buf = &buffers[idx];

        *index = ((idx + 1) % BUF_MSGS);
    } // fi

    return buf;
} // buf4read()

/**
 *  傳回下一個可供 _寫入_ 的訊息物件。
 *
 *  head 指向目前 _環狀緩衝區_ (ring buffer) 可供寫入的訊息物件。
 *  這個函數傳回目前 head 指向的物件指標，同時更新 head 指向下
 *  一個物件。
 *
 *  @return     可 _寫_ 的暫存訊息物件指標 (struct msg *)
 **/
struct msg *buf4write(void) {
    struct msg *buf = &buffers[head];

    head = (head + 1) % BUF_MSGS;

    return buf;
} // buf4write()

// msg_buffer.c
