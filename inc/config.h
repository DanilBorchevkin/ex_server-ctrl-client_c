/**
 * @file      config.h
 *
 * @brief     Config header
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

#ifndef __CONFIG_H_
#define __CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * DEFINES
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/******************************************************************************
 * DEFINES
 ******************************************************************************/

#define CONFIG_BUFFER_SIZE ((size_t)1024)  /**< Buffer size for send \ receive \
                                            */
#define CONFIG_CTRL_PERIOD_SEC ((size_t)2) /**< Controller send period */
#define CONFIG_SRV_PORT ((uint16_t)8888)   /** Server port */

/******************************************************************************
 * END OF HEADER'S CODE
 ******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif  // __CONFIG_H_

/** @}*/
