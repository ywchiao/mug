/**
 *  @file       fileio.c.c
 *  @brief      A sample program showing UNIX's file I/O.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/16/2017 created.
 *  @date       03/16/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  A sample program showing UNIX's file I/O.
 */

#include <string.h>
#include <stdio.h>

#include <unistd.h>

#define BUF_SIZE 128

void main() {
    char *prompt = "\n請輸入你的訊息 : ";
    char *response = "\n你輸入的訊息是 : ";
    char *eol = "\n";

    char buffer[BUF_SIZE] = {0};

    write(1, prompt, strlen(prompt));

    read(0, buffer, BUF_SIZE);

    write(1, response, strlen(response));
    write(1, buffer, strlen(buffer));
    write(1, eol, strlen(eol));
} // main()

// fileio.c
