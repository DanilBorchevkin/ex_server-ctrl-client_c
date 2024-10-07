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

/******************************************************************************
 * DEFINES, CONSTS, ENUMS
 ******************************************************************************/

#define SOCKET_ERR      ((int) -1)  /**< Standard error value for sockets */

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

}

/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
