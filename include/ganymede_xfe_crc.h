/**
 * @file
 * @brief Ganymede XFE CRC class
 * @internal
 *
 * @copyright (C) 2025 Melexis N.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @endinternal
 *
 * @ingroup lib_mlx_crc
 *
 * @details This file contains the definitions of the Ganymede XFE CRC class
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Calculate the Ganymede 18-bit XFE CRC
 *
 * @param[in]  data  data to calculate the crc for.
 * @param[in]  length  length of the data.
 * @param[in]  seed  seed for the calculation.
 *
 * @return  the calculated checksum.
 */
uint32_t crc_calcGanyXfeCrc(const uint16_t * data, size_t length, uint32_t seed);

#ifdef __cplusplus
}
#endif
