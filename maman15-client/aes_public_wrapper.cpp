#include "aes_public_wrapper.h"

using CryptoPP::SecByteBlock;

AESPublicWrapper::~AESPublicWrapper()
{
	delete pmEncryption;
}

void AESPublicWrapper::loadKey(const BYTE* key, int length)
{
	SecByteBlock keyBlock(key, length);
	BYTE* ivBuf = new BYTE[IV_BLOCK_LEN];
	memset(ivBuf, 0, IV_BLOCK_LEN);
	SecByteBlock iv(ivBuf, IV_BLOCK_LEN);

	pmEncryption = new CBC_Mode<AES>::Encryption(keyBlock, keyBlock.size(), iv);
}

vector<BYTE> AESPublicWrapper::encrypt(const BYTE* data, int length)
{
	std::string encryptedStr;
	CryptoPP::ArraySource source(data, length, true,
		new CryptoPP::StreamTransformationFilter(*pmEncryption,
			new CryptoPP::StringSink(encryptedStr)
		)
	);

	vector<BYTE> encrypted(encryptedStr.length());
	memcpy(&encrypted[0], encryptedStr.c_str(), encryptedStr.length());
	return encrypted;
}

vector<BYTE> AESPublicWrapper::encrypt(const vector<BYTE>& data)
{
	std::string encryptedStr;
	CryptoPP::ArraySource source(&data[0], data.size(), true,
		new CryptoPP::StreamTransformationFilter(*pmEncryption,
			new CryptoPP::StringSink(encryptedStr)
		)
	);

	vector<BYTE> encrypted(encryptedStr.length());
	memcpy(&encrypted[0], encryptedStr.c_str(), encryptedStr.length());
	return encrypted;
}
