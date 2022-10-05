#pragma once
#include <map>
#include "request_serializer.h"

using std::map;

class RequestSerializerResolver : public RequestSerializer
{
private:
	map<MessageCode, RequestSerializer*> mSerializers;

public:
	RequestSerializerResolver(const map<MessageCode, RequestSerializer*>& serializers);
	~RequestSerializerResolver();

	virtual vector<BYTE> serializeRequest(const Request* req) override;
};