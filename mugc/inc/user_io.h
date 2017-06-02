/**
 *  @file       user_io.h
 *  @brief      The header file for user_io.c.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       06/01/2017 created.
 *  @date       06/02/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The header file for user_io.c.
 */

#ifndef __USER_IO_H__
#define __USER_IO_H__

#include <poll.h>

#include "msg.h"

int async_keyboard(void);

int user_input(struct pollfd *, char *);
void user_output(struct msg *);

#endif

// user_io.h
