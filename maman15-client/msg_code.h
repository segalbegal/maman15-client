#pragma once

enum MessageCode
{
	RegisterClient = 1100,
	SendPublicKey = 1101,
	SendFile = 1103,

	ValidCRC = 1104,
	InvalidCRCRetry = 1105,
	InvalidCRC = 1106,
};