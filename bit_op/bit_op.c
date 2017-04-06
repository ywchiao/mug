/**
 *  @file       bit_op.c
 *  @brief      A simple program demos bit operator functions.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       04/05/2017 created.
 *  @date       04/06/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  A simple program which demos the C's bit operator functions.
 */

#include <stdio.h>
#include <stdlib.h>

/**
 *  將傳入的十 (10) 進位數轉換成二 (2) 進位數顯示。
 *
 *  @param[in] hint     提示字元
 *  @param[in] dec      要轉換顯示的十 (10) 進位數
 *
 *  @return none.
 **/
void dec2bin(char *hint, int dec) {
    int bit = 1;

    printf("%s: %3d_{10}\n", hint, dec);
    printf("  %3d_{10} <=> ", dec);

    for (int i = 0; i < 8 ; i ++) {
        printf("%d", (dec & (bit << (7 - i)) ? 1 : 0));
    } // od

    printf("_{2}\n");
} // dec2bin()

/**
 *  位元 and 運算 (&)
 *
 *  @param[in] flag     旗標
 *  @param[in] mask     遮罩
 *
 *  @return none.
 **/
void bit_and(int flag, int mask) {
    dec2bin("   flag ", flag);
    dec2bin("   mask ", mask);

    dec2bin("and (&) ", flag & mask);

    printf("\n");
} // bit_and()

/**
 *  位元 neg 運算 (~)
 *
 *  @param[in] flag     旗標
 *  @param[in] mask     遮罩
 *
 *  @return none.
 **/
void bit_neg(int flag, int mask) {
    dec2bin("   flag ", flag);

    dec2bin("neg (~) ", ~ flag);

    printf("\n");
} // bit_neg()

/**
 *  位元 or 運算 (|)
 *
 *  @param[in] flag     旗標
 *  @param[in] mask     遮罩
 *
 *  @return none.
 **/
void bit_or(int flag, int mask) {
    dec2bin("   flag ", flag);
    dec2bin("   mask ", mask);

    dec2bin(" or (|) ", flag | mask);

    printf("\n");
} // bit_or()

/**
 *  位元 xor 運算 (^)
 *
 *  @param[in] flag     旗標
 *  @param[in] mask     遮罩
 *
 *  @return none.
 **/
void bit_xor(int flag, int mask) {
    dec2bin("   flag ", flag);
    dec2bin("   mask ", mask);

    dec2bin("xor (^) ", flag ^ mask);

    printf("\n");
} // bit_xor()

/**
 *  位元左移運算 (<<)
 *
 *  @param[in] flag     旗標
 *  @param[in] offset   位移數
 *
 *  @return none.
 **/
void bit_shift_left(int flag, int offset) {
    dec2bin("   flag ", flag);
    dec2bin(" offset ", offset);

    dec2bin(">> (>>) ", flag >> offset);

    printf("\n");
} // bit_shift_left()

/**
 *  位元右移運算 (>>)
 *
 *  @param[in] flag     旗標
 *  @param[in] offset   位移數
 *
 *  @return none.
 **/
void bit_shift_right(int flag, int offset) {
    dec2bin("   flag ", flag);
    dec2bin(" offset ", offset);

    dec2bin("<< (<<) ", flag << offset);

    printf("\n");
} // bit_shift_right()

/**
 *  印出簡易使用說明
 *
 *  @param  none.
 *
 *  @return none.
 **/
void usage(void) {
    printf("usage:\n");
    printf("    bit_op flag mask\n");

    exit(0);
} // usage();

/**
 *  bit_op 程式進入點。
 *
 *  @param[in]  argc 命令列參數個數。
 *  @param[in]  argv 命令列參數陣列；每個參數都是一個 string。
 *
 *  @return     none.
 **/
void main(int argc, char *argv[]) {
    int flag = strtol(argv[1], NULL, 0);
    int mask = strtol(argv[2], NULL, 0);

    bit_and(flag, mask);
    bit_neg(flag, mask);
    bit_or(flag, mask);
    bit_xor(flag, mask);
    bit_shift_left(flag, mask);
    bit_shift_right(flag, mask);
} // main()

// bit_op.c
