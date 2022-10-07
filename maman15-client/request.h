#pragma once
#include <string>
#include <vector>
#include "msg_code.h"
#include "size.h"

using std::string;
using std::vector;

typedef unsigned char BYTE;

typedef struct Request
{
	MessageCode msgCode;
	BYTE id[ID_LEN];
	int version;
	int payloadSize;
} Request;

typedef struct NameRequest : public Request
{
	string name;
} NameRequest;

typedef struct RegisterRequest : public NameRequest
{
} RegisterRequest;

typedef struct PublicKeyRequest : public NameRequest
{
	BYTE key[PUBLIC_KEY_LEN];
};

typedef struct FileRequest : public Request
{
	string fileName;
	vector<BYTE> content;
};