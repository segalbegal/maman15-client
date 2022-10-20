#pragma once
#include "request_header_serializer.h"

class SendFileRequestSerializer : public RequestHeaderSerializer
{
public:
	virtual vector<BYTE> serializeInnerRequest(const Request* req) override;
};