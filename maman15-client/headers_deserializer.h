#pragma once
#include "response_deserializer.h"

class HeadersDeserializer : public ResponseDeserializer
{
protected:
	virtual Response* deserializeInnerResponse(const vector<BYTE>& source) = 0;

public:
	virtual Response* deserializeResponse(const vector<BYTE>& source) override;
};
