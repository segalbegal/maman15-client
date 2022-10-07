#include "request_serializer_resolver.h"

RequestSerializerResolver::RequestSerializerResolver(const map<MessageCode, RequestSerializer*>& serializers) : mSerializers(serializers)
{
}

RequestSerializerResolver::~RequestSerializerResolver()
{
    for (auto& serializer : mSerializers)
    {
        delete serializer.second;
    }
    mSerializers.clear();
}

vector<BYTE> RequestSerializerResolver::serializeRequest(const Request* req)
{
    return mSerializers[req->msgCode]->serializeRequest(req);
}
