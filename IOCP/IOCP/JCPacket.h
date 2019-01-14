#pragma once

#include <cstdint>
#include <memory>		//std::memcpy()

#include "SerializeStream.h"

class JCHeader
{
public:
	enum Type
	{
		RES,
		REQ
	} type;
	uint32_t packetSize;

	void Write()
	{

	}

	JCHeader()
	{

	}
	JCHeader(Type inType, uint32_t inPacketSize) : type(inType), packetSize(inPacketSize)
	{

	}
	~JCHeader()
	{

	}

};

class JCSysinfo
{
public:
	char name[50];
	char directory[50];

	JCSysinfo()
	{

	}
	JCSysinfo(const char* inName, const char* inDirectory)
	{
		std::memcpy(name, inName, 50);
		std::memcpy(directory, inDirectory, 50);
	}
	~JCSysinfo()
	{

	}

};

class JCPacket
{
public:
	JCHeader header;
	JCSysinfo sysInfo;

	void ObjectToStream(OutputStream& ostrm)
	{
		ostrm.Write(&header.type, sizeof(header.type));
		ostrm.Write(&header.packetSize, sizeof(header.packetSize));

		ostrm.Write(sysInfo.name, sizeof(sysInfo.name));
		ostrm.Write(sysInfo.directory, sizeof(sysInfo.directory));
	}


	void StreamToObject(InputStream istrm)
	{
		istrm.Read(&header.type, sizeof(header.type));
		istrm.Read(&header.packetSize, sizeof(header.packetSize));

		istrm.Read(sysInfo.name, sizeof(sysInfo.name));
		istrm.Read(sysInfo.directory, sizeof(sysInfo.directory));
	}

	JCPacket()
	{

	}
	JCPacket(JCHeader inHeader, JCSysinfo inSysInfo) : header(inHeader), sysInfo(inSysInfo)
	{

	}
	~JCPacket()
	{

	}

};