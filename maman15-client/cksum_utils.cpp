#include "cksum_utils.h"
#include <zlib.h>

crc CksumUtils::calculateCRC32Cksum(const vector<BYTE>& data)
{
    return crc32(0L, &data[0], data.size());
}
