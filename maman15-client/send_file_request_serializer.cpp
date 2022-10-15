#include "send_file_request_serializer.h"
#include "vector_utils.h"

vector<BYTE> SendFileRequestSerializer::serializeInnerRequest(const Request* req)
{
    FileRequest* fileReq = (FileRequest*)req;
    vector<BYTE> data;

    VectorUtils::appendBufferToVector(data, fileReq->id, ID_LEN);
    VectorUtils::appendIntToVector(data, fileReq->content.size(), FILE_SIZE_LEN);
    VectorUtils::appendStringToVector(data, fileReq->fileName, FILENAME_LEN);
    VectorUtils::appendBufferToVector(data, &fileReq->content[0], fileReq->content.size());

    return data;
}
