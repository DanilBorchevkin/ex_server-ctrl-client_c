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

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <server.h>

/******************************************************************************
 * DEFINES, CONSTS, ENUMS
 ******************************************************************************/

#define PORT_SERVER     (1067)

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

/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

int main(int argc, char *argv[]) {
    printf("[SERVER] Started\n");

    server_handle_t server_handle;
    server_conf_t server_conf = {
        .addr = INADDR_ANY,
        .max_clients = 1,
        .port = 1067
    };

    int32_t ret = server_init(&server_handle, &server_conf);
    if (SERVER_ERR_OK != ret) {
        printf("[SERVER] Cannot start server. Exit\n");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] Listening ... \n");

    while (true) {
        server_client_t client;
        ret = server_accept(&server_handle, &client);
        if (SERVER_ERR_OK != ret) {
            printf("[SERVER] Error during accept connection");
        }

        printf("[SERVER] Client connected\n");
        client_data_handler(client.socket_fd, client.socket_fd);
        printf("[SERVER] Client disconnected\n");
    }

    return 0;
}
/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
