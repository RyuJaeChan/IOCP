#pragma once

#include "IOCP.h"
#include "JCPacket.h"

class IOCPServer : public IOCP
{
public:
	IOCPServer();
	~IOCPServer();

	virtual void OnRecvPacket(SocketInfo* socketInfo, char* packet);
	virtual void OnCloseSocket();
};

class JCTcpServer
{
private:
	IOCPServer iocpProxy;
public:
	JCTcpServer();
	~JCTcpServer();
};

class IOCPClient : public IOCP
{
public:
	IOCPClient()
	{
		Connect("127.0.0.1", 3302);
	}
	~IOCPClient()
	{

	}

	virtual void OnRecvPacket(SocketInfo* socketInfo, char* packet)
	{
		fprintf(stdout, "OnRecvPacket in client\n");
		InputStream istrm(packet, 108);
		JCPacket in;
		in.StreamToObject(istrm);
	}
	virtual void OnCloseSocket()
	{
		fprintf(stdout, "OnCloseSocket in client\n");
	}

};

class JCTcpClient
{
private:
	IOCPClient proxy;
public:
	JCTcpClient() {}
	~JCTcpClient() {}

	void Send()
	{
		JCHeader h(JCHeader::Type::RES, sizeof(JCHeader) + sizeof(JCSysinfo));
		JCSysinfo s("test_anem", "test_dir");
		JCPacket p(h, s);

		OutputStream ostrm;
		p.ObjectToStream(ostrm);


		proxy.Send(ostrm.GetBufferPtr());
	}
};