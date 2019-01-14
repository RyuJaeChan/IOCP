#include "JCTcpServer.h"


IOCPServer::IOCPServer()
{
}

IOCPServer::~IOCPServer()
{
}

void IOCPServer::OnRecvPacket(SocketInfo* socketInfo, char* packet)
{
	fprintf(stdout, "OnRecvPacket Called : %s\n", packet);
	//eco for test

	InputStream istrm(packet, 108);

	JCPacket in;
	in.StreamToObject(istrm);

	Send(socketInfo->sock->GetSocket(), packet);
}

void IOCPServer::OnCloseSocket()
{
	fprintf(stdout, "OnCloseSocket Called\n");
}


JCTcpServer::JCTcpServer()
{
	iocpProxy.RunServer(3302);
}


JCTcpServer::~JCTcpServer()
{
}


