#pragma once
#include "headers_deserializer.h"

class SendFileDeserializer : public HeadersDeserializer
{
protected:
	virtual Response* deserializeInnerResponse(const vector<BYTE>& source) override;
};