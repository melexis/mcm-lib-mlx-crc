/**
 * @file
 * @brief Generic crc component
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
 * @details This file contains the implementations of the generic crc component.
 */
#include <stddef.h>
#include <stdint.h>

#include "mlx_crc.h"

uint8_t crc_calcPageChecksum(const uint16_t * data, size_t length) {
    uint32_t u32Checksum = 0u;

    for (size_t word_nr = 0; word_nr < length; word_nr++) {
        /* "adc A, [%[start]++]\n\t"     Add value to CRC
         * "djnz %[len], .-2\n\t"
         */
        u32Checksum += data[word_nr];

        if (u32Checksum > 0xFFFFu) {
            u32Checksum += 1u;
            u32Checksum &= 0xFFFFu;
        }
    }

    /* "adc AL, AH\n\t"     Add CRC-H to CRC-L, including CY
     * "adc AL, #0\n\t"
     */
    u32Checksum = ((u32Checksum >> 8) & 0xFFu) + (u32Checksum & 0xFFu);
    if (u32Checksum > 0xFFu) {
        u32Checksum += 1u;
    }

    return ((uint8_t)(u32Checksum & 0xFFu));
}

uint16_t crc_calc16bitCrc(const uint8_t * data, size_t length, uint16_t seed) {
    uint16_t u16Crc = seed;

    for (size_t word_nr = 0; word_nr < length; word_nr++) {
        u16Crc = ((u16Crc >> 8) & 0xFFu) | (u16Crc << 8);
        u16Crc ^= data[word_nr];
        u16Crc ^= (u16Crc >> 4) & 0x000Fu;
        u16Crc ^= (u16Crc << 8) << 4;
        u16Crc ^= ((u16Crc & 0xFFu) << 4) << 1;
    }

    return (u16Crc);
}

uint32_t crc_calc24bitCrc(const uint16_t * data, size_t length, uint32_t seed) {
    uint32_t u32Crc = seed;

    /* calc the crc but exclude the 2 last words which should be the crc itself */
    for (size_t word_nr = 0; word_nr < length - 2; word_nr++) {
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

        u32Crc = (u32Crc << 1) ^ data[word_nr] ^ d0;
    }

    return u32Crc & 0xFFFFFFu;
}

