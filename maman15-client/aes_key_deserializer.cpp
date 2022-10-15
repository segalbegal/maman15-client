#include "aes_key_deserializer.h"
#include "vector_utils.h"

Response* AesKeyDeserializer::deserializeInnerResponse(const vector<BYTE>& source)
{
    RecievedPublicKeyResponse* res = new RecievedPublicKeyResponse;
    int payloadSize = VectorUtils::extractNumFromVector(source, PAYLOAD_SIZE_LEN, VERSION_LEN + STATUS_LEN);
    int aesKeySize = payloadSize - ID_LEN;
    res->aesKey = new BYTE[aesKeySize];
    VectorUtils::extractBufferFromVector(source, res->aesKey, aesKeySize, RESPONSE_HEADERS_OFFSET + ID_LEN);
    res->aesKeyLen = aesKeySize;

    return res;
}
