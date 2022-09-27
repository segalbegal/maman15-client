#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)

#include "client.h"
#include <vector>
#include "size.h"
#include "msg_code.h"
#include "status.h"

#define DECIMAL_BASE (10)
#define ZERO '0'

using std::to_string;

SOCKET Client::connectToServer()
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(mPort);
	addr.sin_addr.s_addr = inet_addr(mIp.c_str());

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == INVALID_SOCKET)
	{
		return -1;
	}

	return sock;
}

void Client::disconnectFromServer(SOCKET sock)
{
	closesocket(sock);
}

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
	char resBuf[STATUS_LEN] = {0};
	SOCKET sock = connectToServer();

	send(sock, (char*)&data[0], data.size(), 0);
	recv(sock, resBuf, STATUS_LEN, 0);
	
	disconnectFromServer(sock);
	Status status = (Status)std::stoi(string(resBuf));
	return status == success;
}

Client::Client(string ip, int port) : mIp(ip), mPort(port)
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

bool Client::registerClient(string name, char id[ID_LEN])
{
	vector<BYTE> data(MSGCODE_LEN + NAME_LEN);
	copyNumberToVector(data, MessageCode::RegisterClient, MSGCODE_LEN);
	copyArrayToVector(data, (BYTE*)name.c_str(), name.length(), MSGCODE_LEN);

	return sendMessageToServer(data, Status::RegisterSuccess);
}
