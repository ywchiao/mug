/**
 *  @file       mugd.c
 *  @brief      The entry point of MUG server (daemon).
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       03/08/2017 created.
 *  @date       04/05/2017 last modified.
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
int socket_server(int port) {
    int fd_socket;
    struct sockaddr_in server_addr;

    fd_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(port);

    bind(fd_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(fd_socket, 10);

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

int reception(int fd_server) {
    char *welcome = "Welcome to the world of MUG.\n";
    int fd_guest;

    fd_guest = accept(fd_server, (struct sockaddr *)NULL, NULL);

    write(fd_guest, welcome, strlen(welcome) + 1);
    printf("send -> %s", welcome);

    return fd_guest;
} // reception()

/**
 *  MugD 伺服端程式進入點。
 *
 *  @param[in]  argc 命令列參數個數。
 *  @param[in]  argv 命令列參數陣列；每個參數都是一個 string。
 *
 *  @return     程式結束狀態
 **/
int main(int argc, char *argv[]) {
    char buffer[BUF_MSGS][BUF_SIZE];
    int fd_server;
    int n_guests = 1;
    int msg_guest[MAX_GUESTS] = {0};
    int msg_buffer = 0;
    struct pollfd poll_fds[MAX_GUESTS];

    if (argc != 2) {
        usage();
    } // fi

    fd_server = socket_server(strtol(argv[1], NULL, 0));

    printf("MUG server started at %s:%s ...\n", "127.0.0.1", argv[1]);

    poll_fds[0].fd = fd_server;
    poll_fds[0].events = POLLIN;

    while (true) {
        int sockets = 0;

        poll(poll_fds, n_guests, -1);

        sockets = n_guests;

        for (int i = 0; i < sockets; i ++) {
            if ((poll_fds[i].revents & POLLIN) != POLLIN) {
                continue;
            } // fi

            if (poll_fds[i].fd == fd_server) {
                int fd_guest;

                do {
                    fd_guest = reception(fd_server);

                    poll_fds[n_guests].fd = fd_guest;
                    poll_fds[n_guests].events = POLLIN | POLLOUT;

                    n_guests ++;
                } while (fd_guest != -1);
            } // fi
            else {
                memset(buffer[msg_buffer], 0, BUF_SIZE);

                read(poll_fds[i].fd, buffer[msg_buffer], BUF_SIZE);
                printf("recv <- %d:%s", poll_fds[i].fd, buffer[msg_buffer]);

                msg_buffer = (msg_buffer + 1) % BUF_MSGS;
            } // esle
        } // od

        for (int i = 0; i < sockets; i ++) {
            if ((poll_fds[i].revents & POLLOUT) != POLLOUT) {
                continue;
            } // fi

            if (msg_guest[i] != msg_buffer) {
                int idx = msg_guest[i];

                write(poll_fds[i].fd, buffer[idx], strlen(buffer[idx]) + 1);
                printf("send -> %d:%s", poll_fds[i].fd, buffer[idx]);

                msg_guest[i] = (msg_guest[i] + 1) % BUF_MSGS;
            } // fi
        } // od
    } // while
} // main()

// mugd.c
