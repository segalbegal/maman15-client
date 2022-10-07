#include "send_public_key_request_serializer.h"
#include "vector_utils.h"

vector<BYTE> SendPublicKeyRequestSerializer::serializeInnerRequest(const Request* req)
{
    PublicKeyRequest* keyReq = (PublicKeyRequest*)req;
    vector<BYTE> data;
    VectorUtils::appendStringToVector(data, keyReq->name, NAME_LEN);
    VectorUtils::appendBufferToVector(data, keyReq->key, PUBLIC_KEY_LEN);
    
    return data;
}
