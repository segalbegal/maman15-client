#include "send_file_deserializer.h"
#include "vector_utils.h"

Response* SendFileDeserializer::deserializeInnerResponse(const vector<BYTE>& source)
{
    RecievedFileResponse* res = new RecievedFileResponse;
    VectorUtils::extractBufferFromVector(source, res->id, ID_LEN, RESPONSE_HEADERS_OFFSET);
    res->contentSize = VectorUtils::extractNumFromVector(source, FILE_SIZE_LEN, RESPONSE_HEADERS_OFFSET + ID_LEN);
    res->filename = VectorUtils::extractStringFromVector(source, FILENAME_LEN, RESPONSE_HEADERS_OFFSET + ID_LEN + FILE_SIZE_LEN);
    res->cksum = VectorUtils::extractNumFromVector(source, CKSUM_LEN, RESPONSE_HEADERS_OFFSET + ID_LEN + FILE_SIZE_LEN + FILENAME_LEN);

    return res;
}
