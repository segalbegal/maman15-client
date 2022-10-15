#include <zlib.h>
#include <iostream>

void tests()
{
	unsigned char phrase[] = "Hello, World!";
	std::cout << crc32(0L, phrase, strlen((char*)phrase));
}

//int main()
//{
//	tests();
//}