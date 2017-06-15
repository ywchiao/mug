/**
 *  @file       user.c
 *  @brief      The user routines of mugd.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       06/08/2017 created.
 *  @date       06/15/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The user routines of mugd.
 */

#include <string.h>

#include "mug.h"

#include "user.h"

static int guests = 0;
static struct user clients[MAX_CLIENTS];

/**
 *  取得已連線客戶端的總數。
 *
 *  @param      none.
 *
 *  @return     已連線客戶端的總數
 **/
int user_counts(void) {
    return guests;
} // user_counts()

/**
 *  初始化 _新_ 連線的客戶端使用者資料
 *
 *  @param[in]  fd_socket   連線客戶端的 socket fd
 *
 *  @return     none.
 **/
void user_enter(int fd_socket) {
    guests ++;

    memset(&clients[guests], 0, sizeof(struct user));
    clients[guests].fd_socket = fd_socket;
} // user_enter()

/**
 *  標示指定客戶端已經離線
 *
 *  @param[in]  id      連線客戶端索引
 *
 *  @return     none.
 **/
void user_left(int id) {
    clients[id].fd_socket = -1;
} // user_left()

/**
 *  清除已標示離線的客戶端資訊
 *
 *  @param      none.
 *
 *  @return     none.
 **/
void user_clear(void) {
    int links = 0;

    for (int i = 0; i < guests; i ++) {
        if (clients[i].fd_socket != -1) {
            if (links != i) {
                memcpy(&clients[links], &clients[i], sizeof(struct user));
            } // esle

            links ++;
        } // fi
    } // od

    guests = links;
} // user_clear()

/**
 *  取得指定客戶端的連線 socket 的 fd (File Descriptor)
 *
 *  @param[in]  id      連線客戶端索引
 *
 *  @return     指定客戶端的 fd_socket
 **/
int user_get_fd(int idx) {
    return clients[idx].fd_socket;
} // user_get_fd()

/**
 *  取得指定客戶端的使用者 _暱稱_
 *
 *  @param[in]  id      連線客戶端索引
 *
 *  @return     指向特定客戶端的使用者 _暱稱_ 指標
 **/
const char *user_get_nickname(int id) {
    return clients[id].nick;
} // user_get_nickname()

/**
 *  設定客戶的 nickname
 *
 *  @param[in]  id      連線客戶端索引
 *  @param[in]  nick    指向客戶端使用者 _暱稱_ 的指標
 *
 *  @return     none.
 **/
void user_set_nickname(int id, const char *nick) {
    // 設定客戶的 nickname
    strcpy(clients[id].nick, nick);
} // user_set_nickname()

/**
 *  傳回指定客戶端已接收的訊息編號
 *
 *  @param[in]  id      連線客戶端索引
 *
 *  @return     指定客戶端已接收的訊息編號
 **/
int user_msg_idx(int id) {
    return clients[id].msg_idx;
} // user_msg_idx()

/**
 *  更新並傳回指定客戶端準備接收的訊息編號
 *
 *  @param[in]  id      連線客戶端索引
 *
 *  @return     指定客戶端預備接收的訊息編號
 **/
int user_next_msg(int id) {
    return clients[id].msg_idx ++;
} // user_next_msg()

// user.c
