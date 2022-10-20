#include "vector_utils.h"
#include <sstream>
#include <iomanip>

#define BYTE_BASE (256)

void VectorUtils::writeIntToVector(vector<BYTE>& source, int num, int len, int offset)
{
	if (source.size() < offset + len)
	{
		source.resize(offset + len);
	}

	for (int i = 0; i < len; i++)
	{
		source[offset + len - i - 1] = num % BYTE_BASE;
		num /= BYTE_BASE;
	}
}

void VectorUtils::appendIntToVector(vector<BYTE>& source, int num, int len)
{
	VectorUtils::writeIntToVector(source, num, len, source.size());
}

void VectorUtils::writeBufferToVector(vector<BYTE>& source, const BYTE* buf, int len, int offset)
{
	if (source.size() < offset + len)
	{
		source.resize(offset + len);
	}

	memcpy_s(&source[0] + offset, len, buf, len);
}

void VectorUtils::appendBufferToVector(vector<BYTE>& source, const BYTE* buf, int len)
{
	VectorUtils::writeBufferToVector(source, buf, len, source.size());
}

void VectorUtils::writeStringToVector(vector<BYTE>& source, string str, int len, int offset)
{
	if (source.size() < offset + len)
	{
		source.resize(offset + len);
	}

	if (str.length() < len)
	{
		std::ostringstream ss;
		ss << std::left << std::setfill('\0') << std::setw(len) << str;
		str = ss.str();
	}
	memcpy_s(&source[0] + offset, len, &str[0], len);
}

void VectorUtils::appendStringToVector(vector<BYTE>& source, const string& str, int len)
{
	VectorUtils::writeStringToVector(source, str, len, source.size());
}

string VectorUtils::extractStringFromVector(const vector<BYTE>& source, int len, int offset)
{
	BYTE* buffer = new BYTE[len];
	VectorUtils::extractBufferFromVector(source, buffer, len, offset);
	return (char*)buffer;
}

long VectorUtils::extractNumFromVector(const vector<BYTE>& source, int len, int offset)
{
	if (source.size() < offset + len)
	{
		return -1;
	}

	unsigned long num = 0;
	for (int i = 0; i < len; i++)
	{
		num *= BYTE_BASE;
		num += source[offset + i];
	}

	return num;
}

void VectorUtils::extractBufferFromVector(const vector<BYTE>& source, BYTE* buffer, int len, int offset)
{
	if (source.size() >= offset + len)
	{
		memcpy_s(buffer, len, &source[0] + offset, len);
	}
}
