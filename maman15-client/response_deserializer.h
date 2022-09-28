#pragma once
#include <vector>
#include "response.h"

using std::vector;

class ResponseDeserializer
{
public:
	virtual Response* deserializeResponse(const vector<BYTE>& source) = 0;
};