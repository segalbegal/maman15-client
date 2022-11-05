#pragma once
#include <vector>

typedef unsigned char BYTE;
typedef unsigned long crc;

using std::vector;

class CksumUtils
{
public:
	static crc calculateCRC32Cksum(const vector<BYTE>& data);
};