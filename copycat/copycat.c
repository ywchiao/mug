/**
 *  @file       copycat.c
 *  @brief      A sample program showing UNIX's file I/O.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/30/2017 created.
 *  @date       03/30/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  A sample program showing UNIX's file I/O.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>

#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 512

void usage(void) {
    printf("usage:\n");
    printf("    copycat src_file dst_file\n");

    exit(0);
} // usage();

void main(int argc, char *argv[]) {
    int fd_src;
    int fd_dst;

    if (argc < 2) {
        usage(); // 程式使用方式
    } // fi

    fd_src = open(argv[1], O_RDONLY);
    fd_dst = open(
        argv[2],
        O_WRONLY | O_CREAT,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
    );

    while (true) {
        char buffer[BUF_SIZE] = {0};
        int count = 0;

        count = read(fd_src, buffer, BUF_SIZE);

        write(fd_dst, buffer, count);

        if (count < BUF_SIZE) {
            close(fd_src);
            close(fd_dst);

            break;
        } // fi
    } // od

    printf("file copied: %s -> %s.\n", argv[1], argv[2]);
} // main()

// copycat.c
