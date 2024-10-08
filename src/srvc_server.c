/**
 * @file      srvc_server.c
 *
 * @brief     Service - Server
 *
 * @date      2024-10-07
 *
 * @par
 * BSD 2-Clause License
 *
 * Copyright (c) 2024, Danil Borchevkin
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \addtogroup Doxygen group
 *  @{
 */

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include <ctype.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <server.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"
#include "config.h"

/******************************************************************************
 * DEFINES, CONSTS, ENUMS
 ******************************************************************************/

/******************************************************************************
 * PRIVATE TYPES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE DATA
 ******************************************************************************/

/******************************************************************************
 * PUBLIC DATA
 ******************************************************************************/

/******************************************************************************
 * EXTERNAL DATA
 ******************************************************************************/

/******************************************************************************
 * EXTERNAL FUNCTION PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

void client_data_handler(int in_sock_fd, int out_sock_fd) {
    char buf[1024];
    int count = 0;
    char ping[] = "ping";

    ssize_t ret = 1;
    while (ret > 0) {
        printf("send ping \n");

        ret = send(out_sock_fd, ping, 4, MSG_NOSIGNAL);
        if (-1 == ret) {
            close(out_sock_fd);
        }

        sleep(5);
    }
}

void server_run(server_handle_t *p_handle) {
    if (NULL == p_handle) {
        printf("[SERVER] Error params. Exit\n");
        return;
    }

    const int OPEN_MAX = server_max_clients();
    struct pollfd clients[OPEN_MAX];

    printf("[SERVER] Max connections is <%d>\n", OPEN_MAX);

    for (size_t idx = 0; idx < OPEN_MAX; idx++) {
        clients[idx].fd = COMMON_SOCKET_ERR;
    }

    clients[0].fd = p_handle->socket_fd;
    clients[0].events = POLLIN | POLLERR | POLLOUT;

    size_t peak_idx = 0;

    while (true) {
        int count_ready = poll(clients, peak_idx + 1, -1);

        // NOTE: If no no new events, just rerun from start
        if (count_ready <= 0) {
            continue;
        }

        // Check the new connection
        if (clients[0].revents & POLLIN) {
            printf("[SERVER] New connection\n");

            server_client_t client;
            int ret = server_accept(p_handle, &client);
            if (SERVER_ERR_OK != ret) {
                printf("[SERVER] Error during accept connection\n");
                continue;
            }

            size_t idx = 0;
            for (idx = 1; idx < OPEN_MAX; idx++) {
                if (clients[idx].fd < 0) {
                    clients[idx].fd = client.socket_fd;
                    clients[idx].events = POLLIN | POLLERR | POLLOUT;
                    break;
                }
            }

            if (OPEN_MAX == idx) {
                fprintf(stderr, "[SERVER] Error: too many clients\n");
                close(client.socket_fd);
            } else {
                if (idx > peak_idx) {
                    peak_idx = idx;
                }
            }

            --count_ready;
        }

        if (count_ready <= 0) {
            continue;
        }

        char buffer[CONFIG_BUFFER_SIZE];
        ssize_t ret = 0;
        bool is_new_message = false;
        bool ignore_idx = 0;

        for (size_t idx = 0; idx <= peak_idx; idx++) {
            if (COMMON_SOCKET_ERR == clients[idx].fd) {
                continue;
            }

            if (clients[idx].revents & (POLLIN | POLLERR | POLLOUT)) {
                ret = recv(clients[idx].fd, buffer, CONFIG_BUFFER_SIZE,
                           MSG_NOSIGNAL | MSG_DONTWAIT);

                if (COMMON_SOCKET_ERR == ret) {
                    // printf("[SERVER] Error: cannot read from socket fd
                    // <%d>\n", clients[idx].fd); close(clients[idx].fd);
                    // clients[idx].fd = COMMON_SOCKET_ERR;
                } else if (COMMON_SOCKET_CLOSED == ret) {
                    printf("[SERVER] Connection closed for socket fd <%d>\n",
                           clients[idx].fd);
                    close(clients[idx].fd);
                    clients[idx].fd = COMMON_SOCKET_ERR;
                } else if ((ret > 0)) {
                    printf(
                        "[SERVER] Receive <%u> bytes from controller. "
                        "Retranslate it\n",
                        (size_t)ret);
                    is_new_message = true;
                    ignore_idx = idx;
                    break;
                }
            }
        }

        if (is_new_message) {
            for (size_t idx = 1; idx <= peak_idx; idx++) {
                if (idx == ignore_idx) {
                    continue;
                }

                ret = send(clients[idx].fd, buffer, (size_t)ret, MSG_NOSIGNAL);
                if (COMMON_SOCKET_ERR == ret) {
                    printf("[SERVER] Error: cannot read from socket fd <%d>\n",
                           clients[idx].fd);
                    close(clients[idx].fd);
                    clients[idx].fd = COMMON_SOCKET_ERR;
                } else if (COMMON_SOCKET_CLOSED == ret) {
                    printf("[SERVER] Connection closed for socket fd <%d>\n",
                           clients[idx].fd);
                    close(clients[idx].fd);
                    clients[idx].fd = COMMON_SOCKET_ERR;
                }
            }
        }
    }
}

/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

int main(int argc, char *argv[]) {
    server_handle_t server_handle;
    server_conf_t server_conf = {.addr = INADDR_ANY, .port = CONFIG_SRV_PORT};

    int32_t ret = server_init(&server_handle, &server_conf);
    if (SERVER_ERR_OK != ret) {
        printf("[SERVER] Cannot start server. Exit\n");
        exit(EXIT_FAILURE);
    }

    server_run(&server_handle);

    return EXIT_SUCCESS;
}
/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
