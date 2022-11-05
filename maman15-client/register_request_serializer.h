#pragma once
#include "request_header_serializer.h"

class RegisterRequestSerializer : public RequestHeaderSerializer
{
protected:
	virtual vector<BYTE> serializeInnerRequest(const Request* req) override;
};

