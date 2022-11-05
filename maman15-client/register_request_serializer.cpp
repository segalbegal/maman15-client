#include "register_request_serializer.h"
#include "vector_utils.h"

vector<BYTE> RegisterRequestSerializer::serializeInnerRequest(const Request* req)
{
    RegisterRequest* regReq = (RegisterRequest*)req;
    vector<BYTE> data;
    VectorUtils::appendBufferToVector(data, (BYTE*)regReq->name.c_str(), regReq->name.length());
    VectorUtils::appendIntToVector(data, 0, NAME_LEN - regReq->name.length());

    return data;
}
