#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#include "request_handler.h"
#include "vector_utils.h"

#include <iostream>

RequestHandler::RequestHandler(string ip, int port, RequestSerializer* serializer, ResponseDeserializer* deserializer)
    : mIp(ip), mPort(port), mSerializer(serializer), mDeserializer(deserializer), mServerSock(INVALID_SOCKET)
{
    WSAData data;
    WSAStartup(MAKEWORD(2, 2), &data);
}

RequestHandler::~RequestHandler()
{
    delete mSerializer;
    delete mDeserializer;

    WSACleanup();
}

void RequestHandler::beginRequest()
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(mIp.c_str());
    addr.sin_port = htons(mPort);

    auto newSock = socket(AF_INET, SOCK_STREAM, 0);
    connect(newSock, (struct sockaddr*)&addr, sizeof(addr));

    mServerSock = newSock;
}

void RequestHandler::endRequest()
{
    closesocket(mServerSock);
}

Response* RequestHandler::handleRequest(const Request* req)
{
    vector<BYTE> data = mSerializer->serializeRequest(req);
    std::cout << "Sending request with MessageCode: " << req->msgCode << ", Size: " << data.size() << std::endl;
    if (send(mServerSock, (char*)&data[0], data.size(), 0) == SOCKET_ERROR)
    {
        return nullptr;
    }
    
    vector<BYTE> res(RESPONSE_HEADERS_OFFSET);
    if (recv(mServerSock, (char*)&res[0], RESPONSE_HEADERS_OFFSET, 0) == SOCKET_ERROR)
    {
        return nullptr;
    }
    
    int payloadSize = VectorUtils::extractNumFromVector(res, PAYLOAD_SIZE_LEN, VERSION_LEN + STATUS_LEN);
    if (payloadSize > 0)
    {
        res.resize(RESPONSE_HEADERS_OFFSET + payloadSize);
        if (recv(mServerSock, (char*)&res[0] + RESPONSE_HEADERS_OFFSET, payloadSize, 0) == SOCKET_ERROR)
        {
            return nullptr;
        }
    }

    auto response = mDeserializer->deserializeResponse(res);
    std::cout << "Recieved response with Status: " << response->status << ", Size: " << res.size() << std::endl;

    return response;
}
