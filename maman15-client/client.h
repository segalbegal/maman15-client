#pragma once
#include <string>
#include <vector>
#include <WinSock2.h>
#include "status.h"
#include "size.h"
#include "request_handler.h"
#include "rsa_private_wrapper.h"
#include "aes_public_wrapper.h"

#define CLIENT_LOGGER "client"

using std::string;
using std::vector;

class Client
{
private:
	bool mIsRegistered;
	BYTE mId[ID_LEN] = {0};
	string mName;
	vector<BYTE> mKey;
	
	RequestHandler* mRequestHandler;
	RSAPrivateWrapper* mRsaPrivateWrapper;
	AESPublicWrapper* mAesPublicWrapper;

	void loadClientInfo();

	void copyClientDetails(Request* req);
	void saveClientId(string name, BYTE id[ID_LEN]);
	void handlePrivateKey(const vector<BYTE>& encryptedKey);
	vector<BYTE> loadFileContent(const string& filename);
	Request createEmptyRequest(MessageCode msgCode);

public:
	Client(RequestHandler* requestHandler, RSAPrivateWrapper* rsaPrivateWrapper, AESPublicWrapper* aesPublicWrapper);
	~Client();

	bool registered();

	bool registerClient(const string& name);
	bool sendPublicKey();
	bool sendFile(const string& filename);
};
