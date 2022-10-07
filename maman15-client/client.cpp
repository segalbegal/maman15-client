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
	mName = name;
	memcpy_s(mId, ID_LEN, id, ID_LEN);
	
	ofstream f(ME_INFO_FILE);
	f << name << endl;
	f << id << endl;
	f.close();
}

void Client::handlePrivateKey(const BYTE* encryptedKey, int encryptedKeyLen)
{
	mKey = mRsaPrivateWrapper->decrypt(encryptedKey, encryptedKeyLen);
}

Client::Client(RequestHandler* requestHandler, RSAPrivateWrapper* rsaPrivateWrapper) : mRequestHandler(requestHandler), mRsaPrivateWrapper(rsaPrivateWrapper)
{
}

Client::~Client()
{
	delete mRequestHandler;
	delete mRsaPrivateWrapper;
}

bool Client::registerClient(const string& name)
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

bool Client::sendPublicKey()
{
	string key = mRsaPrivateWrapper->getPublicKey();
	PublicKeyRequest req;
	memcpy_s(req.id, ID_LEN, mId, ID_LEN);
	req.msgCode = MessageCode::SendPublicKey;
	req.version = VERSION;
	req.name = mName;
	memcpy_s(req.key, PUBLIC_KEY_LEN, (BYTE*)&key[0], PUBLIC_KEY_LEN);
	req.payloadSize = NAME_LEN + PUBLIC_KEY_LEN;

	Response* res = mRequestHandler->handleRequest(&req);
	if (res->status == Status::RecievedPublicKey)
	{
		RecievedPublicKeyResponse* pubKeyRes = (RecievedPublicKeyResponse*)res;
		handlePrivateKey(pubKeyRes->aesKey, pubKeyRes->aesKeyLen);
	}

	return res->status == Status::RecievedPublicKey;
}
