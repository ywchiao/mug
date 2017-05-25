/**
 *  @file       mugd.c
 *  @brief      The entry point of MUG server (daemon).
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       05/25/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The entry point of MUG server (daemon).
 */

#include "msg.h"
#include "user.h"
#include "msg_buffer.h"

#include "mugd.h"

/**
 *  Set up and return the ServerSocket file descripter.
 *
 *  @param[in]  port 伺服器通訊埠 (port) 號碼
 *
 *  @return     ServerSocket 的 FileDescripter (FD)
 **/
int socket_server(int port, struct pollfd *poll_fds) {
    int fd_socket;
    struct sockaddr_in server_addr;

    // 建立一個 NONBLOCK 的 STREAM (串流) socket
    fd_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    memset(&server_addr, 0, sizeof(server_addr));

    // 設定預定的主機地址，通訊埠 (port)
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);

    // 將 fd_socket 和伺服地址資訊 *綁定*
    bind(fd_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 開始 *聆聽* (listen) 連線要求
    listen(fd_socket, 10);

    // 利用 poll() 來監測 socket 活動 (activities)
    poll_fds[0].fd = fd_socket;

    // 對 server_socket，只監測 input 事件 (客戶端的連線要求)
    poll_fds[0].events = POLLIN;

    return fd_socket;
} // socket_server()

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
 *  接受一個新的連線要求，同時送出歡迎訊息
 *
 *  @param[in]  fd_server Server_socket 的 file descriptor: fd.
 *
 *  @return     新連線的 file descriptor; 或 -1, 沒有更多的連線要求
 **/
