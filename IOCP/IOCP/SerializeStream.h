#pragma once

#include <cstdlib>			//std::realloc(), std::calloc()
#include <cstdint>			//uint32_t
#include <algorithm>		//std::max()

class OutputStream
{
public:
	OutputStream()
	{
		ReallocBuffer(32);
	}

	~OutputStream() { std::free(buffer); }

	//get a pointer to the data in the stream
	const char* GetBufferPtr()	const { return buffer; }
	uint32_t GetLength()		const { return head; }

	void Write(const void* inData, size_t inByteCount);

	template<typename T>
	void Write(T inData)
	{
		static_assert(std::is_arithmetic< T >::value ||
			std::is_enum< T >::value,
			"Generic Write only supports primitive data types");


		Write(&inData, sizeof(inData));
	}

private:
	void		ReallocBuffer(uint32_t inNewLength);

	char*		buffer;
	uint32_t	head;
	uint32_t	capacity;

};

class InputStream
{
public:
	InputStream(char* inBuffer, uint32_t inByteCount) :
		buffer(inBuffer), capacity(inByteCount), head(0)
	{}

	~InputStream() { //std::free(buffer);
	}

	uint32_t GetRemainingDataSize() const
	{
		return capacity - head;
	}

	void Read(void* outData, uint32_t inByteCount);


	template<typename T>
	void Read(T& outData)
	{
		static_assert(std::is_arithmetic< T >::value ||
			std::is_enum< T >::value,
			"Generic Read only supports primitive data types");

		Read(&outData, sizeof(outData));
	}

private:
	char*		buffer;
	uint32_t	head;
	uint32_t	capacity;
};