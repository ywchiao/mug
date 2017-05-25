/**
 *  @file       mugc.h
 *  @brief      The header file for mugc.c.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       05/25/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The header file for mugc.c.
 */

#ifndef __MUGC_H__
#define __MUGC_H__

#include <errno.h>

#include <fcntl.h>

#include <netdb.h>

#include <poll.h>

#include <string.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>

#define STATE_CONNECTING 0
#define STATE_LOGIN      1
#define STATE_CHATTING   2

#endif

// mugc.h
