/**
 * @file
 * @brief The crc component definitions.
 * @internal
 *
 * @copyright (C) 2023 Melexis N.V.
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
 * @details This file contains the definitions of the crc component.
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include "ganymede_kf_crc.h"
#include "ganymede_xfe_crc.h"

#ifdef __cplusplus
extern "C" {
#endif

/** flash memory crc calculation function type */
typedef uint32_t (*flash_crc_func_t) (const uint16_t * data, size_t length, uint32_t seed);

/** Calculate the 8-bit checksum
 *
 * @param[in]  data  data to calculate the checksum for.
 * @param[in]  length  length of the data.
 *
 * @return  the calculated checksum.
 */
uint8_t crc_calcPageChecksum(const uint16_t * data, size_t length);

/** Calculate the 16-bit crc
 *
 * @param[in]  data  data to calculate the crc for.
 * @param[in]  length  length of the data.
 * @param[in]  seed  seed for crc calculation.
 *
 * @return  the calculated crc.
 */
uint16_t crc_calc16bitCrc(const uint8_t * data, size_t length, uint16_t seed);

/** Calculate the 24-bit crc
 *
 * @param[in]  data  data to calculate the crc for.
 * @param[in]  length  length of the data.
 * @param[in]  seed  seed for the calculation.
 *
 * @return  the calculated crc.
 */
uint32_t crc_calc24bitCrc(const uint16_t * data, size_t length, uint32_t seed);

#ifdef __cplusplus
}
#endif
