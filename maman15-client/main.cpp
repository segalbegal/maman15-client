#include <iostream>
#include <fstream>
#include <string>
#include "client.h"
// Request serializers
#include "request_serializer_resolver.h"
#include "register_request_serializer.h"
#include "send_public_key_request_serializer.h"
// Response deserializers
#include "response_deserializer_resolver.h"
#include "register_succ_deserializer.h"
#include "aes_key_deserializer.h"

#include <sstream>
#include <iomanip>

#define TRANSFER_FILE_NAME "transfer.info"
#define CONNECTION_DETAILS_DELIMITER ':'

using std::string;
using std::ifstream;
using std::cout;
using std::endl;

struct ConnectionDetails
{
	string ip;
	int port;
};

struct TransferDetails
{
	struct ConnectionDetails connectionDetails;
	string clientName;
	string fileName;
};

struct TransferDetails readTransferDetails();

RequestHandler* createRequestHandler(TransferDetails details)
{
	RequestSerializer* serializer = new RequestSerializerResolver(map<MessageCode, RequestSerializer*>
	{
		{MessageCode::RegisterClient, new RegisterRequestSerializer()},
		{MessageCode::SendPublicKey, new SendPublicKeyRequestSerializer()}
	});
	ResponseDeserializer* deserializer = new ResponseDeserializerResolver(map<Status, ResponseDeserializer*>
	{
		{Status::RegisterSuccess, new RegisterSuccDeserializer()},
		{Status::RegisterFailure, new HeadersDeserializer()},
		{Status::RecievedPublicKey, new AesKeyDeserializer()}
	});

	return new RequestHandler(details.connectionDetails.ip, details.connectionDetails.port, serializer, deserializer);
}

int main()
{
	TransferDetails details = readTransferDetails();
	RequestHandler* handler = createRequestHandler(details);
	Client c(handler, new RSAPrivateWrapper());

	char id[ID_LEN] = {0};
	if (!c.registerClient(details.clientName))
	{
		cout << "Server responded with an error" << endl;
		return -1;
	}

	if (!c.sendPublicKey())
	{
		cout << "Server responded with an error" << endl;
	}

	return 0;
}

struct TransferDetails readTransferDetails()
{
	ifstream f(TRANSFER_FILE_NAME);
	struct TransferDetails tranDet;

	if (!f.is_open())
	{
		cout << "File " << TRANSFER_FILE_NAME << " does not exists!" << endl;
		return tranDet;
	}
	
	string connDetStr;
	std::getline(f, connDetStr);
	int connDetDelIdx = connDetStr.find(CONNECTION_DETAILS_DELIMITER);
	string ip = connDetStr.substr(0, connDetDelIdx);
	int port = stoi(connDetStr.substr(connDetDelIdx + 1));

	string clientName;
	std::getline(f, clientName);

	string fileName;
	std::getline(f, fileName);

	f.close();

	struct ConnectionDetails connDet;
	connDet.ip = ip;
	connDet.port = port;
	
	tranDet.connectionDetails = connDet;
	tranDet.clientName = clientName;
	tranDet.fileName = fileName;

	return tranDet;
}
