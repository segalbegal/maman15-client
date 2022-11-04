#pragma once
#include <aes.h>
#include <osrng.h>
#include <modes.h>
#include <vector>

#define IV_BLOCK_LEN (16)

typedef unsigned char BYTE;

using CryptoPP::CBC_Mode;
using CryptoPP::AES;
using CryptoPP::AutoSeededRandomPool;
using std::vector;

class AESPublicWrapper
{
private:
	AutoSeededRandomPool mRng;
	CBC_Mode<AES>::Encryption* pmEncryption;

public:
	~AESPublicWrapper();

	void loadKey(const vector<BYTE>& key);
	vector<BYTE> encrypt(const vector<BYTE>& data);
};