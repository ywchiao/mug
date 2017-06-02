/**
 *  @file       socket_io.c
 *  @brief      The socket related routines on the client side.
 *  @author     Yiwei Chiao (ywchiao@gmail.com)
 *  @date       06/01/2017 created.
 *  @date       06/01/2017 last modified.
 *  @version    0.1.0
 *  @copyright  MIT, (C) 2017 Yiwei Chiao
 *  @details
 *
 *  The socket related routines on the client side.
 */

#include <string.h>

#include <arpa/inet.h>

/**
 *  Connect to server ip:port and return the socket.
 *
 *  @param[in]  ip   伺服器 ip 地址
 *  @param[in]  port 伺服器通訊埠 (port) 號碼
 *
 *  @return     通訊 socket 的 File Descripter, FD
 **/
int socket_connect(char *ip, int port) {
    int fd_socket;
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    inet_pton(AF_INET, ip, &(server_addr.sin_addr));

    fd_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    connect(
        fd_socket,
        (struct sockaddr *)&server_addr,
        sizeof(server_addr)
    );

    return fd_socket;
} // socket_connect()

// socket_io.c
