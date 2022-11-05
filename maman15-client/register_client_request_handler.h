#pragma once
#include "request_handler.h"
#include "request_serializer.h"

class RegisterClientRequestHandler : public RequestHandler
{
private:
	RequestSerializer* mSerializer;

public:
	RegisterClientRequestHandler(RequestSerializer* serializer);
	virtual Response handleRequest(const Request* req) override;
};

