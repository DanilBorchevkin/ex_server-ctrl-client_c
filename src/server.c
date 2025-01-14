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

#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "config.h"

/******************************************************************************
 * DEFINES, CONSTS, ENUMS
 ******************************************************************************/

#define POLL_TIMEOUT_INF ((int)-1) /**< Infinite timeout for polling */

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

/**
 * @brief Init server
 *
 * @param p_handle pointer to server handle
 * @param p_conf pointer to server config
 * @return int32_t 0 if OK, error otherwise
 */
int32_t server_init(server_handle_t *p_handle, const server_conf_t *p_conf) {
    if (NULL == p_handle) {
        return SERVER_ERR_PARAMS;
    }

    if (NULL == p_conf) {
        return SERVER_ERR_PARAMS;
    }

    memset(p_handle, 0x00, sizeof(server_handle_t));
    memcpy(&p_handle->conf, p_conf, sizeof(server_conf_t));

    p_handle->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (COMMON_SOCKET_ERR == p_handle->socket_fd) {
        return SERVER_ERR_SOCKET;
    }

    p_handle->sockaddr.sin_family = AF_INET;
    p_handle->sockaddr.sin_addr.s_addr = htonl(p_handle->conf.addr);
    p_handle->sockaddr.sin_port = htons(p_handle->conf.port);

    int ret = bind(p_handle->socket_fd, (struct sockaddr *)&p_handle->sockaddr,
                   sizeof(p_handle->sockaddr));
    if (COMMON_SOCKET_ERR == ret) {
        return SERVER_ERR_SOCKET;
    }

    p_handle->max_clients = server_max_clients();
    ret = listen(p_handle->socket_fd, p_handle->max_clients);
    if (COMMON_SOCKET_ERR == ret) {
        return SERVER_ERR_SOCKET;
    }

    return SERVER_ERR_OK;
}

int32_t server_deinit(server_handle_t *p_handle) {
    if (NULL == p_handle) {
        return SERVER_ERR_PARAMS;
    }

    close(p_handle->socket_fd);
    p_handle->socket_fd = COMMON_SOCKET_ERR;

    return SERVER_ERR_OK;
}

/**
 * @brief Accept incoming connecion
 *
 * @param p_handle pointer to server handle
 * @param p_client pointer to server client
 * @return int32_t 0 if OK, error otherwise
 */
int32_t server_accept(server_handle_t *p_handle, server_client_t *p_client) {
    if (NULL == p_handle) {
        return SERVER_ERR_PARAMS;
    }

    if (NULL == p_client) {
        return SERVER_ERR_PARAMS;
    }

    memset(p_client, 0x00, sizeof(server_client_t));

    p_client->sockaddr_len = sizeof(p_client->sockaddr);
    p_client->socket_fd =
        accept(p_handle->socket_fd, (struct sockaddr *)&p_client->sockaddr,
               &p_client->sockaddr_len);

    return SERVER_ERR_OK;
}

/**
 * @brief Return max possible clients for server
 *
 * @return size_t max count of clients
 */
size_t server_max_clients(void) { return ((size_t)sysconf(_SC_OPEN_MAX)); }

/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
