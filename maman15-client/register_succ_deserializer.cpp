#include "register_succ_deserializer.h"
#include "vector_utils.h"

Response* RegisterSuccDeserializer::deserializeInnerResponse(const vector<BYTE>& source)
{
    RegisterSuccessResponse* res = new RegisterSuccessResponse;
    VectorUtils::extractBufferFromVector(source, res->id, ID_LEN, RESPONSE_HEADERS_OFFSET);

    return res;
}
