#include "vector_utils.h"

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

void VectorUtils::writeBufferToVector(vector<BYTE>& source, BYTE* buf, int len, int offset)
{
	if (source.size() < offset + len)
	{
		source.resize(offset + len);
	}

	memcpy_s(&source[0] + offset, len, buf, len);
}

void VectorUtils::appendBufferToVector(vector<BYTE>& source, BYTE* buf, int len)
{
	VectorUtils::writeBufferToVector(source, buf, len, source.size());
}

int VectorUtils::extractIntFromVector(const vector<BYTE>& source, int len, int offset)
{
	if (source.size() < offset + len)
	{
		return -1;
	}

	int num = 0;
	for (int i = 0; i < len; i++)
	{
		num += source[offset + len - i - 1] * pow(BYTE_BASE, i);
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
