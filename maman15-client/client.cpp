#include "client.h"
#include <vector>
#include <fstream>
#include <iostream>

#include "size.h"
#include "msg_code.h"
#include "status.h"
#include "system_constants.h"

#define DECIMAL_BASE (10)
#define ZERO '0'
#define ME_INFO_FILE "me.info"

using std::to_string;
using std::ofstream;
using std::endl;

void Client::saveClientId(string name, BYTE id[ID_LEN])
{
	ofstream f(ME_INFO_FILE);
	f << name << endl;
	f << id << endl;
	f.close();
}

Client::Client(RequestHandler* requestHandler) : mRequestHandler(requestHandler)
{
}

Client::~Client()
{
	delete mRequestHandler;
}

bool Client::registerClient(string name)
{
	RegisterRequest request;
	request.msgCode = MessageCode::RegisterClient;
	request.version = VERSION;
	request.payloadSize = NAME_LEN;
	request.name = name;

	Response* res = mRequestHandler->handleRequest(&request);
	if (res->status == Status::RegisterSuccess)
	{
		saveClientId(name, ((RegisterSuccessResponse*)res)->id);
	}
	
	return res->status == Status::RegisterSuccess;
}
