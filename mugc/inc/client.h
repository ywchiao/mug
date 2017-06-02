/**
 *  @file       client.h
 *  @brief      The header file for client.c.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       06/01/2017 created.
 *  @date       06/01/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The header file for client.c.
 */

#ifndef __CLIENT_H__
#define __CLIENT_H__

#define STATE_CONNECTING 0
#define STATE_LOGIN      1
#define STATE_CHATTING   2

void client_start(char *, int);

#endif

// client.h
