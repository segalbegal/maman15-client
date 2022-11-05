#include "crc_request_serializer.h"
#include "vector_utils.h"

vector<BYTE> CRCRequestSerializer::serializeInnerRequest(const Request* req)
{
    auto crcReq = (CRCRequest*)req;
    vector<BYTE> data;
    VectorUtils::appendBufferToVector(data, crcReq->id, ID_LEN);
    VectorUtils::appendStringToVector(data, crcReq->filename, FILENAME_LEN);

    return data;
}
