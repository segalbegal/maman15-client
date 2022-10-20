#pragma once
#include <osrng.h>
#include <rsa.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

typedef unsigned char BYTE;

class RSAPrivateWrapper
{
private:
	CryptoPP::RSA::PrivateKey mPrivateKey;
	CryptoPP::AutoSeededRandomPool mRng;

public:
	RSAPrivateWrapper();

	string getPublicKey();

	string decrypt(const string& data);
	string decrypt(const BYTE* data, int len);
};