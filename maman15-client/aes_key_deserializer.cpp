#include "aes_key_deserializer.h"
#include "vector_utils.h"

Response* AesKeyDeserializer::deserializeInnerResponse(const vector<BYTE>& source)
{
    RecievedPublicKeyResponse* res = new RecievedPublicKeyResponse;
    int payloadSize = VectorUtils::extractNumFromVector(source, PAYLOAD_SIZE_LEN, VERSION_LEN + STATUS_LEN);
    int aesKeySize = payloadSize - ID_LEN;
    res->aesKey.resize(aesKeySize);
    VectorUtils::extractBufferFromVector(source, &res->aesKey[0], aesKeySize, RESPONSE_HEADERS_OFFSET + ID_LEN);

    return res;
}
