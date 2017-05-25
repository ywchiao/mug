/**
 *  @file       mugd.h
 *  @brief      The header file for mugd.c.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       05/25/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The header file of mugd.c
 */

#include <errno.h>

#include <netdb.h>

#include <poll.h>

#include <string.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#define MAX_GUESTS  100

// mugd.h
