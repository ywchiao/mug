/**
 *  @file       msg_io.h
 *  @brief      The header file for msg_io.c.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/31/2017 created.
 *  @date       05/31/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The header file of msg_io.c
 */

#ifndef __MSG_IO_H__
#define __MSG_IO_H__

#include <poll.h>

#include "user.h"

void msg_input(struct pollfd *, struct user *, int);
void msg_output(struct pollfd *, struct user *, int);

#endif

// msg_io.h
