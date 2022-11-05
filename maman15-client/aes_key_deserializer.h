#pragma once
#include "headers_deserializer.h"

class AesKeyDeserializer : public HeadersDeserializer
{
public:
	virtual Response* deserializeInnerResponse(const vector<BYTE>& source) override;
};