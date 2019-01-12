#pragma once

#include "IOCP.h"

class IOCPServer : public IOCP
{
public:
	IOCPServer();
	~IOCPServer();

	virtual void OnRecvPacket(SocketInfo* socketInfo, JCPacket packet);
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
