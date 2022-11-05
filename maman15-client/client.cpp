#include "client.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <exception>
#include <filesystem>

#include "size.h"
#include "msg_code.h"
#include "status.h"
#include "system_constants.h"
#include "cksum_utils.h"
#include "logging_utils.h"

#define DECIMAL_BASE (10)
#define ZERO '0'
#define ME_INFO_FILE "me.info"
#define SEND_FILE_RETRY (3)

using std::to_string;
using std::ofstream;
using std::ifstream;
using std::endl;
using std::exception;
using std::ios;

void Client::loadClientInfo()
{
	if (!std::filesystem::exists(ME_INFO_FILE))
	{
		mIsRegistered = false;
		return;
	}

	ifstream f(ME_INFO_FILE, ios::in | ios::binary);
	
	std::getline(f, mName);
	string idStr;
	std::getline(f, idStr);
	memcpy_s(mId, ID_LEN, idStr.c_str(), ID_LEN);

	f.close();
	mIsRegistered = true;
}

void Client::copyClientDetails(Request* req)
{
	req->version = VERSION;
	memcpy_s(req->id, ID_LEN, mId, ID_LEN);
}

void Client::saveClientId(string name, BYTE id[ID_LEN])
{
	mName = name;
	memcpy_s(mId, ID_LEN, id, ID_LEN);
	
	ofstream f(ME_INFO_FILE);
	f << name << endl;
	f << id << endl;
	f.close();
}

void Client::handlePrivateKey(const vector<BYTE>& encryptedKey)
{
	mKey = mRsaPrivateWrapper->decrypt(encryptedKey);
	mAesPublicWrapper->loadKey(mKey);
}

vector<BYTE> Client::loadFileContent(const string& filename)
{
	ifstream f(filename, ios::binary | ios::in);
	if (!f.is_open())
	{
		auto error = "File '" + filename + "' not found.";
		throw exception(error.c_str());
	}

	auto fileSize = std::filesystem::file_size(std::filesystem::path(filename));
	vector<BYTE> source(fileSize);
	f.read((char*)&source[0], fileSize);
	f.close();

	return source;
}

Request Client::createEmptyRequest(MessageCode msgCode)
{
	Request req;
	copyClientDetails(&req);
	req.msgCode = msgCode;
	return req;
}

Client::Client(RequestHandler* requestHandler, RSAPrivateWrapper* rsaPrivateWrapper, AESPublicWrapper* aesPublicWrapper) 
	: mRequestHandler(requestHandler), mRsaPrivateWrapper(rsaPrivateWrapper), mAesPublicWrapper(aesPublicWrapper)
{
	loadClientInfo();
}

Client::~Client()
{
	delete mRequestHandler;
	delete mRsaPrivateWrapper;
	delete mAesPublicWrapper;
}

bool Client::registered()
{
	return mIsRegistered;
}

bool Client::registerClient(const string& name)
{
	Logging::info("Registering client! Name: " + name, CLIENT_LOGGER);

	mRequestHandler->beginRequest();

	RegisterRequest request;
	copyClientDetails(&request);
	request.msgCode = MessageCode::RegisterClient;
	request.name = name;
		
	auto res = mRequestHandler->handleRequest(&request);
	auto success = res->status == Status::RegisterSuccess;
	if (success)
	{
		Logging::info("Cleint registered successfully! Name: " + name, CLIENT_LOGGER);
		saveClientId(name, ((RegisterSuccessResponse*)res)->id);
	}
	
	delete res;
	mRequestHandler->endRequest();

	return success;
}

bool Client::sendPublicKey()
{
	mRequestHandler->beginRequest();

	auto key = mRsaPrivateWrapper->getPublicKey();
	PublicKeyRequest request;
	copyClientDetails(&request);
	request.msgCode = MessageCode::SendPublicKey;
	request.name = mName;
	memcpy_s(request.key, PUBLIC_KEY_LEN, (BYTE*)&key[0], PUBLIC_KEY_LEN);

	auto res = mRequestHandler->handleRequest(&request);
	auto success = res->status == Status::RecievedPublicKey;

	if (success)
	{
		auto pubKeyRes = (RecievedPublicKeyResponse*)res;
		handlePrivateKey(pubKeyRes->aesKey);
	}

	delete res;
	mRequestHandler->endRequest();

	return success;
}

bool Client::sendFile(const string& filename)
{
	mRequestHandler->beginRequest();

	FileRequest request;
	copyClientDetails(&request);
	request.msgCode = MessageCode::SendFile;
	request.fileName = filename;
	auto data = loadFileContent(filename);
	request.content = mAesPublicWrapper->encrypt(data);
	auto invalidCRCRequest = createEmptyRequest(MessageCode::InvalidCRCRetry);;

	auto crc = CksumUtils::calculateCRC32Cksum(data);
	auto isValid = false;
	for (int i = 0; i < SEND_FILE_RETRY && !isValid; i++)
	{
		auto res = mRequestHandler->handleRequest(&request);
		auto success = res->status == Status::RecievedFileCRC;
		if (success)
		{
			auto fileRes = (RecievedFileResponse*)res;
			if (fileRes->cksum == crc)
			{
				isValid = true;
				continue;
			}
			
			if (i + 1 < SEND_FILE_RETRY)
			{
				mRequestHandler->handleRequest(&invalidCRCRequest);
			}
		}

		delete res;
	}

	auto crcRequest = createEmptyRequest(isValid ? MessageCode::ValidCRC : MessageCode::InvalidCRC);
	mRequestHandler->handleRequest(&crcRequest);
	mRequestHandler->endRequest();

	return isValid;
}
