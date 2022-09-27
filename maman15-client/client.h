#pragma once
#include <string>
#include <vector>
#include <WinSock2.h>
#include "status.h"
#include "size.h"

using std::string;
using std::vector;

class Client
{
private:
	string mIp;
	int mPort;

	SOCKET connectToServer();
	void disconnectFromServer(SOCKET sock);

	void copyNumberToVector(vector<BYTE>& source, int num, int len, int offset = 0);
	void copyArrayToVector(vector<BYTE>& source, BYTE* arr, int len, int offset = 0);

	bool sendMessageToServer(const vector<BYTE>& data, Status success);

public:
	Client(string ip, int port);
	~Client() = default;

	bool registerClient(string name, char id[ID_LEN]);
	bool sendPublicKey();
};

