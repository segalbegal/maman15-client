#pragma once
#include "request_serializer.h"

class RequestHeaderSerializer : public RequestSerializer 
{
protected:
	virtual vector<BYTE> serializeInnerRequest(const Request* req);

public:
	virtual vector<BYTE> serializeRequest(const Request* req) override;
};

