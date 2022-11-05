#pragma once
#include <vector>
#include "request.h"

using std::vector;

class RequestSerializer
{
public:
	virtual vector<BYTE> serializeRequest(const Request* req) = 0;
};