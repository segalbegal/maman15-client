#pragma once
#include "headers_deserializer.h"

class RegisterSuccDeserializer : public HeadersDeserializer
{
protected:
	virtual Response* deserializeInnerResponse(const vector<BYTE>& source) override;
};