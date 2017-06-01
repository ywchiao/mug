/**
 *  @file       mugd.c
 *  @brief      The entry point of MUG server (daemon).
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       05/31/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The entry point of MUG server (daemon).
 */

#include <stdio.h>
#include <stdlib.h>

#include "server.h"

#include "mugd.h"

/**
 *  Printout the usage.
 *
 *  @param  none.
 *
 *  @return none.
 **/
void usage(void) {
    printf("usage: \n");
    printf("    mugd port\n");

    exit(0);
} // usage()

/**
 *  MugD 伺服端程式進入點。
 *
 *  @param[in]  argc 命令列參數個數。
 *  @param[in]  argv 命令列參數陣列；每個參數都是一個 string。
 *
 *  @return     程式結束狀態
 **/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        usage();
    } // fi

    server_start(strtol(argv[1], NULL, 0));
} // main()

// mugd.c
