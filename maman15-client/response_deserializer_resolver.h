#pragma once
#include <map>
#include "response_deserializer.h"

using std::map;

class ResponseDeserializerResolver : public ResponseDeserializer
{
private:
	map<Status, ResponseDeserializer*> mDeserializers;

public:
	ResponseDeserializerResolver(const map<Status, ResponseDeserializer*>& deserializers);
	~ResponseDeserializerResolver();

	virtual Response* deserializeResponse(const vector<BYTE>& source) override;
};