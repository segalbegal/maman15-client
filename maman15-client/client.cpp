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

void Client::handlePrivateKey(const BYTE* encryptedKey, int encryptedKeyLen)
{
	mKey = mRsaPrivateWrapper->decrypt(encryptedKey, encryptedKeyLen);
	mAesPublicWrapper->loadKey((BYTE*)&mKey[0], mKey.length());
}

void Client::loadFileContent(vector<BYTE>& source, const string& filename)
{
	ifstream f(filename, ios::binary | ios::in);
	if (!f.is_open())
	{
		auto error = "File '" + filename + "' not found.";
		throw exception(error.c_str());
	}

	auto fileSize = std::filesystem::file_size(std::filesystem::path(filename));
	source.resize(fileSize);
	f.read((char*)&source[0], fileSize);
	f.close();
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
	mRequestHandler->beginRequest();

	RegisterRequest request;
	copyClientDetails(&request);
	request.msgCode = MessageCode::RegisterClient;
	request.payloadSize = NAME_LEN;
	request.name = name;

	auto res = mRequestHandler->handleRequest(&request);
	auto success = res->status == Status::RegisterSuccess;
	if (success)
	{
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
	request.payloadSize = NAME_LEN + PUBLIC_KEY_LEN;

	auto res = mRequestHandler->handleRequest(&request);
	auto success = res->status == Status::RecievedPublicKey;

	if (success)
	{
		auto pubKeyRes = (RecievedPublicKeyResponse*)res;
		handlePrivateKey((BYTE*)pubKeyRes->aesKey, pubKeyRes->aesKeyLen);
		delete[] pubKeyRes->aesKey;
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
	vector<BYTE> data;
	loadFileContent(data, filename);
	request.content = mAesPublicWrapper->encrypt(data);
	request.payloadSize = ID_LEN + FILENAME_LEN + request.content.size();
	
	auto crc = CksumUtils::calculateCRC32Cksum(data);

	bool isValid = false;
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

			Request invalidCRCRequest;
			
			if (i + 1 < SEND_FILE_RETRY)
			{
				copyClientDetails(&invalidCRCRequest);
				invalidCRCRequest.msgCode = MessageCode::InvalidCRCRetry;
				invalidCRCRequest.payloadSize = 0;
				mRequestHandler->handleRequest(&invalidCRCRequest);
			}
		}

		delete res;
	}

	Request crcRequest;
	copyClientDetails(&crcRequest);
	crcRequest.msgCode = isValid ? MessageCode::ValidCRC : MessageCode::InvalidCRC;
	crcRequest.payloadSize = 0;
	mRequestHandler->handleRequest(&crcRequest);
	mRequestHandler->endRequest();

	return isValid;
}
