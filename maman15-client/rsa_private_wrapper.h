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

	vector<BYTE> getPublicKey();
	vector<BYTE> decrypt(const vector<BYTE>& data);
};