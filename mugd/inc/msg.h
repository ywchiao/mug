/**
 *  @file       msg.h
 *  @brief      The defintion of struct msg.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/24/2017 created.
 *  @date       06/08/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The structure defintion of struct msg.
 */

#ifndef __MSG_H__
#define __MSG_H__

#define MSG_LENGTH    128

#define MSG_WELCOME      0
#define MSG_MESSAGE      1
#define MSG_NICKNAME     2

typedef struct msg {
    int type;
    char source[32];
    char text[MSG_LENGTH];
} msg;

void msg_2str(const struct msg *, char *, int);

#endif

// msg.h
