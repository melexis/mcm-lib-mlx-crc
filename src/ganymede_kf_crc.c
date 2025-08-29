/**
 * @file
 * @brief Ganymede KF CRC module
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
 * @details This file contains the implementations of the Ganymede KF CRC module.
 */
#include <stddef.h>
#include <stdint.h>

#include "ganymede_kf_crc.h"

/** Compress a flash word (32-bit + 8-bit ECC) to an 20-bit.
 *
 * @param[in]  data  flash word to compress.
 * @return  compressed data.
 */
static uint32_t compressor_by_2(uint64_t data) {
    uint32_t result = 0;

    for (int i = 0; i < 38; i += 2) {
        uint8_t b0 = (data >> i) & 1;
        uint8_t b1 = (data >> (i + 1)) & 1;
        uint8_t b2 = (data >> (i + 2)) & 1;

        uint8_t compressed_bit = b0 ^ b1 ^ b2;
        result |= (compressed_bit << (i / 2));
    }

    uint8_t b38 = (data >> 38) & 1;
    uint8_t b39 = (data >> 39) & 1;
    uint8_t b0 = data & 1;

    uint8_t last_bit = b38 ^ b39 ^ b0;
    result |= (last_bit << 19);

    return result;
}

uint32_t crc_calcGanyKfCrc(const uint16_t * data, size_t length, uint32_t seed) {
    uint32_t u32Crc = seed;
    uint64_t flash_word;

    /* calc the crc but exclude the 2 last words which should be the crc itself */
    for (size_t word_nr = 0; word_nr < length - 2; word_nr = word_nr + 2) {
        flash_word = ((uint64_t)(data[word_nr + 1]) << 16) | data[word_nr + 0];
        uint32_t comp_word = compressor_by_2(flash_word);
        uint32_t c = u32Crc & 0xE10000u;
        uint32_t d0 = 0u;

        if ((c & 0x800000u) != 0u) {
            d0 ^= 1u;
        }
        if ((c & 0x400000u) != 0u) {
            d0 ^= 1u;
        }
        if ((c & 0x200000u) != 0u) {
            d0 ^= 1u;
        }
        if ((c & 0x010000u) != 0u) {
            d0 ^= 1u;
        }

        u32Crc = (u32Crc << 1) ^ comp_word ^ d0;
    }

    return u32Crc & 0xFFFFFFu;
}

