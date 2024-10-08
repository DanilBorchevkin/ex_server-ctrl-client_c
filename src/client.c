/**
  * @file      client.c
  *
  * @brief     Client Module
  *
  * @date      2024-10-08
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

/** \addtogroup client
*  @{
*/

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include "client.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include "common.h"

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



/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

int32_t client_connect(const char * p_host, const char* p_serv, int * p_socket_fd) {
    if (NULL == p_host) {
        return CLIENT_ERR_PARAM;
    }

    if (NULL == p_serv) {
        return CLIENT_ERR_PARAM;
    }

    if (NULL == p_socket_fd) {
        return CLIENT_ERR_PARAM;
    }

    struct addrinfo hints;
    struct addrinfo * p_result;
    struct addrinfo * p_rp;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int ret = getaddrinfo(p_host, p_serv, &hints, &p_result);

    if (0 != ret) {
        // NOTE: can view error by gai_strerror(s) if need
        return CLIENT_ERR_RESOLVE;
    }

    for (p_rp = p_result; p_rp != NULL; p_rp = p_rp->ai_next) {
        *p_socket_fd = socket(p_rp->ai_family, p_rp->ai_socktype, p_rp->ai_protocol);
        if (COMMON_SOCKET_ERR == *p_socket_fd) {
            continue;
        }

        if (COMMON_SOCKET_ERR != connect(*p_socket_fd, p_rp->ai_addr, p_rp->ai_addrlen)) {
            break; 
        }
        
        close(*p_socket_fd); 
    }

    freeaddrinfo(p_result); 

    if (NULL == p_rp) {
        return CLIENT_ERR_CONNECT;
    }

    return CLIENT_ERR_OK;
}

int32_t client_disconnect(int * p_socket_fd) {
    if (NULL == p_socket_fd) {
        return CLIENT_ERR_PARAM;
    }

    close(*p_socket_fd);
    *p_socket_fd = COMMON_SOCKET_ERR;

    return CLIENT_ERR_OK;
}

/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
