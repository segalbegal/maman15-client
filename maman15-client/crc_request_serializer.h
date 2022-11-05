#pragma once
#include "request_header_serializer.h"

class CRCRequestSerializer : public RequestHeaderSerializer
{
public:
	virtual vector<BYTE> serializeInnerRequest(const Request* req) override;
};