/**
 *  @file       msg.h
 *  @brief      The header file for msg.c.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/24/2017 created.
 *  @date       05/24/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The header file of msg.c
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

#endif

// msg.h
