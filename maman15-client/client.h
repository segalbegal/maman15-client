#pragma once
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "status.h"

using std::string;
using std::vector;
using boost::asio::ip::tcp;
using boost::shared_ptr;

class Client
{
private:
	string mIp;
	int mPort;
	tcp::socket* mSock;

	void copyNumberToVector(vector<BYTE>& source, int num, int len, int offset = 0);
	void copyArrayToVector(vector<BYTE>& source, BYTE* arr, int len, int offset = 0);

	bool sendMessageToServer(const vector<BYTE>& data, Status success);

public:
	Client(string ip, int port);
	~Client() = default;

	bool registerClient(string name);
};

