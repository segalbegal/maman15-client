#pragma once
#include <string>
#include <vector>
#include <WinSock2.h>
#include "status.h"
#include "size.h"
#include "request_handler.h"
#include "rsa_private_wrapper.h"

using std::string;
using std::vector;

class Client
{
private:
	BYTE mId[ID_LEN];
	string mName;
	string mKey;
	
	RequestHandler* mRequestHandler;
	RSAPrivateWrapper* mRsaPrivateWrapper;

	void saveClientId(string name, BYTE id[ID_LEN]);
	void handlePrivateKey(const BYTE* encryptedKey, int encryptedKeyLen);	

public:
	Client(RequestHandler* requestHandler, RSAPrivateWrapper* rsaPrivateWrapper);
	~Client();

	bool registerClient(const string& name);
	bool sendPublicKey();
};

