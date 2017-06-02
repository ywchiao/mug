/**
 *  @file       mugc.c
 *  @brief      The entry point of MUG.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       06/01/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The entry point of MUG.
 */

#include <stdio.h>
#include <stdlib.h>

#include "client.h"

#include "mugc.h"

/**
 *  Printout the usage.
 *
 *  @param  none.
 *
 *  @return none.
 **/
void usage(void) {
    printf("usage: \n");
    printf("    mugc ip port\n");

    exit(0);
} // usage()

/**
 *  MugC 客戶端程式進入點。
 *
 *  @param[in]  argc 命令列參數個數。
 *  @param[in]  argv 命令列參數陣列；每個參數都是一個 string。
 *
 *  @return     程式結束狀態
 **/
int main(int argc, char *argv[]) {
    if (argc != 3) {
        usage();
    } // fi

    client_start(argv[1], strtol(argv[2], NULL, 0));
} // main()

// mugc.c
