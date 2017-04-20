/**
 *  @file       npc_states.c
 *  @brief      A simple program demos struct/union and bit
 *              operations.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       04/20/2017 created.
 *  @date       04/20/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  A simple program demos struct/union and bit operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <string.h>

#define NPC_ATTR_SIZE   8
#define NPC_ATTR_MASK   ((1 << NPC_ATTR_SIZE) - 1)

#define NPC_ATTR_OFFSET_CHARM           24
#define NPC_ATTR_OFFSET_AGILITY         16
#define NPC_ATTR_OFFSET_INTELLIGENCE    8
#define NPC_ATTR_OFFSET_STRENGTH        0

/**
 *  列印 (dump) npc 屬性內容
 *
 *  @param[in] npc  記錄 npc 屬性的結構指標
 *
 *  @return none.
 **/
void states_dump(uint64_t states) {
    char *attr[] = {
        "力量 (strength)",
        "智力 (intelligence)",
        "敏捷 (agility)",
        "魅力 (charm)",
    };

    printf("npc 狀態： {\n");

    for (int i = 0; i < 4; i ++) {
        int offset = NPC_ATTR_SIZE * i;

        printf(
            "  %s: %d\n", attr[i],
            (states & (NPC_ATTR_MASK << offset)) >> offset
        );
    } // od

    printf("}\n\n");
} // states_dump()

/**
 *  根據 *屬性碼* (code) 更改 npc 的屬性值
 *
 *  @param     states   記錄 npc 屬性的 64 位元無號整數
 *  @param[in] attrs    有多少個 (屬性, 值) 要設定
 *  @param[in] values   傳入的 *屬性碼* 和 *屬性值*
 *
 *  @return none.
 **/
void states_set(uint64_t *states, int attrs, char *values[]) {
    for (int i = 1; i < attrs; i += 2) {
        // value_new 代表要設定的屬性 *值*
        int value_new = strtol(values[i + 1], NULL, 0);

        // 根據 *屬性碼* 作相對應的設定
        switch (*values[i]) {
            // 設定 *魅力* (charm) 屬性
            case 'c':
                // 下面的程式碼會
                // 將 value_new 的值記錄在
                // *魅力* (charm) 屬性的位置。

                // 清除原先的 *魅力* (charm) 屬性
                *states &= ~(NPC_ATTR_MASK << NPC_ATTR_OFFSET_CHARM);

                // 設定新的 *魅力* (charm) 屬性
                *states |= (value_new << NPC_ATTR_OFFSET_CHARM);

                break;

            // 設定 *敏捷* (agility) 屬性
            case 'a':
                // 請加上相對應的程式碼，
                // 將 value_new 的值記錄在
                // *敏捷* (agility) 屬性的位置。

                break;

            // 設定 *智力* (intelligence) 屬性
            case 'i':
                // 請加上相對應的程式碼，
                // 將 value_new 的值記錄在
                // *智力* (intelligence) 屬性的位置。

                break;

            // 設定 *力量* (strength) 屬性
            case 's':
                // 請加上相對應的程式碼，
                // 將 value_new 的值記錄在
                // *力量* (strength) 屬性的位置。

                break;

            default:
                printf(
                    "錯誤： *%c* 不是設定的 npc 屬性之一，請檢查你的輸入。\n\n",
                    *values[i]
                );

                break;
        } // esac
    } // od
} // states_set()

/**
 *  印出簡易使用說明
 *
 *  @param  none.
 *
 *  @return none.
 **/
void usage(void) {
    printf("npc_states: 設定人物基本屬性\n\n");
    printf("usage:\n");
    printf("    npc_states {{s|i|a|c} ##}+\n");
    printf("\n");
    printf("      屬性:\n");
    printf("        a: 敏捷 (agility)\n");
    printf("        c: 魅力 (charm)\n");
    printf("        i: 智慧 (intelligence)\n");
    printf("        s: 力量 (strength)\n");
    printf("\n");
    printf("      範例:\n");
    printf("        ./npc_states s 90 i 20\n");

    exit(0);
} // usage();

/**
 *  npc_states 程式進入點。
 *
 *  @param[in]  argc 命令列參數個數。
 *  @param[in]  argv 命令列參數陣列；每個參數都是一個 string。
 *
 *  @return     none.
 **/
void main(int argc, char *argv[]) {
    uint64_t states = 0;

    if (argc < 2) {
        usage();
    } // fi

    states_set(&states, argc, argv);

    states_dump(states);
} // main()

// npc_states.c
