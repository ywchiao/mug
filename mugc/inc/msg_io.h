/**
 *  @file       msg_io.h
 *  @brief      The header file for msg_io.c.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       06/01/2017 created.
 *  @date       06/01/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The header file for msg_io.c.
 */

#ifndef __MSG_IO_H__
#define __MSG_IO_H__

#include <poll.h>

#include "msg.h"

int msg_input(struct pollfd *, struct msg *);
int msg_output(struct pollfd *);
void msg_push(struct msg *);

#endif

// msg_io.h
