#include "aes_public_wrapper.h"

using CryptoPP::SecByteBlock;

AESPublicWrapper::~AESPublicWrapper()
{
	delete pmEncryption;
}

void AESPublicWrapper::loadKey(const vector<BYTE>& key)
{
	SecByteBlock keyBlock(&key[0], key.size());
	BYTE* ivBuf = new BYTE[IV_BLOCK_LEN];
	memset(ivBuf, 0, IV_BLOCK_LEN);
	SecByteBlock iv(ivBuf, IV_BLOCK_LEN);

	pmEncryption = new CBC_Mode<AES>::Encryption(keyBlock, keyBlock.size(), iv);
}

vector<BYTE> AESPublicWrapper::encrypt(const vector<BYTE>& data)
{
	std::string encryptedStr;
	CryptoPP::VectorSource source(data, true,
		new CryptoPP::StreamTransformationFilter(*pmEncryption,
			new CryptoPP::StringSink(encryptedStr)
		)
	);

	auto dataLen = encryptedStr.length();
	vector<BYTE> encrypted(dataLen);
	memcpy_s(&encrypted[0], encryptedStr.length(), encryptedStr.c_str(), dataLen);
	return encrypted;
}
