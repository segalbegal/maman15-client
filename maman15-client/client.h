#pragma once
#include <string>
#include <vector>
#include <WinSock2.h>
#include "status.h"
#include "size.h"
#include "request_handler.h"

using std::string;
using std::vector;

class Client
{
private:
	RequestHandler* mRequestHandler;

	void saveClientId(string name, BYTE id[ID_LEN]);

public:
	Client(RequestHandler* requestHandler);
	~Client();

	bool registerClient(string name);
	//bool sendPublicKey();
};

