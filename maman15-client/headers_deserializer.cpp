#include "headers_deserializer.h"
#include "vector_utils.h"

Response* HeadersDeserializer::deserializeResponse(const vector<BYTE>& source)
{
    Response* res = deserializeInnerResponse(source);
    res->version = VectorUtils::extractIntFromVector(source, VERSION_LEN, 0);
    res->status = (Status)VectorUtils::extractIntFromVector(source, STATUS_LEN, VERSION_LEN);

    return res;
}