int reception(int fd_server) {
    struct msg greeting;
    char *welcome = "Welcome to the world of MUG.\nWhat is your name?";
    int fd_guest = -1;

    memset(&greeting, 0, sizeof(struct msg));

    greeting.type = MSG_WELCOME;
    strcpy(greeting.source, "伺服");
    strcpy(greeting.text, welcome);

    fd_guest = accept(fd_server, (struct sockaddr *)NULL, NULL);

    if (fd_guest > 0) {
        write(fd_guest, &greeting, sizeof(struct msg));
        printf("send -> %s\n", greeting.text);
    } // fi
    else if (!((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
        // if not no further connection requests (EAGAIN || EWOULDBLOCK)
        // then something wrong!!!
        fprintf(
            stderr,
            "Function: %s@%s:line %d: %s\n",
            __func__, __FILE__, __LINE__,
            strerror(errno)
        );

        exit(-1);
    } // esle

    return fd_guest;
} // reception()

/**
 *  將客戶端送來的的訊息， *暫存* 在訊息暫存區 (msg_buffer)
 *
 *  @param[in]  poll_fds        目前已連線 (過) 的客戶端 pollfd 結構
 *  @param[in]  guests          目前的連線客戶端總數
 *
 *  @return     下一個可用的訊息暫存區索引 (index)
 **/
void msg_input(struct pollfd *poll_fds, struct user *clients, int guests) {
    // 處理聊天訊息
    for (int i = 1; i < guests; i ++) {
        // 如果有 _客戶_ (某個 fd) 傳送了資料進來，接收它
        if ((poll_fds[i].revents & POLLIN) == POLLIN) {
            int count = 0;
            struct msg *msg_buffer = NULL;

            msg_buffer = buf4write();

            // 將 _訊息暫存區_ (msg_buffer) 的記憶體空間清空
            memset(msg_buffer, 0, sizeof(struct msg));

            // count 代表 read(...) 讀到的字元 (character) 數
            count = read(poll_fds[i].fd, msg_buffer, sizeof(struct msg));

            // 如果 count == 0，代表客戶端 (client) 已離線
            if (count == 0) {
                // 關閉 (close) 相關的 socket
                close(poll_fds[i].fd);

                // 同時標定 fd = -1，以備後續清理
                poll_fds[i].fd = -1;
            } // fi
            else {
                // 客戶端傳來它的 _暱稱_ (nickname)
                if (msg_buffer->type == MSG_NICKNAME) {
                    char *nick = msg_buffer->text;

                    // 移除字串尾端的 "\r\n" 字元
                    nick[strcspn(nick, "\r\n")] = '\0';

                    // 將客戶 nickname 保留下來
                    strcpy(clients[i].nick, nick);
                } // fi

                printf(
                    "recv <- %d:%s:%s",
                    poll_fds[i].fd,
                    msg_buffer->source,
                    msg_buffer->text
                );

                // 將客戶 nickname 放入 _訊息物件_ 的 source 欄
                strcpy(msg_buffer->source, clients[i].nick);
            } // esle
        } // fi
    } // od
} // msg_input()

/**
 *  將收到的訊息， *廣播* (broadcast) 給群裡的所有人
 *
 *  @param[in]  poll_fds        目前已連線 (過) 的客戶端 pollfd 結構
 *  @param[in]  clients      伺服收到/暫存的訊息
 *  @param[in]  guests          目前的連線客戶端總數
 *
 *  @return     none.
 **/
void msg_output(struct pollfd *poll_fds, struct user *clients, int guests) {
    // 處理訊息 *群播* (multicast)
    // 將聊天訊息傳遞給聊天室的所有人
    for (int i = 0; i < guests; i ++) {
        // 檢查 poll_fds[i] 是否準備好可以接收 output 訊息
        if ((poll_fds[i].revents & POLLOUT) == POLLOUT) {
            // idx 代表特定客戶端 *預計接收* 的訊息索引
            struct msg *message = NULL;

            message = buf4read(&clients[i].msg_idx);

            // 如果有新的訊息等待傳送
            if (message != NULL) {
                write(
                    poll_fds[i].fd,
                    message,
                    sizeof(struct msg)
                );

                printf(
                    "send -> %d:%d:%s:%s\n",
                    clients[i].msg_idx,
                    poll_fds[i].fd,
                    clients[i].nick,
                    message->text
                );
            } // fi
        } // fi
    } // od
} // msg_output()

/**
 *  接受新的客戶端連線，同時放入 poll_fds 中，由 poll() 監控
 *
 *  @param      poll_fds 存放已連線客戶端 pollfd 結構的陣列指標
 *  @param[in]  n_guests 目前已連線 (過) 的客戶端總數
 *  @param[in]  fd_server Server socket 的 file descriptor.
 *
 *  @return     目前已連線的客戶端總數
 **/
int guest_new(struct pollfd *poll_fds, struct user *clients, int n_guests) {
    int fd_guest = -1;
    int fd_server = poll_fds[0].fd;

    do {
        fd_guest = reception(fd_server);

        if (fd_guest > 0) {
            poll_fds[n_guests].fd = fd_guest;

            clients[n_guests].nick[0] = 0;
            clients[n_guests].msg_idx = 0;

            // 設定對 *新*連線，n_guests，要監測它的
            // input 和 output 事件
            poll_fds[n_guests].events = POLLIN | POLLOUT;

            n_guests ++;
        } // fi
    } while (fd_guest != -1);

    return n_guests;
} // guest_new()

/**
 *  MugD 伺服端程式進入點。
 *
 *  @param[in]  argc 命令列參數個數。
 *  @param[in]  argv 命令列參數陣列；每個參數都是一個 string。
 *
 *  @return     程式結束狀態
 **/
int main(int argc, char *argv[]) {
    int n_guests = 1;
    struct pollfd poll_fds[MAX_GUESTS];
    struct user clients[MAX_GUESTS];

    if (argc != 2) {
        usage();
    } // fi

    // 啟動 server_socket
    socket_server(strtol(argv[1], NULL, 0), poll_fds);

    printf("MUG server started at %s:%s ...\n", "127.0.0.1", argv[1]);

    while (true) {
        int sockets = 0;

        // 監控是否有新的 *通訊事件* 發生
        poll(poll_fds, n_guests, -1);

        sockets = n_guests;

        // 處理新的連線要求
        if ((poll_fds[0].revents & POLLIN) == POLLIN) {
            n_guests = guest_new(poll_fds, clients, n_guests);
        } // fi

        // 接收客戶端送來的訊息
        msg_input(poll_fds, clients, sockets);

        // 將客戶端傳來的訊息， *廣播* 給所有連線客戶端
        msg_output(poll_fds, clients, sockets);
    } // while
} // main()

// mugd.c
