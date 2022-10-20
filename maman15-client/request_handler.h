#pragma once
#include <string>
#include <WinSock2.h>
#include "request.h"
#include "response.h"
#include "request_serializer.h"
#include "response_deserializer.h"

using std::string;

class RequestHandler
{
private:
	SOCKET mServerSock;
	string mIp;
	int mPort;
	
	RequestSerializer* mSerializer;
	ResponseDeserializer* mDeserializer;

public:
	RequestHandler(string ip, int port, RequestSerializer* serializer, ResponseDeserializer* deserializer);
	~RequestHandler();

	void beginRequest();
	void endRequest();
	
	Response* handleRequest(const Request* req);
};