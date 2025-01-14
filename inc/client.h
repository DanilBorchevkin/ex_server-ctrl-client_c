/**
 * @file      client.h
 *
 * @brief     Client module
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

/** \addtogroup Doxygen group
 *  @{
 */

#ifndef __CLIENT_H_
#define __CLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * INCLUDES
 ******************************************************************************/

#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 * DEFINES
 ******************************************************************************/

#define CLIENT_ERR_OK ((int32_t)0)      /**< Client error - no error */
#define CLIENT_ERR_PARAM ((int32_t)1)   /**< Client error - parameters error */
#define CLIENT_ERR_RESOLVE ((int32_t)2) /**< Client error - resolve error */
#define CLIENT_ERR_CONNECT ((int32_t)3) /**< Client error - connect error */

/******************************************************************************
 * PUBLIC TYPES
 ******************************************************************************/

/******************************************************************************
 * INLINE FUNCTIONS
 ******************************************************************************/

/******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES
 ******************************************************************************/

int32_t client_connect(const char* p_host, const char* p_serv,
                       int* p_socket_fd);
int32_t client_disconnect(int* p_socket_fd);

/******************************************************************************
 * END OF HEADER'S CODE
 ******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif  // __CLIENT_H_

/** @}*/
