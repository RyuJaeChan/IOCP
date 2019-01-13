#pragma once

#include <memory>		//std::shared_ptr, std::make_shared()
#include <thread>
#include <future>		//std::future<T>, std::async()
#include <vector>

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
	SocketInfo(TCPSocket* sock);

	TCPSocketPtr sock;
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
	~IoData();

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
	int AcceptWork();
	std::future<int> acceptFut;

	bool ioCompletionWorking = false;
	int IoCompletionWork();
	std::future<int> ioCompFut;

	void InitIocp();
public:
	IOCP();
	virtual ~IOCP();

	bool RunServer(UINT16 portNum);

	void Send(char* packet);
	void Send(SOCKET destSock, char* packet);

	virtual void OnRecvPacket(SocketInfo* socketInfo, char* packet) = 0;
	virtual void OnCloseSocket() = 0;
};

