/**
 *  @file       user.h
 *  @brief      The structure definition of struct user.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/24/2017 created.
 *  @date       06/14/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The structure definition of struct user.
 */

#ifndef __USER_H__
#define __USER_H__

typedef struct user {
    char nick[32];
    int fd_socket;
    int msg_idx;
} user;

void user_enter(int);
void user_left(int);

void user_clear(void);

int user_counts(void);
int user_get_fd(int);

const char *user_get_nickname(int);
void user_set_nickname(int, const char *);

int user_msg_idx(int);
int user_next_msg(int);

#endif

// user.h
