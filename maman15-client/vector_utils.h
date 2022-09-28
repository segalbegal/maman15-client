#pragma once
#include <vector>
#include "request.h"

using std::vector;

typedef unsigned char BYTE;

class VectorUtils
{
public:
	static void writeIntToVector(vector<BYTE>& source, int num, int len, int offset = 0);
	static void appendIntToVector(vector<BYTE>& source, int num, int len);
	
	static void writeBufferToVector(vector<BYTE>& source, BYTE* buf, int len, int offset = 0);
	static void appendBufferToVector(vector<BYTE>& source, BYTE* buf, int len);

	static int extractIntFromVector(const vector<BYTE>& source, int len, int offset = 0);
	static void extractBufferFromVector(const vector<BYTE>& source, BYTE* buffer, int len, int offset = 0);
};

