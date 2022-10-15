#pragma once
#include <string>
#include "status.h"
#include "size.h"
#include "cksum_utils.h"

using std::string;

typedef unsigned char BYTE;

struct Response
{
	Status status;
	int version;
};

struct RegisterSuccessResponse : public Response
{
	BYTE id[ID_LEN];
};

struct RecievedPublicKeyResponse : public Response
{
	BYTE id[ID_LEN];
	int aesKeyLen;
	BYTE* aesKey;
};

struct RecievedFileResponse : public Response
{
	BYTE id[ID_LEN];
	int contentSize;
	string filename;
	crc cksum;
};