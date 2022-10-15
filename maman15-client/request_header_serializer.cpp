#include "request_header_serializer.h"
#include "vector_utils.h"

vector<BYTE> RequestHeaderSerializer::serializeInnerRequest(const Request* req)
{
	return vector<BYTE>();
}

vector<BYTE> RequestHeaderSerializer::serializeRequest(const Request* req)
{
    vector<BYTE> data;
	VectorUtils::appendBufferToVector(data, req->id, ID_LEN);
	VectorUtils::appendIntToVector(data, req->version, VERSION_LEN);
	VectorUtils::appendIntToVector(data, req->msgCode, MSGCODE_LEN);
	VectorUtils::appendIntToVector(data, req->payloadSize, PAYLOAD_SIZE_LEN);

	vector<BYTE> innerData = serializeInnerRequest(req);
	data.insert(data.end(), innerData.begin(), innerData.end());

	return data;
}
