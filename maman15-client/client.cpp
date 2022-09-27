#include "client.h"
#include <vector>
#include "size.h"
#include "msg_code.h"
#include "status.h"

#define DECIMAL_BASE (10)
#define ZERO '0'

using std::to_string;

void Client::copyNumberToVector(vector<BYTE>& source, int num, int len, int offset)
{
	int nextNum = 0;
	for (int i = 0; i < len; i++)
	{
		nextNum = num % DECIMAL_BASE + ZERO;
		source[offset + len - i - 1] = nextNum;
		num /= DECIMAL_BASE;
	}
}

void Client::copyArrayToVector(vector<BYTE>& source, BYTE* arr, int len, int offset)
{
	for (int i = 0; i < len; i++)
	{
		source[i + offset] = arr[i];
	}
}

bool Client::sendMessageToServer(const vector<BYTE>& data, Status success)
{
	char resBuf[STATUS_LEN];

	boost::asio::io_context context;
	tcp::socket sock(context);
	tcp::resolver resolver(context);

	boost::asio::connect(sock, resolver.resolve(mIp, to_string(mPort)));
	boost::asio::write(sock, boost::asio::buffer(data, MSGCODE_LEN + NAME_LEN));
	boost::asio::read(sock, boost::asio::buffer(resBuf, STATUS_LEN));
	
	sock.close();

	Status status = (Status)std::stoi(string(resBuf));
	return status == success;
}

Client::Client(string ip, int port) : mIp(ip), mPort(port)
{
}

bool Client::registerClient(string name)
{
	vector<BYTE> data(MSGCODE_LEN + NAME_LEN);
	copyNumberToVector(data, MessageCode::RegisterClient, MSGCODE_LEN);
	copyArrayToVector(data, (BYTE*)name.c_str(), name.length(), MSGCODE_LEN);

	return sendMessageToServer(data, Status::RegisterSuccess);
}
