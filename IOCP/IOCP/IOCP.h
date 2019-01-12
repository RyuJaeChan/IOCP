#pragma once

#include <memory>		//std::shared_ptr, std::make_shared()
#include <thread>

#include "TCPSocket.h"

using TCPSocketPtr = std::shared_ptr<TCPSocket>;


class JCPacketHeader
{
public:
	size_t packetSize;


};


class JCPacket
{
public:
	char data[128];
};

struct SocketInfo
{
public:
	SocketInfo() = default;
	~SocketInfo() = default;
	SocketInfo(TCPSocket sock);

	TCPSocket sock;
};

enum IoDataMode
{
	SEND,
	RECV
};

#define BUF_SIZE 128
class IoData : public OVERLAPPED
{
public:
	IoData();
	IoData(IoDataMode mode);
	//~IoData();

	WSABUF wsaBuf;
	IoDataMode mode;
	char buff[BUF_SIZE];
	//JCPacket packet;
};

class IOCP
{
private:
	TCPSocketPtr tcpSocket;
	HANDLE comPort;


	bool acceptWorking = false;
	void AcceptWork();

	bool IoCompletionWorking = false;
	void IoCompletionWork();

	void InitIocp();
public:
	IOCP();
	~IOCP();

	bool RunServer(UINT16 portNum);

	void Send(JCPacket packet);
	void Send(SOCKET destSock, char* packet);

	virtual void OnRecvPacket(SocketInfo* socketInfo, char* packet) = 0;
	virtual void OnCloseSocket() = 0;
};

