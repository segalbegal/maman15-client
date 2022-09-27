#include <iostream>
#include <fstream>
#include <string>
#include "client.h"

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

int main()
{
	struct TransferDetails details = readTransferDetails();
	Client c(details.connectionDetails.ip, details.connectionDetails.port);

	bool res = c.registerClient(details.clientName);
	if (!res)
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
