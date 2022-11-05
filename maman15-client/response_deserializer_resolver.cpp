#include "response_deserializer_resolver.h"
#include "vector_utils.h"
#include "size.h"

ResponseDeserializerResolver::ResponseDeserializerResolver(const map<Status, ResponseDeserializer*>& deserializers) : mDeserializers(deserializers)
{
}

ResponseDeserializerResolver::~ResponseDeserializerResolver()
{
    for (const auto& deserializer : mDeserializers)
    {
        delete deserializer.second;
    }
    mDeserializers.clear();
}

Response* ResponseDeserializerResolver::deserializeResponse(const vector<BYTE>& source)
{
    Status status = (Status)VectorUtils::extractNumFromVector(source, MSGCODE_LEN, VERSION_LEN);
    return mDeserializers[status]->deserializeResponse(source);
}
