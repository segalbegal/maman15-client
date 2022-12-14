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

CRCRequest Client::createCRCRequest(MessageCode msgCode, const string& filename)
{
	CRCRequest req;
	copyClientDetails(&req);
	req.msgCode = msgCode;
	req.filename = filename;
	return req;
}

string Client::extractFileName(const string& filepath)
{
	std::filesystem::path p(filepath);
	return p.filename().string();
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
	return success;
}

bool Client::sendPublicKey()
{
	Logging::info("Sending public key to server", CLIENT_LOGGER);

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
	return success;
}

bool Client::sendFile(const string& filename)
{
	FileRequest request;
	copyClientDetails(&request);
	request.msgCode = MessageCode::SendFile;
	request.fileName = extractFileName(filename);
	auto data = loadFileContent(filename);
	request.content = mAesPublicWrapper->encrypt(data);
	auto invalidCRCRequest = createCRCRequest(MessageCode::InvalidCRCRetry, filename);;

	auto crc = CksumUtils::calculateCRC32Cksum(data);
	auto isValid = false;
	for (int i = 0; i < SEND_FILE_RETRY && !isValid; i++)
	{
		Logging::info("Sending file to server. FileName: " + request.fileName, CLIENT_LOGGER);
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

	auto crcRequest = createCRCRequest(isValid ? MessageCode::ValidCRC : MessageCode::InvalidCRC, filename);
	mRequestHandler->handleRequest(&crcRequest);

	return isValid;
}
