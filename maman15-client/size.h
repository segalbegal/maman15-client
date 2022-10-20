#pragma once

#define MSGCODE_LEN (2)
#define STATUS_LEN (2)
#define FILE_SIZE_LEN (4)
#define PAYLOAD_SIZE_LEN (4)
#define VERSION_LEN (1)
#define RESPONSE_HEADERS_OFFSET VERSION_LEN + STATUS_LEN + PAYLOAD_SIZE_LEN
#define CKSUM_LEN (4)
#define NAME_LEN (256)
#define FILENAME_LEN (256)
#define ID_LEN (16)
#define PUBLIC_KEY_LEN (160)