/**
 *  @file       args.c
 *  @brief      The sample program showing C main()'s arguments.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/29/2017 created.
 *  @date       03/29/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The sample programm showing C main()'s arguments.
 */

#include <stdio.h>

void main(int argc, char *argv[]) {
    for (int i=0; i<argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    } // od
} // main()

// args.c
