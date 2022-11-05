#pragma once
#include "request_header_serializer.h"

class SendPublicKeyRequestSerializer : public RequestHeaderSerializer
{
protected:
	virtual vector<BYTE> serializeInnerRequest(const Request* req) override;
};