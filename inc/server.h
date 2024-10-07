/**
  * @file      server.h
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

#ifndef __SERVER_H_
#define __SERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include <stddef.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
 * DEFINES
 ******************************************************************************/

#define SERVER_ERR_OK       ((ssize_t) 0);
#define SERVER_ERR_NG       ((ssize_t) 1);
#define SERVER_ERR_PARAMS   ((ssize_t) 2); 

/******************************************************************************
 * PUBLIC TYPES
 ******************************************************************************/

typedef struct server_conf_s {
    uint16_t port;
    uint32_t addr;
    size_t max_clients; 
} server_conf_t;

typedef struct server_handle_s {
    int socket_fd;
} server_handle_t;

typedef struct server_client_s {
    int socket_fd;
    struct sockaddr_in sockaddr;
    size_t sockaddr_len;
} server_client_t;

/******************************************************************************
 * INLINE FUNCTIONS
 ******************************************************************************/



/******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 ******************************************************************************/

ssize_t server_init(server_handle_t * p_handle, const server_conf_t * p_conf);
ssize_t server_deinit(server_handle_t * p_handle);
ssize_t server_accept(server_handle_t * p_handle, server_client_t * p_client);

/******************************************************************************
 * END OF HEADER'S CODE
 ******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif // __SERVER_H_

/** @}*/
