/**
 * @file
 * @brief Ganymede XFE CRC module
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
 * @details This file contains the implementations of the Ganymede XFE CRC module.
 */
#include <stddef.h>
#include <stdint.h>

#include "ganymede_xfe_crc.h"

const uint16_t indices_map[18][7][2] = {
    {{0, 0x0080}, {0, 0x0040}, {0, 0x0020}, {0, 0x0010}, {4, 0x0100}, {4, 0x0020}, {4, 0x0004}},
    {{0, 0x0008}, {0, 0x0004}, {0, 0x0002}, {0, 0x0001}, {0, 0x0040}, {4, 0x0010}, {4, 0x0002}},
    {{1, 0x8000}, {1, 0x4000}, {1, 0x2000}, {1, 0x1000}, {0, 0x0020}, {0, 0x0004}, {4, 0x0001}},
    {{1, 0x0800}, {1, 0x0400}, {1, 0x0200}, {1, 0x0100}, {0, 0x0010}, {0, 0x0002}, {1, 0x4000}},
    {{1, 0x0080}, {1, 0x0040}, {1, 0x0020}, {1, 0x0010}, {1, 0x0400}, {0, 0x0001}, {1, 0x2000}},
    {{1, 0x0008}, {1, 0x0004}, {1, 0x0002}, {1, 0x0001}, {1, 0x0200}, {1, 0x0040}, {1, 0x1000}},
    {{2, 0x8000}, {2, 0x4000}, {2, 0x2000}, {2, 0x1000}, {1, 0x0100}, {1, 0x0020}, {1, 0x0004}},
    {{2, 0x0800}, {2, 0x0400}, {2, 0x0200}, {2, 0x0100}, {2, 0x4000}, {1, 0x0010}, {1, 0x0002}},
    {{2, 0x0080}, {2, 0x0040}, {2, 0x0020}, {2, 0x0010}, {2, 0x2000}, {2, 0x0400}, {1, 0x0001}},
    {{2, 0x0008}, {2, 0x0004}, {2, 0x0002}, {2, 0x0001}, {2, 0x1000}, {2, 0x0200}, {2, 0x0040}},
    {{3, 0x8000}, {3, 0x4000}, {3, 0x2000}, {3, 0x1000}, {2, 0x0004}, {2, 0x0100}, {2, 0x0020}},
    {{3, 0x0800}, {3, 0x0400}, {3, 0x0200}, {3, 0x0100}, {2, 0x0002}, {3, 0x4000}, {2, 0x0010}},
    {{3, 0x0080}, {3, 0x0040}, {3, 0x0020}, {3, 0x0010}, {2, 0x0001}, {3, 0x2000}, {3, 0x0400}},
    {{3, 0x0008}, {3, 0x0004}, {3, 0x0002}, {3, 0x0001}, {3, 0x0040}, {3, 0x1000}, {3, 0x0200}},
    {{4, 0x8000}, {4, 0x4000}, {4, 0x2000}, {4, 0x1000}, {3, 0x0020}, {3, 0x0004}, {3, 0x0100}},
    {{4, 0x0800}, {4, 0x0400}, {4, 0x0200}, {4, 0x0100}, {3, 0x0010}, {3, 0x0002}, {4, 0x4000}},
    {{4, 0x0080}, {4, 0x0040}, {4, 0x0020}, {4, 0x0010}, {4, 0x0400}, {3, 0x0001}, {4, 0x2000}},
    {{4, 0x0008}, {4, 0x0004}, {4, 0x0002}, {4, 0x0001}, {4, 0x0200}, {4, 0x0040}, {4, 0x1000}},
};


/** Compress a flash word (64-bit + 8-bit ECC) to an 18-bit.
 *
 * ECC bits are fixed to 0.
 *
 * @param[in]  data  flash word to compress.
 * @return  compressed data.
 */
static uint32_t compressor_by_4(const uint16_t * data) {
    uint32_t u32Retval = 0;
    for (uint8_t item = 0; item < sizeof(indices_map) / sizeof(indices_map[0]); item++) {
        u32Retval <<= 1;
        u32Retval |=
            (((data[indices_map[item][0][0]] & indices_map[item][0][1]) != 0 ? 1 : 0) ^
             ((data[indices_map[item][1][0]] & indices_map[item][1][1]) != 0 ? 1 : 0) ^
             ((data[indices_map[item][2][0]] & indices_map[item][2][1]) != 0 ? 1 : 0) ^
             ((data[indices_map[item][3][0]] & indices_map[item][3][1]) != 0 ? 1 : 0)) ^
            (((data[indices_map[item][4][0]] & indices_map[item][4][1]) != 0 ? 1 : 0) ^
             ((data[indices_map[item][5][0]] & indices_map[item][5][1]) != 0 ? 1 : 0) ^
             ((data[indices_map[item][6][0]] & indices_map[item][6][1]) != 0 ? 1 : 0));
    }
    return u32Retval;
}

uint32_t crc_calcGanyXfeCrc(const uint16_t * data, size_t length, uint32_t seed) {
    uint32_t u32Crc = seed;
    uint16_t flash_word[5];

    /* calc the crc but exclude the 4 last words which should be the crc itself */
    for (size_t word_nr = 0; word_nr < length - 4; word_nr = word_nr + 4) {
        flash_word[0] = 0u;
        flash_word[1] = data[word_nr + 3];
        flash_word[2] = data[word_nr + 2];
        flash_word[3] = data[word_nr + 1];
        flash_word[4] = data[word_nr + 0];
        uint32_t comp_word = compressor_by_4(flash_word);
        uint32_t temp_d0 = ((u32Crc & 0x20000) != 0) ^ ((u32Crc & 0x40) != 0);
        u32Crc = (u32Crc << 1) ^ comp_word ^ temp_d0;
        u32Crc = u32Crc & 0x3FFFF;
    }

    return u32Crc;
}

