/**
 *  @file       logging.c
 *  @brief      The event logging utilities.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       06/07/2017 created.
 *  @date       06/08/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The event logging utilities.
 */

#include <stdint.h>
#include <stdio.h>

#include <time.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>

#include "logging.h"

static uint64_t line_no = 0;
static FILE *stream_log = NULL;

/**
 *  將傳入的訊息 log 進 log 檔案內。
 *
 *  @param[in]  msg     要 log 的訊息。
 *
 *  @return     none.
 **/
void logging(const char *msg) {
    time_t now = time(NULL);

    struct tm *tm_now = localtime(&now);

    fprintf(
        stream_log,
        "%06ld:[%02d:%02d:%02d %02d/%02d/%d] %s\n",
        line_no,
        tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec,
        tm_now->tm_mon + 1, tm_now->tm_mday, tm_now->tm_year + 1900,
        msg
    );

    fflush(stream_log);

    line_no++;
} // logging()

/**
 *  以_唯寫_方式開啟 log 檔案。
 *
 *  @param[in]  log_file    log 檔的 pathname。
 *
 *  @return     true: log 檔開始成功；false: 失敗。
 **/
bool logging_start(const char *log_file) {
    bool isOk = true;

    stream_log = fopen(log_file, "we");

    if (stream_log == NULL) {
        isOk = false;
    } // fi

    return isOk;
} // logging_start()

// logging.c
