#include "headers_deserializer.h"
#include "vector_utils.h"

Response* HeadersDeserializer::deserializeInnerResponse(const vector<BYTE>& source)
{
    return new Response;
}

Response* HeadersDeserializer::deserializeResponse(const vector<BYTE>& source)
{
    Response* res = deserializeInnerResponse(source);
    res->version = VectorUtils::extractNumFromVector(source, VERSION_LEN, 0);
    res->status = (Status)VectorUtils::extractNumFromVector(source, STATUS_LEN, VERSION_LEN);
    
    return res;
}
