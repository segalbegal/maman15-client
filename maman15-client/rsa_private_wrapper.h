#pragma once
#include <osrng.h>
#include <rsa.h>
#include <string>

using std::string;

typedef unsigned char BYTE;

class RSAPrivateWrapper
{
private:
	CryptoPP::RSA::PrivateKey mPrivateKey;
	CryptoPP::AutoSeededRandomPool mRng;

public:
	RSAPrivateWrapper();

	string getPublicKey();
	string decrypt(string data);
	string decrypt(const BYTE* data, int len);
};