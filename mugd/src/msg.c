/**
 *  @file       msg.c
 *  @brief      The msg utilities.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       06/07/2017 created.
 *  @date       06/08/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The msg utilities.
 */

#include <stdio.h>

#include "msg.h"

/**
 *  將 msg->type 轉換為易讀的_文字格式_。
 *
 *  @param      msgtype 要轉換的訊息 msg type。
 *
 *  @return     msg 型別 (msg->type) 的_文字名稱_
 **/
char *msg_type2str(int msgtype) {
    static char *msg_type[] = {
        "歡迎",
        "訊息",
        "暱稱",
        ""
    };

    if (msgtype > (sizeof(msg_type) / sizeof(char *))) {
        msgtype = (sizeof(msg_type) / sizeof(char *));
    } // fi

    return msg_type[msgtype];
} // msg_type2str()

/**
 *  將 struct msg 轉換為易讀的_文字格式_。
 *
 *  @param[in]  msg 要轉換的 struct msg 指標。
 *  @param[in]  msg_str 轉換後的 str 緩衝區指標。
 *  @param[in]  size 限定轉換後最長的 str 長度。
 *
 *  @return     none.
 **/
void msg_2str(const struct msg *msg, char *msg_str, int size) {
    snprintf(msg_str, size,
        "source[%s] type[%d:%s] text[%s]\n",
        msg->source,
        msg->type, msg_type2str(msg->type),
        msg->text
    );
} // msg_2str()

// msg.c
