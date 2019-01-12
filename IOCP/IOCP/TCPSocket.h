#pragma once

#include <winsock2.h>
#include <WS2tcpip.h>
#include <cstdio>		//fprintf()
#include <string>

#pragma comment (lib, "ws2_32.lib")

class SocketAddress
{
private:
	
public:
	SocketAddress();
	SocketAddress(SOCKADDR_IN sockAddr);
	~SocketAddress();

	static SocketAddress CreateTCPSocket(UINT16 portNum);
	static SocketAddress CreateTCPSocket(std::string host, UINT16 portNum);

	SOCKADDR_IN sockAddr;
	int addrLen;
};

class SocketUtil
{
public:
	static int GetLastSocketError();
};

class TCPSocket
{
private:
	SOCKET sock;
	SocketAddress sockAddr;
	WSADATA wsaData;

	const static int SOCKET_SUCCESS		= 1;
	const static int SOCKET_FAIL		= 0;
public:
	TCPSocket();
	TCPSocket(SOCKET sock, SocketAddress sockAddr);
	~TCPSocket();

	SOCKET GetSocket();
	void SetSocket(SOCKET socket);
	SocketAddress GetSocketAddress();

	int Init();
	int CreateSocket();
	int Bind(UINT16 portNum);
	int Listen(int backlog = 5);
	TCPSocket* Accept();
	int Connect();

};

