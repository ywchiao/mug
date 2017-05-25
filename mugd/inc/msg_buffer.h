/**
 *  @file       msg_buffer.h
 *  @brief      The header file for msg_buffer.c.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       05/24/2017 created.
 *  @date       05/25/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The header file of msg_buffer.c
 */

#ifndef __MSG_BUFFER_H__
#define __MSG_BUFFER_H__

#include <stddef.h>

#include "msg.h"

#define BUF_MSGS    256

struct msg *buf4read(int *);
struct msg *buf4write(void);

#endif

// msg_buffer.h
