#include "request_handler.h"
#include <boost/asio.hpp>
#include "vector_utils.h"

using boost::asio::ip::tcp;

RequestHandler::RequestHandler(string ip, int port, RequestSerializer* serializer, ResponseDeserializer* deserializer)
    : mIp(ip), mPort(port), mSerializer(serializer), mDeserializer(deserializer)
{
}

RequestHandler::~RequestHandler()
{
    delete mSerializer;
    delete mDeserializer;
}

Response* RequestHandler::handleRequest(const Request* req)
{
    boost::asio::io_context context;
    tcp::socket sock(context);
    tcp::resolver resolver(context);

    boost::asio::connect(sock, resolver.resolve(mIp, std::to_string(mPort)));

    vector<BYTE> data = mSerializer->serializeRequest(req);
    boost::asio::write(sock, boost::asio::buffer(data, data.size()));
    
    vector<BYTE> res(VERSION_LEN + STATUS_LEN + PAYLOAD_SIZE_LEN);
    boost::asio::read(sock, boost::asio::buffer(res, res.size()));
    int payloadSize = VectorUtils::extractIntFromVector(res, PAYLOAD_SIZE_LEN, VERSION_LEN + STATUS_LEN);
    if (payloadSize > 0)
    {
        res.resize(res.size() + payloadSize);
        boost::asio::read(sock, boost::asio::buffer(&res[0] + res.size(), payloadSize));
    }

    sock.close();
    return mDeserializer->deserializeResponse(res);
}
