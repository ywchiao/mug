/**
 *  @file       rpg_states.c
 *  @brief      A simple program demos struct/union and bit
 *              operations.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       04/12/2017 created.
 *  @date       04/13/2017 last modified.
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

#define NPC_AGENDER         0
#define NPC_MALE            1
#define NPC_FEMALE          2
#define NPC_BIGENDER        3
#define NPC_GENDER_MASK     3
#define NPC_GENDER_SIZE     4

#define NPC_HEALTHY         0
#define NPC_POISONED        (1 << NPC_GENDER_SIZE)
#define NPC_SICKNESS        (2 << NPC_GENDER_SIZE)
#define NPC_WOUNDED         (4 << NPC_GENDER_SIZE)
#define NPC_HEALTH_MASK     (NPC_POISONED | NPC_SICKNESS | NPC_WOUNDED)

struct npc_state {
    uint64_t states;
};

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

    printf("    %s: %3d_{10}\n", hint, dec);
    printf("      %3d_{10} <=> ", dec);

    for (int i = 0; i < 8 ; i ++) {
        printf("%d", (dec & (bit << (7 - i)) ? 1 : 0));
    } // od

    printf("_{2}\n");
} // dec2bin()

/**
 *  列印 (dump) npc 屬性內容
 *
 *  @param[in] npc  記錄 npc 屬性的結構指標
 *
 *  @return none.
 **/
void states_dump(struct npc_state *npc) {
    char *gender[] = {
        "無性",
        "男性",
        "女性",
        "雙性",
    };
    uint64_t comma = 0;

    printf("npc 狀態： {\n");
    printf("    性別 (gender): %s\n",
        gender[npc->states & NPC_GENDER_MASK]);

    printf("    健康 (health): ");
    printf("%s",
        (npc->states & NPC_POISONED) ? "中毒 (poisoned)" : "");

    comma = (npc->states & NPC_POISONED);

    printf("%s",
        (comma && (npc->states & NPC_SICKNESS)) ? ", " : "");

    printf("%s",
        (npc->states & NPC_SICKNESS) ? "生病 (sickness)" : "");

    comma |= (npc->states & NPC_SICKNESS);

    printf("%s",
        (comma && (npc->states & NPC_WOUNDED)) ? ", " : "");

    comma |= (npc->states & NPC_WOUNDED);

    printf("%s",
        (npc->states & NPC_WOUNDED) ? "受傷 (wounded)" : "");

    printf("%s",
        (comma) ? "" : "沒有異常");

    printf("\n");

    dec2bin("屬性 (attrib)", npc->states);

    printf("}\n\n");
} // states_dump()

/**
 *  根據 *屬性碼* (code) 設定相對應的 npc 屬性
 *
 *  @param     npc  記錄 npc 屬性的結構指標
 *  @param[in] code 屬性碼
 *
 *  @return none.
 **/
void states_set(struct npc_state *npc, char *code) {
    for (int i = 0; i < strlen(code); i++) {
        switch (code[i]) {
            case 'a':
                npc->states &= ~NPC_GENDER_MASK;

                break;

            case 'b':
                npc->states |= NPC_BIGENDER;

                break;

            case 'f':
                npc->states |= NPC_FEMALE;

                break;

            case 'm':
                npc->states |= NPC_MALE;

                break;

            case 'p':
                npc->states |= NPC_POISONED;

                break;

            case 's':
                npc->states |= NPC_SICKNESS;

                break;

            case 'w':
                npc->states |= NPC_WOUNDED;

                break;

            default:
                break;
        } // esac
    } // od
} // states_set()

/**
 *  根據 *屬性碼* (code) 移除相對應的 npc 屬性
 *
 *  @param     npc  記錄 npc 屬性的結構指標
 *  @param[in] code 屬性碼
 *
 *  @return none.
 **/
void states_unset(struct npc_state *npc, char *code) {
    for (int i = 0; i < strlen(code); i++) {
        switch (code[i]) {
            case 'f':
                npc->states &= ~NPC_FEMALE;

                break;

            case 'm':
                npc->states &= ~NPC_MALE;

                break;

            case 'p':
                npc->states &= ~NPC_POISONED;

                break;

            case 's':
                npc->states &= ~NPC_SICKNESS;

                break;

            case 'w':
                npc->states &= ~NPC_WOUNDED;

                break;

            default:
                break;
        } // esac
    } // od
} // states_unset()

/**
 *  印出簡易使用說明
 *
 *  @param  none.
 *
 *  @return none.
 **/
void usage(void) {
    printf("usage:\n");
    printf("    rpg_states {[+|-]{p|s|w|a|b|m|f}}+\n");
    printf("      操作:\n");
    printf("        +: 記錄屬性\n");
    printf("        -: 移除屬性\n");
    printf("\n");
    printf("      屬性:\n");
    printf("        a: 無性 (agender)\n");
    printf("        b: 雙性 (bigender)\n");
    printf("        f: 女性 (female)\n");
    printf("        m: 男性 (male)\n");
    printf("        p: 中毒 (posioned)\n");
    printf("        s: 生病 (sickness)\n");
    printf("        w: 受傷 (wounded)\n");
    printf("\n");
    printf("      範例:\n");
    printf("        ./rpg_states +msw -s\n");

    exit(0);
} // usage();

/**
 *  rpg_states 程式進入點。
 *
 *  @param[in]  argc 命令列參數個數。
 *  @param[in]  argv 命令列參數陣列；每個參數都是一個 string。
 *
 *  @return     none.
 **/
void main(int argc, char *argv[]) {
    struct npc_state npc = {0};

    if (argc < 2) {
        usage();
    } // fi

    states_dump(&npc);

    for (int i = 1; i < argc; i++) {
        printf("操作: %s\n", argv[i]);

        if (*argv[i] == '+') {
            states_set(&npc, argv[i]++);
        } // fi
        else if (*argv[i] == '-') {
            states_unset(&npc, argv[i]++);
        } // if
        else {
            printf("錯誤!!! 不支援的操作。\n");
            usage();
        } // esle

        states_dump(&npc);
    } // od
} // main()

// rpg_states.c
