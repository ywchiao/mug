/**
 *  @file       user.h
 *  @brief      The structure definition of struct user.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/24/2017 created.
 *  @date       05/31/2017 last modified.
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
    int msg_idx;
} user;

#endif

// user.h
