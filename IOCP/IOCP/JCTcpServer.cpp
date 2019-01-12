#include "JCTcpServer.h"


IOCPServer::IOCPServer()
{
}

IOCPServer::~IOCPServer()
{
}

void IOCPServer::OnRecvPacket(LPSOCKET_INFO socketInfo, char* packet)
{
	fprintf(stdout, "OnRecvPacket Called : %s\n", packet);
	Send(socketInfo->clientSocket, packet);
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


