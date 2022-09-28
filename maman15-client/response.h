#pragma once
#include <string>
#include "status.h"
#include "size.h"

using std::string;

typedef unsigned char BYTE;

typedef struct Response
{
	Status status;
	int version;
} Response;

typedef struct FailureResponse : public Response
{
} FailureResponse;

typedef struct RegisterSuccessResponse : public Response
{
	BYTE id[ID_LEN];
} RegisterSuccessResponse;

typedef struct RecievedPublicKeyResponse : public Response
{
	BYTE id[ID_LEN];
} RecievedPublicKeyResponse;

typedef struct RecievedValidFile : public Response
{
	BYTE id[ID_LEN];
	int contentSize;
	string filename;
	BYTE cksum[CKSUM_LEN];
};