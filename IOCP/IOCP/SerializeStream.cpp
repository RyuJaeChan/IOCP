#include "SerializeStream.h"

void OutputStream::ReallocBuffer(uint32_t inNewLength)
{
	buffer = static_cast<char*>(std::realloc(buffer, inNewLength));



	//handle realloc failure
	//...
	capacity = inNewLength;
}

void OutputStream::Write(const void* inData, size_t inByteCount)
{
	//make sure we have space...
	uint32_t resultHead = head + static_cast<uint32_t>(inByteCount);
	if (resultHead > capacity)
	{
		ReallocBuffer(std::max(capacity * 2, resultHead));
	}

	//copy into buffer at head
	std::memcpy(buffer + head, inData, inByteCount);

	//increment head for next write
	head = resultHead;
}


void InputStream::Read(void* outData, uint32_t inByteCount)
{
	uint32_t resultHead = head + inByteCount;
	if (resultHead > capacity)
	{
		//handle error, no data to read!
		//...
	}

	std::memcpy(outData, buffer + head, inByteCount);

	head = resultHead;
}