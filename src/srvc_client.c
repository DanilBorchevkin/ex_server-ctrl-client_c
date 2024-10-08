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
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <client.h>
#include "config.h"
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

static int  g_socket_fd = COMMON_SOCKET_ERR;      /**< Global socket variable */

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

static void sigint_handler(int ctx);

/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

static void sigint_handler(int sig) {
    signal(sig, SIG_IGN);
    printf("\n\n[CLIENT] Ctrl+C was pressed. Exit\n\n");

    client_disconnect(&g_socket_fd);

    exit(EXIT_SUCCESS);
}

/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler);

    if (argc < 3) {
        fprintf(stderr, "\nUsage: %s, <host> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int32_t ret = client_connect(argv[1], argv[2], &g_socket_fd);
    if (CLIENT_ERR_OK != ret) {
        printf("[CLIENT] Cannot connect to server. Error <%d> Exit\n", ret);    
        exit(EXIT_FAILURE);
    }

    printf("[CLIENT] Connected to server. Press Ctr+C for exit\n");

    char buffer[CONFIG_BUFFER_SIZE];

    while (true) {
        ssize_t count = 0;
        count = read(g_socket_fd, buffer, CONFIG_BUFFER_SIZE);

        if (COMMON_SOCKET_ERR == count) {
            printf("[CLIENT] Socket error. Exit\n");
            break;
        } else if (COMMON_SOCKET_CLOSED == count) {
            printf("[CLIENT] Connection is closed. Exit\n");
            break;
        } else {
            printf("[CLIENT] Received <%ld> bytes: <%s>\n", (size_t) count, buffer);
        }
    }

    client_disconnect(&g_socket_fd);

    exit(EXIT_SUCCESS);
}


/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
