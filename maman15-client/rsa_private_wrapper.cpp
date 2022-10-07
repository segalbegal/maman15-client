#include "rsa_private_wrapper.h"

#define BITS (1024U)

RSAPrivateWrapper::RSAPrivateWrapper()
{
    mPrivateKey.Initialize(mRng, BITS);
}

string RSAPrivateWrapper::getPublicKey()
{
    CryptoPP::RSAFunction publicKeyFunc(mPrivateKey);
    std::string publicKey;
    CryptoPP::StringSink publicSs(publicKey);
    publicKeyFunc.Save(publicSs);

    return publicKey;
}

string RSAPrivateWrapper::decrypt(string data)
{
    std::string decrypted;
    CryptoPP::RSAES_OAEP_SHA_Decryptor d(mPrivateKey);
    CryptoPP::StringSource ss_cipher(data, true, new CryptoPP::PK_DecryptorFilter(mRng, d, new CryptoPP::StringSink(decrypted)));
    
    return decrypted;
}

string RSAPrivateWrapper::decrypt(const BYTE* data, int len)
{
    std::string decrypted;
    CryptoPP::RSAES_OAEP_SHA_Decryptor d(mPrivateKey);
    CryptoPP::StringSource ss_cipher(reinterpret_cast<const CryptoPP::byte*>(data), len, true, new CryptoPP::PK_DecryptorFilter(mRng, d, new CryptoPP::StringSink(decrypted)));
    return decrypted;
}
