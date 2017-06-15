/**
 *  @file       msg_buffer.c
 *  @brief      The ring-buffer of messages.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/24/2017 created.
 *  @date       06/15/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The ring-buffer of messages.
 */

#include <stddef.h>

#include "msg_buffer.h"

static int head = 0;
static int tail = 0;
static struct msg buffers[BUF_MSGS];

/**
 *  檢查是否 _所有_ 訊息均被讀取
 *
 *  @param  index 目前客戶端已讀取訊息的素引
 *
 *  @return true: 所有訊息均已讀取; false: otherwise.
 **/
bool is_empty(int index) {
    return ((index % BUF_MSGS) == head);
} // is_empty()

/**
 *  檢查訊息緩衝區是否 _已滿_
 *
 *  @param  none.
 *
 *  @return true: 訊息緩衝區已滿; false: otherwise.
 **/
bool is_full(void) {
    bool isFull = false;

    // 加上檢查的程式碼；

    return isFull;
} // is_full()

/**
 *  傳回下一個可供 _讀取_ 的訊息物件。
 *
 *  將已讀取訊息索引 (index) 和暫存訊息的索引 (head) 作比較；
 *    如果有尚未讀取的訊息 ->
 *      傳回那個訊息;
 *    如果訊息均已讀取 ->
 *      傳回 NULL;
 *
 *  @param  index 目前客戶端已讀取訊息的素引
 *
 *  @return 下一個可讀的暫存訊息物件指標 (struct msg *)
 **/
struct msg *buf4read(int index) {
    struct msg *buf = NULL;
    int idx = index % BUF_MSGS;

    if (idx != head) {
        buf = &buffers[idx];
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
