/**
 * @file  checksum.cpp
 * @date
 * @brief ""
 *
 * Copyright (C) 2020 NIIC EDA
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "util/checksum.h"
#include "util/monitor.h"
#include "util/io_manager.h"

namespace open_edi {
namespace util {

// Class CheckSum
uint16_t CheckSum::__from32to16(uint32_t x) {
    /* add up 16-bit and 16-bit for 16+c bit */
    x = (x & 0xffff) + (x >> 16);
    /* add up carry.. */
    x = (x & 0xffff) + (x >> 16);
    return x;
}

bool CheckSum::check(uint32_t sum_value, uint32_t ref_value, bool debug) {
    uint32_t invert = ~(ref_value);
    if (debug) {
        std::cout << "DEBUGINFO: check sum " << sum_value << " with ref_value "
                  << ref_value << ".\n";
    }
    return (__from32to16(invert + sum_value) == 0xffff) ? true : false;
}

uint32_t CheckSum::summary(const std::string &filename, int len, bool debug) {
    if (len <= 0) {
        return 0;
    }
    IOManager io_manager;
    if (false == io_manager.open(filename.c_str(), "rb")) {
        return 0;
    }
    io_manager.seek(0, SEEK_END);
    uint64_t filesize = io_manager.tell();
    if (filesize < len) {
        std::cout << "Error: do summary - filesize " << filesize
                  << " with length " << len << ".\n";
        return 0;
    }
    io_manager.seek(0, SEEK_SET);
    unsigned char *buff = new unsigned char[len];
    io_manager.read((char *)(buff), len);
    if (debug) {
        std::cout << "DEBUGINFO: do summary - starting from "
                  << *(uint32_t *)buff << " with length " << len << ".\n";
    }
    uint32_t retn = summary((const unsigned char *)buff, len, debug);
    delete[] buff;
    io_manager.close();
    return retn;
}

uint32_t CheckSum::summary(const unsigned char *buff, int len, bool debug) {
    int odd;
    uint32_t result = 0;

    if (len <= 0) return 0;
    odd = 1 & (unsigned long)buff;
    if (odd) {
        result += (*buff << 8);
        len--;
        buff++;
    }
    if (len >= 2) {
        if (2 & (unsigned long)buff) {
            result += *(unsigned short *)buff;
            len -= 2;
            buff += 2;
        }
        if (len >= 4) {
            const unsigned char *end = buff + ((unsigned)len & ~3);
            unsigned int carry = 0;
            do {
                uint32_t w = *(uint32_t *)buff;
                buff += 4;
                result += carry;
                result += w;
                carry = (w > result);
            } while (buff < end);
            result += carry;
            result = (result & 0xffff) + (result >> 16);
        }
        if (len & 2) {
            result += *(unsigned short *)buff;
            buff += 2;
        }
    }

    if (len & 1) {
        result += *buff;
    }
    if (debug) {
        std::cout << "DEBUGINFO: do summary - result (before) is " << result
                  << ".\n";
    }
    result = __from32to16(result);
    if (debug) {
        std::cout << "DEBUGINFO: do summary - result (after) is " << result
                  << ".\n";
    }
    if (odd) {
        result = ((result >> 8) & 0xff) | ((result & 0xff) << 8);
    }
    return result;
}

}  // namespace util
}  // namespace open_edi
