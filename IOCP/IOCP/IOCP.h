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

class SocketInfo
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

class IoData : public OVERLAPPED
{
public:
	IoData(IoDataMode mode);
	~IoData();

	WSABUF wsaBuf;
	IoDataMode mode;
	char buff[128];
	//JCPacket packet;
};


typedef struct _SOCKET_INFO
{
	SOCKET clientSocket;
	SOCKADDR_IN clientAddr;
	size_t roomId;
} SOCKET_INFO, *LPSOCKET_INFO;

//이걸 커스텀해서 써야함
#define BUF_SIZE 128
typedef struct _IO_DATA : public OVERLAPPED
{
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	IoDataMode mode;
} IO_DATA, *LPIO_DATA;

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

	virtual void OnRecvPacket(LPSOCKET_INFO socketInfo, char* packet) = 0;
	virtual void OnCloseSocket() = 0;
};

