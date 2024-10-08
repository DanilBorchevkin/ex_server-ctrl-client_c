/**
  * @file      srvc_server.c
  *
  * @brief     Service - COntroller
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
#include <string.h>
#include <signal.h>
#include "client.h"
#include "common.h"
#include "config.h"

/******************************************************************************
 * DEFINES, CONSTS, ENUMS
 ******************************************************************************/

#define ARGS_COUNT ((size_t)3)    /**< Args count for invocations*/
#define ARGS_IDX_HOST ((size_t)1) /**< Host arg index */
#define ARGS_IDX_PORT ((size_t)2) /**< Port arg index */

/******************************************************************************
 * PRIVATE TYPES
 ******************************************************************************/

/******************************************************************************
 * PRIVATE DATA
 ******************************************************************************/

static int g_socket_fd = COMMON_SOCKET_ERR; /**< Global socket variable */

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

/**
 * @brief SIGINT handler
 * 
 * @param sig received signal
 */
static void sigint_handler(int sig) {
    signal(sig, SIG_IGN);
    printf("\n\n[CONTROLLER] Ctrl+C was pressed. Exit\n\n");

    client_disconnect(&g_socket_fd);

    exit(EXIT_SUCCESS);
}

/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler);

    if (argc != ARGS_COUNT) {
        fprintf(stderr, "\nUsage: %s, <host> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int32_t ret =
        client_connect(argv[ARGS_IDX_HOST], argv[ARGS_IDX_PORT], &g_socket_fd);
    if (CLIENT_ERR_OK != ret) {
        printf("[CONTROLLER] Cannot connect to server. Error <%d> Exit\n", ret);
        exit(EXIT_FAILURE);
    }

    printf("[CONTROLLER] Connected to server. Press Ctr+C for exit\n");

    uint64_t id = 0;

    while (true) {
        char buffer[CONFIG_BUFFER_SIZE];

        ssize_t ret = 0;

        id++;
        ret = snprintf(buffer, CONFIG_BUFFER_SIZE - 1, "{\"id\" : %u}", id);
        if (0 > ret) {
            printf("[CONTROLLER] Internal error. Exit\n");
            break;
        }

        ret = send(g_socket_fd, buffer, (size_t) ret, MSG_NOSIGNAL);

        if (COMMON_SOCKET_ERR == ret) {
            printf("[CONTROLLER] Socket error. Exit\n");
            break;
        } else if (COMMON_SOCKET_CLOSED == ret) {
            printf("[CONTROLLER] Connection is closed. Exit\n");
            break;
        } else {
            printf("[CONTROLLER] Send <%ld> bytes: <%s>\n", (size_t)ret,
                   buffer);
        }

        sleep(CONFIG_CTRL_PERIOD_SEC);
    }

    client_disconnect(&g_socket_fd);

    exit(EXIT_SUCCESS);
}

/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
