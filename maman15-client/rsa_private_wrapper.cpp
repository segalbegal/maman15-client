#include "rsa_private_wrapper.h"

#define BITS (1024U)

RSAPrivateWrapper::RSAPrivateWrapper()
{
    mPrivateKey.Initialize(mRng, BITS);
}

vector<BYTE> RSAPrivateWrapper::getPublicKey()
{
    CryptoPP::RSAFunction publicKeyFunc(mPrivateKey);
    vector<BYTE> publicKey;
    CryptoPP::VectorSink publicSs(publicKey);
    publicKeyFunc.Save(publicSs);

    return publicKey;
}

vector<BYTE> RSAPrivateWrapper::decrypt(const vector<BYTE>& data)
{
    vector<BYTE> decrypted;
    CryptoPP::RSAES_OAEP_SHA_Decryptor d(mPrivateKey);
    CryptoPP::VectorSource ss_cipher(data, true, new CryptoPP::PK_DecryptorFilter(mRng, d, new CryptoPP::VectorSink(decrypted)));
    return decrypted;
}
