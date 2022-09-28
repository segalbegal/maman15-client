#include "register_client_request_handler.h"
#include <vector>
#include "size.h"
#include "vector_utils.h"

using std::vector;

RegisterClientRequestHandler::RegisterClientRequestHandler(RequestSerializer* serializer) : mSerializer(serializer)
{
}

Response RegisterClientRequestHandler::handleRequest(const Request* req)
{
    vector<BYTE> data = mSerializer->serializeRequest(req);

}
