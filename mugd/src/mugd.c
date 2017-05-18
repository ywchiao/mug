/**
 *  @file       mugd.c
 *  @brief      The entry point of MUG server (daemon).
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       05/17/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The entry point of MUG server (daemon).
 */

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
    char *welcome = "Welcome to the world of MUG.\n";
    int fd_guest = -1;

    fd_guest = accept(fd_server, (struct sockaddr *)NULL, NULL);

    if (fd_guest > 0) {
        write(fd_guest, welcome, strlen(welcome) + 1);
        printf("send -> %s", welcome);
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
 *  @param[in]  msg_buffer      訊息暫存區
 *  @param[in]  msg_index       訊息暫存區 (msg_buffer) 的空位索引 (index)
 *  @param[in]  guests          目前的連線客戶端總數
 *
 *  @return     下一個可用的訊息暫存區索引 (index)
 **/
int msg_input(
    struct pollfd *poll_fds, char msg_buffer[][BUF_SIZE],
    int msg_index, int guests
) {
    // 處理聊天訊息
    for (int i = 1; i < guests; i ++) {
        // 如果有 *客戶* (某個 fd) 傳送了資料進來，接收它
        if ((poll_fds[i].revents & POLLIN) == POLLIN) {
            int count = 0;

            // 將 *訊息暫存區* (msg_buffer) 的記憶體空間清空
            memset(msg_buffer[msg_index], 0, BUF_SIZE);

            // count 代表 read(...) 讀到的字元 (character) 數
            count = read(poll_fds[i].fd, msg_buffer[msg_index], BUF_SIZE);

            // 如果 count == 0，代表客戶端 (client) 已離線
            if (count == 0) {
                // 關閉 (close) 相關的 socket
                close(poll_fds[i].fd);

                // 同時標定 fd = -1，以備後續清理
                poll_fds[i].fd = -1;
            } // fi
            else {
                printf("recv <- %d:%s", poll_fds[i].fd, msg_buffer[msg_index]);

                // 更新 *訊息暫存區* 的索引 
                msg_index = (msg_index + 1) % BUF_MSGS;
            } // esle
        } // fi
    } // od

    return msg_index;
} // msg_input()

/**
 *  將收到的訊息， *廣播* (broadcast) 給群裡的所有人
 *
 *  @param[in]  poll_fds        目前已連線 (過) 的客戶端 pollfd 結構
 *  @param[in]  msg_buffer      伺服收到/暫存的訊息
 *  @param[in]  msg_index       訊息暫存區 (msg_buffer) 的空位索引 (index)
 *  @param[in]  guests          目前的連線客戶端總數
 *  @param[in]  msg_guest_idx   目前客戶端 *已* 收到的訊息索引
 *
 *  @return     none.
 **/
void msg_output(
    struct pollfd *poll_fds, char msg_buffer[][BUF_SIZE],
    int msg_index, int guests, int *msg_guest_idx
) {
    // 處理訊息 *群播* (multicast)
    // 將聊天訊息傳遞給聊天室的所有人
    for (int i = 0; i < guests; i ++) {
        // 檢查 poll_fds[i] 是否準備好可以接收 output 訊息
        if ((poll_fds[i].revents & POLLOUT) == POLLOUT) {
            // idx 代表特定客戶端 *預計接收* 的訊息索引
            int idx = msg_guest_idx[i];

            // 如果有新的訊息等代接收
            if (idx != msg_index) {
                write(
                    poll_fds[i].fd,
                    msg_buffer[idx],
                    strlen(msg_buffer[idx])
                );

                printf("send -> %d:%s", poll_fds[i].fd, msg_buffer[idx]);

                // 更新 *下一次* 要接收的訊息索引
                msg_guest_idx[i] = (idx + 1) % BUF_MSGS;
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
int guest_new(struct pollfd *poll_fds, int n_guests) {
    int fd_guest = -1;
    int fd_server = poll_fds[0].fd;

    do {
        fd_guest = reception(fd_server);

        if (fd_guest > 0) {
            poll_fds[n_guests].fd = fd_guest;
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
    char msg_buffer[BUF_MSGS][BUF_SIZE];
    int n_guests = 1;
    int msg_guest_idx[MAX_GUESTS] = {0};
    int msg_index = 0;
    struct pollfd poll_fds[MAX_GUESTS];

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
            n_guests = guest_new(poll_fds, n_guests);
        } // fi

        // 接收客戶端送來的訊息
        msg_index = msg_input(
            poll_fds, msg_buffer, msg_index, sockets
        );

        // 將客戶端傳來的訊息， *廣播* 給所有連線客戶端
        msg_output(
            poll_fds, msg_buffer, msg_index, sockets, msg_guest_idx
        );
    } // while
} // main()

// mugd.c
