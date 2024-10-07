/**
  * @file      server.c
  *
  * @brief     Server module implementation
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

/** \addtogroup server
*  @{
*/

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include "server.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>
#include <stdio.h>

/******************************************************************************
 * DEFINES, CONSTS, ENUMS
 ******************************************************************************/

#define SOCKET_ERR          ((int) -1)  /**< Standard error value for sockets */
#define POLL_TIMEOUT_INF    ((int) -1)  /**< Infinite timeout for polling */    

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



/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

int32_t server_init(server_handle_t * p_handle, const server_conf_t * p_conf) {
    if (NULL == p_handle) {
        return SERVER_ERR_PARAMS;
    }

    if (NULL == p_conf) {
        return SERVER_ERR_PARAMS;
    }

    memset(p_handle, 0x00, sizeof(server_handle_t));
    memcpy(&p_handle->conf, p_conf, sizeof(server_conf_t));

    p_handle->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (SOCKET_ERR == p_handle->socket_fd) {
        return SERVER_ERR_SOCKET;
    }

    p_handle->sockaddr.sin_family = AF_INET;
    p_handle->sockaddr.sin_addr.s_addr = htonl(p_handle->conf.addr);
    p_handle->sockaddr.sin_port = htons(p_handle->conf.port);

    int ret = bind(p_handle->socket_fd, (struct sockaddr *) &p_handle->sockaddr, sizeof(p_handle->sockaddr));
    if (SOCKET_ERR == ret) {
        return SERVER_ERR_SOCKET;
    }

    ret = listen(p_handle->socket_fd, p_handle->conf.max_clients);
    if (SOCKET_ERR == ret) {
        return SERVER_ERR_SOCKET;
    }

    return SERVER_ERR_OK;
}

int32_t server_deinit(server_handle_t * p_handle) {
    if (NULL == p_handle) {
        return SERVER_ERR_PARAMS;
    }

    close(p_handle->socket_fd);

    return SERVER_ERR_OK;
}

int32_t server_accept(server_handle_t * p_handle, server_client_t * p_client) {
    if (NULL == p_handle) {
        return SERVER_ERR_PARAMS;
    }

    if (NULL == p_client) {
        return SERVER_ERR_PARAMS;
    }

    memset(p_client, 0x00, sizeof(server_client_t));

    p_client->sockaddr_len = sizeof(p_client->sockaddr);
    p_client->socket_fd = accept(p_handle->socket_fd, (struct sockaddr *) &p_client->sockaddr, &p_client->sockaddr_len);

    return SERVER_ERR_OK;
}

int32_t server_concurrent(server_handle_t * p_handle) {
    if (NULL == p_handle) {
        return SERVER_ERR_PARAMS;
    }

    const int OPEN_MAX = sysconf(_SC_OPEN_MAX); 
    struct pollfd clients[OPEN_MAX];

    printf("[SERVER] Max connections is <%d>\n", OPEN_MAX);

    for (size_t idx = 0; idx < OPEN_MAX; idx++) {
        clients[idx].fd = SOCKET_ERR;
    }

    clients[0].fd = p_handle->socket_fd;
    clients[0].events = POLLIN;

    size_t peak_idx = 0;

    while (true) {
        int count_ready = poll(clients, peak_idx + 1, 0);

        sleep(1);

        for (size_t idx = 2; idx <= peak_idx; idx++) {
            if (SOCKET_ERR == clients[idx].fd) {
                continue;
            }
        }

        // Check the new connection
        server_client_t client;
        if (clients[0].revents & POLLIN) {
            printf("[SERVER] New connection\n");
            int ret = server_accept(p_handle, &client);
            if (SERVER_ERR_OK != ret) {
                printf("[SERVER] Error during accept connection\n");
                continue;
            }
            --count_ready;
        }

        // Save save clients
        size_t idx = 0;
        for (idx = 0; idx < OPEN_MAX; idx++) {
            if (clients[idx].fd < 0) {
                clients[idx].fd = client.socket_fd;
                if (1 == idx) {
                    printf("[SERVER] Registered controller client\n");
                }
                break;
            }
        }

        if (OPEN_MAX == idx) {
            fprintf(stderr, "[SERVER] Error: too many clients\n");
            close(client.socket_fd);
        } else {
            clients[idx].events = POLLIN;

            if (idx > peak_idx) {
                peak_idx = idx;
            }
        }

        if (count_ready <= 0) {
            continue;
        }
    }
}

/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
