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

static int  g_socket_fd = -1;
static void free_ctx(void);
static void sigint_handler(int ctx);

/******************************************************************************
 * PRIVATE FUNCTIONS
 ******************************************************************************/

static void free_ctx(void) {
    close(g_socket_fd);
}

static void sigint_handler(int sig) {
    signal(sig, SIG_IGN);
    printf("\n\nCtrl+C was pressed. Exit\n\n");
    free_ctx();
    exit(0);
}

/******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler);

    struct addrinfo hints;
    struct addrinfo * result;
    struct addrinfo * rp;

    int s = 0;
    int j = 0;
    size_t len = 0;
    ssize_t nread;

    #define BUF_SIZE 1024
    char buf[BUF_SIZE];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s, host port  \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Obtain addreses matching host / port */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    s = getaddrinfo(argv[1], argv[2], &hints, &result);

    if(s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(4);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        g_socket_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (g_socket_fd == -1) {
            continue;
        }

        if (connect(g_socket_fd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break; 
        }

        close(g_socket_fd); 
    }
    freeaddrinfo(result); 

    if (NULL == rp) {
        fprintf(stderr, "[CONTROLLER] Could not connect. Exit\n");
        exit(EXIT_FAILURE);
    }

    printf("[CONTROLLER] Connected to server. Press Ctr+C for exit if need\n");

    while (true) {
        send(g_socket_fd, "ping", 4, 0);
        sleep(5);
    }

    exit(EXIT_SUCCESS);
}


/******************************************************************************
 * END OF SOURCE'S CODE
 ******************************************************************************/
/** @}*/
