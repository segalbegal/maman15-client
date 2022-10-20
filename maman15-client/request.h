#pragma once
#include <string>
#include <vector>
#include "msg_code.h"
#include "size.h"

using std::string;
using std::vector;

typedef unsigned char BYTE;

struct Request
{
	MessageCode msgCode;
	BYTE id[ID_LEN];
	int version;
	int payloadSize;
};

struct NameRequest : public Request
{
	string name;
};

struct RegisterRequest : public NameRequest
{
};

struct PublicKeyRequest : public NameRequest
{
	BYTE key[PUBLIC_KEY_LEN];
};

struct FileRequest : public Request
{
	string fileName;
	vector<BYTE> content;
};