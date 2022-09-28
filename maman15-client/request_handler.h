#pragma once
#include <string>
#include "request.h"
#include "response.h"
#include "request_serializer.h"
#include "response_deserializer.h"

using std::string;

class RequestHandler
{
private:
	string mIp;
	int mPort;
	
	RequestSerializer* mSerializer;
	ResponseDeserializer* mDeserializer;

public:
	RequestHandler(string ip, int port, RequestSerializer* serializer, ResponseDeserializer* deserializer);
	~RequestHandler();

	Response* handleRequest(const Request* req);
};